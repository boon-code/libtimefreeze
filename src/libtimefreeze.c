#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

/* definitions */

#define TIMEFREEZE_DEBUG_VERBOSE

#define LOG_PREFIX "[LTF]: "
#define LOG_PREFIX_PLACEHOLDER "       "

/* macros */

#define do_nothing() do { } while(0)

#ifdef TIMEFREEZE_DEBUG_VERBOSE
# define dbgv(...) dbg(__VA_ARGS__)
#else
# define dbgv(...) do_nothing()
#endif /* TIMEFREEZE_DEBUG_VERBOSE */

#ifdef TIMEFREEZE_DEBUG
# define dbg(fmt, ...) do { \
	if (s_debug) { \
		fprintf(stderr, LOG_PREFIX fmt "\n", ##__VA_ARGS__); \
	} \
} while(0)
#else
# define dbg(fmt, ...) do_nothing()
#endif /* TIMEFREEZE_DEBUG */

#define ABORT_LOAD_FAILED(x) do { \
	if ((x) == NULL) { \
		dbg("Failed to load " #x); \
		abort(); \
	} else { \
		dbg("Load " #x "successfully"); \
	} \
} while(0)

#ifdef TIMEFREEZE_DEBUG
static int s_debug = 0;
#endif

static int s_initialized = 0;
static const char *s_empty_str = "";
static char *s_dt_str = NULL;
static struct timespec s_dt_cache;
static int s_timefreeze = 0;

/* real functions */
static int (*real__clock_gettime)(clockid_t, struct timespec *) = NULL;


/* prototypes */

static void lib_init(void);
static void enable_debug_if(void);
static int shall_timefreeze(void);
static int strcmp_empty(const char *s1, const char *s2);
static int parse_time_str(const char *dt, struct timespec *ts);
static void ts_to_tv(const struct timespec *ts, struct timeval *tv);
static void tv_to_ts(const struct timeval *tv, struct timespec *ts);
static int tm_to_ts(struct tm *tm, struct timespec *ts);

/* public */


int clock_gettime(clockid_t clk_id, struct timespec *ts)
{
	int ret = 0;
	int is_wall_clock = (clk_id == CLOCK_REALTIME) || (clk_id == CLOCK_REALTIME_COARSE);

	if (shall_timefreeze() && is_wall_clock) {  /* fake call */
		dbg("FAKE clock_gettime(%d, %p)", (int)clk_id, ts);
		*ts = s_dt_cache;
		ret = 0;
	} else {  /* real call */
		dbg("REAL clock_gettime(%d, %p)", (int)clk_id, ts);
		ret = real__clock_gettime(clk_id, ts);
	}

	return ret;
}

int timefreeze_gettimeofday(struct timeval *tv)
{
	struct timespec ts;

	tv_to_ts(tv, &ts);
	int ret = clock_gettime(CLOCK_REALTIME, &ts);
	ts_to_tv(&ts, tv);

	return ret;
}

/* private */

static void lib_init(void)
{
	/* if fail -> abort */
	enable_debug_if();

	real__clock_gettime = dlsym(RTLD_NEXT, "clock_gettime");
	ABORT_LOAD_FAILED(real__clock_gettime);

	dbg("libtimefreeze is initialized");
	s_initialized = 1;
}

static void enable_debug_if(void)
{
#ifdef TIMEFREEZE_DEBUG
	const char *dbg = getenv("TIMEFREEZE_DEBUG");
	if (strcmp_empty(dbg, "1") == 0) {
		s_debug = 1;
		dbg("Debug output is enabled");
	}
#endif /* TIMEFREEZE_DEBUG */
}

static int shall_timefreeze(void)
{
	if (!s_initialized) {
		lib_init();
	}

	const char *new_dt_str = getenv("TIMEFREEZE");

	if (strcmp_empty(new_dt_str, s_dt_str) != 0) { /* slow path */
		free(s_dt_str);
		s_dt_str = NULL;
		if (new_dt_str == NULL)  {
			dbgv("Slow path: disable timefreeze");
			s_timefreeze = 0;
		} else {
			s_dt_str = strdup(new_dt_str);
			int err = parse_time_str(s_dt_str, &s_dt_cache);
			if (err) {
				dbgv("Slow path: failed to set time for %s", s_dt_str);
				s_timefreeze = 0;
			} else {
				dbgv("Slow path: set time for %s to %lld",
				     s_dt_str, (long long)s_dt_cache.tv_sec);
				s_timefreeze = 1;
			}
		}
	}

	return s_timefreeze;
}

static int strcmp_empty(const char *s1, const char *s2)
{
	s1 = (s1 == NULL ? s_empty_str : s1);
	s2 = (s2 == NULL ? s_empty_str : s2);
	return strcmp(s1, s2);
}

static int parse_time_str(const char *dt, struct timespec *ts)
{
	struct tm tm;
	memset(&tm, 0, sizeof(tm));

	const char *ret = strptime(dt, "%Y-%m-%d %T", &tm);
	if (ret == NULL) {
		return -1;
	} else {
		tm_to_ts(&tm, ts);

		dbgv("Parsing %s"
		     "\n" LOG_PREFIX_PLACEHOLDER
		     "- tm: s=%d, m=%d, h=%d, %d,%d,%d,%d,%d,%d"
		     "\n" LOG_PREFIX_PLACEHOLDER
		     "- ts: %lld %lld",
		     dt,
		     tm.tm_sec, tm.tm_min, tm.tm_hour,
		     tm.tm_mday, tm.tm_mon, tm.tm_year,
		     tm.tm_wday, tm.tm_yday, tm.tm_isdst,
		     (long long)ts->tv_sec,
		     (long long)ts->tv_nsec);

		return 0;
	}
}

static void ts_to_tv(const struct timespec *ts, struct timeval *tv)
{
	tv->tv_sec = ts->tv_sec;
	tv->tv_usec = ts->tv_nsec / 1000;
}

static void tv_to_ts(const struct timeval *tv, struct timespec *ts)
{
	ts->tv_sec = tv->tv_sec;
	ts->tv_nsec = tv->tv_usec * 1000;
}

static int tm_to_ts(struct tm *tm, struct timespec *ts)
{
	time_t ret = mktime(tm);
	if (ret == -1) {
		return -1;
	} else {
		ts->tv_sec = ret;
		ts->tv_nsec = 0;
		return 0;
	}
}

#undef NDEBUG
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define TIMEBUF_SIZE 1024

static char s_timebuf[TIMEBUF_SIZE];
static const char s_test_time[] = "2022-01-02 15:00:00";

static const char *tfget(void)
{
	return getenv("TIMEFREEZE");
}

static void tfset(const char *dt)
{
	setenv("TIMEFREEZE", dt, 1);
}

static int print_time(const char *desc, struct timespec *out)
{
	struct timespec ts;
	struct tm tm;
	int ret = clock_gettime(CLOCK_REALTIME, &ts);

	localtime_r(&ts.tv_sec, &tm);

	memset(s_timebuf, 0, sizeof(s_timebuf));
	strftime(s_timebuf, sizeof(s_timebuf), "%Y-%m-%d %T", &tm);
	printf("%s: %s %lld %lld (ret=%d)\n",
			desc,
			s_timebuf,
			(long long)ts.tv_sec,
			(long long)ts.tv_nsec,
			ret);

	if (out != NULL) {
		*out = ts;
	}

	return ret;
}

static void test_invalid(void)
{
	int ret;
	struct timespec orig;
	struct timespec ts;
	memset(&orig, 15, sizeof(orig));
	/* poison ts */
	memcpy(&ts, &orig, sizeof(ts));

	ret = clock_gettime(246, &ts);
	assert((ret == -1) && "clock_gettime call with invalid clk_id must fail");
	assert((memcmp(&ts, &orig, sizeof(ts)) == 0) && "ts has not been modified");
}

static void init_libtimefreeze(void)
{
	struct timespec ts;
	printf("Initialize libtimefreeze\n");
	clock_gettime(CLOCK_REALTIME, &ts);
	printf("\n\n");
}

static void all_tests(void)
{
	test_invalid();
}

static void run_tests(void)
{
	printf("Run tests without TIMEFREEZE:\n");
	tfset("");
	all_tests();
	printf("\n\n");

	printf("Run tests WITH TIMEFREEZE:\n");
	tfset(s_test_time);
	all_tests();
	printf("\n\n");

	tfset("");
}

int main(void)
{
	init_libtimefreeze();
	run_tests();

	print_time("Current time", NULL);

	tfset(s_test_time);
	printf("Adjust TIMEFREEZE: %s\n", tfget());
	print_time("Get adjusted time", NULL);
	printf("Test time: %s, printed time: %s\n", s_test_time, s_timebuf);
	assert((strcmp(s_timebuf, s_test_time) == 0) && "Set time equals printed time");

	tfset("");
	printf("Adjust TIMEFREEZE: %s\n", tfget());
	print_time("Get curent time again", NULL);

	return EXIT_SUCCESS;
}

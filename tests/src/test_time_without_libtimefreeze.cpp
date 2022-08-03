#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

extern "C" {
#include <sys/time.h>
} /* extern "C" */

#include "data.h"


static const time_t test_time_sec = TEST_TIME_SEC;

TEST(TestTimeWithout, clock_gettime_CLOCK_REALTIME)
{
	timespec ts;
	memset(&ts, 0, sizeof(ts));

	ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
	EXPECT_NE(test_time_sec, ts.tv_sec);
}

TEST(TestTimeWithout, clock_gettime_CLOCK_REALTIME_COARSE)
{
	timespec ts;
	memset(&ts, 0, sizeof(ts));

	ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME_COARSE, &ts));
	EXPECT_NE(test_time_sec, ts.tv_sec);
}

TEST(TestTimeWithout, gettimeofday)
{
	timeval tv;
	memset(&tv, 0, sizeof(tv));

	ASSERT_EQ(0, gettimeofday(&tv, nullptr));
	EXPECT_NE(test_time_sec, tv.tv_sec);
}

TEST(TestTimeWithout, time)
{
	EXPECT_NE(test_time_sec, time(nullptr));
}

TEST(TestTimeWithout, time_With_tloc)
{
	time_t val = 0;
	EXPECT_NE(test_time_sec, time(&val));
}

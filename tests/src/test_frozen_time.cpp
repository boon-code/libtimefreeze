#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

extern "C" {
#include <sys/time.h>
} /* extern "C" */

#include "data.h"


class TestTime : public ::testing::Test {
public:
	TestTime()
	{
		setenv("TIMEFREEZE", TEST_TIME_STR, 1); // UTC
	}

	~TestTime()
	{
		setenv("TIMEFREEZE", "", 1);  // Disable again
	}

protected:
	static const time_t test_time_sec;
};

const time_t TestTime::test_time_sec = TEST_TIME_SEC;

TEST_F(TestTime, clock_gettime_CLOCK_REALTIME)
{
	timespec ts;
	memset(&ts, 0, sizeof(ts));

	ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
	EXPECT_EQ(0, ts.tv_nsec);
	EXPECT_EQ(test_time_sec, ts.tv_sec);
}

TEST_F(TestTime, clock_gettime_CLOCK_REALTIME_COARSE)
{
	timespec ts;
	memset(&ts, 0, sizeof(ts));

	ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME_COARSE, &ts));
	EXPECT_EQ(0, ts.tv_nsec);
	EXPECT_EQ(test_time_sec, ts.tv_sec);
}

TEST_F(TestTime, gettimeofday)
{
	timeval tv;
	memset(&tv, 0, sizeof(tv));

	ASSERT_EQ(0, gettimeofday(&tv, nullptr));
	EXPECT_EQ(0, tv.tv_usec);
	EXPECT_EQ(test_time_sec, tv.tv_sec);
}

TEST_F(TestTime, time)
{
	EXPECT_EQ(test_time_sec, time(nullptr));
}

TEST_F(TestTime, time_With_tloc)
{
	time_t val = 0;
	EXPECT_EQ(test_time_sec, time(&val));
}

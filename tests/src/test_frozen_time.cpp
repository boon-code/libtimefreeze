#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

extern "C" {
#include <sys/time.h>
} /* extern "C" */

#define TEST_TIME_SEC 1641027601

class TestTime : public ::testing::Test {
public:
	TestTime()
	{
		setenv("TIMEFREEZE", "2022-01-01 09:00:01", 1); // UTC
	}

	~TestTime()
	{
		setenv("TIMEFREEZE", "", 1);  // Disable again
	}

private:
	static const time_t test_time_sec = 1641027601;
};

TEST_F(TestTime, clock_gettime)
{
	timespec ts;
	memset(&ts, 0, sizeof(ts));
	ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
	EXPECT_EQ(0, ts.tv_nsec);
	EXPECT_EQ(1641027601, ts.tv_sec);
}

TEST_F(TestTime, gettimeofday)
{
	timeval tv;
	memset(&tv, 0, sizeof(tv));
	ASSERT_EQ(0, gettimeofday(&tv, nullptr));
	EXPECT_EQ(0, tv.tv_usec);
	EXPECT_EQ(1641027601, tv.tv_sec);
}

TEST_F(TestTime, time)
{
	EXPECT_EQ(1641027601, time(nullptr));

	time_t val = 0;
	EXPECT_EQ(1641027601, time(&val));
}

TEST_F(TestTime, time_With_tloc)
{
	time_t val = 0;
	EXPECT_EQ(1641027601, time(&val));
}

#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

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
};

TEST_F(TestTime, VerifyClockGetTime)
{
	timespec ts;
	memset(&ts, 0, sizeof(ts));
	ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
	EXPECT_EQ(0, ts.tv_nsec);
	EXPECT_EQ(1641027601, ts.tv_sec);
}

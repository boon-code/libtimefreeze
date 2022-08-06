#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include "data.h"

class TestEnvVarUnset : public ::testing::Test {
public:
	TestEnvVarUnset()
	{

		setenv("TIMEFREEZE", "2022-01-01 09:00:27", 1);
	}

	~TestEnvVarUnset()
	{
		setenv("TIMEFREEZE", "", 1);
	}

protected:
	static const time_t test_time_sec;
};

const time_t TestEnvVarUnset::test_time_sec = 1641027627;

TEST_F(TestEnvVarUnset, clock_gettime_CLOCK_REALTIME)
{
	timespec ts;
	memset(&ts, 0, sizeof(ts));

	ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
	EXPECT_EQ(test_time_sec, ts.tv_sec);

	unsetenv("TIMEFREEZE");

	ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &ts));
	EXPECT_NE(test_time_sec, ts.tv_sec);
}

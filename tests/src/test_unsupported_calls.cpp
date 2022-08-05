#include <gtest/gtest.h>
#include <ctime>
#include <cstring>

extern "C" {
#include <sys/timeb.h>
} /* extern "C" */

TEST(UnsupportedDeath, ftime)
{
	struct timeb tb;
	memset(&tb, 0, sizeof(tb));

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	EXPECT_DEATH({
		ftime(&tb);
	}, "ftime is not supported");
#pragma GCC diagnostic pop
}

TEST(UnsupportedDeath, timespec_get)
{
	struct timespec ts;
	memset(&ts, 0, sizeof(ts));

	EXPECT_DEATH({
		timespec_get (&ts, 0);
	}, "timespec_get is not supported");
}

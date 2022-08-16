libtimefreeze
=============

[![pipeline](https://gitlab.com/zahnputzmonster/libtimefreeze/badges/main/pipeline.svg)](https://gitlab.com/zahnputzmonster/libtimefreeze/-/commits/main)

[![codecov](https://codecov.io/gl/zahnputzmonster/libtimefreeze/branch/main/graph/badge.svg?token=719ZBL64D6)](https://codecov.io/gl/zahnputzmonster/libtimefreeze)

*Obsolete*: Use `libfaketime` instead (please read below)

This is a small shared library that can be preloaded to allow to set / freeze
the real time clock while f.e. executing test cases that ddepend on the wall
clock.
A good example of this use-case is testing certificate handling and time
dependend signatures. During the test case, the environment variable
`TIMEFREEZE` may be adjusted as needed or disabled. Certificates can be
statically created and the same certificate can be used for failing and passing
tests by only changing `TIMEFREEZE` accordingly.

I recommend using `libfaketime` as it is a much more complete, widely support
and prooven in use solution to fake the current time:
- https://github.com/wolfcw/libfaketime.git

This library is heavily inspired by the awsome `libfaketime`.

Originally, it seemed that this use case is not possible with `libfaketime`.
By setting the environment variable `FAKETIME_NO_CACHE` to `1`, the use case can
be accomplished in a similar way.


# Usage

The shared library has to be preloaded using `LD_PRELOAD`. The following
environment variables can be used to manipulate the behavior:
- `TIMEFREEZE`: Can be used to specify a UTC timestamp in the format
  `%Y-%m-%d %T` (`2022-08-03 09:21:07`). If set, this time will be provided
  (frozen). By deleting the environment variable or setting it to an empty
  string, the mechanism can be disabled again and the actual wall clock can be
  retrieved again.
- `TIMEFREEZE_DEBUG`: set to `1` to enable debug messages to `stderr`. Has to
  be configured before initialization. The library is initialized on the first
  invocation of one of the replaced time related functions.

Use the environment variable `LD_PRELOAD` to preload this library when
executing the application (in this case `date`):

        LD_PRELOAD=./.build/libtimefreeze.so.0 TIMEFREEZE="2022-01-01 10:01:02" date

With debug logs enabled:

        LD_PRELOAD=./.build/libtimefreeze.so.0 TIMEFREEZE_DEBUG=1 TIMEFREEZE="2022-01-01 10:01:02" date

However, the main use case for this library is to run test cases with some
testing framework (like GoogleTest). Assuming you compile your tests into an
executable called `tests`, you would typically run your tests like this:

        LD_PRELOAD=<path>/libtimefreeze.so.0 ./tests [gtest options]

This way, your test cases are not affected at first, as `TIMEFREEZE` is not
yet set. The `TIMEFREEZE` variable will only be set in the test cases that
require a faked time to succeed.
A GoogleTest example can be found here: [test\_time.cpp](examples/gtest_example/src/test_time.cpp)

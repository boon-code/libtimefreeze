libtimefreeze
=============

[![pipeline](https://gitlab.com/zahnputzmonster/libtimefreeze/badges/main/pipeline.svg)](https://gitlab.com/zahnputzmonster/libtimefreeze/-/commits/main)

[![codecov](https://codecov.io/gl/zahnputzmonster/libtimefreeze/branch/main/graph/badge.svg?token=719ZBL64D6)](https://codecov.io/gl/zahnputzmonster/libtimefreeze)

This is a small shared library that can be preloaded to allow to set / freeze
the real time clock while f.e. executing test cases that ddepend on the wall
clock.
A good example of this use-case is testing certificate handling and time
dependend signatures. During the test case, the environment variable
`TIMEFREEZE` may be adjusted as needed or disabled. Certificates can be
statically created and the same certificate can be used for failing and passing
tests by only changing `TIMEFREEZE` accordingly.

For most other use cases, you should checkout libfaketime, which is a much more
complete and prooven in use solution to fake the current time:
- https://github.com/wolfcw/libfaketime.git

This library is heavily inspired by the awsome `libfaketime`.


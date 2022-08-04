#!/bin/bash

_cmake_build() {
    if which ninja >/dev/null; then
        cmake -G Ninja $@ && ninja
    else
        cmake $@ && make
    fi
}

_run_gtest_example() {
    # Update README.md if the soversion changes
    local libname="libtimefreeze.so.0"

    ( \
        cd example/gtest_example/ \
        && mkdir -p build/ \
        && cd build \
        && _cmake_build ..
    ) \
	&& TIMEFREEZE_DEBUG=1 \
	   LD_PRELOAD="./.build/$libname" \
	   ./example/gtest_example/build/tests
}

_run_unit_tests() {
    # Run unit tests with and without TIMEFREEZE_DEBUG

    ( \
        cd tests/ \
        && mkdir -p build/ \
        && cd build \
        && _cmake_build ..
    ) \
	&& TIMEFREEZE_DEBUG=1 \
	   LD_PRELOAD=./.build/libtimefreeze.so \
	   ./tests/build/tests \
	&& LD_PRELOAD=./.build/libtimefreeze.so \
	   ./tests/build/tests
}

_run_c_example() {
    ./run_example.sh
}

./build.sh -N -c \
    && _run_unit_tests \
    && _run_c_example \
    && _run_gtest_example \
    && gcovr . --xml-pretty > coverage.cobertura.xml

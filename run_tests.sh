#!/bin/bash

_cmake_build() {
    if which ninja >/dev/null; then
        cmake -G Ninja $@ && ninja
    else
        cmake $@ && make
    fi
}

_run_gtest_example() {
    ( \
        cd example/gtest_example/ \
        && mkdir build/ \
        && cd build \
        && _cmake_build ..
    ) \
	&& TIMEFREEZE_DEBUG=1 \
	   LD_PRELOAD=./.build/libtimefreeze.so \
	   ./example/gtest_example/build/tests
}

_run_c_example() {
    ./run_example.sh
}

./build.sh -N -c \
    && _run_c_example \
    && _run_gtest_example \
    && gcovr . --xml-pretty > coverage.cobertura.xml

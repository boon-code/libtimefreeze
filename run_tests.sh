#!/bin/bash

_run_gtest_example() {
    # Update README.md if the soversion changes
    local libname="libtimefreeze.so.0"

    TIMEFREEZE_DEBUG=1 \
        LD_PRELOAD="./.build/$libname" \
        ".build/examples/gtest_example/example-tests"
}

_run_unit_tests() {
    # Run unit tests with and without TIMEFREEZE_DEBUG

    TIMEFREEZE_DEBUG=1 \
        LD_PRELOAD=./.build/libtimefreeze.so \
        ./.build/tests/tests \
    && LD_PRELOAD=./.build/libtimefreeze.so \
        ./.build/tests/tests
}

_run_c_example() {
    ./run_example.sh
}

_try_capture_coverage() {
    if which gcovr >/dev/null 2>/dev/null; then
        gcovr . --xml-pretty > coverage.cobertura.xml
    else
        echo "WARNING: gcovr is not available - skip capturing coverage data" >&2
    fi
}

./build.sh -N -c \
    && _run_unit_tests \
    && _run_c_example \
    && _run_gtest_example \
    && _try_capture_coverage

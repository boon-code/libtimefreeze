#!/bin/bash

./build.sh -N -c && \
    ./run_example.sh && \
    gcovr . --xml-pretty > coverage.cobertura.xml

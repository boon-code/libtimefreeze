#!/bin/bash

./build.sh -i \
	&& TIMEFREEZE_DEBUG=1 \
	   LD_PRELOAD=./.build/libtimefreeze.so \
	   ./.build/examples/simple/example

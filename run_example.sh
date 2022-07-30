#!/bin/bash

./build.sh && \
	TIMEFREEZE_DEBUG=1 \
	LD_PRELOAD=./.build/libtimefreeze.so \
	./.build/example

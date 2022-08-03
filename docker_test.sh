#!/bin/bash

docker build -t registry.gitlab.com/zahnputzmonster/libtimefreeze:latest ci/
docker run --rm -ti -v $(pwd):/ws:ro registry.gitlab.com/zahnputzmonster/libtimefreeze:latest /bin/bash -c "cd / && rm -rf /tmp/ltf/ && cp -r /ws /tmp/ltf && cd /tmp/ltf && rm -rf .build/ && ./run_tests.sh"

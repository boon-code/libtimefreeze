FROM ubuntu:latest

# Woraround for gitlab:
# Ensure that we have small layers, as I am unable to push larger layers

RUN DEBIAN_FRONTEND=noninteractive apt-get update \
 && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        perl \
        xz-utils \
        bzip2 \
 && rm -rf /var/lib/apt/lists/*

RUN DEBIAN_FRONTEND=noninteractive apt-get update \
 && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        binutils \
 && rm -rf /var/lib/apt/lists/*

RUN DEBIAN_FRONTEND=noninteractive apt-get update \
 && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        libc-dev-bin \
        libc6 \
        libc6-dev \
 && rm -rf /var/lib/apt/lists/*

RUN DEBIAN_FRONTEND=noninteractive apt-get update \
 && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        gcc \
 && rm -rf /var/lib/apt/lists/*

RUN DEBIAN_FRONTEND=noninteractive apt-get update \
 && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        g++ \
 && rm -rf /var/lib/apt/lists/*

RUN DEBIAN_FRONTEND=noninteractive apt-get update \
 && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        build-essential \
 && rm -rf /var/lib/apt/lists/*

RUN DEBIAN_FRONTEND=noninteractive apt-get update \
 && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        ninja-build \
        cmake \
 && rm -rf /var/lib/apt/lists/*

RUN DEBIAN_FRONTEND=noninteractive apt-get update \
 && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        gdb \
 && rm -rf /var/lib/apt/lists/*

RUN DEBIAN_FRONTEND=noninteractive apt-get update \
 && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        cppcheck \
 && rm -rf /var/lib/apt/lists/*

RUN DEBIAN_FRONTEND=noninteractive apt-get update \
 && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        python3 \
 && rm -rf /var/lib/apt/lists/*

RUN DEBIAN_FRONTEND=noninteractive apt-get update \
 && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        gcovr \
 && rm -rf /var/lib/apt/lists/*

RUN DEBIAN_FRONTEND=noninteractive apt-get update \
 && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        curl \
        ca-certificates \
 && rm -rf /var/lib/apt/lists/*

RUN DEBIAN_FRONTEND=noninteractive apt-get update \
 && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        git \
 && rm -rf /var/lib/apt/lists/*

RUN DEBIAN_FRONTEND=noninteractive apt-get update \
 && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        libgtest-dev \
 && rm -rf /var/lib/apt/lists/*

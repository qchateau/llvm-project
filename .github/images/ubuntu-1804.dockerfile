FROM ubuntu:18.04

RUN apt-get update && apt-get install -y \
    build-essential \
    ninja-build \
    libz-dev \
    python3 \
    wget

RUN cd /tmp && \
    wget -q https://github.com/Kitware/CMake/releases/download/v3.19.2/cmake-3.19.2-Linux-x86_64.sh && \
    chmod +x ./cmake-3.19.2-Linux-x86_64.sh && \
    ./cmake-3.19.2-Linux-x86_64.sh --skip-license --prefix=/usr/local/ && \
    rm ./cmake-3.19.2-Linux-x86_64.sh

COPY build.sh /
RUN chmod +x /build.sh

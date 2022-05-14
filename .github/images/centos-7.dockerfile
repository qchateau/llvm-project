FROM centos:7

RUN yum install -y \
    centos-release-scl \
    http://repo.okay.com.mx/centos/7/x86_64/release/okay-release-1-1.noarch.rpm

RUN yum install -y \
    python3 \
    wget \
    devtoolset-8-gcc-c++ \
    ninja-build \
    zlib-devel

RUN cd /tmp && \
    wget -q https://github.com/Kitware/CMake/releases/download/v3.19.2/cmake-3.19.2-Linux-x86_64.sh && \
    chmod +x ./cmake-3.19.2-Linux-x86_64.sh && \
    ./cmake-3.19.2-Linux-x86_64.sh --skip-license --prefix=/usr/local/ && \
    rm ./cmake-3.19.2-Linux-x86_64.sh

COPY build.sh /
RUN chmod +x /build.sh

COPY centos-7-entrypoint.sh /
RUN chmod +x /centos-7-entrypoint.sh

ENTRYPOINT [ "/centos-7-entrypoint.sh" ]

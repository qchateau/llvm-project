#!/bin/bash
set -e

echo "Repository at to $1, output at $2"

mkdir build && cd build
cflags="-O3 -DNDEBUG"
cmake -G Ninja $1/llvm/ \
    "-DCMAKE_BUILD_TYPE=Release" \
    "-DCLANG_PLUGIN_SUPPORT=OFF" \
    "-DLLVM_ENABLE_ASSERTIONS=OFF" \
    "-DLLVM_ENABLE_BACKTRACES=ON" \
    "-DLLVM_ENABLE_PLUGINS=OFF" \
    "-DLLVM_ENABLE_PROJECTS=clang;clang-tools-extra" \
    "-DLLVM_ENABLE_TERMINFO=OFF" \
    "-DLLVM_ENABLE_ZLIB=FORCE_ON" \
    "-DCMAKE_C_FLAGS_RELEASE=$cflags" \
    "-DCMAKE_CXX_FLAGS_RELEASE=$cflags"
cmake --build . --target clangd
cp ./bin/clangd $2

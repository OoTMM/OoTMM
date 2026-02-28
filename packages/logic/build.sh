#!/bin/sh

mkdir -p build
emcmake cmake -S src -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j 16

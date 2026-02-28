#!/bin/sh

mkdir -p build
emcmake cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j 16

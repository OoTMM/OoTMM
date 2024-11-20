#!/bin/bash

mkdir -p third_party

[ ! -d "third_party/ultralib" ] && git clone https://github.com/decompals/ultralib --depth 50 third_party/ultralib

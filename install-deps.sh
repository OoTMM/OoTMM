#!/bin/sh

set -e

mkdir -p third_party
rm -rf third_party/*
git clone https://github.com/decompals/ultralib third_party/ultralib
touch third_party/.stamp

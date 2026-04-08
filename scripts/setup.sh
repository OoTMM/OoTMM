#!/bin/sh

if [ ! -d "third_party/ultralib" ]; then
  mkdir -p third_party
  git clone --depth=1 https://github.com/decompals/ultralib third_party/ultralib
fi

pnpm install

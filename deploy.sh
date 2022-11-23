#!/bin/sh

rm -rf tmp
mkdir -p tmp
git clone --depth=50 --branch=develop https://github.com/OoTMM/core tmp || exit 1
cd tmp
npm i || exit 1
npm run build || exit 1
npm link || exit 1
cd ..
npm link @ootmm/core
npm i || exit 1
npm run build || exit 1

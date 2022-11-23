#!/bin/sh

rm -rf vendor/core
mkdir -p vendor/core
git clone --depth=50 --branch=develop https://github.com/OoTMM/core vendor/core || exit 1
cd vendor/core
npm i || exit 1
npm run build || exit 1
cd ../..
npm i || exit 1
npm run build || exit 1

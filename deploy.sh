#!/bin/sh

rm -rf vendor/core
mkdir -p vendor/core
if [ "stable" -eq "$1" ]; then
  git clone --depth=50 --branch=master https://github.com/OoTMM/core vendor/core || exit 1
  tag="$(git tag --list --sort='-version:refname' | head -n1)"
  git checkout "$tag"
  cd vendor/core
  export VERSION="$tag"
else
  git clone --depth=50 --branch=develop https://github.com/OoTMM/core vendor/core || exit 1
  cd vendor/core
  export VERSION="$(git rev-parse --short HEAD)"
fi
npm i || exit 1
npm run build || exit 1
cd ../..
npm i || exit 1
npm run build || exit 1

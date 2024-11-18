#!/bin/bash

if [ -z "$VERSION" ]; then
  echo "VERSION is not set"
  exit 1
fi

rm -rf tmp
mkdir -p tmp
cd dist
zip -r ../tmp/$VERSION.zip .
cd ..

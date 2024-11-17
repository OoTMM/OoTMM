#!/bin/bash

# Make the tree
rm -rf dist
mkdir -p dist/tree/_public

# Copy data
cp -r ../gui/dist/* dist/tree/_public

# Hash the index.html
index_hash=$(sha256sum dist/tree/_public/index.html | cut -d ' ' -f 1 | head -c 16)
index_file_name="generator-$index_hash.html"
mv dist/tree/_public/index.html dist/tree/_public/$index_file_name

# TODO: This can be changed later
echo "{\"generator\":{\"path\": \"${index_file_name}\", \"type\":\"iframe\"}}" > dist/tree/config.json

pushd dist/tree
zip -r ../static.zip .
popd


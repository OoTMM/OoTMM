#!/bin/bash

TMP_DIR=tmp
DIST_DIR=dist

# Make the tree
rm -rf $TMP_DIR
mkdir -p $TMP_DIR

# Copy data
cp -r ../gui/dist/* $TMP_DIR/

# Hash the index.html
index_hash=$(sha256sum $TMP_DIR/index.html | cut -d ' ' -f 1 | head -c 16)
index_file_name="generator-$index_hash.html"
mv $TMP_DIR/index.html $TMP_DIR/$index_file_name

# TODO: This can be changed later
echo "{\"generator\":{\"path\": \"${index_file_name}\", \"type\":\"iframe\"}}" > $TMP_DIR/manifest.json

# Create a ZIP
rm -rf $DIST_DIR
mkdir -p $DIST_DIR

pushd $TMP_DIR
zip -r static.zip .
popd

mv $TMP_DIR/static.zip $DIST_DIR/static.zip

# Cleanup
rm -rf $TMP_DIR

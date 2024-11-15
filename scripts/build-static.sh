#!/bin/bash

TMP_DIR=tmp/static
DIST_DIR=dist/static

# Make the tree
rm -rf $TMP_DIR
mkdir -p $TMP_DIR

# Copy data
cp -r packages/gui/dist $TMP_DIR/generator

# Create a ZIP
rm -rf $DIST_DIR
mkdir -p $DIST_DIR

pushd $TMP_DIR
zip -r static.zip .
popd

mv $TMP_DIR/static.zip $DIST_DIR/static.zip

# Cleanup
rm -rf tmp

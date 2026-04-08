#!/bin/sh

rm -rf resources
mkdir resources
cp -r src/* resources/
cp -r ../gui/dist/desktop/* resources/
mkdir -p resources/icons
cp -r ../gui/src/assets/logo.png resources/icons/appIcon.png

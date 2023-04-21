#!/bin/sh

# Truncate the env file
echo -n > .env

# Fetch the version
if [ "stable" = "$1" ]; then
  VERSION="$(git tag --list --sort='-version:refname' | head -n1)"
else
  VERSION="$(git rev-parse --short HEAD)"
fi

# Write the env file
echo "VERSION=$VERSION" >> .env

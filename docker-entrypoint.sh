#!/bin/sh

set -e

mkdir -p third_party
if [ ! -e third_party/.stamp ]; then
  ./install-deps.sh
fi

mkdir -p node_modules build dist
chown ootmm node_modules build
chown -R ootmm dist

su ootmm -c 'npm install'
exec su ootmm -c 'command "$@"' -- argv0 "$@"

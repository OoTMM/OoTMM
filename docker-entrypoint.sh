#!/bin/sh

set -e

mkdir -p node_modules build dist out third_party
chown ootmm node_modules build
chown -R ootmm dist out third_party

su ootmm -c 'npm install'
exec su ootmm -c 'command "$@"' -- argv0 "$@"

#!/bin/sh

set -e

mkdir -p third_party
if [ ! -e third_party/.stamp ]; then
  ./install-deps.sh
fi

chown ootmm .bundle build vendor
chown -R ootmm dist

su ootmm -c 'bundle config path vendor/bundle'
su ootmm -c 'bundle install'

exec su ootmm -c "exec $@"

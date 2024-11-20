#!/bin/bash

./scripts/setup.sh

if [ -e ./devcontainer-setup.local.sh ]; then
  ./devcontainer-setup.local.sh
fi

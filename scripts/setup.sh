#!/bin/sh

corepack install
corepack enable
./scripts/install-deps.sh
pnpm install

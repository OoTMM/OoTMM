#!/bin/sh

corepack install
corepack enable

./install-deps.sh
pnpm install

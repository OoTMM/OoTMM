#!/bin/bash

function die() {
  echo $1
  exit 1
}

# Deploy to Netlify
../../node_modules/.bin/netlify deploy -d "$PWD/dist" --site $NETLIFY_SITE_ID_WEB --prod || die "Failed to deploy"

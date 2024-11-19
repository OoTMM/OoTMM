#!/bin/bash

function die() {
  echo $1
  exit 1
}

# Deploy to Netlify
netlify deploy -d "$PWD/dist" --auth $NETLIFY_AUTH_TOKEN --site $NETLIFY_SITE_ID_WEB --prod || die "Failed to deploy"

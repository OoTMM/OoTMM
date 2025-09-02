#!/bin/bash

function die() {
  echo $1
  exit 1
}

if [ ! -z "$DEPLOY_STABLE" ]; then
  echo "No need to deploy web on stable"
  exit 0
fi

# Deploy to Netlify
../../node_modules/.bin/netlify deploy -d "$PWD/dist" --site $NETLIFY_SITE_ID_WEB --prod || die "Failed to deploy"

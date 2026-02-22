#!/bin/bash

function die() {
  echo $1
  exit 1
}

if [ ! "$DEPLOY_TYPE" == "dev" ]; then
  echo "No need to deploy web"
  exit 0
fi

# Deploy to Netlify
../../node_modules/.bin/netlify deploy -d "$PWD/dist" --site $NETLIFY_SITE_ID_WEB --prod || die "Failed to deploy"

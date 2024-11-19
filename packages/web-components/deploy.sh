#!/bin/bash

function die() {
  echo $1
  exit 1
}

if [ -z "$VERSION" ]; then
  die "VERSION is not set"
fi

# Clean
rm -rf tmp
mkdir -p tmp

# Make a zip of the current tree
cd dist/current
zip -r ../../tmp/$VERSION.zip . || die "Failed to zip"
cd ../..

# Upload to S3
aws s3 cp tmp/$VERSION.zip $S3_BUCKET || die "Failed to upload"
rm -rf tmp/$VERSION.zip

# Make the dirs
mkdir -p tmp/tree
mkdir -p tmp/configs

# Copy the files to the tmp tree
cp -r dist/current/* tmp/tree/
cp dist/current/config.json tmp/configs/$VERSION.json

# Download from S3
mkdir -p tmp/download
aws s3 sync --exclude $VERSION.zip $S3_BUCKET tmp/download || die "Failed to download"
for file in `ls -1 tmp/download`; do
  base=`basename $file .zip`
  unzip -o tmp/download/$file -d tmp/tree/ || die "Failed to unzip"
  mv tmp/tree/config.json tmp/configs/$base.json || die "Failed to move config"
done
rm -rf tmp/download

# Merge configs
npx tsx ./merge-configs.ts || die "Failed to merge configs"
rm -rf tmp/configs

# Deploy to Netlify
netlify deploy -d "$PWD/tmp/tree" --auth $NETLIFY_AUTH_TOKEN --site $NETLIFY_SITE_ID_STATIC --prod || die "Failed to deploy"

# Final cleanup
rm -rf tmp

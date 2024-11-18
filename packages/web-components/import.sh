#!/bin/sh

mkdir -p tmp/download
aws s3 sync --exclude $VERSION.zip $S3_BUCKET tmp/download

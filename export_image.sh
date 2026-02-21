#!/bin/sh
set -e

PROJECT_DIR="/data"

SRC_DIR="$PROJECT_DIR/projects/yocto/build-rpi/tmp/deploy/images/raspberrypi4"
DST_DIR="$PROJECT_DIR/projects/images"

IMAGE_NAME="core-image-full-cmdline-raspberrypi4.wic.bz2"
GENERIC_NAME="image.wic"

TIMESTAMP=$(date +"%Y%m%d_%H%M%S")

SRC_IMAGE="${SRC_DIR}/${IMAGE_NAME}"
DST_IMAGE_TS="${DST_DIR}/core-image-${TIMESTAMP}.wic.bz2"

# Create destination directory if needed
mkdir -p "${DST_DIR}"

# Copy image with timestamp
cp -f "${SRC_IMAGE}" "${DST_IMAGE_TS}"

# Decompress but keep original .bz2
bunzip2 -fk "${DST_IMAGE_TS}"

# Rename decompressed image to generic name
mv -f "${DST_DIR}/core-image-${TIMESTAMP}.wic" "${DST_DIR}/${GENERIC_NAME}"

echo "Image exported successfully:"
echo "  Compressed: ${DST_IMAGE_TS}"
echo "  Decompressed: ${DST_DIR}/${GENERIC_NAME}"


#!/usr/bin/env bash

echo "window.BUNDLED_IMAGES = {" > "$1"
for file in ../images/*.pbm
do
  echo "image-bundling $file"
  echo "  '$(basename "$file")': \`$(cat "$file")\`," >> "$1"
done
echo "};" >> "$1"

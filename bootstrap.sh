#!/usr/bin/env bash

set -e

which convert > /dev/null 2>&1
hasConvert=$?

cd "$(dirname "$0")"

prefix=./app/resources/monsters

rm -rf $prefix
cp -r ./resources/sprites $prefix

find $prefix -type f | while read -r file; do
  fileColor="${file%.*}~color.png"
  fileBw="${file%.*}~bw.png"
  if [[ $hasConvert ]]; then
    convert "$file" -sample 44x\> "$file"
    convert "$file" -flatten -alpha off +dither -remap ./resources/pebble_colors_64.gif -trim -type palette "$fileColor"
    convert "$file" -flatten -alpha off -monochrome -trim -colors 2 -type palette "$fileBw"
  else
    cp "$file" "${file%.*}~color.png"
    cp "$file" "${file%.*}~bw.png"
  fi
done

find $prefix -type f -not -iname "*~*" -delete

#!/usr/bin/env bash

set -e

which convert > /dev/null 2>&1
hasConvert=$?

cd "$(dirname "$0")"

prefix=./app/resources/monsters

rm -rf $prefix
cp -r ./resources/sprites $prefix


if [[ $hasConvert ]]; then
  convert "$prefix/007_kingslime.png"  -level 0%,100%,1.2 "$prefix/007_kingslime.png"
  convert "$prefix/002_boxslime.png"   -level 0%,100%,1.2 "$prefix/002_boxslime.png"
  convert "$prefix/012_slabbit.png"    -level 0%,100%,1.1 "$prefix/012_slabbit.png"
  convert "$prefix/023_chameleon.png"  -level 0%,100%,1.2 "$prefix/023_chameleon.png"
  convert "$prefix/037_orochi.png"     -level 0%,100%,1.1 "$prefix/037_orochi.png"
  convert "$prefix/041_skydragon.png"  -level 0%,100%,0.8 "$prefix/041_skydragon.png"
  convert "$prefix/096_facetree.png"   -level 0%,100%,1.1 "$prefix/096_facetree.png"
  convert "$prefix/187_iceman.png"     -level 0%,100%,1.1 "$prefix/187_iceman.png"
  convert "$prefix/204_deathmore2.png" -level 0%,100%,1.2 "$prefix/204_deathmore2.png"
  convert "$prefix/211_mirudraas2.png" -level 0%,100%,1.2 "$prefix/211_mirudraas2.png"
fi

find $prefix -type f | while read -r file; do
  fileColor="${file%.*}~color.png"
  fileBw="${file%.*}~bw.png"
  if [[ $hasConvert ]]; then
    convert "$file" -strip -sample 44x\> "$file"
    convert "$file" -strip -flatten -alpha off +dither -remap ./resources/pebble_colors_64.gif -trim -type palette "$fileColor"
    convert "$file" -strip -flatten -alpha off -monochrome -trim -colors 2 -type palette "$fileBw"
  else
    cp "$file" "${file%.*}~color.png"
    cp "$file" "${file%.*}~bw.png"
  fi
done

find $prefix -type f -not -iname "*~*" -delete

#!/usr/bin/env bash

make contour_of || exit

for image in images/zebres-*.pbm images/courbe_hilbert_*.pbm
do

echo
echo "contour_of $image -d 0"
command time -f "%U" ./contour_of "$image" -d 0

done

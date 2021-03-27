#!/usr/bin/env bash

# Génère les 15 images pour le CR de les tâches 7-*, en PNG, dimensions 400x400.
# Il faut: `gs`, ImageMagick (pour le redimensionnement), `sponge` (paquet "moreutils" sur apt), clang + GNU make

make contour_of

files="Asterix3.pbm lettre-L-cursive.pbm Picasso-ColombesDeLaPaix.pbm"

if [ "$1" == "2" ]
then
  flag=b
  wd=cr71
elif [ "$1" == "3" ]
then
  flag=B
  wd=cr72
else
  >&2 echo "utilisation: ./cr7 <2|3>"
  exit 1
fi

mkdir -p $wd
pushd $wd > /dev/null || exit

for image in $files
do
  ln -s "../images/$image" .
done

for d in 0 1 3 10 30
do
  for image in $files
  do
    img="$image-$d.png"
    if [ $d = 0 ]
    then
      dd=""
    else
      dd="-$flag $d"
    fi
    >&2 ../contour_of $image $dd -3 -c
    gs -dSAFER -dBATCH -dNOPAUSE -dEPSCrop -r600 -sDEVICE=png16m -sOutputFile=$img $image"-mode3.eps" > /dev/null
    convert $img -resize 400x400 -colorspace RGB png:- | sponge $img
  done
done

rm ./*.contours

popd > /dev/null || exit

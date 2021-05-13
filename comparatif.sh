#!/usr/bin/env bash

make contour_of > /dev/null || exit 1

pattern=': ([0-9]+)'

wd=cr82

mkdir $wd > /dev/null 2>&1
pushd > /dev/null $wd || exit 1

for img in Asterix3 lettre-L-cursive Picasso-ColombesDeLaPaix; do
  img=$img".pbm"
  echo "#### \`$img\`"
  echo
  ln -sf ../images/$img .

  ss="0 0.5 1 2 4 8 16"

  # Entête du tableau / première ligne
  echo -n "| Modes \\ d= | "
  echo -n "$ss" | sed "s/ / | /g"
  echo " |"
  echo -n "|---"
  echo -n "$ss" | sed -E 's/[0-9.]+ ?/|---/g'
  echo "|"

  # Autres lignes
  for mode in d b B; do
    if [ $mode = "d" ]; then
      echo -n "| **Segments** "
    elif [ $mode = "b" ]; then
      echo -n "| **Bezier 2** "
    elif [ $mode = "B" ]; then
      echo -n "| **Bezier 3** "
    else
      exit 1 # Inatteignable normalement
    fi

    for thresh in $ss; do
      if [ $img = "Asterix3.pbm" ] && [[ "1 2 4 8" =~ $thresh ]]; then
        ../contour_of $img "-$mode" "$thresh" -3 > /dev/null
        final_png="$img-$mode-$thresh.png"
        gs -dSAFER -dBATCH -dNOPAUSE -dEPSCrop -r600 -sDEVICE=png16m -sOutputFile="$final_png" "$img-mode3.eps" > /dev/null

        if [ $mode = d ]; then
          color=Khaki
        elif [ $mode = b ]; then
          color=Plum
        elif [ $mode = B ]; then
          color=Violet
        fi

        convert "$final_png" -resize 400x400 \
            -colorspace RGB \
            -background $color label:"mode=$mode seuil=$thresh" -gravity Center -append \
            png:- | sponge "$final_png"
      fi

      out=$(../contour_of $img "-$mode" "$thresh" -c)
      if [[ $out =~ $pattern ]]; then
        echo -n "| ${BASH_REMATCH[1]} "
      else
        echo -n "| $out "
      fi
    done
    echo "|"
  done
  echo
done

popd > /dev/null || exit 1

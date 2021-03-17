# Listes génériques

Comme on va avoir besoin de plusieurs types de listes, pour éviter les répétitions, j'émule des types génériques en copiant des fichiers de template `liste_.h` et `liste_.c` et en y remplaçant certaines chaînes de caractères.
Le `Makefile` s'occupe de cette "expension".

## Fonctionnement

  * `/listes/liste_.h` et `/listes/liste_.c` contiennent des templates où les occurence du type sont "TTTTT"
  * `/listes/*.env` contiennent des modèles pour remplacer les "TTTTT" (et les variantes majuscules / minuscules), ainsi que les headers à inclure
  * Avec `sed`, le Makefile peut générer tout `/liste_*.{c,h}` où `*` correspond à un fichier `.env`

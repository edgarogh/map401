.PHONY: clean bundle.js

map401_c.js map401_c.wasm: web_utils.c ../image.c ../liste_points.c ../contour.c ../geom2d.c ../sortie_eps.c
	emcc $^ -o map401_c.js -s WASM=1 -s MODULARIZE -s EXPORTED_RUNTIME_METHODS=['ccall','FS'] -s FORCE_FILESYSTEM=1 -s ALLOW_MEMORY_GROWTH=1 \
	-s EXPORTED_FUNCTIONS=['_web_lire_fichier_image','_web_image_width','_web_image_height','_web_image_free','_web_contour','_web_contour_len','_web_contour_points']

image_bundle.js: make_image_bundle.sh ../images/*.pbm
	./make_image_bundle.sh $@

bundle.js: main.js map401_w.js map401_c.js
	browserify main.js -o $@

bundle: bundle.js image_bundle.js

clean:
	rm bundle.js image_bundle.js map401_c.*

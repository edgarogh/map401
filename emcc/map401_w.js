// Wrapper for the generated C library

const c = require('./map401_c');

async function init() {
    const instance = await c();
    const FS = instance.FS;

    let id = 0;

    window.Module = instance;

    class CObject {
        constructor(ptr) {
            this.ptr = ptr;
        }

        destroy() {
            instance._free(this.ptr);
        }
    }

    class CImage extends CObject {
        static read(id) {
           return  new CImage(instance._web_lire_fichier_image(id));
        }

        destroy() {
            instance._web_image_free(this.ptr);
        }

        get width() {
            return instance._web_image_width(this.ptr);
        }

        get height() {
            return instance._web_image_height(this.ptr);
        }
    }

    class CContour extends CObject {
        static from_image(image) {
            return new CContour(instance._web_contour(image.ptr));
        }

        get length() {
            return instance._web_contour_len(this.ptr);
        }

        get segments() {
            return this.length - 1;
        }

        get points() {
            const offset = instance._web_contour_points(this.ptr) / 8;
            const points = Array(this.length).fill(undefined);

            let i = 0;
            const ret = points.map(() => {
                return {
                    x: instance.HEAPF64[offset + (i++)],
                    y: instance.HEAPF64[offset + (i++)],
                };
            });

            // Cache
            Object.defineProperty(this, 'points', {
                writable: false,
                value: Object.freeze(ret),
            });

            return ret;
        }
    }

    function save_image(text_content) {
        FS.writeFile(`/${id}.pbm`,  text_content);
        return id++;
    }

    return {
        CImage,
        CContour,

        save_image,
    }
}

exports.default = init;

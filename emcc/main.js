const init = require('./map401_w').default;

const input_files = document.querySelector('#input-files');
/** @type {HTMLInputElement} */
const files = document.querySelector('input[type=file]');
const view = document.querySelector('#main');

const as = [];
const images = [];

const loaded_objects = [];

let locked = null;
function lock() {
    as.forEach(a => a.setAttribute('aria-disabled', true));
    locked = () => {
        as.forEach(a => a.setAttribute('aria-disabled', false));
    };
    return locked;
}

const C_TABLE = Symbol();

function struct(name, obj) {
    const props = Object.entries(obj).map(([key, value]) => {
        if (value === C_TABLE) {
            value = '<span style="color: gray">[...]</span>';
        } else {
            value = `<i>${value}</i>`;
        }

        return `&emsp;.<b>${key}</b> = ${value},<br>`;
    });

    return `<p class="struct">
    (<b>${name}</b>) {<br>
        ${props.join('')}
    }<br>
    </p>`;
}

function html(html) {
    return new DOMParser().parseFromString(html, 'text/html').body.firstChild;
}

async function main() {
    const { CImage, CContour, save_image } = await init();

    function add_image(name, text_content) {
        const node = document.createElement('a');
        const id = save_image(text_content);
        node.href = '#' + id;
        node.innerText = name;
        as.push(node);
        images[id] = name;
        input_files.insertBefore(node, files);
    }

    function load_image(image, id) {
        view.innerHTML = 'Chargement...';

        if (typeof image === 'number') {
            const unlock = lock();
            const i = CImage.read(image);
            unlock();
            return load_image(i, id);
        }

        const unlock = lock();

        for (const obj of loaded_objects) {
            obj.destroy();
        }
        loaded_objects.length = 0;

        loaded_objects.push(image);
        view.innerText = '';
        view.append(html(`<h1>${images[id]}</h1>`));
        view.append(html(`<section><h2><pre>image = lire_fichier_image()</pre></h2>${struct('Image', { L: image.width, H: image.height, tab: C_TABLE })}</section>`))

        const contour = CContour.from_image(image);
        loaded_objects.push(contour);
        view.append(html(`<section><h2><pre>contour(image)</pre></h2>${struct('Contour', { len: contour.length, inner: C_TABLE })}</section>`))

        unlock();
    }

    for (const [key, value] of Object.entries(BUNDLED_IMAGES)) {
        add_image(key, value);
    }

    files.onchange = async (_) => {
        const f = Array.from(files.files)[0];
        if (!f) return;

        add_image(f.name, '+' + await f.text());
    };

    window.onhashchange = (_) => {
        const hash = location.hash.substring(1);
        const id = parseInt(hash);
        load_image(id, id);
    }
}

main();

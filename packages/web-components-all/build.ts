import { promises as fs } from 'fs';

async function buildImportFromConfig(manifest: any) {
  const data = `const CONFIG = ${JSON.stringify(manifest)}; export default CONFIG;`;
  await fs.writeFile('dist/index.js', data);
}

async function buildDev() {
  /* Copy files */
  await fs.rm('dist', { recursive: true, force: true });
  await fs.cp('../web-components/dist/tree', 'dist', { recursive: true });

  /* Read config */
  const config = JSON.parse(await fs.readFile('dist/config.json', 'utf8'));

  /* Delete old config file */
  await fs.unlink('dist/config.json');

  /* Build import */
  await buildImportFromConfig({ dev: config });
}

async function buildProd() {
  throw new Error('Not implemented');
}

async function build() {
  /* Build */
  if (process.env.NODE_ENV === 'production') {
    await buildProd();
  } else {
    await buildDev();
  }
}

build().catch(err => {
  console.error(err);
  process.exit(1);
});

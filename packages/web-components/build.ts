import { promises as fs, statSync } from 'fs';
import path from 'path';

const WEB_COMPONENTS = ['gui'];

async function build() {
  const dst = path.resolve(__dirname, 'dist');
  await fs.rm(dst, { recursive: true, force: true });
  await fs.mkdir(dst, { recursive: true });

  let globalConfig = {};
  for (const component of WEB_COMPONENTS) {
    let config: {} | null = null;
    const src = path.resolve(__dirname, 'node_modules/@ootmm/', component, 'dist');
    const files = await fs.readdir(src, { recursive: true });
    const dirs = files.filter(file => statSync(path.resolve(src, file)).isDirectory());
    const filesNoDirs = files.filter(file => !statSync(path.resolve(src, file)).isDirectory());

    /* Make dir structure */
    for (const dir of dirs) {
      await fs.mkdir(path.resolve(dst, dir), { recursive: true });
    }

    /* Process files */
    for (const file of filesNoDirs) {
      const filePath = path.resolve(src, file);
      if (file === 'config.json') {
        config = JSON.parse(await fs.readFile(filePath, 'utf8'));
      } else {
        await fs.cp(filePath, path.resolve(dst, file), { force: true });
      }
    }

    if (config === null) {
      throw new Error('Config not found for component ' + component);
    }

    globalConfig = { ...globalConfig, ...config };
  }

  await fs.writeFile(path.resolve(dst, 'config.json'), JSON.stringify(globalConfig));
}

build().catch(err => {
  console.error(err);
  process.exit(1);
});

import { promises as fs } from 'fs';
import path from 'path';

async function build() {
  const versionFiles = await fs.readdir('tmp/configs');
  const config: any = {};
  for (const version of versionFiles) {
    const versionConfig = JSON.parse(await fs.readFile(path.resolve('tmp/configs', version), 'utf8'));
    const versionName = path.basename(version, '.json');
    config[versionName] = versionConfig;
  }

  await fs.writeFile(path.resolve(__dirname, 'tmp/tree/config.json'), JSON.stringify(config));
}

build().catch(err => {
  console.error(err);
  process.exit(1);
});

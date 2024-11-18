import { promises as fs } from 'fs';
import path from 'path';
import { createHash } from 'crypto';

async function run() {
  /* Move the entry point */
  const indexPath = path.resolve(import.meta.dirname, 'dist/index.html');
  const indexContent = await fs.readFile(indexPath, 'utf8');
  const digest = createHash('sha512');
  digest.update(indexContent);
  const hash = digest.digest('hex');
  const indexHashSmall = hash.slice(0, 16);
  const newIndexFilename = `index.generator-${indexHashSmall}.html`;
  const newIndexPath = path.resolve(import.meta.dirname, 'dist', newIndexFilename);
  await fs.rename(indexPath, newIndexPath);

  /* Create the config file */
  const cfg = { generator: { iframe: { path: newIndexFilename } } };
  await fs.writeFile(path.resolve(import.meta.dirname, 'dist/config.json'), JSON.stringify(cfg));
}

run().catch(err => {
  console.error(err);
  process.exit(1);
});

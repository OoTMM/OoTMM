import { makeRandoFileSystem } from './combo.new/file-system-builder';

async function run() {
  const fs = await makeRandoFileSystem();
  console.log(fs);
}

run().catch((err) => {
  console.error(err);
  process.exit(1);
});

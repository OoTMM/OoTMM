import { setupAssetsMap } from './setup/setup-assets-map';

export async function setup() {
  await setupAssetsMap();
}

setup().catch((error) => {
  console.error(error);
  process.exit(1);
});

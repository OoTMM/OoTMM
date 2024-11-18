import { defineConfig } from 'vite';
import preact from '@preact/preset-vite';
import mdx from '@mdx-js/rollup';
import MultipleAssets from 'vite-multiple-assets';

const env = process.env.NODE_ENV || 'development';
const STATIC_URL = process.env.STATIC_URL || '';
const isProd = env === 'production';
const assetsDirs = ['public/**'];

if (!isProd) {
  assetsDirs.push('node_modules/@ootmm/web-components-all/dist/**');
}

// https://vitejs.dev/config/
export default defineConfig({
  publicDir: false,
	plugins: [MultipleAssets(assetsDirs, { ssr: false }), preact(), mdx()],
  define: {
    'process.env.STATIC_URL': JSON.stringify(STATIC_URL),
  }
});

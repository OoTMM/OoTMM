import { defineConfig } from 'vite';
import preact from '@preact/preset-vite';
import { visualizer } from 'rollup-plugin-visualizer';

const VERSION = process.env.VERSION || 'XXX';
const VERSION_SUFFIX = process.env.VERSION_SUFFIX || '';
const VERSION_FULL = [VERSION, VERSION_SUFFIX].filter(Boolean).join('-');

const STATIC_URL = process.env.STATIC_URL || '/';

export default defineConfig({
  base: STATIC_URL,
  plugins: [preact(),visualizer({ open: true, filename: 'bundle-visualization.html' })],
  define: {
    'process.env.VERSION': JSON.stringify(VERSION_FULL),
    'process.env.__IS_BROWSER__': JSON.stringify(true),
    'process.env.DEBUG_LOG_LARGE_FILES': JSON.stringify(false),
  },
  resolve: {
    alias: {
      lodash: 'lodash-es',
    }
  },
  optimizeDeps: {
    exclude: ['yaz0']
  },
});

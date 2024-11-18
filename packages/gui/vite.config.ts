import { defineConfig } from 'vite';
import preact from '@preact/preset-vite';
import { visualizer } from 'rollup-plugin-visualizer';

const VERSION = process.env.VERSION || 'XXX';
const STATIC_URL = process.env.STATIC_URL || '/';

export default defineConfig({
  base: STATIC_URL,
  plugins: [
    preact(),
    visualizer({ open: true, filename: 'bundle-visualization.html' }),
  ],
  define: {
    'process.env.VERSION': JSON.stringify(VERSION),
    'process.env.BROWSER': JSON.stringify(true),
    global: 'globalThis',
  },
  resolve: {
    alias: {
      lodash: 'lodash-es',
    }
  },
});

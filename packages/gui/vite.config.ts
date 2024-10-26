import { defineConfig } from 'vite';
import preact from '@preact/preset-vite';
import { visualizer } from 'rollup-plugin-visualizer';

const VERSION = process.env.VERSION || 'XXX';

export default defineConfig({
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
  }
});

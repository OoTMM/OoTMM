import { defineConfig } from 'vite';
import preact from '@preact/preset-vite';
import { visualizer } from 'rollup-plugin-visualizer';

const NODE_ENV = process.env.NODE_ENV || 'development';
const VERSION = process.env.VERSION || 'XXX';
const isDev = (NODE_ENV !== 'production');

const baseUrl = isDev ? '/' : 'https://static.ootmm.com/';

export default defineConfig({
  base: baseUrl,
  plugins: [
    preact(),
    visualizer({ open: true, filename: 'bundle-visualization.html' }),
  ],
  define: {
    'process.env.NODE_ENV': JSON.stringify(NODE_ENV),
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

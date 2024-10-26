import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import { visualizer } from 'rollup-plugin-visualizer';

const VERSION = process.env.VERSION || 'XXX';

export default defineConfig({
  esbuild: {
    //jsxFactory: 'h',
    //jsxFragment: 'Fragment',
    //jsxInject: `import { h, Fragment } from 'preact'`,
  },
  plugins: [
    react(),
    visualizer({ open: true, filename: 'bundle-visualization.html' }),
  ],
  define: {
    'process.env.VERSION': JSON.stringify(VERSION),
    'process.env.BROWSER': JSON.stringify(true),
    global: 'globalThis',
  },
  resolve: {
    alias: {
      react: 'preact/compat',
      'react-dom': 'preact/compat',
      lodash: 'lodash-es',
    }
  }
});

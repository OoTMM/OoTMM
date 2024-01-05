import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import { nodePolyfills } from 'vite-plugin-node-polyfills';

const VERSION = process.env.VERSION || 'XXX';

export default defineConfig({
  plugins: [
    react(),
    nodePolyfills(),
  ],
  define: {
    'process.env.VERSION': JSON.stringify(VERSION),
    'process.env.BROWSER': JSON.stringify(true),
  }
})

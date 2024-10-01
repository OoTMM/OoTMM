import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';

const VERSION = process.env.VERSION || 'XXX';

export default defineConfig({
  plugins: [
    react(),
  ],
  define: {
    'process.env.VERSION': JSON.stringify(VERSION),
    'process.env.BROWSER': JSON.stringify(true),
    global: 'globalThis',
  },
})

import { defineConfig } from 'vitest/config';

export default defineConfig({
  test: {
    testTimeout: 15 * 60 * 1000,
  },
})

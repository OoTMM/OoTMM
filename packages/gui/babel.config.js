'use strict';

module.exports = {
  presets: [
    '@babel/preset-react',
    ['@babel/preset-env', {
      corejs: 3,
      useBuiltIns: 'usage',
    }],
    '@babel/preset-typescript',
  ],
  plugins: ['macros'],
};

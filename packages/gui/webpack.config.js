'use strict';

const webpack = require('webpack');
const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const CopyWebpackPlugin = require('copy-webpack-plugin');
const MiniCssExtractPlugin = require("mini-css-extract-plugin");

const env = process.env.NODE_ENV || 'development';
const dev = env === 'development';

const VERSION = process.env.VERSION || "XXX";

module.exports = {
  mode: dev ? 'development' : 'production',
  entry: './app/index.ts',
  output: {
    path: path.resolve(__dirname, 'dist'),
    publicPath: '/',
    filename: dev ? 'app.js' : 'app.[contenthash].min.js',
  },
  module: {
    rules: [
      {
        test: /\.(j|t)sx?$/,
        exclude: /node_modules/,
        use: 'babel-loader'
      },
      {
        test: /\.css$/,
        use: [
          dev ? 'style-loader' : MiniCssExtractPlugin.loader,
          'css-loader',
          'postcss-loader'
        ]
      },
      {
        test: /\.png$/,
        use: [
          'file-loader',
        ]
      }
    ],
  },
  resolve: {
    extensions: ['.js', '.jsx', '.ts', '.tsx'],
    fallback: {
      buffer: require.resolve('buffer/'),
      crypto: require.resolve('crypto-browserify'),
      stream: require.resolve('stream-browserify'),
    },
  },
  plugins: [
    new webpack.DefinePlugin({
      'process.env.NODE_ENV': JSON.stringify(env),
      'process.env.VERSION': JSON.stringify(VERSION),
    }),
    new CopyWebpackPlugin({
      patterns: [
        { from: `${path.dirname(require.resolve('@ootmm/core/package.json'))}/dist/data.zip`, to: `data-${VERSION}.zip` },
      ]
    }),
    new MiniCssExtractPlugin({
      filename: dev ? 'app.css' : 'app.[contenthash].min.css',
    }),
    new webpack.ProvidePlugin({
      Buffer: ['buffer', 'Buffer']
    }),
    new HtmlWebpackPlugin({
      template: './app/index.html'
    }),
  ]
};

import './register';

import fs from 'fs/promises';
import path from 'path';
import rollup from 'rollup';
import typescript from '@rollup/plugin-typescript';
import jsonPlugin from '@rollup/plugin-json';
import replace from '@rollup/plugin-replace';
import yamlPlugin from '@rollup/plugin-yaml';
import dsvPlugin from '@rollup/plugin-dsv';
import externals from 'rollup-plugin-node-externals';
import terser from '@rollup/plugin-terser';
import glob from 'glob';

import { build as comboBuild } from './combo/build';
import { codegen as comboCodegen } from './combo/codegen';
import { customAssets } from './combo/custom';
import { Monitor } from './combo/monitor';
import { DEFAULT_SETTINGS } from './combo';
import { DEFAULT_COSMETICS } from './combo/cosmetics';

const VERSION = process.env.VERSION || 'XXX';

async function build() {
  const inputOptions = {
    input: 'lib/combo/index.ts',
    plugins: [
      replace({
        preventAssignment: true,
        'process.env.NODE_ENV': JSON.stringify('production'),
        'process.env.ROLLUP': JSON.stringify(true),
        'process.env.VERSION': JSON.stringify(VERSION),
      }),
      typescript({ tsconfig: './tsconfig.json', declaration: true, declarationDir: 'dist' }),
      externals({ builtinsPrefix: 'strip' }),
      jsonPlugin(),
      yamlPlugin(),
      dsvPlugin({
        processRow: (row) => {
          const data: any = {};
          Object.keys(row).forEach((key) => {
            const value = row[key];
            data[key.trim()] = (value || "").trim();
          });
          return data;
        }
      }),
      terser(),
    ],
  };
  const outputOptions = {
    file: 'dist/index.node.min.js',
    format: 'es',
  } as const;
  const bundle = await rollup.rollup(inputOptions);
  await bundle.write(outputOptions);
}

async function copyData() {
  await comboCodegen(dummyMonitor);
  const b = await comboBuild({ debug: false, seed: 'ROLLUP', settings: DEFAULT_SETTINGS, cosmetics: DEFAULT_COSMETICS });
  await fs.mkdir('dist/data', { recursive: true });
  await Promise.all(
    Object.entries(b).map(async ([game, { payload, patches }]) => {
      await Promise.all([
        fs.writeFile(`dist/data/${game}_payload.bin`, payload),
        fs.writeFile(`dist/data/${game}_patch.bin`, patches),
      ]);
    })
  );

  /* Copy the extra assets */
  await customAssets();
  let promises: Promise<void>[] = [];
  glob.sync('build/assets/*.bin').forEach((filename) => {
    promises.push(fs.copyFile(filename, `dist/data/${path.basename(filename)}`));
  });
  glob.sync('data/static/*.bin').forEach((filename) => {
    promises.push(fs.copyFile(filename, `dist/data/${path.basename(filename)}`));
  });
  await Promise.all(promises);
}

const dummyMonitor = new Monitor({});

build().then(copyData).catch((err) => {
  console.error(err);
  process.exit(1);
});

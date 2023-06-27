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
import JSZip from 'jszip';

import { build as comboBuild } from './combo/build';
import { codegen as comboCodegen } from './combo/codegen';
import { customAssets } from './combo/custom';
import { Monitor } from './combo/monitor';
import { DEFAULT_SETTINGS } from './combo';
import { DEFAULT_COSMETICS, cosmeticsAssets } from './combo/cosmetics';
import { DEFAULT_RANDOM_SETTINGS } from './combo/settings/random';

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
  const opts = {} as any;
  await Promise.all([
    comboCodegen(dummyMonitor),
    cosmeticsAssets(opts),
  ]);
  const b = await comboBuild({ debug: false, seed: 'ROLLUP', settings: DEFAULT_SETTINGS, cosmetics: DEFAULT_COSMETICS, random: DEFAULT_RANDOM_SETTINGS });

  /* Create the zip */
  const zip = new JSZip();

  /* Add the payload and the patches */
  await Promise.all(
    Object.entries(b).map(async ([game, { payload, patches }]) => {
      zip.file(`${game}_payload.bin`, payload);
      zip.file(`${game}_patch.bin`, patches);
    })
  );

  /* Add the extra assets */
  await customAssets(opts);
  for (const basePath of ["build/assets", "data/static"]) {
    const matches = glob.sync(['**/*.bin', '**/*.zobj'], { cwd: basePath });
    for (const filename of matches) {
      const data = await fs.readFile(path.join(basePath, filename));
      zip.file(filename, data);
    }
  }

  /* Compress the data */
  const zipBuf = await zip.generateAsync({ type: 'nodebuffer', compression: 'DEFLATE', compressionOptions: { level: 9 } });
  await fs.writeFile('dist/data.zip', zipBuf);
}

const dummyMonitor = new Monitor({});

build().then(copyData).catch((err) => {
  console.error(err);
  process.exit(1);
});

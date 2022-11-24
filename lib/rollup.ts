import './register';

import fs from 'fs/promises';
import rollup from 'rollup';
import typescript from '@rollup/plugin-typescript';
import jsonPlugin from '@rollup/plugin-json';
import replace from '@rollup/plugin-replace';
import yamlPlugin from '@rollup/plugin-yaml';
import dsvPlugin from '@rollup/plugin-dsv';
import externals from 'rollup-plugin-node-externals';
import terser from '@rollup/plugin-terser';

import { build as comboBuild } from './combo/build';
import { decompressGames as comboDecompress } from './combo/decompress';
import { codegen as comboCodegen } from './combo/codegen';
import { custom as comboCustom } from './combo/custom';
import { Monitor } from './combo/monitor';

async function build() {
  const inputOptions = {
    input: 'lib/combo/index.ts',
    plugins: [
      replace({
        preventAssignment: true,
        'process.env.NODE_ENV': JSON.stringify('production'),
        'process.env.ROLLUP': JSON.stringify(true),
      }),
      typescript(),
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
  const b = await comboBuild({});
  await fs.mkdir('dist/data', { recursive: true });
  await Promise.all(
    Object.entries(b).map(async ([game, { payload, patches }]) => {
      await Promise.all([
        fs.writeFile(`dist/data/${game}_payload.bin`, payload),
        fs.writeFile(`dist/data/${game}_patch.bin`, patches),
      ]);
    })
  );
}

const dummyMonitor = new Monitor({});

build().then(copyData).catch((err) => {
  console.error(err);
  process.exit(1);
});

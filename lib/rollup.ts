import fs from 'fs/promises';
import rollup from 'rollup';
import typescript from '@rollup/plugin-typescript';
import jsonPlugin from '@rollup/plugin-json';
import replace from '@rollup/plugin-replace';
import yamlPlugin from '@rollup/plugin-yaml';
import alias from '@rollup/plugin-alias';
import externals from 'rollup-plugin-node-externals';

import { build as comboBuild } from './combo/build';

async function build() {
  const inputOptions = {
    input: 'lib/combo/index.ts',
    plugins: [
      replace({
        preventAssignment: true,
        'process.env.ROLLUP': JSON.stringify(true),
      }),
      typescript(),
      alias({
        entries: { find: './data', replacement: './data-bundler' },
      }),
      externals(),
      jsonPlugin(),
      yamlPlugin(),
    ],
  };
  const outputOptions = {
    file: 'dist/ootmm.js',
    format: 'cjs',
  } as const;
  const bundle = await rollup.rollup(inputOptions);
  await bundle.write(outputOptions);
}

async function copyData() {
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

build().then(copyData).catch((err) => {
  console.error(err);
  process.exit(1);
});

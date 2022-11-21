import rollup from 'rollup';
import typescript from '@rollup/plugin-typescript';
import { nodeResolve } from '@rollup/plugin-node-resolve';
import commonjs from '@rollup/plugin-commonjs';
import nativePlugin from 'rollup-plugin-natives';
import jsonPlugin from '@rollup/plugin-json';
import replace from '@rollup/plugin-replace';
import yamlPlugin from '@rollup/plugin-yaml';
import alias from '@rollup/plugin-alias';

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
      commonjs(),
      nodeResolve(),
      jsonPlugin(),
      yamlPlugin(),
      nativePlugin({
        copyTo: 'dist/node',
        destDir: './node'
      }),
    ],
  };
  const outputOptions = {
    file: 'dist/ootmm.js',
    format: 'cjs',
  } as const;
  const bundle = await rollup.rollup(inputOptions);
  await bundle.write(outputOptions);
}

build().catch((err) => {
  console.error(err);
  process.exit(1);
});

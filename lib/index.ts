import { spawn } from 'child_process';
import { decompressGames } from './decompress';
import { custom } from './custom';
import { pack } from './pack';
import { codegen } from './codegen';
import { makeOptions, Options } from './options';

const build = async (opts: Options) => {
  return new Promise((resolve, reject) => {
    const args = ['-j', '32'];
    if (opts.debug) {
      args.push('DEBUG=1');
    }
    const proc = spawn('make', args, { stdio: 'inherit' });
    proc.on('close', (code) => {
      if (code === 0) {
        resolve(null);
      } else {
        reject(new Error(`make exited with code ${code}`));
      }
    });
  });
};

const run = async (opts: Options) => {
  await decompressGames();
  await codegen();
  await custom();
  await build(opts);
  await pack(opts);
};

const options = makeOptions(process.argv.slice(2));
run(options);

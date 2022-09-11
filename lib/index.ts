import { spawn } from 'child_process';
import { decompressGames } from './decompress';
import { custom } from './custom';
import { pack } from './pack';

const build = async () => {
  return new Promise((resolve, reject) => {
    const proc = spawn('make', ['-j', '32'], { stdio: 'inherit' });
    proc.on('close', (code) => {
      if (code === 0) {
        resolve(null);
      } else {
        reject(new Error(`make exited with code ${code}`));
      }
    });
  });
};

const run = async () => {
  await decompressGames();
  await custom();
  await build();
  await pack();
};

run();

import { spawn } from "child_process";
import { Options } from "./options";

export const build = async (opts: Options) => {
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

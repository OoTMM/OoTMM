import { promises as fs } from 'fs';
import path from 'path';

import { GAMES } from '../combo/config';
import { RandoFileSystem } from './file-system';

export class RandoFileSystemBuilder {
  async run() {
    const rfs = new RandoFileSystem();
    const dir = path.resolve(__dirname, '..', '..', 'node_modules', '@ootmm', 'game', 'dist');
    let gameId = 0;
    for (const game of GAMES) {
      gameId++;
      const metadata = JSON.parse(await fs.readFile(path.join(dir, `${game}.json`), 'utf8'));
      const romBuffer = await fs.readFile(path.join(dir, `${game}.z64`));
      const rom = new Uint8Array(romBuffer.buffer, romBuffer.byteOffset, romBuffer.byteLength);
      const files = metadata.files;
      let fileId = 0;
      for (const f of files) {
        const name = `${game}/${f.name}`;
        const data = rom.slice(f.start, f.start + f.size);
        const id = (gameId << 24 | fileId) >>> 0;
        fileId++;
        rfs.addFile({ id, name, data });
      }

      /* Metadata */
      rfs.meta.loaderOffsets.configs = metadata.loaderOffsets.configs;
      rfs.meta.loaderOffsets.file = metadata.loaderOffsets.file;
      rfs.meta.games[game].bootproc = metadata.game.bootproc;
      rfs.meta.games[game].ram = metadata.game.ram;
    }

    /* Make the loader & makerom unique */
    rfs.deleteFile('mm/makerom');
    rfs.deleteFile('mm/loader');

    const makerom = rfs.findFile('oot/makerom')!;
    const loader = rfs.findFile('oot/loader')!;
    makerom.name = 'makerom';
    loader.name = 'loader';
    makerom.id = 0;
    loader.id = 1;

    return rfs;
  }
}

export async function makeRandoFileSystem() {
  const fsb = new RandoFileSystemBuilder();
  return fsb.run();
}

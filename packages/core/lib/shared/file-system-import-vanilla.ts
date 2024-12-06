import * as Yaz0 from 'yaz0';
import { FILES } from '@ootmm/data';

import { CONFIG, GAMES } from '../combo/config';
import { GameFile, GameFileSystem, GameFileType } from './file-system';
import { bufReadU32BE } from '../combo/util/buffer';

export async function makeVanillaFileSystem(roms: { oot: Uint8Array, mm: Uint8Array }): Promise<GameFileSystem> {
  const fileSystem = new GameFileSystem();
  const files: GameFile[] = [];

  for (const game of GAMES) {
    /* Fetch relevant config and rom */
    const config = CONFIG[game];
    const rom = roms[game];
    const filenames = FILES[game];

    /* Extract the DMA table */
    const dma = rom.subarray(config.dmaAddr, config.dmaAddr + config.dmaCount * 16);
    for (let i = 0; i < config.dmaCount; ++i) {
      if (i === 2) {
        /* Skip DMA Data */
        continue;
      }

      const dmaEntry = dma.subarray(i * 16, i * 16 + 16);
      const filename = `${game}/${filenames[i]}`;
      let data: Uint8Array;
      let type: GameFileType;

      /* Fetch the DMA fields */
      const virtStart = bufReadU32BE(dmaEntry, 0x00);
      const virtEnd = bufReadU32BE(dmaEntry, 0x04);
      const physStart = bufReadU32BE(dmaEntry, 0x08);
      const physEnd = bufReadU32BE(dmaEntry, 0x0c);

      /* Handle dummy file */
      if (physStart === 0xffffffff) {
        type = 'dummy';
        data = new Uint8Array(0);
      } else if (physEnd === 0) {
        type = 'uncompressed';
        data = rom.slice(physStart, physStart + virtEnd - virtStart);
      } else {
        type = 'compressed';
        data = rom.subarray(physStart, physEnd);
      }

      files.push({ name: filename, type, data });
    }
  }

  /* Actually decompress the compressed files */
  await Promise.all(files.map(async (file) => {
    if (file.type === 'compressed') {
      file.data = await Yaz0.decompress(file.data);
    }
  }));

  /* Add the files to the file system */
  for (const file of files) {
    fileSystem.addRawFile(file);
  }

  return fileSystem;
}

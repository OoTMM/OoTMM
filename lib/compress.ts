import fs from 'fs/promises';
import path from 'path';
import { createYaz0Stream } from 'yaz0';

import { Game, CONFIG, PATH_BUILD } from './config';
import { DmaData } from './dma';

const compressFiles = async (rom: Buffer, dmaOld: DmaData, dmaNew: DmaData) => {
  const promises: Promise<Buffer>[] = [];
  for (let i = 0; i < dmaNew.count(); ++i) {
    const entryOld = dmaOld.read(i);
    const entryNew = dmaNew.read(i);

    /* Skip dummy files */
    if (entryOld.physEnd === 0xffffffff) {
      promises.push(Promise.resolve(Buffer.alloc(0)));
      continue;
    }

    /* Don't compress unless the file was compressed in the commercial ROM */
    if (entryOld.physEnd === 0) {
      promises.push(Promise.resolve(rom.subarray(entryOld.virtStart, entryOld.virtEnd)));
      continue;
    };

    /* We need to compress */
    promises.push(new Promise((resolve, reject) => {
      const stream = createYaz0Stream('compress', { size: entryOld.virtEnd - entryOld.virtStart });
      const chunks: Buffer[] = [];
      stream.on('data', (chunk) => chunks.push(chunk));
      stream.on('end', () => resolve(Buffer.concat(chunks)));
      stream.on('error', reject);
      stream.push(rom.subarray(entryOld.virtStart, entryOld.virtEnd));
      stream.push(null);
    }));
  }
  return Promise.all(promises);
};

export const compressGame = async (game: Game, rom: Buffer) => {
  console.log("Compressing " + game + "...");

  const conf = CONFIG[game];
  const dmaOld = new DmaData(await fs.readFile(path.resolve(PATH_BUILD, 'roms', `${game}_dma.bin`)));
  const dmaNew = new DmaData(Buffer.from(rom.subarray(conf.dmaAddr, conf.dmaAddr + conf.dmaCount * 16)));
  const newRom = Buffer.alloc(32 * 1024 * 1024);
  const files = await compressFiles(rom, dmaOld, dmaNew);

  let paddr = 0;
  for (let i = 0; i < dmaNew.count(); ++i) {
    const entryOld = dmaOld.read(i);
    const entryNew = dmaNew.read(i);

    /* Skip dummy files */
    if (entryOld.physEnd === 0xffffffff) {
      continue;
    }

    /* Copy the file */
    const f = files[i];
    const size = f.length;
    const sizeAligned = (size + 0xf) & ~0xf;
    f.copy(newRom, paddr);
    if (entryOld.physEnd === 0) {
      /* Uncompressed file */
      entryNew.physStart = paddr;
      entryNew.physEnd = 0;
    } else {
      /* Compressed file */
      entryNew.physStart = paddr;
      entryNew.physEnd = paddr + sizeAligned;
    }
    paddr += sizeAligned;
    dmaNew.write(i, entryNew);
  }

  /* Copy the DMA table */
  dmaNew.data().copy(newRom, conf.dmaAddr);

  return rom;
};

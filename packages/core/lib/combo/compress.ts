import fs from 'fs/promises';
import path from 'path';
import crypto from 'crypto';
import { Buffer } from 'buffer';

import Yaz0 from 'yaz0';

import { Game, CONFIG, GAMES } from './config';
import { DmaData } from './dma';
import { fileExists } from './util';
import { Monitor } from './monitor';
import { DecompressedRoms } from './decompress';

function fsRetry<T>(fn: () => Promise<T>, retries = 5): Promise<T> {
  return fn().catch((err) => {
    if (retries > 0) {
      return new Promise((reesolve, reject) => {
        setTimeout(() => {
          fsRetry(fn, retries - 1).then(reesolve, reject);
        }, 100);
      });
    }
    throw err;
  });
}

export const compressFile = async (data: Buffer): Promise<Buffer> => {
  let filename = "";

  if (!process.env.ROLLUP) {
    const hash = crypto.createHash('sha256').update(data).digest('hex');
    const dir = path.resolve('build', 'cache', 'yaz0', hash.slice(0, 2));
    filename = path.resolve(dir, hash);

    /* Check for the file in cache */
    await fs.mkdir(dir, { recursive: true });
    if (await fileExists(filename)) {
      return fsRetry(() => fs.readFile(filename));
    }
  }

  /* Cache miss - compress */
  const compressed = await Yaz0.compress(data, 7);
  if (!process.env.ROLLUP) {
    await fsRetry(() => fs.writeFile(filename, compressed));
  }
  return compressed;
};

const compressFiles = async (rom: Buffer, dmaOld: DmaData) => {
  const promises: Promise<Buffer>[] = [];
  for (let i = 0; i < dmaOld.count(); ++i) {
    const entryOld = dmaOld.read(i);

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
    promises.push(compressFile(rom.subarray(entryOld.virtStart, entryOld.virtEnd)));
  }
  return Promise.all(promises);
};

const compressGame = async (game: Game, rom: Buffer, dma: Buffer) => {
  const conf = CONFIG[game];
  const dmaOld = new DmaData(Buffer.from(dma));
  const dmaNew = new DmaData(Buffer.from(rom.subarray(conf.dmaAddr, conf.dmaAddr + conf.dmaCount * 16)));
  const newRom = Buffer.alloc(32 * 1024 * 1024);
  const files = await compressFiles(rom, dmaOld);

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

  return newRom;
};

export async function compress(monitor: Monitor, roms: DecompressedRoms) {
  return Promise.all(GAMES.map((game) => {
    const { rom, dma } = roms[game];
    return compressGame(game, rom, dma);
  }));
}

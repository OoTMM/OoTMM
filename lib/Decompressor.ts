import { createYaz0Stream } from 'yaz0';
import { Readable } from 'stream';
import fs from 'fs/promises';
import path from 'path';

import { DmaData } from './DmaData';
import { CONFIG, Game, GAMES, PATH_BUILD, PATH_ROMS } from './Config';

export const copyFile = async (src: Buffer, dst: Buffer, compressed: boolean) => {
  let stream = Readable.from(src);
  if (compressed) {
    stream = stream.pipe(createYaz0Stream('decompress'));
  }
  let offset = 0;
  for await (const chunk of stream) {
    chunk.copy(dst, offset);
    offset += chunk.length;
  }
  return null;
};

export const decompressGame = async (game: Game, rom: Buffer): Promise<Buffer> => {
  const conf = CONFIG[game];
  const dma = new DmaData(Buffer.from(rom.subarray(conf.dmaAddr, conf.dmaAddr + conf.dmaCount * 16)));
  const out = Buffer.alloc(64 * 1024 * 1024);
  const promises: Promise<null>[] = [];

  for (let i = 0; i < dma.count(); ++i) {
    const record = dma.read(i);
    let physEnd = record.physEnd;
    let compressed = true;
    if (physEnd === 0xffffffff) {
      continue;
    }
    if (physEnd === 0) {
      physEnd = record.physStart + (record.virtEnd - record.virtStart);
      compressed = false;
    }
    dma.write(i, { virtStart: record.virtStart, virtEnd: record.virtEnd, physStart: record.virtStart, physEnd: 0 });
    const src = rom.subarray(record.physStart, physEnd);
    const dst = out.subarray(record.virtStart, record.virtEnd);
    promises.push(copyFile(src, dst, compressed));
  }

  await Promise.all(promises);

  dma.data().copy(out, conf.dmaAddr);

  return out;
};

export const decompressGames = async () => {
  await fs.mkdir(path.resolve(PATH_BUILD, 'roms'), { recursive: true });
  for (const g of GAMES) {
    console.log("Decompressing " + g + "...");
    const rom = await fs.readFile(path.resolve(PATH_ROMS, `${g}.z64`));
    const out = await decompressGame(g, rom);
    await fs.writeFile(path.resolve(PATH_BUILD, 'roms', `${g}_decompressed.z64`), out);
  }
};

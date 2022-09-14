import { Readable } from 'stream';
import fs from 'fs/promises';
import path from 'path';
import { createYaz0Stream } from 'yaz0';

import { DmaData } from './dma';
import { CONFIG, Game, GAMES, PATH_BUILD, PATH_ROMS } from './config';
import { fileExists } from './util';

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

type DecompressedGame = {
  rom: Buffer,
  dma: Buffer,
};

export const decompressGame = async (game: Game, rom: Buffer): Promise<DecompressedGame> => {
  const conf = CONFIG[game];
  const dmaBuffer = Buffer.from(rom.subarray(conf.dmaAddr, conf.dmaAddr + conf.dmaCount * 16));
  const dma = new DmaData(Buffer.from(dmaBuffer));
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

  return { rom: out, dma: dmaBuffer };
};

export const decompressGames = async () => {
  const outDir = path.resolve(PATH_BUILD, 'roms');
  await fs.mkdir(outDir, { recursive: true });
  for (const g of GAMES) {
    const stampPath = path.resolve(outDir, `${g}.stamp`);
    if (await fileExists(stampPath))
      continue;
    console.log("Decompressing " + g + "...");
    const f = await fs.readFile(path.resolve(PATH_ROMS, `${g}.z64`));
    const { rom, dma } = await decompressGame(g, f);
    await Promise.all([
      fs.writeFile(path.resolve(outDir, `${g}_decompressed.z64`), rom),
      fs.writeFile(path.resolve(outDir, `${g}_dma.bin`), dma),
    ]);
    await fs.writeFile(stampPath, "");
  }
};

import { Readable } from 'stream';
import fs from 'fs/promises';
import crypto from 'crypto';
import Yaz0 from 'yaz0';

import { DmaData } from './dma';
import { CONFIG, Game, GAMES } from './config';

export const copyFile = async (src: Buffer, dst: Buffer, compressed: boolean) => {
  if (compressed) {
    src = await Yaz0.decompress(src);
  }
  src.copy(dst);
};

type DecompressedGame = {
  rom: Buffer,
  dma: Buffer,
};

const checkGameHash = (game: Game, rom: Buffer) => {
  const h = crypto.createHash('sha256').update(rom).digest('hex');
  const hashes = CONFIG[game].sha256;
  if (!hashes.includes(h)) {
    throw new Error(`Bad hash for ${game}, got ${h}`);
  }
};

export const decompressGame = async (game: Game, rom: Buffer): Promise<DecompressedGame> => {
  checkGameHash(game, rom);
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

type DecompressGamesParams = {
  oot: string;
  mm: string;
};

export type DecompressedRoms = {
  oot: DecompressedGame;
  mm: DecompressedGame;
};

export const decompressGames = async (params: DecompressGamesParams): Promise<DecompressedRoms> => {
  console.log("Decompressing...");
  const [oot, mm] = await Promise.all(GAMES.map(async (g) => {
    const f = await fs.readFile(params[g]);
    const { rom, dma } = await decompressGame(g, f);
    return { rom, dma };
  }));
  return { oot, mm };
};

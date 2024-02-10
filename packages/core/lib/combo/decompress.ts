import Yaz0 from 'yaz0';
import CRC32 from 'crc-32';

import { DmaData } from './dma';
import { CONFIG, Game, GAMES } from './config';
import { Monitor } from './monitor';

export const copyFile = async (src: Buffer, dst: Buffer, compressed: boolean) => {
  if (compressed) {
    src = await Yaz0.decompress(src);
  }
  src.copy(dst);
};

export type DecompressedGame = {
  rom: Buffer,
  dma: Buffer,
};

const swapIfNecessary = (rom: Buffer) => {
  const sig = rom.readUInt32BE(0) >>> 0;
  let swap32 = false;
  let swap16 = false;
  switch (sig) {
  case 0x80371240:
    /* Big endian - no further work needed */
    break;
  case 0x40123780:
    /* Little endian - swap 32-bit words */
    swap32 = true;
    break;
  case 0x37804012:
    /* Mixed endian - swap 16-bit words */
    swap16 = true;
    break;
  case 0x12400387:
    /* Mixed endian - swap 16-bit words and 32-bit words */
    swap32 = true;
    swap16 = true;
    break;
  default:
    throw new Error(`Bad n64 rom signature ${sig.toString(16)}`);
  }
  if (swap32) {
    for (let i = 0; i < rom.length; i += 4) {
      rom.writeUInt32BE(rom.readUInt32LE(i), i);
    }
  }
  if (swap16) {
    for (let i = 0; i < rom.length; i += 2) {
      rom.writeUInt16BE(rom.readUInt16LE(i), i);
    }
  }
};

const checkGameHash = (game: Game, rom: Buffer) => {
  const h = CRC32.buf(rom, 0) >>> 0;
  const hashes = CONFIG[game].crc32;
  if (!hashes.includes(h)) {
    let romInfo = '';
    if (game == 'oot') {
      romInfo = 'For OOT, use a ROM with version 1.0, U or J.';
    } else if (game == 'mm') {
      romInfo = 'For MM, use a ROM with version U.';
    }
    throw new Error(`Incompatible ROM file for ${game} (hash: ${h.toString(16)}). ${romInfo}`);
  }
};

export const decompressGame = async (game: Game, rom: Buffer): Promise<DecompressedGame> => {
  const conf = CONFIG[game];
  const dmaBuffer = Buffer.from(rom.subarray(conf.dmaAddr, conf.dmaAddr + conf.dmaCount * 16));
  const dma = new DmaData(Buffer.from(dmaBuffer));
  const out = Buffer.alloc(64 * 1024 * 1024);
  const promises: Promise<void>[] = [];

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
  oot: Buffer;
  mm: Buffer;
};

export type DecompressedRoms = {
  oot: DecompressedGame;
  mm: DecompressedGame;
};

export const decompressGames = async (monitor: Monitor, params: DecompressGamesParams): Promise<DecompressedRoms> => {
  for (const g of GAMES) {
    monitor.log("Validating " + g);
    swapIfNecessary(params[g]);
    checkGameHash(g, params[g]);
  }
  monitor.log("Decompressing");
  const [oot, mm] = await Promise.all(GAMES.map(async (g) => {
    const f = params[g];
    const { rom, dma } = await decompressGame(g, f);
    return { rom, dma };
  }));
  return { oot, mm };
};

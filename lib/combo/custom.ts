import path from 'path';
import fs from 'fs/promises';
import { Buffer } from 'buffer';

import { Game, DATA_FILES, CUSTOM_ADDR } from './config';
import { DmaData } from './dma';
import { splitObject } from './split';
import { align, arrayToIndexMap } from './util';
import { compressFile } from './compress';
import { CodeGen } from './codegen';
import { DecompressedRoms } from './decompress';
import { Monitor } from './monitor';

const FILES_TO_INDEX_OOT = arrayToIndexMap(DATA_FILES.oot);
const FILES_TO_INDEX_MM = arrayToIndexMap(DATA_FILES.mm);
const FILES_TO_INDEX = {
  oot: FILES_TO_INDEX_OOT,
  mm: FILES_TO_INDEX_MM,
};

type CustomEntry = {
  game: Game,
  name: string,
  file: string,
  offsets: number[],
};

const ENTRIES: CustomEntry[] = [
  { game: 'mm',  name: "GI_REMAINS_ODOLWA",     file: "objects/object_bsmask",       offsets: [0x0690] },
  { game: 'mm',  name: "GI_REMAINS_GOHT",       file: "objects/object_bsmask",       offsets: [0x3ad0] },
  { game: 'mm',  name: "GI_REMAINS_GYORG",      file: "objects/object_bsmask",       offsets: [0x1d80] },
  { game: 'mm',  name: "GI_REMAINS_TWINMOLD",   file: "objects/object_bsmask",       offsets: [0x5020] },
  { game: 'oot', name: "GI_MASTER_SWORD",       file: "objects/object_toki_objects", offsets: [0x1bd0] },
];

const getObjectBuffer = async (roms: DecompressedRoms, entry: CustomEntry) => {
  const rom = roms[entry.game].rom;
  const dma = new DmaData(roms[entry.game].dma);
  const index = FILES_TO_INDEX[entry.game][entry.file];
  if (index === undefined) {
    throw new Error(`File ${entry.file} not found in game ${entry.game}`);
  }
  const dmaEntry = dma.read(index);
  return Buffer.from(rom.subarray(dmaEntry.virtStart, dmaEntry.virtEnd));
};

/* TODO: Cache this */
const makeSplitObject = async (roms: DecompressedRoms, entry: CustomEntry) => {
  const buf = await getObjectBuffer(roms, entry);
  const obj = splitObject(buf, entry.offsets);

  if (!process.env.ROLLUP) {
    const outDir = path.resolve('build', 'custom');
    const outBasename = entry.name.toLowerCase();
    const outFilename = path.resolve(outDir, `${outBasename}.zobj`);
    await fs.mkdir(outDir, { recursive: true });
    await fs.writeFile(outFilename, obj.data);
  }

  return obj;
};

export const custom = async (monitor: Monitor, roms: DecompressedRoms) => {
  monitor.log("Building custom objects");
  const cgPath = process.env.ROLLUP ? '' : path.resolve('build', 'include', 'combo', 'custom.h');
  const cg = new CodeGen(cgPath, 'CUSTOM_H');
  const objects = await Promise.all(ENTRIES.map(x => makeSplitObject(roms, x)));
  const objectDmaBuffer = Buffer.alloc(0x10 * objects.length);
  const objectDma = new DmaData(objectDmaBuffer);
  const objTable = Buffer.alloc(0x08 * objects.length);
  let vaddr = 0x08000000;
  let paddr = CUSTOM_ADDR;
  const data: Buffer[] = [];
  for (let i = 0; i < objects.length; ++i) {
    const obj = objects[i];
    const objEntry = ENTRIES[i];
    const objCompressed = await compressFile(obj.data);
    const objCompressedSize = align(objCompressed.byteLength, 0x10);
    const virtStart = vaddr;
    const virtEnd = vaddr + obj.data.byteLength;
    const physStart = paddr;
    const physEnd = paddr + objCompressedSize;
    objectDma.write(i, { virtStart, virtEnd, physStart, physEnd });
    objTable.writeUInt32BE(virtStart, i * 8 + 0);
    objTable.writeUInt32BE(virtEnd, i * 8 + 4);
    data.push(objCompressed);
    if (objCompressed.byteLength !== objCompressedSize) {
      data.push(Buffer.alloc(objCompressedSize - objCompressed.byteLength));
    }
    vaddr = virtEnd;
    paddr += objCompressedSize;
    cg.define(['CUSTOM_OBJECT_ID', objEntry.name].join('_'), 0x2000 | i);
    for (let j = 0; j < obj.offsets.length; ++j) {
      const offset = obj.offsets[j];
      cg.define(['CUSTOM_OBJECT', objEntry.name, j].join('_'), offset);
    }
  }
  cg.define('CUSTOM_DMA_ADDR', paddr);
  cg.define('CUSTOM_DMA_SIZE', objects.length);
  data.push(objectDmaBuffer);
  paddr += objectDmaBuffer.byteLength;
  cg.define('CUSTOM_OBJECTS_ADDR', paddr);
  cg.define('CUSTOM_OBJECTS_SIZE', objects.length);
  data.push(objTable);

  if (!process.env.ROLLUP) {
    await cg.emit();
  }

  return Buffer.concat(data);
};

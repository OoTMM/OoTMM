import path from 'path';
import fs from 'fs/promises';

import { Game, PATH_BUILD, DATA_FILES, CUSTOM_ADDR } from './config';
import { DmaData } from './dma';
import { splitObject } from './split';
import { align, arrayToIndexMap, fileExists } from './util';
import { compressFile } from './compress';
import { CodeGen } from './codegen';

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

const DMA: {[k in Game]?: DmaData} = {
  oot: undefined,
  mm: undefined,
};

const getDmaData = async (game: Game) => {
  let dma = DMA[game];
  if (dma === undefined) {
    const filename = path.resolve(PATH_BUILD, 'roms', `${game}_dma.bin`);
    const buf = await fs.readFile(filename);
    dma = new DmaData(buf);
    DMA[game] = dma;
  }
  return dma;
};

const getObjectBuffer = async (entry: CustomEntry) => {
  const index = FILES_TO_INDEX[entry.game][entry.file];
  if (index === undefined) {
    throw new Error(`File ${entry.file} not found in game ${entry.game}`);
  }
  const dma = await getDmaData(entry.game);
  const dmaEntry = dma.read(index);
  const filename = path.resolve(PATH_BUILD, 'roms', `${entry.game}_decompressed.z64`);
  const f = await fs.open(filename, 'r');
  const buf = Buffer.alloc(dmaEntry.virtEnd - dmaEntry.virtStart);
  await f.read(buf, 0, buf.byteLength, dmaEntry.virtStart);
  await f.close();
  return buf;
};

/* TODO: Cache this */
const makeSplitObject = async (entry: CustomEntry) => {
  const outDir = path.resolve(PATH_BUILD, 'custom');
  const outBasename = entry.name.toLowerCase();
  await fs.mkdir(outDir, { recursive: true });

  const buf = await getObjectBuffer(entry);
  const obj = splitObject(buf, entry.offsets);

  const outFilename = path.resolve(outDir, `${outBasename}.zobj`);
  await fs.writeFile(outFilename, obj.data);

  return obj;
};

export const custom = async () => {
  console.log("Building custom objects...");
  const cg = new CodeGen(path.resolve(PATH_BUILD, 'include', 'combo', 'custom.h'), 'CUSTOM_H');
  const objects = await Promise.all(ENTRIES.map(x => makeSplitObject(x)));
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

  await Promise.all([
    fs.writeFile(path.resolve(PATH_BUILD, 'custom.bin'), Buffer.concat(data)),
    cg.emit(),
  ]);
};

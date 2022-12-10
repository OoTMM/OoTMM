import path from 'path';
import fs from 'fs/promises';
import { Buffer } from 'buffer';

import { Game, DATA_FILES, CUSTOM_ADDR } from '../config';
import { DmaData } from '../dma';
import { splitObject } from './split';
import { arrayToIndexMap } from '../util';
import { CodeGen } from '../codegen';
import { DecompressedRoms } from '../decompress';
import { Monitor } from '../monitor';
import { CustomArchive } from './archive';

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

export const customExtractedObjects = async (roms: DecompressedRoms, archive: CustomArchive, cg: CodeGen) => {
  for (const entry of ENTRIES) {
    const obj = await makeSplitObject(roms, entry);
    const objectId = await archive.addObject(obj.data);
    cg.define('CUSTOM_OBJECT_ID_' + entry.name, objectId);
    for (let i = 0; i < obj.offsets.length; ++i) {
      cg.define('CUSTOM_OBJECT_' + entry.name + '_' + i, obj.offsets[i]);
    }
  }
};

export const custom = async (monitor: Monitor, roms: DecompressedRoms) => {
  monitor.log("Building custom objects");
  const cgPath = process.env.ROLLUP ? '' : path.resolve('include', 'combo', 'custom.h');
  const cg = new CodeGen(cgPath, 'CUSTOM_H');
  const archive = new CustomArchive();

  /* Extract some objects */
  await customExtractedObjects(roms, archive, cg);

  /* Emit the custom header and data */
  const pack = archive.pack();
  cg.define('CUSTOM_DMA_ADDR', pack.dmaAddr);
  cg.define('CUSTOM_DMA_SIZE', pack.dmaCount);
  cg.define('CUSTOM_OBJECTS_ADDR', pack.objectTableAddr);
  cg.define('CUSTOM_OBJECTS_SIZE', pack.objectTableCount);

  if (!process.env.ROLLUP) {
    await cg.emit();
  }

  return pack.data;
};

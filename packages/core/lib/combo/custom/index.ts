import path from 'path';
import fs from 'fs/promises';
import { Buffer } from 'buffer';

import { Game, DATA_FILES } from '../config';
import { DmaData } from '../dma';
import { splitObject } from './split';
import { arrayToIndexMap, toU32Buffer } from '../util';
import { CodeGen } from '../codegen';
import { DecompressedRoms } from '../decompress';
import { Monitor } from '../monitor';
import { KeepFile } from './keep';
import { png } from '../util/png';
import { font } from './font';
import { raw } from './raw';
import { Options } from '../options';
import { Patchfile } from '../patch-build/patchfile';

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
  /*{ game: 'mm',  name: "GI_OWL",                file: "objects/object_tsg",          offsets: [0x3770] },*/
  { game: 'oot', name: "GI_MASTER_SWORD",       file: "objects/object_toki_objects", offsets: [0x1bd0] },
  { game: 'oot', name: "GI_STONE_EMERALD",      file: "objects/object_gi_jewel",     offsets: [0x1240, 0x10e0] },
  { game: 'oot', name: "GI_STONE_RUBY",         file: "objects/object_gi_jewel",     offsets: [0x20a0, 0x1fb0] },
  { game: 'oot', name: "GI_STONE_SAPPHIRE",     file: "objects/object_gi_jewel",     offsets: [0x3530, 0x3370] },
  { game: 'oot', name: "GI_MEDALLION_FOREST",   file: "objects/object_gi_medal",     offsets: [0x0cb0, 0x0e18] },
  { game: 'oot', name: "GI_MEDALLION_FIRE",     file: "objects/object_gi_medal",     offsets: [0x1af0, 0x0e18] },
  { game: 'oot', name: "GI_MEDALLION_WATER",    file: "objects/object_gi_medal",     offsets: [0x2830, 0x0e18] },
  { game: 'oot', name: "GI_MEDALLION_SPIRIT",   file: "objects/object_gi_medal",     offsets: [0x3610, 0x0e18] },
  { game: 'oot', name: "GI_MEDALLION_SHADOW",   file: "objects/object_gi_medal",     offsets: [0x4330, 0x0e18] },
  { game: 'oot', name: "GI_MEDALLION_LIGHT",    file: "objects/object_gi_medal",     offsets: [0x5220, 0x0e18] },
];

const getObjectBuffer = async (roms: DecompressedRoms, game: Game, file: string) => {
  const rom = roms[game].rom;
  const dma = new DmaData(roms[game].dma);
  const index = FILES_TO_INDEX[game][file];
  if (index === undefined) {
    throw new Error(`File ${file} not found in game ${game}`);
  }
  const dmaEntry = dma.read(index);
  return Buffer.from(rom.subarray(dmaEntry.virtStart, dmaEntry.virtEnd));
};

/* TODO: Cache this */
const makeSplitObject = async (roms: DecompressedRoms, entry: CustomEntry) => {
  const buf = await getObjectBuffer(roms, entry.game, entry.file);
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

export const customAssets = async (opts: Options): Promise<{[k: string]: Buffer}> => ({
  DPAD: await png(opts, 'dpad', 'rgba32'),
  CHEST_MAJOR_FRONT: await png(opts, 'chest_front_major', 'rgba16'),
  CHEST_MAJOR_SIDE: await png(opts, 'chest_side_major', 'rgba16'),
  CHEST_KEY_FRONT: await png(opts, 'chest_front_key', 'rgba16'),
  CHEST_KEY_SIDE: await png(opts, 'chest_side_key', 'rgba16'),
  CHEST_SPIDER_FRONT: await png(opts, 'chest_front_spider', 'rgba16'),
  CHEST_SPIDER_SIDE: await png(opts, 'chest_side_spider', 'rgba16'),
  CHEST_FAIRY_FRONT: await png(opts, 'chest_front_fairy', 'rgba16'),
  CHEST_FAIRY_SIDE: await png(opts, 'chest_side_fairy', 'rgba16'),
  CHEST_HEART_FRONT: await png(opts, 'chest_front_heart', 'rgba16'),
  CHEST_HEART_SIDE: await png(opts, 'chest_side_heart', 'rgba16'),
  FONT: await font(opts, 'font_8x12'),
  SMALL_ICON_KEY: await png(opts, 'small_icon_key', 'rgba32'),
  SMALL_ICON_BOSS_KEY: await png(opts, 'small_icon_boss_key', 'rgba32'),
  SMALL_ICON_MAP: await png(opts, 'small_icon_map', 'rgba32'),
  SMALL_ICON_COMPASS: await png(opts, 'small_icon_compass', 'rgba32'),
  SMALL_ICON_FAIRY: await png(opts, 'small_icon_fairy', 'rgba32'),
  SMALL_ICON_SKULL: await png(opts, 'small_icon_skull', 'rgba32'),
  SMALL_ICON_TRIFORCE: await png(opts, 'small_icon_triforce', 'rgba32'),
  SMALL_ICON_RUPEE: await png(opts, 'small_icon_rupee', 'rgba32'),
});

const extractRaw = async (roms: DecompressedRoms, game: Game, file: string, offset: number, size: number) => {
  const obj = await getObjectBuffer(roms, game, file);
  return obj.subarray(offset, offset + size);
};

export const extractedAssets = async (roms: DecompressedRoms): Promise<{[k: string]: Buffer}> => ({
  SF_TEXTURE_1: await extractRaw(roms, 'mm', 'objects/gameplay_keep', 0x2c030, 16 * 32),
  SF_TEXTURE_2: await extractRaw(roms, 'mm', 'objects/gameplay_keep', 0x2c630, 16 * 16),
  SF_TEXTURE_3: await extractRaw(roms, 'mm', 'objects/gameplay_keep', 0x2bc30, 32 * 32),
});

type ObjectRef = {
  vstart: number;
  vend: number;
}

class CustomAssetsBuilder {
  private cg: CodeGen;
  private vrom: number;
  private objectId: number;
  private objectVroms: ObjectRef[];

  constructor(
    private opts: Options,
    private monitor: Monitor,
    private roms: DecompressedRoms,
    private patch: Patchfile,
  ) {
    const cgPath = process.env.ROLLUP ? '' : path.resolve('include', 'combo', 'custom.h');
    this.cg = new CodeGen(cgPath, 'CUSTOM_H');
    this.vrom = 0x08000000;
    this.objectId = 0x2000;
    this.objectVroms = [];
  }

  addObjectEntry(vstart: number, size: number) {
    const objectId = this.objectId++;
    this.objectVroms.push({ vstart, vend: vstart + size });
    return objectId;
  }

  addRawData(data: Buffer, compressed: boolean) {
    const sizeAligned = (data.length + 0xf) & ~0xf;
    const vrom = this.vrom;
    this.vrom += sizeAligned;
    this.patch.addNewFile(vrom, data, compressed);
    return vrom;
  }

  async addFile(define: string, filename: string, compressed: boolean) {
    const data = await raw(this.opts, filename);
    const vrom = this.addRawData(data, compressed);
    this.cg.define('CUSTOM_' + define + '_ADDR', vrom);
    return vrom;
  }

  async addCustomObject(name: string, data: Buffer, defines: number[]) {
    const vrom = this.addRawData(data, true);
    const objectId = this.addObjectEntry(vrom, data.length);
    this.cg.define('CUSTOM_OBJECT_ID_' + name, objectId);
    for (let i = 0; i < defines.length; ++i) {
      this.cg.define('CUSTOM_OBJECT_' + name + '_' + i, defines[i]);
    }
  }

  async addObjectFile(name: string, filename: string, defines: number[]) {
    const file = await raw(this.opts, filename);
    await this.addCustomObject(name, file, defines);
  }


  async addCustomExtractedObject(entry: CustomEntry) {
    const obj = await makeSplitObject(this.roms, entry);
    await this.addCustomObject(entry.name, obj.data, obj.offsets);
  }

  async addCustomKeepFiles() {
    const keep = new KeepFile();
    const cAssets = await customAssets(this.opts);
    const eAssets = await extractedAssets(this.roms);
    const assets = { ...cAssets, ...eAssets };
    for (const k in assets) {
      const off = await keep.addData(assets[k]);
      this.cg.define('CUSTOM_KEEP_' + k, off);
    }

    const customKeepVrom = this.addRawData(keep.pack(), true);
    this.cg.define('CUSTOM_KEEP_VROM', customKeepVrom);
  }

  async run() {
    this.monitor.log("Building custom objects");

    /* Extract some objects */
    for (const entry of ENTRIES) {
      await this.addCustomExtractedObject(entry);
    }

    /* Setup custom keep */
    await this.addCustomKeepFiles();

    /* Load MQ data */
    await this.addFile('MQ_ROOMS', 'mq_rooms.bin', false);
    await this.addFile('MQ_SCENES', 'mq_scenes.bin', false);
    await this.addFile('MQ_MAPS', 'mq_maps.bin', true);

    /* Load custom objects */
    await this.addObjectFile('TRIFORCE', 'triforce.zobj', [0x06000a30]);

    /* Add the object table */
    const objectTableBuffer = toU32Buffer(this.objectVroms.map(o => [o.vstart, o.vend]).flat());
    const objectTableVrom = this.addRawData(objectTableBuffer, true);
    this.cg.define('CUSTOM_OBJECT_TABLE_VROM', objectTableVrom);
    this.cg.define('CUSTOM_OBJECT_TABLE_SIZE', this.objectVroms.length);

    if (!process.env.ROLLUP) {
      await this.cg.emit();
    }
  }
}

export function custom(opts: Options, monitor: Monitor, roms: DecompressedRoms, patch: Patchfile) {
  const builder = new CustomAssetsBuilder(opts, monitor, roms, patch);
  return builder.run();
}

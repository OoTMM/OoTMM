import path from 'path';
import fs from 'fs';
import { FILES } from '@ootmm/data';

import { Game } from '../config';
import { DmaData } from '../dma';
import { splitObject } from './split';
import { arrayToIndexMap, toU32Buffer } from '../util';
import { CodeGen } from '../util/codegen';
import { DecompressedRoms } from '../decompress';
import { Monitor } from '../monitor';
import { KeepFile } from './keep';
import { png } from '../util/png';
import { font } from './font';
import { raw } from './raw';
import { Options } from '../options';
import { Patchfile } from '../patch-build/patchfile';
import { grayscale } from '../image';

const FILES_TO_INDEX = {
  oot: arrayToIndexMap(FILES.oot),
  mm: arrayToIndexMap(FILES.mm),
};

type CustomEntry = {
  game: Game,
  name: string,
  file: string,
  seg?: { in: number, out: number },
  offsets: number[],
};

const ENTRIES: CustomEntry[] = [
  { game: 'mm',  name: "GI_REMAINS_ODOLWA",     file: "objects/object_bsmask",       offsets: [0x0690] },
  { game: 'mm',  name: "GI_REMAINS_GOHT",       file: "objects/object_bsmask",       offsets: [0x3ad0] },
  { game: 'mm',  name: "GI_REMAINS_GYORG",      file: "objects/object_bsmask",       offsets: [0x1d80] },
  { game: 'mm',  name: "GI_REMAINS_TWINMOLD",   file: "objects/object_bsmask",       offsets: [0x5020] },
  { game: 'mm',  name: "GI_MASK_MAJORA",        file: "objects/object_stk",          offsets: [0x6bb0] },
  /*{ game: 'mm',  name: "GI_OWL",                file: "objects/object_tsg",          offsets: [0x3770] },*/
  { game: 'oot', name: "GI_MASTER_SWORD",       file: "objects/object_toki_objects",  offsets: [0x1bd0] },
  { game: 'oot', name: "GI_STONE_EMERALD",      file: "objects/object_gi_jewel",      offsets: [0x1240, 0x10e0] },
  { game: 'oot', name: "GI_STONE_RUBY",         file: "objects/object_gi_jewel",      offsets: [0x20a0, 0x1fb0] },
  { game: 'oot', name: "GI_STONE_SAPPHIRE",     file: "objects/object_gi_jewel",      offsets: [0x3530, 0x3370] },
  { game: 'oot', name: "GI_MEDALLION_FOREST",   file: "objects/object_gi_medal",      offsets: [0x0cb0, 0x0e18] },
  { game: 'oot', name: "GI_MEDALLION_FIRE",     file: "objects/object_gi_medal",      offsets: [0x1af0, 0x0e18] },
  { game: 'oot', name: "GI_MEDALLION_WATER",    file: "objects/object_gi_medal",      offsets: [0x2830, 0x0e18] },
  { game: 'oot', name: "GI_MEDALLION_SPIRIT",   file: "objects/object_gi_medal",      offsets: [0x3610, 0x0e18] },
  { game: 'oot', name: "GI_MEDALLION_SHADOW",   file: "objects/object_gi_medal",      offsets: [0x4330, 0x0e18] },
  { game: 'oot', name: "GI_MEDALLION_LIGHT",    file: "objects/object_gi_medal",      offsets: [0x5220, 0x0e18] },
  { game: 'mm',  name: "GI_CLOCK",              file: "objects/object_moguri",        offsets: [0xf518, 0xcf28, 0xbee8, 0xc368] },

  /* Extracted OoT Masks - used for adult masks */
  { game: 'oot', name: "MASK_OOT_SKULL",        file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x2ad40] },
  { game: 'oot', name: "MASK_OOT_SPOOKY",       file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x2af70] },
  { game: 'oot', name: "MASK_OOT_KEATON",       file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x2b060] },
  { game: 'oot', name: "MASK_OOT_TRUTH",        file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x2b1f0] },
  { game: 'oot', name: "MASK_OOT_GORON",        file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x2b350] },
  { game: 'oot', name: "MASK_OOT_ZORA",         file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x2b580] },
  { game: 'oot', name: "MASK_OOT_GERUDO",       file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x2b788] },
  { game: 'oot', name: "MASK_OOT_BUNNY",        file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x2ca38] },

  { game: 'oot', name: "EQ_DEKU_STICK",         file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x6cc0] },
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
  const seg = entry.seg || { in: 6, out: 6 };
  const obj = splitObject(buf, entry.offsets, seg.in, seg.out);

  if (!process.env.BROWSER) {
    const outDir = path.resolve('build', 'custom');
    const outBasename = entry.name.toLowerCase();
    const outFilename = path.resolve(outDir, `${outBasename}.zobj`);
    await fs.promises.mkdir(outDir, { recursive: true });
    await fs.promises.writeFile(outFilename, obj.data);
  }

  return obj;
};

const extractFileData = async (roms: DecompressedRoms, game: Game, file: string, offset: number, size: number) => {
  const objBuffer = await getObjectBuffer(roms, game, file);
  const tex = objBuffer.subarray(offset, offset + size);
  return tex;
};

export const customExtractedFiles = async (roms: DecompressedRoms): Promise<{[k: string]: Buffer}> => ({
  GRASS: await extractFileData(roms, 'oot', 'objects/gameplay_field_keep', 0xb140, 32 * 32 * 2).then(t => grayscale(t, 'rgba16', 0.25)),
  GRASS_ALT: await extractFileData(roms, 'oot', 'objects/gameplay_keep', 0x35BD0, 32 * 32 * 2).then(t => grayscale(t, 'rgba16', 0.25)),
});

export const customFiles = async (opts: Options): Promise<{[k: string]: Buffer}> => ({
  CHEST_MAJOR_FRONT: await png(opts, 'chests/major_front', 'rgba16'),
  CHEST_MAJOR_SIDE: await png(opts, 'chests/major_side', 'rgba16'),
  CHEST_KEY_FRONT: await png(opts, 'chests/key_front', 'rgba16'),
  CHEST_KEY_SIDE: await png(opts, 'chests/key_side', 'rgba16'),
  CHEST_SPIDER_FRONT: await png(opts, 'chests/spider_front', 'rgba16'),
  CHEST_SPIDER_SIDE: await png(opts, 'chests/spider_side', 'rgba16'),
  CHEST_FAIRY_FRONT: await png(opts, 'chests/fairy_front', 'rgba16'),
  CHEST_FAIRY_SIDE: await png(opts, 'chests/fairy_side', 'rgba16'),
  CHEST_HEART_FRONT: await png(opts, 'chests/heart_front', 'rgba16'),
  CHEST_HEART_SIDE: await png(opts, 'chests/heart_side', 'rgba16'),
  CHEST_SOUL_FRONT: await png(opts, 'chests/soul_front', 'rgba16'),
  CHEST_SOUL_SIDE: await png(opts, 'chests/soul_side', 'rgba16'),
  POT_MAJOR_SIDE: await png(opts, 'pots/major_side', 'rgba16'),
  POT_MAJOR_TOP: await png(opts, 'pots/major_top', 'rgba16'),
  POT_SPIDER_SIDE: await png(opts, 'pots/spider_side', 'rgba16'),
  POT_SPIDER_TOP: await png(opts, 'pots/spider_top', 'rgba16'),
  POT_KEY_SIDE: await png(opts, 'pots/key_side', 'rgba16'),
  POT_FAIRY_SIDE: await png(opts, 'pots/fairy_side', 'rgba16'),
  POT_FAIRY_TOP: await png(opts, 'pots/fairy_top', 'rgba16'),
  POT_HEART_SIDE: await png(opts, 'pots/heart_side', 'rgba16'),
  POT_HEART_TOP: await png(opts, 'pots/heart_top', 'rgba16'),
  POT_BOSSKEY_SIDE: await png(opts, 'pots/bosskey_side', 'rgba16'),
  POT_BOSSKEY_TOP: await png(opts, 'pots/bosskey_top', 'rgba16'),
  POT_SOUL_SIDE: await png(opts, 'pots/soul_side', 'rgba16'),
  POT_SOUL_TOP: await png(opts, 'pots/soul_top', 'rgba16'),
});

export const customAssetsKeep = async (opts: Options): Promise<{[k: string]: Buffer}> => ({
  DPAD: await png(opts, 'dpad', 'rgba16'),
  FONT: await font(opts, 'font_8x12'),
  SMALL_ICON_KEY: await png(opts, 'small_icon_key', 'rgba16'),
  SMALL_ICON_BOSS_KEY: await png(opts, 'small_icon_boss_key', 'rgba16'),
  SMALL_ICON_MAP: await png(opts, 'small_icon_map', 'rgba16'),
  SMALL_ICON_COMPASS: await png(opts, 'small_icon_compass', 'rgba16'),
  SMALL_ICON_FAIRY: await png(opts, 'small_icon_fairy', 'rgba16'),
  SMALL_ICON_SKULL: await png(opts, 'small_icon_skull', 'rgba16'),
  SMALL_ICON_TRIFORCE: await png(opts, 'small_icon_triforce', 'rgba16'),
  SMALL_ICON_RUPEE: await png(opts, 'small_icon_rupee', 'rgba16'),
  SMALL_ICON_COIN: await png(opts, 'small_icon_coin', 'rgba16'),
});

const extractRaw = async (roms: DecompressedRoms, game: Game, file: string, offset: number, size: number) => {
  const obj = await getObjectBuffer(roms, game, file);
  return obj.subarray(offset, offset + size);
};

export const extractedAssets = async (roms: DecompressedRoms): Promise<{[k: string]: Buffer}> => ({
  SF_TEXTURE_1: await extractRaw(roms, 'mm', 'objects/gameplay_keep', 0x2c030, 16 * 32),
  SF_TEXTURE_2: await extractRaw(roms, 'mm', 'objects/gameplay_keep', 0x2c630, 16 * 16),
  SF_TEXTURE_3: await extractRaw(roms, 'mm', 'objects/gameplay_keep', 0x2bc30, 32 * 32),
  MAGIC_WIND_TEXTURE: await extractRaw(roms, 'oot', 'actors/ovl_Magic_Wind', 0x8E0, 32 * 64),
  MAGIC_DARK_TEXTURE: await extractRaw(roms, 'oot', 'actors/ovl_Magic_Dark', 0xC90, 32 * 64),
  MAGIC_FIRE_TEXTURE: await extractRaw(roms, 'oot', 'actors/ovl_Magic_Fire', 0xB20, 64 * 64),
  BOOTS_IRON_TEXTURE: await extractRaw(roms, 'oot', 'objects/object_link_boy', 0xd1b8, 16 * 16),
  BOOTS_IRON_TLUT: await extractRaw(roms, 'oot', 'objects/object_link_boy', 0xcb40, 16 * 16 * 2),
  BOOTS_HOVER_HEEL_TEXTURE: await extractRaw(roms, 'oot', 'objects/object_link_boy', 0xa580, 16 * 8 * 2),
  BOOTS_HOVER_JET_TEXTURE: await extractRaw(roms, 'oot', 'objects/object_link_boy', 0xa680, 32 * 32 * 2),
  BOOTS_HOVER_FEATHER_TEXTURE: await extractRaw(roms, 'oot', 'objects/object_link_boy', 0xae80, 32 * 16 * 2),
  BOOTS_HOVER_CIRCLE_TEXTURE: await extractRaw(roms, 'oot', 'objects/gameplay_keep', 0x37e00, 16 * 32),
  GORON_BRACELET_TEXTURE: await extractRaw(roms, 'oot', 'objects/object_link_child', 0x7208, 8 * 8 * 2),
  GORON_SYMBOL_TEXTURE: await extractRaw(roms, 'oot', 'objects/object_link_child', 0x7288, 16 * 32 * 2),
  GAUNTLET1_TEXTURE: await extractRaw(roms, 'oot', 'objects/object_link_boy', 0x9980, 16 * 32 * 2),
  GAUNTLET2_TEXTURE: await extractRaw(roms, 'oot', 'objects/object_link_boy', 0x9d80, 32 * 32 * 2),
  ELEGY_SHELL_HUMAN_MOUTH_TEXTURE: await extractRaw(roms, 'mm', 'objects/gameplay_keep', 0x1caf8, 32 * 32 * 2),
  ELEGY_SHELL_HUMAN_EYE_AND_NOSE_TEXTURE: await extractRaw(roms, 'mm', 'objects/gameplay_keep', 0x1d2f8, 32 * 64 * 2),
  ELEGY_SHELL_HUMAN_NOSTRILS_AND_SKIN_TEXTURE: await extractRaw(roms, 'mm', 'objects/gameplay_keep', 0x1e2f8, 16 * 8 * 2),
  ELEGY_SHELL_HUMAN_BOOTS_TEXTURE: await extractRaw(roms, 'mm', 'objects/gameplay_keep', 0x1e3f8, 16 * 16 * 2),
  ELEGY_SHELL_HUMAN_PLATFORM_TEXTURE: await extractRaw(roms, 'mm', 'objects/gameplay_keep', 0x1e5f8, 8 * 16 * 2),
  ELEGY_SHELL_BELT_AND_TUNIC_TEXTURE: await extractRaw(roms, 'mm', 'objects/gameplay_keep', 0x1e6f8, 32 * 16 * 2),
  ELEGY_SHELL_HUMAN_HAIR_TEXTURE: await extractRaw(roms, 'mm', 'objects/gameplay_keep', 0x1eaf8, 16 * 16 * 2),
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
    const cgPath = process.env.BROWSER ? '' : path.resolve('include', 'combo', 'custom.h');
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
    this.patch.addNewFile(null, vrom, data, compressed);
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
    const cAssets = await customAssetsKeep(this.opts);
    const eAssets = await extractedAssets(this.roms);
    const assets = { ...cAssets, ...eAssets };
    for (const k in assets) {
      const off = await keep.addData(assets[k]);
      this.cg.define('CUSTOM_KEEP_' + k, off);
    }

    const customKeepVrom = this.addRawData(keep.pack(), true);
    this.cg.define('CUSTOM_KEEP_VROM', customKeepVrom);
  }

  async addCustomFiles() {
    const cfiles = await customFiles(this.opts);
    for (const [name, data] of Object.entries(cfiles)) {
      const vrom = this.addRawData(data, true);
      this.cg.define('CUSTOM_' + name + '_ADDR', vrom);
    }
  }

  async addCustomExtractedFiles() {
    const cfiles = await customExtractedFiles(this.roms);
    for (const [name, data] of Object.entries(cfiles)) {
      const vrom = this.addRawData(data, true);
      this.cg.define('CUSTOM_' + name + '_ADDR', vrom);
    }
  }

  async run() {
    this.monitor.log("Building custom objects");

    /* Extract some objects */
    for (const entry of ENTRIES) {
      await this.addCustomExtractedObject(entry);
    }

    /* Setup custom keep */
    await this.addCustomKeepFiles();
    await this.addCustomFiles();
    await this.addCustomExtractedFiles();

    /* Load MQ data */
    await this.addFile('MQ_ROOMS', 'mq_rooms.bin', false);
    await this.addFile('MQ_SCENES', 'mq_scenes.bin', false);
    await this.addFile('MQ_MAPS', 'mq_maps.bin', true);
    await this.addFile('XFLAG_TABLE_OOT_SCENES', 'xflag_table_oot_scenes.bin', false);
    await this.addFile('XFLAG_TABLE_OOT_SETUPS', 'xflag_table_oot_setups.bin', false);
    await this.addFile('XFLAG_TABLE_OOT_ROOMS',  'xflag_table_oot_rooms.bin', false);
    await this.addFile('XFLAG_TABLE_MM_SCENES',  'xflag_table_mm_scenes.bin', false);
    await this.addFile('XFLAG_TABLE_MM_SETUPS',  'xflag_table_mm_setups.bin', false);
    await this.addFile('XFLAG_TABLE_MM_ROOMS',   'xflag_table_mm_rooms.bin', false);

    /* Load custom objects */
    await this.addObjectFile('TRIFORCE', 'triforce.zobj', [0x06000a30]);
    await this.addObjectFile('BTN_A', 'btn_a.zobj', [0x06000da0]);
    await this.addObjectFile('BTN_C_HORIZONTAL', 'btn_c_horizontal.zobj', [0x06000e10]);
    await this.addObjectFile('BTN_C_VERTICAL', 'btn_c_vertical.zobj', [0x06000960]);
    await this.addObjectFile('BOMBCHU_BAG', 'bombchu_bag.zobj', [0x060006A0, 0x060008E0, 0x06001280]);

    /* Add the object table */
    const objectTableBuffer = toU32Buffer(this.objectVroms.map(o => [o.vstart, o.vend]).flat());
    const objectTableVrom = this.addRawData(objectTableBuffer, true);
    this.cg.define('CUSTOM_OBJECT_TABLE_VROM', objectTableVrom);
    this.cg.define('CUSTOM_OBJECT_TABLE_SIZE', this.objectVroms.length);

    if (!process.env.BROWSER) {
      await this.cg.emit();
    }
  }
}

export function custom(opts: Options, monitor: Monitor, roms: DecompressedRoms, patch: Patchfile) {
  const builder = new CustomAssetsBuilder(opts, monitor, roms, patch);
  return builder.run();
}

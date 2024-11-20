import path from 'path';
import fs from 'fs';
import { FILES } from '@ootmm/data';

import { Game } from '../config';
import { DmaData } from '../dma';
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
import { CustomObjectsBuilder } from './custom-objects-builder';
import { bufReadU32BE, bufWriteU32BE } from '../util/buffer';
import { ObjectEditor } from './object-editor';

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
  { game: 'mm',  name: "GI_REMAINS_ODOLWA",     file: "objects/object_bsmask",       offsets: [0x06000690] },
  { game: 'mm',  name: "GI_REMAINS_GOHT",       file: "objects/object_bsmask",       offsets: [0x06003ad0] },
  { game: 'mm',  name: "GI_REMAINS_GYORG",      file: "objects/object_bsmask",       offsets: [0x06001d80] },
  { game: 'mm',  name: "GI_REMAINS_TWINMOLD",   file: "objects/object_bsmask",       offsets: [0x06005020] },
  { game: 'mm',  name: "GI_MASK_MAJORA",        file: "objects/object_stk",           offsets: [0x06006bb0] },
  { game: 'oot', name: "GI_MASTER_SWORD",       file: "objects/object_toki_objects",  offsets: [0x06001bd0] },
  { game: 'oot', name: "GI_STONE_EMERALD",      file: "objects/object_gi_jewel",      offsets: [0x06001240, 0x060010e0] },
  { game: 'oot', name: "GI_STONE_RUBY",         file: "objects/object_gi_jewel",      offsets: [0x060020a0, 0x06001fb0] },
  { game: 'oot', name: "GI_STONE_SAPPHIRE",     file: "objects/object_gi_jewel",      offsets: [0x06003530, 0x06003370] },
  { game: 'oot', name: "GI_MEDALLION_FOREST",   file: "objects/object_gi_medal",      offsets: [0x06000cb0, 0x06000e18] },
  { game: 'oot', name: "GI_MEDALLION_FIRE",     file: "objects/object_gi_medal",      offsets: [0x06001af0, 0x06000e18] },
  { game: 'oot', name: "GI_MEDALLION_WATER",    file: "objects/object_gi_medal",      offsets: [0x06002830, 0x06000e18] },
  { game: 'oot', name: "GI_MEDALLION_SPIRIT",   file: "objects/object_gi_medal",      offsets: [0x06003610, 0x06000e18] },
  { game: 'oot', name: "GI_MEDALLION_SHADOW",   file: "objects/object_gi_medal",      offsets: [0x06004330, 0x06000e18] },
  { game: 'oot', name: "GI_MEDALLION_LIGHT",    file: "objects/object_gi_medal",      offsets: [0x06005220, 0x06000e18] },
  { game: 'mm',  name: "GI_CLOCK",              file: "objects/object_moguri",        offsets: [0x0600f518, 0x0600cf28, 0x0600bee8, 0x0600c368] },
  /*{ game: 'mm',  name: "GI_OWL",                file: "objects/object_tsg",          offsets: [0x3770] },*/

  /* Extracted OoT Masks - used for adult masks */
  { game: 'oot', name: "MASK_OOT_SKULL",        file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x0602ad40] },
  { game: 'oot', name: "MASK_OOT_SPOOKY",       file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x0602af70] },
  { game: 'oot', name: "MASK_OOT_KEATON",       file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x0602b060] },
  { game: 'oot', name: "MASK_OOT_TRUTH",        file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x0602b1f0] },
  { game: 'oot', name: "MASK_OOT_GORON",        file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x0602b350] },
  { game: 'oot', name: "MASK_OOT_ZORA",         file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x0602b580] },
  { game: 'oot', name: "MASK_OOT_GERUDO",       file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x0602b788] },
  { game: 'oot', name: "MASK_OOT_BUNNY",        file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x0602ca38] },

  { game: 'oot', name: "EQ_DEKU_STICK",         file: "objects/object_link_child",   seg: { in: 0x06, out: 0x0a }, offsets: [0x06006cc0] },

  { game: 'mm',  name: "OBJECT_TORCH2",         file: "objects/gameplay_keep",       seg: { in: 0x04, out: 0x06 }, offsets: [0x0401c430] },
  { game: 'oot', name: 'BOOTS_IRON',            file: "objects/object_link_boy",     seg: { in: 0x06, out: 0x0a }, offsets: [0x06025918, 0x06025a60] },
  { game: 'oot', name: 'BOOTS_HOVER',           file: "objects/object_link_boy",     seg: { in: 0x06, out: 0x0a }, offsets: [0x06025ba8, 0x06025db0] },
  { game: 'oot', name: 'GAUNTLETS',             file: "objects/object_link_boy",     seg: { in: 0x06, out: 0x0a }, offsets: [0x06025218, 0x060252d8, 0x06025438, 0x06025598, 0x06025658, 0x060257b8] },
];

const AUDIO_COPIES_OOT: {[k: number]: number} = {
  0x6e: 0x1c, /* Deku Tree -> Grottos */
  0x6f: 0x18, /* DC -> Graves */
  0x70: 0x18, /* DC -> GTG */
};

const getObjectBuffer = async (roms: DecompressedRoms, game: Game, file: string) => {
  const rom = roms[game].rom;
  const dma = new DmaData(roms[game].dma);
  const index = FILES_TO_INDEX[game][file];
  if (index === undefined) {
    throw new Error(`File ${file} not found in game ${game}`);
  }
  const dmaEntry = dma.read(index);
  return rom.slice(dmaEntry.virtStart, dmaEntry.virtEnd);
};

function splitObject(object: Uint8Array, offsets: number[], segIn: number, segOut: number) {
  const editor = new ObjectEditor(segOut);
  editor.loadSegment(segIn, object);

  for (const offset of offsets) {
    editor.submitListAddr(offset);
  }

  return editor.build();
};

/* TODO: Cache this */
const makeSplitObject = async (roms: DecompressedRoms, entry: CustomEntry) => {
  const buf = await getObjectBuffer(roms, entry.game, entry.file);
  const seg = entry.seg || { in: 6, out: 6 };
  const obj = splitObject(buf, entry.offsets, seg.in, seg.out);

  if (!process.env.__IS_BROWSER__) {
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

export const customExtractedFiles = async (roms: DecompressedRoms): Promise<{[k: string]: Uint8Array}> => ({
  GRASS: await extractFileData(roms, 'oot', 'objects/gameplay_field_keep', 0xb140, 32 * 32 * 2).then(t => grayscale(t, 'rgba16', 0.25)),
  GRASS_ALT: await extractFileData(roms, 'oot', 'objects/gameplay_keep', 0x35BD0, 32 * 32 * 2).then(t => grayscale(t, 'rgba16', 0.25)),
  HIVE: await extractFileData(roms, 'mm', 'objects/object_comb', 0x0000, 32 * 32 * 2).then(t => grayscale(t, 'rgba16', 0.25)),
  BUTTERFLY: await extractFileData(roms, 'oot', 'objects/gameplay_field_keep', 0x2680, 32 * 64 * 2).then(t => grayscale(t, 'rgba16', 0.25)),
});

export const customFiles = async (): Promise<{[k: string]: Uint8Array}> => ({
  CHEST_MAJOR_FRONT: await png('chests/major_front', 'rgba16'),
  CHEST_MAJOR_SIDE: await png('chests/major_side', 'rgba16'),
  CHEST_KEY_FRONT: await png('chests/key_front', 'rgba16'),
  CHEST_KEY_SIDE: await png('chests/key_side', 'rgba16'),
  CHEST_SPIDER_FRONT: await png('chests/spider_front', 'rgba16'),
  CHEST_SPIDER_SIDE: await png('chests/spider_side', 'rgba16'),
  CHEST_FAIRY_FRONT: await png('chests/fairy_front', 'rgba16'),
  CHEST_FAIRY_SIDE: await png('chests/fairy_side', 'rgba16'),
  CHEST_HEART_FRONT: await png('chests/heart_front', 'rgba16'),
  CHEST_HEART_SIDE: await png('chests/heart_side', 'rgba16'),
  CHEST_SOUL_FRONT: await png('chests/soul_front', 'rgba16'),
  CHEST_SOUL_SIDE: await png('chests/soul_side', 'rgba16'),
  CHEST_MAP_FRONT: await png('chests/map_front', 'rgba16'),
  CHEST_MAP_SIDE: await png('chests/map_side', 'rgba16'),
  CRATE_BOSS_KEY: await png('crates/boss_key', 'rgba16'),
  POT_MAJOR_SIDE: await png('pots/major_side', 'rgba16'),
  POT_MAJOR_TOP: await png('pots/major_top', 'rgba16'),
  POT_SPIDER_SIDE: await png('pots/spider_side', 'rgba16'),
  POT_SPIDER_TOP: await png('pots/spider_top', 'rgba16'),
  POT_KEY_SIDE: await png('pots/key_side', 'rgba16'),
  POT_FAIRY_SIDE: await png('pots/fairy_side', 'rgba16'),
  POT_FAIRY_TOP: await png('pots/fairy_top', 'rgba16'),
  POT_HEART_SIDE: await png('pots/heart_side', 'rgba16'),
  POT_HEART_TOP: await png('pots/heart_top', 'rgba16'),
  POT_BOSSKEY_SIDE: await png('pots/bosskey_side', 'rgba16'),
  POT_BOSSKEY_TOP: await png('pots/bosskey_top', 'rgba16'),
  POT_SOUL_SIDE: await png('pots/soul_side', 'rgba16'),
  POT_SOUL_TOP: await png('pots/soul_top', 'rgba16'),
  POT_MAP_SIDE: await png('pots/map_side', 'rgba16'),
  GLITTER: await png('glitter', 'i4'),
});

export const customAssetsKeep = async (): Promise<{[k: string]: Uint8Array}> => ({
  DPAD: await png('dpad', 'rgba16'),
  FONT: await font('font_8x12'),
  SMALL_ICON_KEY: await png('small_icon_key', 'rgba16'),
  SMALL_ICON_BOSS_KEY: await png('small_icon_boss_key', 'rgba16'),
  SMALL_ICON_MAP: await png('small_icon_map', 'rgba16'),
  SMALL_ICON_COMPASS: await png('small_icon_compass', 'rgba16'),
  SMALL_ICON_FAIRY: await png('small_icon_fairy', 'rgba16'),
  SMALL_ICON_SKULL: await png('small_icon_skull', 'rgba16'),
  SMALL_ICON_TRIFORCE: await png('small_icon_triforce', 'rgba16'),
  SMALL_ICON_RUPEE: await png('small_icon_rupee', 'rgba16'),
  SMALL_ICON_COIN: await png('small_icon_coin', 'rgba16'),
  SMALL_ICON_SUN: await png('small_icon_sun', 'rgba16'),
  SMALL_ICON_MOON: await png('small_icon_moon', 'rgba16'),
});

const extractRaw = async (roms: DecompressedRoms, game: Game, file: string, offset: number, size: number) => {
  const obj = await getObjectBuffer(roms, game, file);
  return obj.subarray(offset, offset + size);
};

export const extractedAssets = async (roms: DecompressedRoms): Promise<{[k: string]: Uint8Array}> => ({
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
});

type ObjectRef = {
  vstart: number;
  vend: number;
}

class CustomAssetsBuilder {
  private defines: Map<string, number>;
  private cg: CodeGen;
  private vrom: number;
  private objectId: number;
  private objectVroms: ObjectRef[];

  constructor(
    private monitor: Monitor,
    private roms: DecompressedRoms,
    private patch: Patchfile,
  ) {
    this.defines = new Map();
    const cgPath = process.env.__IS_BROWSER__ ? '' : path.resolve('include', 'combo', 'custom.h');
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

  addRawData(name: string | null, data: Uint8Array, compressed: boolean) {
    const sizeAligned = (data.length + 0xf) & ~0xf;
    const vrom = this.vrom;
    this.vrom += sizeAligned;
    this.patch.addNewFile({ name: name ?? undefined, vrom, data, compressed });
    return vrom;
  }

  async addFile(define: string, filename: string, compressed: boolean) {
    const data = await raw(filename);
    const vrom = this.addRawData(null, data, compressed);
    this.cg.define('CUSTOM_' + define + '_ADDR', vrom);
    return vrom;
  }

  async addCustomObject(name: string, data: Uint8Array, defines: number[]) {
    const vrom = this.addRawData(`custom/${name.toLowerCase()}`, data, true);
    const objectId = this.addObjectEntry(vrom, data.length);
    this.cg.define('CUSTOM_OBJECT_ID_' + name, objectId);
    for (let i = 0; i < defines.length; ++i) {
      this.cg.define('CUSTOM_OBJECT_' + name + '_' + i, defines[i]);
    }
  }

  async addObjectFile(name: string, filename: string, defines: number[]) {
    const file = await raw(filename);
    await this.addCustomObject(name, file, defines);
  }

  async addCustomExtractedObject(entry: CustomEntry) {
    const obj = await makeSplitObject(this.roms, entry);
    await this.addCustomObject(entry.name, obj.data, obj.offsets);
  }

  async addCustomKeepFiles() {
    const keep = new KeepFile();
    const cAssets = await customAssetsKeep();
    const eAssets = await extractedAssets(this.roms);
    const assets = { ...cAssets, ...eAssets };
    for (const k in assets) {
      const off = await keep.addData(assets[k]);
      this.cg.define('CUSTOM_KEEP_' + k, off);
    }

    const customKeepVrom = this.addRawData(null, keep.pack(), true);
    this.cg.define('CUSTOM_KEEP_VROM', customKeepVrom);
  }

  async addCustomFiles() {
    const cfiles = await customFiles();
    for (const [name, data] of Object.entries(cfiles)) {
      const vrom = this.addRawData(null, data, true);
      this.cg.define('CUSTOM_' + name + '_ADDR', vrom);
    }
  }

  async extractSeqTable(game: Game, count: number, codeOffset: number, romOffset: number) {
    const seqTableDataOrig = await extractRaw(this.roms, game, 'code', codeOffset, count * 0x10);
    const seqTableDataPatched = new Uint8Array(0x80 * 0x10);
    seqTableDataPatched.set(seqTableDataOrig);
    for (let i = 0; i < count; ++i) {
      let addr = bufReadU32BE(seqTableDataOrig, i * 0x10);
      let size = bufReadU32BE(seqTableDataOrig, i * 0x10 + 4);
      if (!size) {
        size = bufReadU32BE(seqTableDataOrig, addr * 0x10 + 4);
        addr = bufReadU32BE(seqTableDataOrig, addr * 0x10);
      }
      addr += romOffset;
      bufWriteU32BE(seqTableDataPatched, i * 0x10, addr);
      bufWriteU32BE(seqTableDataPatched, i * 0x10 + 4, size);
    }

    if (game === 'oot') {
      for (const [newSeq, oldSeq] of Object.entries(AUDIO_COPIES_OOT)) {
        const newSeqNum = Number(newSeq);
        const newOffset = newSeqNum * 0x10;
        const oldOffset = oldSeq * 0x10;
        const buf = seqTableDataPatched.subarray(oldOffset, oldOffset + 0x10);
        seqTableDataPatched.set(buf, newOffset);
      }
    }

    const seqTableDataVrom = this.addRawData(`${game}/seq_table`, seqTableDataPatched, false);
    this.cg.define(`CUSTOM_SEQ_TABLE_${game.toUpperCase()}_VROM`, seqTableDataVrom);
  }

  async extractAudioTable(game: Game, count: number, codeOffset: number, romOffset: number) {
    const dataOrig = await extractRaw(this.roms, game, 'code', codeOffset, count * 0x10);
    const dataPatched = new Uint8Array(8 * 0x10);
    dataPatched.set(dataOrig);
    for (let i = 0; i < count; ++i) {
      let addr = bufReadU32BE(dataOrig, i * 0x10);
      let size = bufReadU32BE(dataOrig, i * 0x10 + 4);
      if (size) {
        addr += romOffset;
      }
      bufWriteU32BE(dataPatched, i * 0x10, addr);
    }
    const dataVrom = this.addRawData(`${game}/audio_table`, dataPatched, false);
    this.cg.define(`CUSTOM_AUDIO_TABLE_${game.toUpperCase()}_VROM`, dataVrom);
  }

  async extractBankTable(game: Game, count: number, codeOffset: number, romOffset: number) {
    const dataOrig = await extractRaw(this.roms, game, 'code', codeOffset, count * 0x10);
    const dataPatched = new Uint8Array(0x30 * 0x10);
    dataPatched.set(dataOrig);
    for (let i = 0; i < count; ++i) {
      let addr = bufReadU32BE(dataOrig, i * 0x10);
      let size = bufReadU32BE(dataOrig, i * 0x10 + 4);
      if (!size) {
        size = bufReadU32BE(dataOrig, addr * 0x10 + 4);
        addr = bufReadU32BE(dataOrig, addr * 0x10);
      }
      addr += romOffset;
      bufWriteU32BE(dataPatched, i * 0x10, addr);
      bufWriteU32BE(dataPatched, i * 0x10 + 4, size);
    }
    const dataVrom = this.addRawData(`${game}/bank_table`, dataPatched, false);
    this.cg.define(`CUSTOM_BANK_TABLE_${game.toUpperCase()}_VROM`, dataVrom);
  }

  async extractCustomBankTable() {
    const data = new Uint8Array((0xf0 - 0x60) * 0x10);
    const vrom = this.addRawData(`custom/bank_table`, data, false);
    this.cg.define(`CUSTOM_BANK_TABLE_CUSTOM_VROM`, vrom);
  }

  async extractSeqBanks(game: Game, count: number, codeOffset: number) {
    const seqBankDataRaw = await extractRaw(this.roms, game, 'code', codeOffset, count * 2);
    const seqBankData = new Uint8Array(0x80 * 2);
    for (let i = 0; i < count; ++i) {
      const bankId = seqBankDataRaw[i * 2];
      seqBankData[i + 1] = bankId;
    }

    if (game === 'oot') {
      for (const [newSeq, oldSeq] of Object.entries(AUDIO_COPIES_OOT)) {
        const newSeqNum = Number(newSeq);
        const bankId = seqBankData[oldSeq];
        seqBankData[newSeqNum] = bankId;
      }
    }

    const seqBanksDataVrom = this.addRawData(`${game}/seq_banks`, seqBankData, false);
    this.cg.define(`CUSTOM_SEQ_BANKS_${game.toUpperCase()}_VROM`, seqBanksDataVrom);
  }

  async addCustomExtractedFiles() {
    const cfiles = await customExtractedFiles(this.roms);
    for (const [name, data] of Object.entries(cfiles)) {
      const vrom = this.addRawData(null, data, true);
      this.cg.define('CUSTOM_' + name + '_ADDR', vrom);
    }

    /* Audio */
    const mmBase = 0x4d9f40;
    await this.extractSeqTable('oot', 0x6e, 0x102ae0, 0x29de0);
    await this.extractSeqTable('mm',  0x80, 0x13bb80, 0x46af0 + mmBase);

    await this.extractBankTable('oot', 0x26, 0x1026b0, 0xd390);
    await this.extractBankTable('mm',  0x29, 0x13b6d0, 0x20700 + mmBase);
    await this.extractCustomBankTable();

    await this.extractAudioTable('oot', 0x07, 0x1031d0, 0x79470);
    await this.extractAudioTable('mm',  0x03, 0x13c390, 0x97f70 + mmBase);

    await this.extractSeqBanks('oot', 0x6d, 0x1029f0);
    await this.extractSeqBanks('mm',  0x7f, 0x13ba64);
  }

  async run() {
    this.monitor.log("Building custom objects");

    /* Build custom objects */
    const customObjectsBuilder = new CustomObjectsBuilder(this.roms);
    const customObjects = await customObjectsBuilder.build();
    for (const co of customObjects) {
      await this.addCustomObject(co.name, co.data, co.offsets);

      if (!process.env.__IS_BROWSER__) {
        const outDir = path.resolve('build', 'custom');
        const outBasename = co.name.toLowerCase();
        const outFilename = path.resolve(outDir, `${outBasename}.zobj`);
        await fs.promises.mkdir(outDir, { recursive: true });
        await fs.promises.writeFile(outFilename, co.data);
      }
    }

    /* Build custom objects (legacy) */
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
    await this.addObjectFile('GI_POND_FISH', 'gi_pond_fish.zobj', [0x06001160]);
    await this.addObjectFile('BOMBCHU_BAG', 'bombchu_bag.zobj', [0x060006A0, 0x060008E0, 0x06001280]);
    await this.addObjectFile('MM_ADULT_LINK', 'mm_adult_link.zobj', [
      0x060122C4, 0x0600bb00, 0x0601c0c0, 0x0601c0d0, 0x0601c130, 0x0601BFE8, 0x0601BFF8, 0x0601C008,
      0x0601C028, 0x0601C048, 0x0601BFC8, 0x0601BFA8, 0x0601BEC8, 0x0601C0B0, 0x06010000, 0x0600AE40,
      0x0601DC68, 0x0601C068, 0x0601C080, 0x0601C098, 0x06010D50, 0x0600B3F0, 0x0601C0F0, 0x0601C100,
      0x0601C0E0, 0x0600A8E8, 0x060103D8, 0x0601C120, 0x0601C110, 0x0601be60
    ]);
    await this.addObjectFile('MM_ADULT_EPONA', 'mm_adult_epona.zobj', []);
    await this.addObjectFile('MM_ADULT_LINK_SPIN_ATTACK_VTX_1', 'mm_adult_link_spin_attack_vtx_1.bin', []);
    await this.addObjectFile('MM_ADULT_LINK_SPIN_ATTACK_VTX_2', 'mm_adult_link_spin_attack_vtx_2.bin', []);
    await this.addObjectFile('MM_ADULT_LINK_SPIN_ATTACK_VTX_3', 'mm_adult_link_spin_attack_vtx_3.bin', []);
    await this.addObjectFile('MM_ADULT_LINK_MASK_MTX', 'mm_adult_link_mask_mtx.bin', []);

    /* Add the object table */
    const objectTableBuffer = toU32Buffer(this.objectVroms.map(o => [o.vstart, o.vend]).flat());
    const objectTableVrom = this.addRawData(null, objectTableBuffer, true);
    this.cg.define('CUSTOM_OBJECT_TABLE_VROM', objectTableVrom);
    this.cg.define('CUSTOM_OBJECT_TABLE_SIZE', this.objectVroms.length);

    if (!process.env.__IS_BROWSER__) {
      await this.cg.emit();
    }

    return this.defines;
  }
}

export function custom(monitor: Monitor, roms: DecompressedRoms, patch: Patchfile) {
  const builder = new CustomAssetsBuilder(monitor, roms, patch);
  return builder.run();
}

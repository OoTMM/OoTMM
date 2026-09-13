import type { Game } from '@ootmm/data';
import type { DecompressedRoms } from '../decompress';

import path from 'node:path';
import fs from 'node:fs';
import { FILES } from '@ootmm/data';
import { Monitor } from '@ootmm/core';
import { concatUint8Arrays } from 'uint8array-extras';

import { DmaData } from '../dma';
import { arrayToIndexMap, toU32Buffer } from '../util';
import { CodeGen } from '../util/codegen';
import { KeepFile } from './keep';
import { png } from '../util/png';
import { font } from './font';
import { raw } from './raw';
import { Patchfile } from '../patch-build/patchfile';
import { grayscale } from '../image';
import { CustomObjectsBuilder } from './custom-objects-builder';
import { bufReadU32BE, bufWriteU32BE } from '../util/buffer';
import { ObjectEditor } from './object-editor';
import { patchAnimationPorts } from './custom-animation-builder';

const AGE_MODEL_CMD_END     = 0x00000000;
const AGE_MODEL_CMD_WRITE16 = 0x00000001;
const AGE_MODEL_CMD_WRITE32 = 0x00000002;
const AGE_MODEL_CMD_COPY32  = 0x00000003;

type AddedCustomObject = {
  objectId: number;
  defines: number[];
};

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
  ROCK: await extractFileData(roms, 'oot', 'objects/gameplay_field_keep', 0xa940, 32 * 32 * 2).then(t => grayscale(t, 'rgba16', 0.25)),
  FORKED_TREE_LEAVES: await extractFileData(roms, 'mm', 'objects/object_tree', 0x10b8, 32 * 32 * 2).then(t => grayscale(t, 'rgba16', 0.25)),
  //BUSH: await extractFileData(roms, 'oot', 'objects/object_wood02', 0x4790, 32 * 64).then(t => grayscale(t, 'ia8', 0.25)),
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
  CRATE_MAJOR: await png('crates/major', 'rgba16'),
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
  ADULT_MASK_ICON: await png('adult_mask_icon', 'rgba32'),
  ADULT_MASK_TEXT: await png('adult_mask_text', 'ia4'),
  GLITTER: await png('glitter', 'i4'),
  SONG_TAG_LULLABY: await png('song_tags/lullaby', 'ia8'),
  SONG_TAG_EPONA: await png('song_tags/epona', 'ia8'),
  SONG_TAG_SARIA: await png('song_tags/saria', 'ia8'),
  SONG_TAG_SUN: await png('song_tags/sun', 'ia8'),
  SONG_TAG_TIME: await png('song_tags/time', 'ia8'),
  SONG_TAG_STORMS: await png('song_tags/storms', 'ia8'),
  SONG_TAG_PAD: await png('song_tags/pad', 'ia8'),
  SONG_TAG_HEALING: await png('song_tags/healing', 'ia8'),
  SONG_TAG_SOARING: await png('song_tags/soaring', 'ia8'),
  SONG_TAG_AWAKENING: await png('song_tags/awakening', 'ia8'),
  SONG_TAG_GORON: await png('song_tags/goron', 'ia8'),
  SONG_TAG_GORON_HALF: await png('song_tags/goronHalf', 'ia8'),
  SONG_TAG_ZORA: await png('song_tags/zora', 'ia8'),
  SONG_TAG_EMPTINESS: await png('song_tags/emptiness', 'ia8'),
  SONG_TAG_ORDER: await png('song_tags/order', 'ia8'),
  SONG_TAG_MINUET: await png('song_tags/minuet', 'ia8'),
  SONG_TAG_BOLERO: await png('song_tags/bolero', 'ia8'),
  SONG_TAG_SERENADE: await png('song_tags/serenade', 'ia8'),
  SONG_TAG_NOCTURNE: await png('song_tags/nocturne', 'ia8'),
  SONG_TAG_REQUIEM: await png('song_tags/requiem', 'ia8'),
  SONG_TAG_PRELUDE: await png('song_tags/prelude', 'ia8'),
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
  DROP_DEKU_SEEDS_TEXTURE: await extractRaw(roms, 'oot', 'objects/gameplay_keep', 0x43650, 32 * 32 * 2,),
});

type ObjectRef = {
  vstart: number;
  vend: number;
}

const OOT_ADULT_EFFECTS: readonly number[] = [
  ...Array.from({ length: 28 }, (_, i) => i),
  55, 56, 60, 61, 67, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 134];

const adultEffectSlot = (effect: number) => OOT_ADULT_EFFECTS.indexOf(effect);
const readU16BE = (data: Uint8Array, off: number) => (data[off] << 8) | data[off + 1];

const writeU16BE = (data: Uint8Array, off: number, value: number) => {
  data[off] = value >>> 8;
  data[off + 1] = value;
};

const alignUp = (value: number, alignment: number) =>
    Math.ceil(value / alignment) * alignment;

const readSoundfont0 = (table: Uint8Array) => ({
  addr: bufReadU32BE(table, 0),
  size: bufReadU32BE(table, 4),
  sampleBank1: table[0x0A],
  sampleBank2: table[0x0B],
  numSfx: readU16BE(table, 0x0E),
});

class MutableBinary {
  private readonly bytes: number[];

  constructor(data: Uint8Array) {
    this.bytes = [...data];
  }

  align(alignment: number) {
    while (this.bytes.length % alignment) this.bytes.push(0);
  }

  alloc(size: number, alignment = 1) {
    this.align(alignment);
    const off = this.bytes.length;
    this.bytes.length += size;
    this.bytes.fill(0, off);
    return off;
  }

  write(off: number, data: Uint8Array) {
    data.forEach((v, i) => this.bytes[off + i] = v);
  }

  append(data: Uint8Array, alignment = 1) {
    const off = this.alloc(data.length, alignment);
    this.write(off, data);
    return off;
  }

  writeU8(off: number, value: number) {
    this.bytes[off] = value & 0xff;
  }

  writeU32BE(off: number, value: number) {
    for (let i = 0; i < 4; ++i)
      this.bytes[off + i] = (value >>> (24 - i * 8)) & 0xff;
  }

  finish() {
    return Uint8Array.from(this.bytes);
  }
}

const buildMmAdultVoiceHybridBank = async (roms: DecompressedRoms, mmBankTable: Uint8Array) => {
  const [ootBankTable, mmAudiobank, ootAudiobank] = await Promise.all([
    extractRaw(roms, 'oot', 'code', 0x1026B0, 0x26 * 0x10),
    getObjectBuffer(roms, 'mm', 'Audiobank'),
    getObjectBuffer(roms, 'oot', 'Audiobank'),
  ]);

  const mm = readSoundfont0(mmBankTable);
  const oot = readSoundfont0(ootBankTable);
  const mmFont = mmAudiobank.subarray(mm.addr, mm.addr + mm.size);
  const ootFont = ootAudiobank.subarray(oot.addr, oot.addr + oot.size);
  const mmSfxTable = bufReadU32BE(mmFont, 4);
  const ootSfxTable = bufReadU32BE(ootFont, 4);

  const firstSample = bufReadU32BE(ootFont, ootSfxTable + OOT_ADULT_EFFECTS[0] * 8);
  const sourceSelector = (ootFont[firstSample] >>> 2) & 3;
  const ootSampleBank = sourceSelector ? oot.sampleBank2 : oot.sampleBank1;
  const foreignSampleBank = 8 + ootSampleBank;

  const sampleBankIds = [mm.sampleBank1, mm.sampleBank2];
  const targetSampleSelector = sampleBankIds.lastIndexOf(0xff);

  if (targetSampleSelector < 0)
    throw new Error('MM Soundfont 0 has no free samplebank slot');

  sampleBankIds[targetSampleSelector] = foreignSampleBank;

  const adultEffectBase = alignUp(mm.numSfx, 0x40);
  const newNumSfx = adultEffectBase + OOT_ADULT_EFFECTS.length;
  const out = new MutableBinary(mmFont);
  const newSfxTable = out.alloc(newNumSfx * 8, 0x10);
  const mmEffect0 = mmFont.subarray(mmSfxTable, mmSfxTable + 8);

  out.write(newSfxTable, mmFont.subarray(mmSfxTable, mmSfxTable + mm.numSfx * 8));

  for (let i = mm.numSfx; i < adultEffectBase; ++i)
    out.write(newSfxTable + i * 8, mmEffect0);

  out.writeU32BE(4, newSfxTable);

  const sampleMap = new Map<number, number>();
  const loopMap = new Map<number, number>();
  const bookMap = new Map<number, number>();

  const cloneBlob = (
      map: Map<number, number>,
      src: number,
      size: number,
      alignment = 8
  ) => {
    const cached = map.get(src);
    if (cached !== undefined) return cached;

    const dst = out.append(ootFont.subarray(src, src + size), alignment);
    map.set(src, dst);
    return dst;
  };

  const cloneLoop = (src: number) =>
      src ? cloneBlob(loopMap, src, bufReadU32BE(ootFont, src + 8) ? 0x30 : 0x10) : 0;

  const cloneBook = (src: number) => {
    if (!src) return 0;
    const order = bufReadU32BE(ootFont, src);
    const predictors = bufReadU32BE(ootFont, src + 4);
    return cloneBlob(bookMap, src, 8 + 16 * order * predictors);
  };

  const cloneSample = (src: number): number => {
    const cached = sampleMap.get(src);
    if (cached !== undefined) return cached;

    const dst = out.append(ootFont.subarray(src, src + 0x10), 0x10);
    out.writeU8(dst, (ootFont[src] & ~0x0c) | (targetSampleSelector << 2));
    out.writeU32BE(dst + 8, cloneLoop(bufReadU32BE(ootFont, src + 8)));
    out.writeU32BE(dst + 0x0C, cloneBook(bufReadU32BE(ootFont, src + 0x0C)));

    sampleMap.set(src, dst);
    return dst;
  };

  for (const effect of OOT_ADULT_EFFECTS) {
    const src = ootSfxTable + effect * 8;
    const dst = newSfxTable + (adultEffectBase + adultEffectSlot(effect)) * 8;
    out.writeU32BE(dst, cloneSample(bufReadU32BE(ootFont, src)));
    out.write(dst + 4, ootFont.subarray(src + 4, src + 8));
  }

  out.align(0x10);

  return {
    data: out.finish(),
    sampleBankId1: sampleBankIds[0],
    sampleBankId2: sampleBankIds[1],
    numSfx: newNumSfx,
  };
};

type AseqTarget = number | string;

class AseqAppendBuilder {
  private readonly data: number[] = [];
  private readonly labels = new Map<string, number>();
  private readonly fixups: { offset: number; label: string }[] = [];

  constructor(private readonly base: number) {}

  label(name: string) {
    this.labels.set(name, this.base + this.data.length);
  }

  getLabel(name: string) {
    return this.labels.get(name)!;
  }

  u8(...values: number[]) {
    this.data.push(...values);
  }

  private ref(target: AseqTarget) {
    if (typeof target === 'string') {
      this.fixups.push({ offset: this.data.length, label: target });
      target = 0;
    }
    this.u8(target >>> 8, target);
  }

  opPtr(op: number, target: AseqTarget, arg?: number) {
    this.u8(op);
    if (arg !== undefined) this.u8(arg);
    this.ref(target);
  }

  finish() {
    const out = Uint8Array.from(this.data);
    for (const { offset, label } of this.fixups)
      writeU16BE(out, offset, this.getLabel(label));
    return out;
  }
}

type AdultNote = readonly [effect: number, ...args: number[]];

function patchMmAdultVoiceSequence(seq: Uint8Array, adultEffectBase: number): Uint8Array {
  const adultBlock = adultEffectBase >>> 6;
  const b = new AseqAppendBuilder(seq.length);

  const addRandomVoice = (
      name: string,
      effects: readonly number[],
      velocity: number | readonly number[]
  ) => {
    const velocities: readonly number[] =
        typeof velocity === 'number' ? Array(effects.length).fill(velocity) : velocity;
    const l = (s: string) => `${name}${s}`;

    b.label(name);
    b.u8(0xB8, effects.length, 0x77, 0x56);
    b.opPtr(0xFA, l('D'));
    b.u8(0x67);
    b.opPtr(0xFB, l('S'));

    b.label(l('D'));
    b.u8(0x67, 0xC8, 0xFF, 0x77, 0xC8, effects.length);
    b.opPtr(0xFA, l('W'));
    b.u8(0x67);
    b.opPtr(0xFB, l('S'));

    b.label(l('W'));
    b.u8(0xCC, 0x00, 0x77);

    b.label(l('S'));
    b.u8(0x76, 0x3E, 0x06, 0x3F, 0x06);
    b.opPtr(0xCB, l('E'));
    b.opPtr(0xC7, l('N'), 0x40);
    b.u8(0x67);
    b.opPtr(0xCB, l('V'));
    b.opPtr(0xC7, l('X'), 0x00);
    b.opPtr(0x88, l('L'));
    b.u8(0xFF);

    b.label(l('E')); b.u8(...effects.map(adultEffectSlot));
    b.label(l('V')); b.u8(...velocities);
    b.label(l('L')); b.u8(0xC2, adultBlock);
    b.label(l('N')); b.u8(0x40, 0x00);
    b.label(l('X')); b.u8(0x7F, 0xFF);
  };

  const addFixedVoice = (name: string, notes: readonly AdultNote[]) => {
    const layer = `${name}L`;

    b.label(name);
    b.opPtr(0x88, layer);
    b.u8(0xFF);

    b.label(layer);
    b.u8(0xC2, adultBlock);
    for (const [effect, ...args] of notes)
      b.u8(0x40 + adultEffectSlot(effect), ...args);
    b.u8(0xFF);
  };

  const randomVoices = [
    ['adultSwordN', [0, 1, 2, 3], 105],
    ['adultSwordL', [4, 5], 110],
    ['adultLash', [21, 22], 105],
    ['adultHang', [6, 25], [95, 105]],
    ['adultClimbEnd', [7, 8], [72, 80]],
    ['adultDamageS', [9, 10, 11], 117],
    ['adultFreeze', [12, 13, 14], 113],
    ['adultFallS', [17, 18], 100],
    ['adultFallL', [15, 16], 110],
    ['adultBreathRest', [19, 23], 90],
    ['adultTakenAway', [15, 16], 105],
  ] as const;

  const fixedVoices = [
    ['adultBreathDrink', [[56, 0x00, 96]]],
    ['adultDown', [[77, 0x57, 0x64], [78, 0x61, 0x64], [79, 0x47, 0x64]]],
    ['adultSneeze', [[80, 0x7F, 0x64], [81, 0x81, 0x18, 0x64], [82, 0x81, 0x3E, 0x64]]],
    ['adultSweat', [[83, 0x81, 0x22, 0x64], [84, 0x80, 0xA3, 0x64], [85, 0x35, 0x64]]],
    ['adultRelax', [[86, 0x80, 0xB9, 0x64], [87, 0x80, 0x86, 0x64], [88, 0x74, 0x64]]],
    ['adultSwordPutaway', [[0, 0x00, 100]]],
    ['adultGroan', [[67, 0x00, 50]]],
    ['adultMagicNale', [[5, 0x00, 110]]],
    ['adultSurprise', [[134, 0x00, 85]]],
    ['adultMagicFrol', [[4, 0x00, 95]]],
    ['adultPush', [[7, 0x00, 82]]],
    ['adultHookshotHang', [[6, 0x00, 95]]],
    ['adultLandDamage', [[24, 0x00, 110]]],
    ['adultNull1B', [[60, 0x00, 100]]],
    ['adultMagicAttack', [[61, 0x00, 110]]],
    ['adultDemoDamage', [[13, 0x00, 113]]],
  ] as const;

  for (const [name, effects, velocity] of randomVoices)
    addRandomVoice(name, effects, velocity);

  for (const [name, notes] of fixedVoices)
    addFixedVoice(name, notes);

  b.label('adultDrink');
  b.opPtr(0x88, 'adultDrinkL');
  b.u8(0xFF);
  b.label('adultDrinkL');
  b.u8(0xC2, adultBlock, 0x40 + adultEffectSlot(55), 0x50, 0x50, 0xF4, 0xFB);

  b.label('adultAutoJump');
  b.u8(0x66, 0xC8, 0xFF, 0xC9, 0x01, 0x77, 0x76, 0x3E, 0x06, 0x3F, 0x06);
  b.opPtr(0xCB, 'adultAutoJumpE');
  b.opPtr(0xC7, 'adultAutoJumpN', 0x40);
  b.u8(0xB8, 0x02);
  b.opPtr(0xCB, 'adultAutoJumpV');
  b.opPtr(0xC7, 'adultAutoJumpW', 0x00);
  b.opPtr(0x88, 'adultAutoJumpL');
  b.u8(0xFF);

  b.label('adultAutoJumpE'); b.u8(adultEffectSlot(26), adultEffectSlot(27));
  b.label('adultAutoJumpV'); b.u8(0x50, 0x55);
  b.label('adultAutoJumpL'); b.u8(0xC2, adultBlock);
  b.label('adultAutoJumpN'); b.u8(0x40, 0x00);
  b.label('adultAutoJumpW'); b.u8(0x50, 0xFF);

  const adultVoiceTargets = [
    'adultSwordN', 'adultSwordL', 'adultLash', 'adultHang', 'adultClimbEnd', 'adultDamageS', 'adultFreeze', 'adultFallS',
    'adultFallL', 'adultBreathRest', 'adultBreathDrink', 'adultDown', 'adultTakenAway', 'adultDamageS', 'adultSneeze', 'adultSweat',
    'adultDrink', 'adultRelax', 'adultSwordPutaway', 'adultGroan', 'adultAutoJump', 'adultMagicNale', 'adultSurprise', 'adultMagicFrol',
    'adultPush', 'adultHookshotHang', 'adultLandDamage', 'adultNull1B', 'adultMagicAttack', 'adultFallL', 'adultDemoDamage', 'adultSwordN',
  ] as const;

  const out = concatUint8Arrays([seq, b.finish()]);
  adultVoiceTargets.forEach((target, i) => writeU16BE(out, 0xB30E + i * 2, b.getLabel(target)));

  return out;
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

  async addCustomObject(name: string, data: Uint8Array, defines: number[]): Promise<AddedCustomObject> {
    const vrom = this.addRawData(`custom/${name.toLowerCase()}`, data, true);
    const objectId = this.addObjectEntry(vrom, data.length);
    this.cg.define('CUSTOM_OBJECT_ID_' + name, objectId);
    for (let i = 0; i < defines.length; ++i) {
      this.cg.define('CUSTOM_OBJECT_' + name + '_' + i, defines[i]);
    }
    return { objectId, defines };
  }

  async addObjectFile(name: string, filename: string, defines: number[]): Promise<AddedCustomObject> {
    return this.addCustomObject(name, await raw(filename), defines);
  }

  private extractMmCodeRamRange(ramStart: number, size: number): Uint8Array {
    const off = 0x00b3c000 + ramStart - 0x800a5ac0;
    return this.roms.mm.rom.slice(off, off + size);
  }

  private u32pair(a: number, b: number): Uint8Array {
    const data = new Uint8Array(8);
    bufWriteU32BE(data, 0, a);
    bufWriteU32BE(data, 4, b);
    return data;
  }

  async addHumanAgeProperties() {
    const AGE_BASE = 0x8085BA38;
    const AGE_SIZE = 0xDC;
    const PLAYER_ACTOR_VRAM = 0x8082DA90;
    const PLAYER_ACTOR_ROM = 0x00CA7F00;

    const read = (vram: number) => {
      const rom = PLAYER_ACTOR_ROM + vram - PLAYER_ACTOR_VRAM;
      const data = this.roms.mm.rom.slice(rom, rom + AGE_SIZE);

      if (rom < 0 || data.length !== AGE_SIZE) {
        throw new Error(
            `Player actor ROM read failed: ` +
            `vram=0x${vram.toString(16)}, ` +
            `rom=0x${rom.toString(16)}, ` +
            `size=0x${AGE_SIZE.toString(16)}, ` +
            `got=0x${data.length.toString(16)}`
        );
      }

      return data;
    };

    const childHuman = read(AGE_BASE + AGE_SIZE * 4);
    const adultHuman = new Uint8Array(AGE_SIZE);

    const view = new DataView(
        adultHuman.buffer,
        adultHuman.byteOffset,
        adultHuman.byteLength
    );

    const writeU16 = (offset: number, value: number) => {
      view.setUint16(offset, value & 0xffff, false);
    };

    const writeS16 = (offset: number, value: number) => {
      view.setInt16(offset, value, false);
    };

    const writeU32 = (offset: number, value: number) => {
      view.setUint32(offset, value >>> 0, false);
    };

    const writeF32 = (offset: number, value: number) => {
      view.setFloat32(offset, value, false);
    };

    const writeVec3s = (
        offset: number,
        value: readonly [number, number, number]
    ) => {
      writeS16(offset + 0x0, value[0]);
      writeS16(offset + 0x2, value[1]);
      writeS16(offset + 0x4, value[2]);
    };

    const writeVec3sArray = (
        offset: number,
        values: readonly (readonly [number, number, number])[]
    ) => {
      values.forEach((value, index) => {
        writeVec3s(offset + index * 6, value);
      });
    };

    // OoT adult Link PlayerAgeProperties.
    writeF32(0x00, 56.0);
    writeF32(0x04, 90.0);
    writeF32(0x08, 1.0);
    writeF32(0x0C, 111.0);
    writeF32(0x10, 70.0);
    writeF32(0x14, 79.4);
    writeF32(0x18, 59.0);
    writeF32(0x1C, 41.0);
    writeF32(0x20, 19.0);
    writeF32(0x24, 36.0);
    writeF32(0x28, 44.8);
    writeF32(0x2C, 56.0);
    writeF32(0x30, 68.0);
    writeF32(0x34, 70.0);
    writeF32(0x38, 18.0);
    writeF32(0x3C, 15.0);
    writeF32(0x40, 70.0);

    /* PlayerAgeProperties::unk_44 */
    writeVec3s(0x44, [9, 0x123F, 0x0167]);

    /* PlayerAgeProperties::unk_4A[4] */
    writeVec3sArray(0x4A, [
      [8, 0x1256, 0x017C],
      [9, 0x17EA, 0x0167],
      [8, 0x1256, 0x017C],
      [9, 0x17EA, 0x0167],
    ]);

    /* PlayerAgeProperties::unk_62[4] */
    writeVec3sArray(0x62, [
      [9, 0x17EA, 0x0167],
      [9, 0x1E0D, 0x017C],
      [9, 0x17EA, 0x0167],
      [9, 0x1E0D, 0x017C],
    ]);

    /* PlayerAgeProperties::unk_7A[4] */
    writeVec3sArray(0x7A, [
      [8, 0x1256, 0x017C],
      [9, 0x17EA, 0x0167],
      [-0x638, 0x1256, 0x017C],
      [-0x637, 0x17EA, 0x0167],
    ]);

    /* Adult voice, human surface sounds. */
    writeU16(0x92, 0x0000);
    writeU16(0x94, 0x0080);

    writeF32(0x98, 22.0);
    writeF32(0x9C, 36.0);

    const animations: readonly (readonly [number, number])[] = [
      [0xA0, 0x0400D540], // openChestAnim
      [0xA4, 0x0400D548],
      [0xA8, 0x0400D660],

      [0xAC, 0x0400DB90], // climb start A
      [0xB0, 0x0400DB98], // climb start B

      [0xB4, 0x0400DBA0], // climb up L
      [0xB8, 0x0400DBA8], // climb up R

      [0xBC, 0x0400DAB0], // climb side L
      [0xC0, 0x0400DAB8], // climb side R

      [0xC4, 0x0400DA90], // climb side L
      [0xC8, 0x0400DA98], // climb side R

      [0xCC, 0x0400DB70], // climb end A L
      [0xD0, 0x0400DB78], // climb end A R

      [0xD4, 0x0400DB88], // climb end B R
      [0xD8, 0x0400DB80], // climb end B L
    ];

    for (const [offset, address] of animations) {
      writeU32(offset, address);
    }

    this.cg.define(
        'CUSTOM_MM_CHILD_HUMAN_AGE_PROPERTIES_VROM',
        this.addRawData(
            'custom/mm_child_human_age_properties',
            childHuman,
            false
        )
    );

    this.cg.define(
        'CUSTOM_MM_ADULT_HUMAN_AGE_PROPERTIES_VROM',
        this.addRawData(
            'custom/mm_adult_human_age_properties',
            adultHuman,
            false
        )
    );
  }

  async addChildAgeModelTables() {
    const ranges = [
      [0x801bfe00, 0x4f8],
      [0x801c0d78, 0x20],
      [0x801c2730, 0x10],
      [0x801dca58, 0x14],
    ];

    const parts = ranges.flatMap(([addr, size]) => [
      this.u32pair(addr, size),
      this.extractMmCodeRamRange(addr, size),
    ]);
    parts.push(this.u32pair(0, 0));

    const data = concatUint8Arrays(parts);
    const vrom = this.addRawData('custom/mm_age_model_child_tables', data, false);

    this.cg.define('CUSTOM_MM_AGE_MODEL_CHILD_TABLES_VROM', vrom);
    this.cg.define('CUSTOM_MM_AGE_MODEL_CHILD_TABLES_SIZE', data.length);
  }

  async addAdultAgeModelTables(adultLink: AddedCustomObject) {
    const writes: { op: number; addr: number; value: number }[] = [];
    const { objectId: id, defines: d } = adultLink;

    const w = (op: number, addr: number, value: number) => writes.push({ op, addr, value });
    const w16 = (addr: number, value: number) => w(AGE_MODEL_CMD_WRITE16, addr, value);
    const w32 = (addr: number, value: number) => w(AGE_MODEL_CMD_WRITE32, addr, value);
    const copy32 = (addr: number, value: number) => w(AGE_MODEL_CMD_COPY32, addr, value);
    const w32a = (base: number, ids: number[]) => ids.forEach((i, n) => w32(base + n * 4, d[i]));

    w16(0x801c2738, id);      // playerFormObjectIds[HUMAN]
    w32(0x801bfe10, d[0]);   // playerSkeletons[HUMAN]
    w32a(0x801c001c, [1, 1]); // playerWaistDLs
    w32a(0x801c0024, [2, 2, 3, 3]); // playerHandHoldingShieldDLs
    w32a(0x801c0054, [4, 4]); // playerSheath12DLs
    w32a(0x801c007c, [4, 4]); // playerSheath13DLs
    w32a(0x801c00a4, [4, 4]); // playerSheath14DLs
    w32a(0x801c00ac, [5, 5, 6, 6]); // playerShieldDLs
    w32a(0x801c00bc, [7, 7, 8, 8, 9, 9]); // playerSheathedSwordDLs
    w32a(0x801c00d4, [10, 10, 11, 11, 12, 12]); // playerSwordSheathsDLs
    w32a(0x801c010c, [13, 13]); // playerLeftHandTwoHandSwordDLs
    w32a(0x801c0134, [14, 14]); // playerLeftHandOpenDLs
    w32a(0x801c015c, [15, 15]); // playerLeftHandClosedDLs
    w32a(0x801c0184, [16, 16]); // playerLeftHandOneHandSwordDLs
    w32a(0x801c018c, [17, 17, 18, 18, 19, 19]); // playerEquipValueDLs
    w32a(0x801c01c4, [20, 20]); // playerRightHandOpenDLs
    w32a(0x801c01ec, [21, 21]); // playerRightHandClosedDLs
    w32a(0x801c0214, [22, 22]); // playerRightHandBowDLs
    w32a(0x801c023c, [23, 23]); // playerRightHandInstrumentDLs
    w32a(0x801c0264, [24, 24]); // playerRightHandHookshotDLs
    w32a(0x801c028c, [25, 25]); // playerLeftHandBottleDLs
    w32(0x801c02a4, d[4]);  // playerFirstPersonLeftForearmDLs[HUMAN]
    w32(0x801c02b8, d[15]); // playerFirstPersonLeftHandDLs[HUMAN]
    w32(0x801c02cc, d[26]); // playerFirstPersonRightShoulderDLs[HUMAN]
    w32(0x801c02e0, d[27]); // playerFirstPersonRightHandDLs[HUMAN]
    w32(0x801c02f4, d[28]); // playerFirstPersonRightHandHookshotDLs[HUMAN]
    w32(0x801c0d94, d[29]); // Extra bow hand DL pointer
    w32(0x801c0d7c, 0x457a0000); // meleeWeaponLengths[1] = 4000.0f
    w32(0x801c0d80, 0x457a0000); // meleeWeaponLengths[2] = 4000.0f
    w32(0x801c0d84, 0x45abe000); // meleeWeaponLengths[3] = 5500.0f
    w32(0x801c0d88, 0x45abe000); // meleeWeaponLengths[4] = 5500.0f
    copy32(0x801dca68, 0x801dca60); // playerHeightJtbl[HUMAN] = 68.0f

    const data = new Uint8Array((writes.length + 1) * 0x0c);
    [...writes, { op: AGE_MODEL_CMD_END, addr: 0, value: 0 }].forEach(({ op, addr, value }, i) => {
      const off = i * 0x0c;
      bufWriteU32BE(data, off + 0x00, op);
      bufWriteU32BE(data, off + 0x04, addr);
      bufWriteU32BE(data, off + 0x08, value);
    });

    const vrom = this.addRawData('custom/mm_age_model_tables', data, false);
    this.cg.define('CUSTOM_MM_AGE_MODEL_TABLES_VROM', vrom);
    this.cg.define('CUSTOM_MM_AGE_MODEL_TABLES_SIZE', data.length);
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

    let mmAudioseq: Uint8Array | undefined;
    let adultEffectBase = 0;
    if (game === 'mm') {
      const [audioseq, mmBankTable] = await Promise.all([
        getObjectBuffer(this.roms, 'mm', 'Audioseq'),
        extractRaw(this.roms, 'mm', 'code', 0x13B6D0, 0x29 * 0x10),
      ]);
      mmAudioseq = audioseq;
      adultEffectBase = alignUp(readSoundfont0(mmBankTable).numSfx, 0x40);
    }

    for (let i = 0; i < count; ++i) {
      let addr = bufReadU32BE(seqTableDataOrig, i * 0x10);
      let size = bufReadU32BE(seqTableDataOrig, i * 0x10 + 4);
      if (!size) {
        size = bufReadU32BE(seqTableDataOrig, addr * 0x10 + 4);
        addr = bufReadU32BE(seqTableDataOrig, addr * 0x10);
      }

      if (game === 'mm' && i === 0) {
        const seq = mmAudioseq!.slice(addr, addr + size);
        const data = patchMmAdultVoiceSequence(seq, adultEffectBase);
        addr = this.addRawData('mm/seq_0_oot_adult_voice', data, false);
        size = data.length;
      } else
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

    if (game === 'mm') {
      const hybrid = await buildMmAdultVoiceHybridBank(this.roms, dataOrig);
      const hybridVrom = this.addRawData('mm/bank_0_oot_adult_voice', hybrid.data, false);
      bufWriteU32BE(dataPatched, 0x00, hybridVrom);
      bufWriteU32BE(dataPatched, 0x04, hybrid.data.length);
      dataPatched[0x0A] = hybrid.sampleBankId1;
      dataPatched[0x0B] = hybrid.sampleBankId2;
      writeU16BE(dataPatched, 0x0E, hybrid.numSfx);
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

    /* Build custom animations */
    if (!process.env.__IS_BROWSER__) {
      await patchAnimationPorts(this.roms);
    }

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

    /* Load custom objects */
    await this.addObjectFile('TRIFORCE', 'triforce.zobj', [0x06000a30]);
    await this.addObjectFile('SKELETON_KEY', 'skeleton_key.zobj', [0x06001f70]);
    await this.addObjectFile('BTN_A', 'btn_a.zobj', [0x06000da0]);
    await this.addObjectFile('BTN_C_HORIZONTAL', 'btn_c_horizontal.zobj', [0x06000e10]);
    await this.addObjectFile('BTN_C_VERTICAL', 'btn_c_vertical.zobj', [0x06000960]);
    await this.addObjectFile('GI_POND_FISH', 'gi_pond_fish.zobj', [0x06001160]);
    await this.addObjectFile('BOMBCHU_BAG', 'bombchu_bag.zobj', [0x060006A0, 0x060008E0, 0x06001280]);
    const mmAdultLink = await this.addObjectFile('MM_ADULT_LINK', 'mm_adult_link.zobj', [
      0x060122C4, 0x0600bb00, 0x0601c0c0, 0x0601c0d0, 0x0601c130, 0x0601BFE8, 0x0601BFF8, 0x0601C008,
      0x0601C028, 0x0601C048, 0x0601BFC8, 0x0601BFA8, 0x0601BEC8, 0x0601C0B0, 0x06010000, 0x0600AE40,
      0x0601DC68, 0x0601C068, 0x0601C080, 0x0601C098, 0x06010D50, 0x0600B3F0, 0x0601C0F0, 0x0601C100,
      0x0601C0E0, 0x0600A8E8, 0x060103D8, 0x0601C120, 0x0601C110, 0x0601be60
    ]);
    await this.addChildAgeModelTables();
    await this.addAdultAgeModelTables(mmAdultLink);
    await this.addHumanAgeProperties();
    await this.addObjectFile('MM_ADULT_EPONA', 'mm_adult_epona.zobj', []);
    await this.addObjectFile('MM_ADULT_LINK_SPIN_ATTACK_VTX_1', 'mm_adult_link_spin_attack_vtx_1.bin', []);
    await this.addObjectFile('MM_ADULT_LINK_SPIN_ATTACK_VTX_2', 'mm_adult_link_spin_attack_vtx_2.bin', []);
    await this.addObjectFile('MM_ADULT_LINK_SPIN_ATTACK_VTX_3', 'mm_adult_link_spin_attack_vtx_3.bin', []);
    await this.addObjectFile('MM_ADULT_LINK_MASK_MTX', 'mm_adult_link_mask_mtx.bin', []);
    await this.addObjectFile('MASK_ADULT', 'object_gi_maskadult.zobj', [0x060009B0, 0x06000B90,]);
    await this.addObjectFile('MASK_ADULT_TRANSFORM_PLAYER', 'object_mask_adult.zobj', [0x0a000900,]);
    await this.addObjectFile('ADULT_MASK_EQUIPMENT', 'adult_mask_equipment_standalone.zobj', [0x0a000920,]);

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

import type { CustomAnimation, Game } from '@ootmm/data';
import type { DecompressedRoms } from '../decompress.ts';

import { DATA_ANIMATIONS, FILES as GAME_FILES } from '@ootmm/data';

import { DmaData } from '../dma.ts';
import { arrayToIndexMap } from '../util.ts';
import { CodeGen } from '../util/codegen.ts';

type AnimationDef = {
  name: string;
  home_game: Game;
  frame_data_offset: number | string;
  header_offset: number | string;
  frame_data_size?: number | string;
  frame_count?: number | string;
};

type SourcePlayerAnimHeader = {
  frameCount: number;
  frameDataSegmented: number;
};

type ImportedAnimation = {
  def: AnimationDef;
  srcGame: Game;
  dstGame: Game;
  frameCount: number;
  frameDataOffset: number;
  frameSize: number;
};

const PLAYER_LIMB_COUNT = 0x16;
const VEC3S_SIZE = 6;
const PLAYER_ANIM_FRAME_SIZE = PLAYER_LIMB_COUNT * VEC3S_SIZE + 2;
const IMPORTED_LINK_ANIM_SEGMENT = 0x08;

const FILES_TO_INDEX = {
  oot: arrayToIndexMap(GAME_FILES.oot),
  mm: arrayToIndexMap(GAME_FILES.mm),
};

function assertGame(game: string, label: string): asserts game is Game {
  if (game !== 'oot' && game !== 'mm') {
    throw new Error(`${label}: invalid game "${game}", expected "oot" or "mm"`);
  }
}

function assertRange(file: string, buf: Uint8Array, off: number, size: number) {
  if (off < 0 || size < 0 || off + size > buf.length) {
    throw new Error(
      `${file}: range ${hex(off)}..${hex(off + size)} exceeds file size ${hex(buf.length)}`
    );
  }
}

function parseNum(value: number | string | undefined, label: string): number {
  if (value === undefined || value === null) {
    throw new Error(`Missing required animation field: ${label}`);
  }

  if (typeof value === 'number') {
    if (!Number.isFinite(value)) {
      throw new Error(`Invalid ${label}: ${value}`);
    }

    return value;
  }

  const text = value.trim();
  const isHex = text.startsWith('0x') || text.startsWith('0X');

  if (isHex && !/^0x[0-9a-fA-F]+$/.test(text)) {
    throw new Error(`Invalid ${label}: ${value}`);
  }

  if (!isHex && !/^[0-9]+$/.test(text)) {
    throw new Error(`Invalid ${label}: ${value}`);
  }

  const parsed = Number.parseInt(text, isHex ? 16 : 10);

  if (!Number.isFinite(parsed)) {
    throw new Error(`Invalid ${label}: ${value}`);
  }

  return parsed;
}

function otherGame(game: Game): Game {
  return game === 'oot' ? 'mm' : 'oot';
}

function hex(n: number, width = 0): string {
  const s = (n >>> 0).toString(16).toUpperCase();
  return `0x${width ? s.padStart(width, '0') : s}`;
}

function readU16BE(buf: Uint8Array, off: number): number {
  return (buf[off] << 8) | buf[off + 1];
}

function readU32BE(buf: Uint8Array, off: number): number {
  return (
    ((buf[off] << 24) |
    (buf[off + 1] << 16) |
    (buf[off + 2] << 8) |
    buf[off + 3]) >>> 0
  );
}

function findFileKey(game: Game, wanted: string): string {
  const keys = Object.keys(FILES_TO_INDEX[game]);
  const exact = keys.find((key) => key === wanted);

  if (exact) {
    return exact;
  }

  const basename = wanted.split('/').pop();
  const byBasename = keys.find((key) => key.split('/').pop() === basename);

  if (byBasename) {
    return byBasename;
  }

  const candidates = keys.filter((key) => key.toLowerCase().includes('anim'));

  throw new Error(
    `${game}: file not found in file table: ${wanted}\n` +
    `Animation-ish file keys:\n${candidates.join('\n')}`
  );
}

function getFileFromRom(roms: DecompressedRoms, game: Game, file: string): Uint8Array {
  const key = findFileKey(game, file);
  const index = FILES_TO_INDEX[game][key];
  const dma = new DmaData(roms[game].dma);
  const entry = dma.read(index);

  return roms[game].rom.slice(entry.virtStart, entry.virtEnd);
}

function parseSourcePlayerAnimHeader(buf: Uint8Array, off: number): SourcePlayerAnimHeader {
  assertRange('gameplay_keep', buf, off, 8);

  return {
    frameCount: readU16BE(buf, off + 0x00),
    frameDataSegmented: readU32BE(buf, off + 0x04),
  };
}

function buildImportedAnimation(roms: DecompressedRoms, def: CustomAnimation): ImportedAnimation {
  const srcGame = def.home_game;
  const dstGame = otherGame(srcGame);
  const frameDataOffset = parseNum(def.frame_data_offset, `${def.name}.frame_data_offset`);
  const headerOffset = parseNum(def.header_offset, `${def.name}.header_offset`);
  const srcGameplayKeep = getFileFromRom(roms, srcGame, 'objects/gameplay_keep');
  const srcLinkAnim = getFileFromRom(roms, srcGame, 'link_animetion');
  const sourceHeader = parseSourcePlayerAnimHeader(srcGameplayKeep, headerOffset);
  const sourceFrameDataOffset = sourceHeader.frameDataSegmented & 0x00ffffff;
  const frameCount = sourceHeader.frameCount;
  const frameDataSize = sourceHeader.frameCount * PLAYER_ANIM_FRAME_SIZE;
  const expectedFrameDataSize = sourceHeader.frameCount * PLAYER_ANIM_FRAME_SIZE;

  if (sourceHeader.frameCount <= 0) {
    throw new Error(`${def.name}: invalid frame count ${sourceHeader.frameCount}`);
  }

  if (sourceFrameDataOffset !== frameDataOffset) {
    throw new Error(
      `${def.name}: header points to ${hex(sourceFrameDataOffset)}, ` +
      `but YAML frame_data_offset is ${hex(frameDataOffset)}`
    );
  }

  if (frameCount !== sourceHeader.frameCount) {
    throw new Error(
      `${def.name}: direct foreign DMA does not support resampling; ` +
      `frame_count ${frameCount} must match source frame count ${sourceHeader.frameCount}`
    );
  }

  if (frameDataSize !== expectedFrameDataSize) {
    throw new Error(
      `${def.name}: direct foreign DMA requires full contiguous frame data; ` +
      `frame_data_size ${hex(frameDataSize)} must equal ${hex(expectedFrameDataSize)}`
    );
  }

  assertRange(`${srcGame}:link_animetion`, srcLinkAnim, frameDataOffset, frameDataSize);

  return {
    def,
    srcGame,
    dstGame,
    frameCount,
    frameDataOffset,
    frameSize: PLAYER_ANIM_FRAME_SIZE,
  };
}

function buildImportedAnimationPack(roms: DecompressedRoms): ImportedAnimation[] {
  return DATA_ANIMATIONS.map(anim => buildImportedAnimation(roms, anim));
}

function defineBaseForAnimation(name: string): string {
  return `COMBO_IMPORTED_ANIM_${name.replace(/[^a-zA-Z0-9]/g, '_').toUpperCase()}`;
}

function cSymbolForAnimation(name: string): string {
  const symbol = name.replace(/[^a-zA-Z0-9_]/g, '_');

  if (!/^[a-zA-Z_][a-zA-Z0-9_]*$/.test(symbol)) {
    throw new Error(`${name}: cannot convert animation name to valid C symbol`);
  }

  return symbol;
}

function pushAnimationListMacro(cg: CodeGen, macroName: string, entries: ImportedAnimation[]) {
  const slash = '\\';

  if (entries.length === 0) {
    cg.raw(`#define ${macroName}(_)`);
    return;
  }

  cg.raw(`#define ${macroName}(_) ${slash}`);

  for (let i = 0; i < entries.length; i++) {
    const entry = entries[i];
    const base = defineBaseForAnimation(entry.def.name);
    const symbol = cSymbolForAnimation(entry.def.name);
    const continuation = i + 1 === entries.length ? '' : ` ${slash}`;

    cg.raw(
      `    _(${symbol}, ${base}_SOURCE_OFFSET, ${base}_FRAMECOUNT, ${base}_FRAMESIZE)${continuation}`
    );
  }
}

async function emitImportedAnimationsHeader(roms: DecompressedRoms, pack: ImportedAnimation[]) {
  const ootLinkAnimIndex = FILES_TO_INDEX.oot[findFileKey('oot', 'link_animetion')];
  const mmLinkAnimIndex = FILES_TO_INDEX.mm[findFileKey('mm', 'link_animetion')];
  const ootLinkAnimDma = new DmaData(roms.oot.dma).read(ootLinkAnimIndex);
  const mmLinkAnimDma = new DmaData(roms.mm.dma).read(mmLinkAnimIndex);
  const cg = new CodeGen('include/combo/custom_animations.h', 'COMBO_IMPORTED_ANIMATIONS_H');

  cg.include('combo.h');
  cg.define('COMBO_IMPORTED_LINK_ANIM_SEGMENT', IMPORTED_LINK_ANIM_SEGMENT);
  cg.define('COMBO_IMPORTED_LINK_ANIM_FRAME_SIZE', PLAYER_ANIM_FRAME_SIZE);
  cg.define('COMBO_LINK_ANIMETION_OOT_VROM', ootLinkAnimDma.virtStart);
  cg.define('COMBO_LINK_ANIMETION_MM_VROM', mmLinkAnimDma.virtStart);
  cg.raw('typedef struct ComboImportedPlayerAnimationHeader {');
  cg.raw('    u16 frameCount;');
  cg.raw('    s16 frameSize;');
  cg.raw('    u32 linkAnimSegment;');
  cg.raw('} ComboImportedPlayerAnimationHeader;');
  cg.raw('');

  for (const entry of pack) {
    const base = defineBaseForAnimation(entry.def.name);
    const totalSize = entry.frameCount * entry.frameSize;

    cg.raw(`#define ${base}_FRAMECOUNT ${entry.frameCount}`);
    cg.raw(`#define ${base}_FRAMESIZE ${hex(entry.frameSize)}`);
    cg.raw(`#define ${base}_TOTALSIZE ${hex(totalSize)}`);
    cg.raw(`#define ${base}_SOURCE_OFFSET ${hex(entry.frameDataOffset)}`);
    cg.raw(`#define ${base}_SRC_${entry.srcGame.toUpperCase()} 1`);
    cg.raw(`#define ${base}_DST_${entry.dstGame.toUpperCase()} 1`);
    cg.raw('');
  }

  pushAnimationListMacro(
    cg,
    'COMBO_IMPORTED_LINK_ANIMATION_LIST_OOT',
    pack.filter((entry) => entry.dstGame === 'oot')
  );
  cg.raw('');

  pushAnimationListMacro(
    cg,
    'COMBO_IMPORTED_LINK_ANIMATION_LIST_MM',
    pack.filter((entry) => entry.dstGame === 'mm')
  );
  cg.raw('');

  cg.raw('#define COMBO_IMPORTED_ANIM_EXTERN(symbol, sourceOffset, frameCount, frameSize) \\');
  cg.raw('    extern const ComboImportedPlayerAnimationHeader symbol;');
  cg.raw('');
  cg.raw('#if defined(GAME_OOT)');
  cg.raw('COMBO_IMPORTED_LINK_ANIMATION_LIST_OOT(COMBO_IMPORTED_ANIM_EXTERN)');
  cg.raw('#elif defined(GAME_MM)');
  cg.raw('COMBO_IMPORTED_LINK_ANIMATION_LIST_MM(COMBO_IMPORTED_ANIM_EXTERN)');
  cg.raw('#endif');
  cg.raw('');
  cg.raw('#undef COMBO_IMPORTED_ANIM_EXTERN');
  cg.raw('');

  cg.raw('#if defined(GAME_OOT)');
  cg.raw('# define COMBO_LINK_ANIMETION_VROM COMBO_LINK_ANIMETION_OOT_VROM');
  cg.raw('# define COMBO_IMPORTED_LINK_ANIMATION_LIST(_) COMBO_IMPORTED_LINK_ANIMATION_LIST_OOT(_)');
  cg.raw('#elif defined(GAME_MM)');
  cg.raw('# define COMBO_LINK_ANIMETION_VROM COMBO_LINK_ANIMETION_MM_VROM');
  cg.raw('# define COMBO_IMPORTED_LINK_ANIMATION_LIST(_) COMBO_IMPORTED_LINK_ANIMATION_LIST_MM(_)');
  cg.raw('#else');
  cg.raw('# define COMBO_IMPORTED_LINK_ANIMATION_LIST(_)');
  cg.raw('#endif');
  cg.raw('');

  await cg.emit();
}

export async function patchAnimationPorts(roms: DecompressedRoms) {
  const pack = buildImportedAnimationPack(roms);
  await emitImportedAnimationsHeader(roms, pack);
}

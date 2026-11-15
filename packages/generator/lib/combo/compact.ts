import { DATA_ANIMATIONS, FILES, GAMES, type Game } from '@ootmm/data';
import type { DecompressedRoms } from './decompress';
import type { RomBuilder, RomFile } from './rom-builder';
import type { Patchfile } from './patch-build/patchfile';
import { DmaData } from './dma';
import { align, arrayToIndexMap } from './util';
import { bufEqual, bufReadU16BE, bufReadU32BE, bufWriteU32BE } from './util/buffer';

function hashBytes(data: Uint8Array): number {
  let h = 0x811c9dc5;
  for (let i = 0; i < data.length; ++i) {
    h ^= data[i];
    h = Math.imul(h, 0x01000193);
  }
  return h >>> 0;
}

const FILES_TO_INDEX = {
  oot: arrayToIndexMap(FILES.oot),
  mm: arrayToIndexMap(FILES.mm),
};

export function gameFileDma(roms: DecompressedRoms, game: Game, name: string) {
  const indices = FILES_TO_INDEX[game];
  if (!Object.hasOwn(indices, name))
    throw new Error(game + ': file not found in file table: ' + name);
  return new DmaData(roms[game].dma).read(indices[name]);
}

export function getDecompressedGameFile(roms: DecompressedRoms, game: Game, name: string): Uint8Array {
  const entry = gameFileDma(roms, game, name);
  return roms[game].rom.subarray(entry.virtStart, entry.virtEnd);
}

export const PLAYER_ANIM_FRAME_SIZE = 0x16 * 6 + 2;

const NATIVE_SEGMENT = 0x07;

const FOREIGN_SEGMENT = 0x08;

const MM_NON_ANIMATION_HEADERS = [0x58fe0, 0x59050, 0x590b0];

type PlayerAnimHeader = { headerOffset: number; dataOffset: number; data: Uint8Array };

type PlayerAnimSite = { data: Uint8Array; offset: number; relative: number };

class PlayerAnimationPool {
  private readonly chunks: Uint8Array[] = [];
  private readonly frameIndex = new Map<number, PlayerAnimSite[]>();
  private size = 0;

  find(data: Uint8Array): number | undefined {
    if (!data.length || data.length % PLAYER_ANIM_FRAME_SIZE) return undefined;
    const key = hashBytes(data.subarray(0, PLAYER_ANIM_FRAME_SIZE));
    for (const site of this.frameIndex.get(key) ?? []) {
      if (site.data.length - site.relative < data.length) continue;
      if (!bufEqual(site.data.subarray(site.relative, site.relative + data.length), data)) continue;
      return site.offset + site.relative;
    }
    return undefined;
  }

  append(data: Uint8Array) {
    const match = this.find(data);
    if (match !== undefined) return match;

    const offset = this.size;
    const copy = new Uint8Array(align(data.length, 16));
    copy.set(data);
    this.chunks.push(copy);
    this.size += copy.length;

    this.index(copy.subarray(0, data.length), offset);
    return offset;
  }

  index(data: Uint8Array, offset: number) {
    for (let relative = 0; relative + PLAYER_ANIM_FRAME_SIZE <= data.length; relative += PLAYER_ANIM_FRAME_SIZE) {
      const key = hashBytes(data.subarray(relative, relative + PLAYER_ANIM_FRAME_SIZE));
      const sites = this.frameIndex.get(key);
      const site = { data, offset, relative };
      if (sites) sites.push(site);
      else this.frameIndex.set(key, [site]);
    }
  }

  finish() {
    const out = new Uint8Array(this.size);
    let offset = 0;
    for (const chunk of this.chunks) {
      out.set(chunk, offset);
      offset += chunk.length;
    }
    return out;
  }
}

export type PlayerAnimationDedupePlan = {
  mmLinkAnimData: Uint8Array;
  mmHeaderPointers: ReadonlyMap<number, number>;
  ootLocalOffsets: ReadonlySet<number>;
  mmLocalOffsetMap: ReadonlyMap<number, number>;
};

function parseNum(value: number | string | undefined, label: string) {
  if (value === undefined || value === null) throw new Error(`Missing ${label}`);
  if (typeof value === 'number') return value;
  const text = value.trim();
  const n = Number.parseInt(text, /^0x/i.test(text) ? 16 : 10);
  if (!Number.isFinite(n)) throw new Error(`Invalid ${label}: ${value}`);
  return n;
}

function scanPlayerAnimationHeaders(gameplayKeep: Uint8Array, linkAnim: Uint8Array): PlayerAnimHeader[] {
  const out: PlayerAnimHeader[] = [];
  for (let off = 0; off + 8 <= gameplayKeep.length; off += 8) {
    const frameCount = bufReadU16BE(gameplayKeep, off);
    const ptr = bufReadU32BE(gameplayKeep, off + 4);
    const dataOffset = ptr & 0x00ffffff;
    if ((ptr >>> 24) !== NATIVE_SEGMENT || bufReadU16BE(gameplayKeep, off + 2) || !frameCount || frameCount > 0x1000) continue;
    const size = frameCount * PLAYER_ANIM_FRAME_SIZE;
    if (dataOffset + size > linkAnim.length) continue;
    out.push({ headerOffset: off, dataOffset, data: linkAnim.subarray(dataOffset, dataOffset + size) });
  }
  return out;
}

const segmented = (segment: number, offset: number) => ((segment << 24) | offset) >>> 0;

function uniquePlayerSources(headers: PlayerAnimHeader[]) {
  const byOffset = new Map<number, Uint8Array>();
  for (const header of headers) {
    const current = byOffset.get(header.dataOffset);
    if (!current || header.data.length > current.length) byOffset.set(header.dataOffset, header.data);
  }
  return [...byOffset]
    .map(([dataOffset, data]) => ({ dataOffset, data }))
    .sort((a, b) => b.data.length - a.data.length || a.dataOffset - b.dataOffset);
}

function buildLocalPool(headers: PlayerAnimHeader[]) {
  const pool = new PlayerAnimationPool();
  const oldToNew = new Map<number, number>();
  for (const source of uniquePlayerSources(headers))
    oldToNew.set(source.dataOffset, pool.append(source.data));
  return { data: pool.finish(), oldToNew };
}

function assertHeaderCount(game: Game, count: number) {
  const [min, max] = game === 'oot' ? [500, 800] : [600, 900];
  if (count < min || count > max)
    throw new Error(`Unexpected ${game === 'oot' ? 'OoT' : 'MM'} player animation header count: ${count}`);
}

function buildMmState(headers: PlayerAnimHeader[], ootPool: PlayerAnimationPool) {
  const pinned = new Set<number>();
  for (const anim of DATA_ANIMATIONS)
    if (anim.home_game === 'mm')
      pinned.add(parseNum(anim.frame_data_offset, anim.name + '.frame_data_offset'));
  const pointers = new Map<number, number>();
  const localHeaders: PlayerAnimHeader[] = [];
  for (const header of headers) {
    const match = pinned.has(header.dataOffset) ? undefined : ootPool.find(header.data);
    pointers.set(header.headerOffset, match === undefined ? 0 : segmented(FOREIGN_SEGMENT, match));
    if (match === undefined) localHeaders.push(header);
  }

  const local = buildLocalPool(localHeaders);
  for (const header of localHeaders) {
    const offset = local.oldToNew.get(header.dataOffset);
    if (offset === undefined) throw new Error('Missing MM player animation 0x' + header.dataOffset.toString(16));
    pointers.set(header.headerOffset, segmented(NATIVE_SEGMENT, offset));
  }
  return { ...local, pointers };
}

export function buildPlayerAnimationDedupePlan(roms: DecompressedRoms): PlayerAnimationDedupePlan {
  const ootKeep = getDecompressedGameFile(roms, 'oot', 'objects/gameplay_keep');
  const mmKeep = getDecompressedGameFile(roms, 'mm', 'objects/gameplay_keep');
  const ootLinkAnim = getDecompressedGameFile(roms, 'oot', 'link_animetion');
  const mmLinkAnim = getDecompressedGameFile(roms, 'mm', 'link_animetion');
  const ootHeaders = scanPlayerAnimationHeaders(ootKeep, ootLinkAnim);
  const mmHeaders = scanPlayerAnimationHeaders(mmKeep, mmLinkAnim).filter(header =>
    !MM_NON_ANIMATION_HEADERS.includes(header.headerOffset));
  assertHeaderCount('oot', ootHeaders.length);
  assertHeaderCount('mm', mmHeaders.length);

  const ootPool = new PlayerAnimationPool();
  const ootLocalOffsets = new Set<number>();
  for (const { data, dataOffset } of uniquePlayerSources(ootHeaders)) {
    ootPool.index(data, dataOffset);
    ootLocalOffsets.add(dataOffset);
  }
  const mm = buildMmState(mmHeaders, ootPool);
  return {
    mmLinkAnimData: mm.data,
    mmHeaderPointers: mm.pointers,
    ootLocalOffsets,
    mmLocalOffsetMap: mm.oldToNew,
  };
}

export function installCompactPlayerAnimations(patch: Patchfile, plan: PlayerAnimationDedupePlan) {
  if (GAMES.some(game => patch.symbols[game].get('COMPACT_LINK_ANIM_VROMS')?.length !== 1))
    throw new Error('Rebuild the payload for compact player animations');
  const addresses = new Uint8Array(8);
  const start = 0xee000000;
  const end = start + align(plan.mmLinkAnimData.length, 16);
  if (end > 0xf0000000 || patch.newFiles.some(file =>
    file.vrom < end && file.vrom + align(file.data.length, 16) > start))
    throw new Error('Compact animation bank overlaps an existing ROM file');
  patch.addNewFile({
    name: 'custom/mm_compact_link_anim', vrom: start,
    data: plan.mmLinkAnimData, compressed: false
  });
  patch.removedFiles.push('mm/link_animetion');
  bufWriteU32BE(addresses, 4, start);
  for (const [offset, pointer] of plan.mmHeaderPointers) {
    const data = new Uint8Array(4);
    bufWriteU32BE(data, 0, pointer);
    patch.addPatch('mm/objects/gameplay_keep', offset + 4, data);
  }
  patch.addSymbolPatch('COMPACT_LINK_ANIM_VROMS', addresses);
}

export function remapImportedPlayerAnimationOffset(plan: PlayerAnimationDedupePlan, game: Game, oldOffset: number) {
  const mapped = game === 'oot'
    ? (plan.ootLocalOffsets.has(oldOffset) ? oldOffset : undefined)
    : plan.mmLocalOffsetMap.get(oldOffset);
  if (mapped === undefined)
    throw new Error(`${game.toUpperCase()} imported player animation 0x${oldOffset.toString(16)} was not retained in compact link_animetion`);
  return mapped;
}

const MUTATED_OR_LAYOUT_SENSITIVE = new Set([
  'oot/makerom', 'oot/boot', 'oot/dmadata', 'oot/Audiobank', 'oot/Audioseq', 'oot/Audiotable',
  'mm/makerom', 'mm/boot', 'mm/dmadata', 'mm/Audiobank', 'mm/Audioseq', 'mm/Audiotable',
  'oot/seq_table', 'oot/bank_table', 'oot/audio_table', 'oot/seq_banks',
  'mm/seq_table', 'mm/bank_table', 'mm/audio_table', 'mm/seq_banks',
  'custom/bank_table',
  'oot/payload', 'mm/payload',
]);

function canDeduplicateFile(file: RomFile): boolean {
  if (file.injected || file.alias || file.type === 'dummy' || !file.data.length) return false;
  if (file.name && MUTATED_OR_LAYOUT_SENSITIVE.has(file.name)) return false;
  if ((file.game === 'oot' || file.game === 'mm') && file.index !== undefined && file.index < 6) return false;
  return true;
}

export function applyWholeFileDedupe(builder: RomBuilder) {
  const canonicalFiles = new Map<string, RomFile>();
  for (const file of builder.allFiles()) {
    if (!canDeduplicateFile(file)) continue;
    const key = [file.type, file.data.length, hashBytes(file.data)].join(':');
    const canonical = canonicalFiles.get(key);
    if (!canonical) {
      canonicalFiles.set(key, file);
    } else if (bufEqual(canonical.data, file.data)) {
      file.alias = canonical;
      file.data = canonical.data;
    }
  }
}

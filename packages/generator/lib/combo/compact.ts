import type { CustomAnimation, Game } from '@ootmm/data';
import type { Monitor } from '@ootmm/core';
import type { DecompressedRoms } from './decompress';
import type { RomBuilder, RomFile } from './rom-builder';
import { DATA_ANIMATIONS, FILES } from '@ootmm/data';
import { DmaData } from './dma';
import { arrayToIndexMap } from './util';
import { bufReadU16BE, bufReadU32BE, bufWriteU16BE, bufWriteU32BE } from './util/buffer';

export function hashBytes(data: Uint8Array): number {
  let h = 0x811c9dc5;
  for (let i = 0; i < data.length; ++i) {
    h ^= data[i];
    h = Math.imul(h, 0x01000193);
  }
  return h >>> 0;
}

export function bytesEqual(a: Uint8Array, b: Uint8Array): boolean {
  if (a.length !== b.length) {
    return false;
  }
  for (let i = 0; i < a.length; ++i) {
    if (a[i] !== b[i]) {
      return false;
    }
  }
  return true;
}

export function align16(n: number): number {
  return (n + 0xf) & ~0xf;
}

const FILES_TO_INDEX = {
  oot: arrayToIndexMap(FILES.oot),
  mm: arrayToIndexMap(FILES.mm),
};

export function findGameFileKey(game: Game, wanted: string): string {
  const keys = Object.keys(FILES_TO_INDEX[game]);
  const exact = keys.find((x) => x === wanted);
  if (exact) {
    return exact;
  }

  const basename = wanted.split('/').pop();
  const byBasename = keys.find((x) => x.split('/').pop() === basename);
  if (byBasename) {
    return byBasename;
  }

  throw new Error(`${game}: file not found in file table: ${wanted}`);
}

export function getDecompressedGameFile(roms: DecompressedRoms, game: Game, wanted: string): Uint8Array {
  const key = findGameFileKey(game, wanted);
  const index = FILES_TO_INDEX[game][key];
  const entry = new DmaData(roms[game].dma).read(index);
  return roms[game].rom.slice(entry.virtStart, entry.virtEnd);
}

export function gameFileIndex(game: Game, wanted: string): number {
  const key = findGameFileKey(game, wanted);
  return FILES_TO_INDEX[game][key];
}

type Candidate = { data: Uint8Array; offset: number; protected: boolean };

export class ExactPool {
  private readonly chunks: Uint8Array[] = [];
  private readonly buckets = new Map<string, Candidate[]>();
  private size = 0;

  append(data: Uint8Array, dedupe = true, protectedBlob = false) {
    const key = `${data.length}:${hashBytes(data).toString(16)}`;
    const candidates = this.buckets.get(key) ?? [];

    if (dedupe && !protectedBlob) {
      const match = candidates.find(c => !c.protected && bytesEqual(c.data, data));
      if (match) return { offset: match.offset };
    }

    const offset = this.size;
    const copy = new Uint8Array(align16(data.length));
    copy.set(data);
    this.chunks.push(copy);
    this.size += copy.length;
    candidates.push({ data: copy.subarray(0, data.length), offset, protected: protectedBlob });
    this.buckets.set(key, candidates);
    return { offset };
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

export const PLAYER_ANIM_FRAME_SIZE = 0x86;
const NATIVE_SEGMENT = 0x07;
const FOREIGN_SEGMENT = 0x08;

type PlayerAnimHeader = { headerOffset: number; dataOffset: number; data: Uint8Array };
type PlayerAnimMatch = { offset: number };
type PlayerAnimSite = { data: Uint8Array; offset: number; relative: number };
type CompactPool = {
  data: Uint8Array;
  oldToNew: Map<number, number>;
  pool: PlayerAnimationPool;
};

class PlayerAnimationPool {
  private readonly chunks: Uint8Array[] = [];
  private readonly frameIndex = new Map<number, PlayerAnimSite[]>();
  private size = 0;

  find(data: Uint8Array): PlayerAnimMatch | undefined {
    if (!data.length || data.length % PLAYER_ANIM_FRAME_SIZE) return undefined;
    const key = hashBytes(data.subarray(0, PLAYER_ANIM_FRAME_SIZE));
    for (const site of this.frameIndex.get(key) ?? []) {
      if (site.data.length - site.relative < data.length) continue;
      if (!bytesEqual(site.data.subarray(site.relative, site.relative + data.length), data)) continue;
      return { offset: site.offset + site.relative };
    }
    return undefined;
  }

  append(data: Uint8Array) {
    const match = this.find(data);
    if (match) return match;

    const offset = this.size;
    const copy = new Uint8Array(align16(data.length));
    copy.set(data);
    this.chunks.push(copy);
    this.size += copy.length;

    const raw = copy.subarray(0, data.length);
    for (let relative = 0; relative + PLAYER_ANIM_FRAME_SIZE <= raw.length; relative += PLAYER_ANIM_FRAME_SIZE) {
      const key = hashBytes(raw.subarray(relative, relative + PLAYER_ANIM_FRAME_SIZE));
      pushMap(this.frameIndex, key, { data: raw, offset, relative });
    }
    return { offset };
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
  ootLinkAnimData: Uint8Array;
  mmLinkAnimData: Uint8Array;
  ootHeaderPointers: Map<number, number>;
  mmHeaderPointers: Map<number, number>;
  ootLocalOffsetMap: Map<number, number>;
  mmLocalOffsetMap: Map<number, number>;
};

function parseNum(value: number | string | undefined, label: string) {
  if (value === undefined || value === null) throw new Error(`Missing ${label}`);
  if (typeof value === 'number') return value;
  const n = Number.parseInt(value.trim(), /^0x/i.test(value.trim()) ? 16 : 10);
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

function importedOffsets(game: Game) {
  const out = new Set<number>();
  for (const anim of DATA_ANIMATIONS as CustomAnimation[]) {
    if (anim.home_game === game) out.add(parseNum(anim.frame_data_offset, `${anim.name}.frame_data_offset`));
  }
  return out;
}

function uniquePlayerSources(headers: PlayerAnimHeader[]) {
  const byOffset = new Map<number, Uint8Array>();
  for (const header of headers) {
    const current = byOffset.get(header.dataOffset);
    if (!current || header.data.length > current.length) byOffset.set(header.dataOffset, header.data);
  }
  return [...byOffset].map(([dataOffset, data]) => ({ dataOffset, data }))
      .sort((a, b) => b.data.length - a.data.length || a.dataOffset - b.dataOffset);
}

function buildLocalPool(headers: PlayerAnimHeader[]): CompactPool {
  const pool = new PlayerAnimationPool();
  const oldToNew = new Map<number, number>();
  for (const source of uniquePlayerSources(headers))
    oldToNew.set(source.dataOffset, pool.append(source.data).offset);
  return { data: pool.finish(), oldToNew, pool };
}

function assertHeaderCount(game: Game, count: number) {
  const [min, max] = game === 'oot' ? [500, 800] : [600, 900];
  if (count < min || count > max)
    throw new Error(`Unexpected ${game === 'oot' ? 'OoT' : 'MM'} player animation header count: ${count}`);
}

function buildOotState(headers: PlayerAnimHeader[], compact: CompactPool) {
  const pointers = new Map<number, number>();
  for (const header of headers) {
    const offset = compact.oldToNew.get(header.dataOffset);
    if (offset === undefined) throw new Error(`Missing OoT player animation 0x${header.dataOffset.toString(16)}`);
    pointers.set(header.headerOffset, segmented(NATIVE_SEGMENT, offset));
  }
  return pointers;
}

function buildMmState(headers: PlayerAnimHeader[], ootPool: PlayerAnimationPool) {
  const pinned = importedOffsets('mm');
  const foreign = new Map<number, PlayerAnimMatch>();
  const localHeaders: PlayerAnimHeader[] = [];
  for (const header of headers) {
    const match = pinned.has(header.dataOffset) ? undefined : ootPool.find(header.data);
    if (match) foreign.set(header.headerOffset, match);
    else localHeaders.push(header);
  }

  const local = buildLocalPool(localHeaders);
  const pointers = new Map<number, number>();
  for (const header of headers) {
    const match = foreign.get(header.headerOffset);
    if (match) pointers.set(header.headerOffset, segmented(FOREIGN_SEGMENT, match.offset));
    else {
      const offset = local.oldToNew.get(header.dataOffset);
      if (offset === undefined) throw new Error(`Missing MM player animation 0x${header.dataOffset.toString(16)}`);
      pointers.set(header.headerOffset, segmented(NATIVE_SEGMENT, offset));
    }
  }
  return { data: local.data, pointers, oldToNew: local.oldToNew };
}

export function buildPlayerAnimationDedupePlan(roms: DecompressedRoms): PlayerAnimationDedupePlan {
  const ootKeep = getDecompressedGameFile(roms, 'oot', 'objects/gameplay_keep');
  const mmKeep = getDecompressedGameFile(roms, 'mm', 'objects/gameplay_keep');
  const ootLinkAnim = getDecompressedGameFile(roms, 'oot', 'link_animetion');
  const mmLinkAnim = getDecompressedGameFile(roms, 'mm', 'link_animetion');
  const ootHeaders = scanPlayerAnimationHeaders(ootKeep, ootLinkAnim);
  const mmHeaders = scanPlayerAnimationHeaders(mmKeep, mmLinkAnim);
  assertHeaderCount('oot', ootHeaders.length);
  assertHeaderCount('mm', mmHeaders.length);

  const ootPool = buildLocalPool(ootHeaders);
  const mm = buildMmState(mmHeaders, ootPool.pool);
  return {
    ootLinkAnimData: ootPool.data,
    mmLinkAnimData: mm.data,
    ootHeaderPointers: buildOotState(ootHeaders, ootPool),
    mmHeaderPointers: mm.pointers,
    ootLocalOffsetMap: ootPool.oldToNew,
    mmLocalOffsetMap: mm.oldToNew,
  };
}

function patchHeaders(file: Uint8Array, pointers: Map<number, number>, game: string) {
  for (const [offset, ptr] of pointers) {
    if (offset + 8 > file.length)
      throw new Error(`${game} gameplay_keep player animation header out of range: 0x${offset.toString(16)}`);
    bufWriteU16BE(file, offset + 2, 0);
    bufWriteU32BE(file, offset + 4, ptr);
  }
}

export function applyPlayerAnimationDedupe(builder: RomBuilder, plan: PlayerAnimationDedupePlan) {
  patchHeaders(builder.fileByNameRequired('oot/objects/gameplay_keep').data, plan.ootHeaderPointers, 'OoT');
  patchHeaders(builder.fileByNameRequired('mm/objects/gameplay_keep').data, plan.mmHeaderPointers, 'MM');
  builder.replaceFileData('oot/link_animetion', plan.ootLinkAnimData);
  builder.replaceFileData('mm/link_animetion', plan.mmLinkAnimData);
}

export function remapImportedPlayerAnimationOffset(plan: PlayerAnimationDedupePlan, game: Game, oldOffset: number) {
  const mapped = (game === 'oot' ? plan.ootLocalOffsetMap : plan.mmLocalOffsetMap).get(oldOffset);
  if (mapped === undefined)
    throw new Error(`${game.toUpperCase()} imported player animation 0x${oldOffset.toString(16)} was not retained in compact link_animetion`);
  return mapped;
}

export const AUDIO_ENTRY_SIZE = 0x10;

export type AudioEntry = { romAddr: number; size: number };

export const AUDIO_SPECS = {
  oot: {
    fontOffset: 0x1026b0,
    fontCount: 0x26,
    sampleOffset: 0x1031d0,
    sampleCount: 0x07,
    seqOffset: 0x102ae0,
    seqCount: 0x6e,
  },
  mm: {
    fontOffset: 0x13b6d0,
    fontCount: 0x29,
    sampleOffset: 0x13c390,
    sampleCount: 0x03,
    seqOffset: 0x13bb80,
    seqCount: 0x80,
  },
} as const;

export function parseAudioEntries(data: Uint8Array, offset: number, count: number): AudioEntry[] {
  return Array.from({ length: count }, (_, i) => {
    const off = offset + i * AUDIO_ENTRY_SIZE;
    if (off + AUDIO_ENTRY_SIZE > data.length)
      throw new Error(`Audio entry ${i} at 0x${off.toString(16)} out of range`);
    return { romAddr: bufReadU32BE(data, off), size: bufReadU32BE(data, off + 4) };
  });
}

export function resolveAudioEntry(entries: AudioEntry[], index: number) {
  const seen = new Set<number>();
  for (;;) {
    if (index < 0 || index >= entries.length)
      throw new Error(`Audio entry alias ${index} out of range`);
    if (seen.has(index))
      throw new Error(`Audio entry alias cycle at ${index}`);
    seen.add(index);
    const entry = entries[index];
    if (entry.size) return { index, entry };
    index = entry.romAddr;
  }
}

export const audioBlobKey = (offset: number, size: number) => `${offset}:${size}`;
export const audioSpec = (game: Game) => AUDIO_SPECS[game];

export function pushMap<K, V>(map: Map<K, V[]>, key: K, value: V) {
  const list = map.get(key);
  if (list) list.push(value);
  else map.set(key, [value]);
}

const CUSTOM_BANK_FIRST = 0x60;
const CUSTOM_BANK_LAST = 0xf0;
const MIN_EMPTY_BANK_SIZE = 0x10;
const PROTECT_NATIVE_FONT0_SFX = true;

type FontKind = 'native' | 'custom';
type UsageKind = 'instrument' | 'drum' | 'sfx';
type Slots = [number, number];
type Range = { start: number; end: number };
type OotCandidate = { bankId: number; sampleOffset: number; codec: number; data: Uint8Array };
type GameCompaction = { data: Uint8Array; layouts: Map<number, BankLayout> };

type FontInstance = {
  key: string; kind: FontKind; id: number; data: Uint8Array;
  backingFile: RomFile; backingBase: number; metaData: Uint8Array; metaOffset: number;
  originalSlots: Slots; slots: Slots; numInstruments: number; numDrums: number; numSfx: number;
};

type LocalSource = {
  realBankId: number; sampleOffset: number; size: number; codec: number; data: Uint8Array;
};

type SampleRecord = {
  data: Uint8Array; sampleOffsetInFont: number; fonts: Set<FontInstance>;
  localSources: Map<string, LocalSource>; source: LocalSource | null; mixedSource: boolean;
  protected: boolean; foreignRedirect: { bankId: number; sampleOffset: number; selector: 0 | 1 } | null;
  localBankRedirect: { bankId: number; sampleOffset: number; selector: 0 | 1 } | null;
  localRedirectOffset: number | null;
};

type BankLayout = {
  oldStart: number; oldSize: number; newStart: number; newSize: number; removals: Range[];
};

type LocalCollection = {
  game: Game; records: SampleRecord[]; fonts: FontInstance[]; sampleEntries: AudioEntry[];
  originalAudiotable: Uint8Array; pinnedBanks: Set<number>;
};

export type AudioDedupeResult = { finalizePhysicalTables(): void };

function sampleInfo(font: Uint8Array, off: number) {
  if (off <= 0 || off + 0x10 > font.length) return null;
  return {
    codec: (font[off] >> 4) & 7,
    selector: (font[off] >> 2) & 3,
    size: bufReadU32BE(font, off) & 0x00ffffff,
    sampleAddr: bufReadU32BE(font, off + 4),
  };
}

function walkFontSamples(font: FontInstance, callback: (sampleOff: number, kind: UsageKind) => void) {
  const data = font.data;
  if (data.length < 8) throw new Error(`${font.key}: soundfont too small`);

  const drumsPtr = bufReadU32BE(data, 0);
  const sfxPtr = bufReadU32BE(data, 4);
  const instCount = Math.min(font.numInstruments, 126);
  if (8 + instCount * 4 > data.length) throw new Error(`${font.key}: instrument pointer table out of range`);

  for (let i = 0; i < instCount; ++i) {
    const inst = bufReadU32BE(data, 8 + i * 4);
    if (!inst) continue;
    if (inst + 0x20 > data.length) throw new Error(`${font.key}: instrument ${i} out of range`);
    if (data[inst + 1]) callback(bufReadU32BE(data, inst + 0x08), 'instrument');
    callback(bufReadU32BE(data, inst + 0x10), 'instrument');
    if (data[inst + 2] !== 0x7f) callback(bufReadU32BE(data, inst + 0x18), 'instrument');
  }

  if (font.numDrums) {
    if (!drumsPtr || drumsPtr + font.numDrums * 4 > data.length)
      throw new Error(`${font.key}: drum table out of range`);
    for (let i = 0; i < font.numDrums; ++i) {
      const drum = bufReadU32BE(data, drumsPtr + i * 4);
      if (!drum) continue;
      if (drum + 0x10 > data.length) throw new Error(`${font.key}: drum ${i} out of range`);
      callback(bufReadU32BE(data, drum + 4), 'drum');
    }
  }

  if (font.numSfx) {
    if (!sfxPtr || sfxPtr + font.numSfx * 8 > data.length)
      throw new Error(`${font.key}: SFX table out of range`);
    for (let i = 0; i < font.numSfx; ++i) callback(bufReadU32BE(data, sfxPtr + i * 8), 'sfx');
  }
}

function findFileAtVrom(builder: RomBuilder, vrom: number, size: number): { file: RomFile; offset: number } | null {
  for (const file of builder.allFiles()) {
    if (file.vaddr === undefined || file.type === 'dummy') continue;
    const start = file.vaddr >>> 0;
    if (vrom >= start && vrom + size <= start + file.data.length) return { file, offset: vrom - start };
  }
  return null;
}

function originalNativeTables(game: Game, roms: DecompressedRoms) {
  const spec = audioSpec(game);
  const code = getDecompressedGameFile(roms, game, 'code');
  const fontEntries = parseAudioEntries(code, spec.fontOffset, spec.fontCount);
  const sampleEntries = parseAudioEntries(code, spec.sampleOffset, spec.sampleCount);
  const audiobank = getDecompressedGameFile(roms, game, 'Audiobank');
  const fonts = fontEntries.map((_, i) => {
    const entry = resolveAudioEntry(fontEntries, i).entry;
    if (entry.romAddr + entry.size > audiobank.length)
      throw new Error(`${game}: soundfont ${i} out of Audiobank range`);
    return entry;
  });
  return { fonts, sampleEntries, audiotable: getDecompressedGameFile(roms, game, 'Audiotable') };
}

function makeFont(
    game: Game, kind: FontKind, id: number, table: Uint8Array, metaOffset: number,
    backingFile: RomFile, backingBase: number, data: Uint8Array,
): FontInstance {
  const slots: Slots = [table[metaOffset + 0x0a], table[metaOffset + 0x0b]];
  return {
    key: `${game}:${kind}:${id}:${backingFile.name || 'unnamed'}:${backingBase}`,
    kind, id, data, backingFile, backingBase, metaData: table, metaOffset,
    originalSlots: [...slots], slots: [...slots],
    numInstruments: table[metaOffset + 0x0c], numDrums: table[metaOffset + 0x0d],
    numSfx: bufReadU16BE(table, metaOffset + 0x0e),
  };
}

function runtimeNativeFonts(game: Game, builder: RomBuilder, roms: DecompressedRoms): FontInstance[] {
  const source = originalNativeTables(game, roms);
  const table = builder.fileByNameRequired(`${game}/bank_table`).data;
  const audiobank = builder.fileByNameRequired(`${game}/Audiobank`);
  const out: FontInstance[] = [];

  for (let id = 0; id < source.fonts.length; ++id) {
    const off = id * AUDIO_ENTRY_SIZE;
    if (off + AUDIO_ENTRY_SIZE > table.length) throw new Error(`${game}/bank_table is too small`);

    const addr = bufReadU32BE(table, off);
    const size = bufReadU32BE(table, off + 4);
    if (!size) continue;

    let file = audiobank, base = source.fonts[id].romAddr, data: Uint8Array;
    if (addr >= 0x08000000) {
      const found = findFileAtVrom(builder, addr, size);
      if (!found) throw new Error(`${game} runtime soundfont ${id} VROM 0x${addr.toString(16)} not found`);
      file = found.file; base = found.offset;
      data = file.data.subarray(base, base + size);
    } else {
      if (base + size > audiobank.data.length)
        throw new Error(`${game} native soundfont ${id} out of Audiobank range`);
      data = audiobank.data.subarray(base, base + size);
    }
    out.push(makeFont(game, 'native', id, table, off, file, base, data));
  }
  return out;
}

function runtimeCustomFonts(game: Game, builder: RomBuilder): FontInstance[] {
  const table = builder.fileByName('custom/bank_table')?.data;
  if (!table) return [];
  const out: FontInstance[] = [];

  for (let id = CUSTOM_BANK_FIRST; id < CUSTOM_BANK_LAST; ++id) {
    const off = (id - CUSTOM_BANK_FIRST) * AUDIO_ENTRY_SIZE;
    if (off + AUDIO_ENTRY_SIZE > table.length) break;
    const addr = bufReadU32BE(table, off), size = bufReadU32BE(table, off + 4);
    if (!size) continue;

    const found = findFileAtVrom(builder, addr, size);
    if (!found) throw new Error(`Custom soundfont ${id.toString(16)} VROM 0x${addr.toString(16)} not found`);
    out.push(makeFont(
        game, 'custom', id, table, off, found.file, found.offset,
        found.file.data.subarray(found.offset, found.offset + size),
    ));
  }
  return out;
}

function localBankId(game: Game, font: FontInstance, raw: number): number | null {
  if (raw === 0xff) return null;
  if (font.kind === 'native' || game === 'oot') return raw < 8 ? raw : null;
  return raw >= 8 && raw < 16 ? raw ^ 8 : null;
}

const desiredOotRawBank = (font: FontInstance, bankId: number) => font.kind === 'native' ? bankId + 8 : bankId;
const desiredLocalRawBank = (game: Game, font: FontInstance, bankId: number) =>
    font.kind === 'native' || game === 'oot' ? bankId : bankId ^ 8;
const sourceKey = (s: LocalSource) => `${s.realBankId}:${s.sampleOffset}:${s.size}:${s.codec}`;
const sampleHashKey = (s: Pick<LocalSource, 'size' | 'codec' | 'data'>) =>
    `${s.size}:${s.codec}:${hashBytes(s.data).toString(16)}`;
const resolvedBank = (c: LocalCollection, id: number) => resolveAudioEntry(c.sampleEntries, id).entry;
const realBankIds = (entries: AudioEntry[]) => [...new Set(entries.map((_, i) => resolveAudioEntry(entries, i).index))];

function sampleRecord(records: Map<string, SampleRecord>, key: string, data: Uint8Array, off: number) {
  let record = records.get(key);
  if (record) return record;
  record = {
    data, sampleOffsetInFont: off, fonts: new Set(), localSources: new Map(),
    source: null, mixedSource: false, protected: false, foreignRedirect: null,
    localBankRedirect: null, localRedirectOffset: null,
  };
  records.set(key, record);
  return record;
}

function collectLocalSamples(game: Game, builder: RomBuilder, roms: DecompressedRoms): LocalCollection {
  const native = originalNativeTables(game, roms);
  const fonts = [...runtimeNativeFonts(game, builder, roms), ...runtimeCustomFonts(game, builder)];
  const records = new Map<string, SampleRecord>();

  for (const font of fonts) walkFontSamples(font, (sampleOff, kind) => {
    const info = sampleInfo(font.data, sampleOff);
    if (!info || !info.size || info.selector > 1) return;

    const record = sampleRecord(
        records, `${font.backingFile.name || font.key}:${font.backingBase + sampleOff}`, font.data, sampleOff,
    );
    record.fonts.add(font);
    if (PROTECT_NATIVE_FONT0_SFX && font.kind === 'native' && font.id === 0 && kind === 'sfx') record.protected = true;

    const logicalBank = localBankId(game, font, font.slots[info.selector]);
    if (logicalBank === null) return;
    if (logicalBank >= native.sampleEntries.length)
      throw new Error(`${font.key}: ${game} sample bank ${logicalBank} out of range`);

    const bank = resolveAudioEntry(native.sampleEntries, logicalBank);
    if (info.sampleAddr + info.size > bank.entry.size)
      throw new Error(`${font.key}: sample 0x${info.sampleAddr.toString(16)}+0x${info.size.toString(16)} outside ${game} sample bank ${logicalBank}`);

    const absolute = bank.entry.romAddr + info.sampleAddr;
    const source: LocalSource = {
      realBankId: bank.index, sampleOffset: info.sampleAddr, size: info.size, codec: info.codec,
      data: native.audiotable.subarray(absolute, absolute + info.size),
    };
    record.localSources.set(sourceKey(source), source);
  });

  const pinnedBanks = new Set<number>();
  for (const record of records.values()) {
    const sources = [...record.localSources.values()];
    if (sources.length === 1) record.source = sources[0];
    else if (sources.length > 1) {
      record.mixedSource = true;
      for (const source of sources) pinnedBanks.add(source.realBankId);
    }
  }

  return {
    game, records: [...records.values()], fonts, sampleEntries: native.sampleEntries,
    originalAudiotable: native.audiotable, pinnedBanks,
  };
}

function groupByPhysical(records: SampleRecord[]) {
  const out = new Map<string, SampleRecord[]>();
  for (const record of records)
    if (record.source && !record.mixedSource) pushMap(out, sourceKey(record.source), record);
  return out;
}

function localSelector(collection: LocalCollection, record: SampleRecord, bankId: number): 0 | 1 | null {
  let best: 0 | 1 | null = null, bestFree = Number.MAX_SAFE_INTEGER;
  for (const selector of [0, 1] as const) {
    let free = 0, blocked = false;
    for (const font of record.fonts) {
      const desired = desiredLocalRawBank(collection.game, font, bankId);
      const slot = font.slots[selector];
      if (slot === 0xff) free++;
      else if (slot !== desired) { blocked = true; break; }
    }
    if (!blocked && free < bestFree) { best = selector; bestFree = free; }
  }
  return best;
}

function assignIntraGameDedupe(collection: LocalCollection) {
  const candidates = new Map<string, { source: LocalSource; group: SampleRecord[] }[]>();
  const ordered = [...groupByPhysical(collection.records).values()].sort((a, b) => {
    const sa = a[0].source!, sb = b[0].source!;
    return sa.realBankId - sb.realBankId || sa.sampleOffset - sb.sampleOffset;
  });

  for (const group of ordered) {
    const source = group[0].source!;
    if (collection.pinnedBanks.has(source.realBankId) || group.some(r => r.protected || r.foreignRedirect)) continue;

    const key = sampleHashKey(source);
    const list = candidates.get(key) ?? [];
    const exact = list.filter(c => !c.group.some(r => r.protected) && bytesEqual(c.source.data, source.data));
    const sameBank = exact.find(c => c.source.realBankId === source.realBankId);
    if (sameBank) {
      for (const record of group) record.localRedirectOffset = sameBank.source.sampleOffset;
      continue;
    }

    let redirected = false;
    for (const canonical of exact) {
      const selectors = group.map(record => localSelector(collection, record, canonical.source.realBankId));
      if (selectors.some(selector => selector === null)) continue;
      for (let i = 0; i < group.length; ++i) {
        const record = group[i], selector = selectors[i]!;
        for (const font of record.fonts) {
          const desired = desiredLocalRawBank(collection.game, font, canonical.source.realBankId);
          if (font.slots[selector] === 0xff) font.slots[selector] = desired;
        }
        record.localBankRedirect = {
          bankId: canonical.source.realBankId,
          sampleOffset: canonical.source.sampleOffset,
          selector,
        };
      }
      redirected = true;
      break;
    }

    if (!redirected) {
      list.push({ source, group });
      candidates.set(key, list);
    }
  }
}function rangeForSample(source: LocalSource, offset = source.sampleOffset, bankSize?: number): Range {
  const start = offset & ~0xf, end = align16(offset + source.size);
  return { start, end: bankSize === undefined ? end : Math.min(end, bankSize) };
}

function mergeRanges(ranges: Range[]): Range[] {
  const out: Range[] = [];
  for (const range of [...ranges].sort((a, b) => a.start - b.start || a.end - b.end)) {
    if (range.end <= range.start) continue;
    const last = out[out.length - 1];
    if (last && range.start <= last.end) last.end = Math.max(last.end, range.end);
    else out.push({ ...range });
  }
  return out;
}

function complementRanges(size: number, live: Range[]): Range[] {
  const out: Range[] = [];
  let cursor = 0;
  for (const range of live) {
    if (range.start > cursor) out.push({ start: cursor, end: range.start });
    cursor = Math.max(cursor, range.end);
  }
  if (cursor < size) out.push({ start: cursor, end: size });
  return out;
}

function buildLiveRanges(collection: LocalCollection) {
  const live = new Map<number, Range[]>();
  const add = (bankId: number, range: Range) => pushMap(live, bankId, range);

  for (const record of collection.records) {
    if (record.mixedSource) {
      for (const source of record.localSources.values())
        add(source.realBankId, rangeForSample(source, source.sampleOffset, resolvedBank(collection, source.realBankId).size));
      continue;
    }
    if (!record.source || record.foreignRedirect) continue;
    const source = record.source;
    if (record.localBankRedirect) {
      const target = record.localBankRedirect;
      add(target.bankId, rangeForSample(
          source, target.sampleOffset, resolvedBank(collection, target.bankId).size,
      ));
      continue;
    }
    add(source.realBankId, rangeForSample(
        source, record.localRedirectOffset ?? source.sampleOffset, resolvedBank(collection, source.realBankId).size,
    ));
  }

  for (const real of realBankIds(collection.sampleEntries)) {
    const bank = collection.sampleEntries[real];
    if (collection.pinnedBanks.has(real)) live.set(real, [{ start: 0, end: bank.size }]);
    else {
      const ranges = mergeRanges(live.get(real) ?? []);
      if (!ranges.length) ranges.push({ start: 0, end: Math.min(MIN_EMPTY_BANK_SIZE, bank.size) });
      live.set(real, ranges);
    }
  }
  return live;
}

function removedBefore(removals: Range[], offset: number) {
  let total = 0;
  for (const range of removals) {
    if (range.end > offset) break;
    total += range.end - range.start;
  }
  return total;
}

function compactAudiotable(collection: LocalCollection): GameCompaction {
  const ordered = realBankIds(collection.sampleEntries)
      .map(id => ({ id, entry: collection.sampleEntries[id] }))
      .sort((a, b) => a.entry.romAddr - b.entry.romAddr);
  const live = buildLiveRanges(collection);
  const chunks: Uint8Array[] = [];
  const layouts = new Map<number, BankLayout>();
  let oldCursor = 0, newSize = 0;
  const append = (data: Uint8Array) => { if (data.length) { chunks.push(data); newSize += data.length; } };

  for (const { id, entry } of ordered) {
    if (entry.romAddr < oldCursor || entry.romAddr + entry.size > collection.originalAudiotable.length)
      throw new Error(`${collection.game} Audiotable bank ${id} has overlapping/out-of-range layout`);

    append(collection.originalAudiotable.slice(oldCursor, entry.romAddr));
    const newStart = newSize;
    const bank = collection.originalAudiotable.subarray(entry.romAddr, entry.romAddr + entry.size);
    const removals = complementRanges(entry.size, mergeRanges(live.get(id) ?? []));
    let cursor = 0, removed = 0;
    for (const range of removals) {
      append(bank.slice(cursor, range.start));
      removed += range.end - range.start;
      cursor = range.end;
    }
    append(bank.slice(cursor));
    layouts.set(id, {
      oldStart: entry.romAddr, oldSize: entry.size, newStart,
      newSize: entry.size - removed, removals,
    });
    oldCursor = entry.romAddr + entry.size;
  }
  append(collection.originalAudiotable.slice(oldCursor));

  const data = new Uint8Array(newSize);
  let dst = 0;
  for (const chunk of chunks) { data.set(chunk, dst); dst += chunk.length; }
  return { data, layouts };
}

function patchLocalSampleOffsets(collection: LocalCollection, layouts: Map<number, BankLayout>) {
  for (const record of collection.records) {
    if (record.mixedSource || !record.source || record.foreignRedirect) continue;
    const source = record.source;
    const redirect = record.localBankRedirect;
    const bankId = redirect?.bankId ?? source.realBankId;
    const target = redirect?.sampleOffset ?? record.localRedirectOffset ?? source.sampleOffset;
    const layout = layouts.get(bankId);
    if (!layout) throw new Error(`Missing ${collection.game} bank layout ${bankId}`);
    if (redirect) {
      record.data[record.sampleOffsetInFont] =
          (record.data[record.sampleOffsetInFont] & ~0x0c) | (redirect.selector << 2);
    }
    bufWriteU32BE(record.data, record.sampleOffsetInFont + 4, target - removedBefore(layout.removals, target));
  }
}

function syncFontSlots(collection: LocalCollection) {
  for (const font of collection.fonts) {
    font.metaData[font.metaOffset + 0x0a] = font.slots[0];
    font.metaData[font.metaOffset + 0x0b] = font.slots[1];
  }
}

function identityBankLayouts(collection: LocalCollection) {
  const layouts = new Map<number, BankLayout>();
  for (const id of realBankIds(collection.sampleEntries)) {
    const entry = collection.sampleEntries[id];
    layouts.set(id, {
      oldStart: entry.romAddr, oldSize: entry.size, newStart: entry.romAddr,
      newSize: entry.size, removals: [],
    });
  }
  return layouts;
}

function buildOotCanonicalIndex(collection: LocalCollection, layouts: Map<number, BankLayout>) {
  const out = new Map<string, OotCandidate[]>();
  const protectedKeys = new Set<string>();
  for (const [key, group] of groupByPhysical(collection.records))
    if (group.some(r => r.protected)) protectedKeys.add(key);

  const seen = new Set<string>();
  for (const record of collection.records) {
    if (!record.source || record.mixedSource || protectedKeys.has(sourceKey(record.source))) continue;
    const source = record.source, layout = layouts.get(source.realBankId);
    if (!layout) continue;

    const target = record.localRedirectOffset ?? source.sampleOffset;
    const offset = target - removedBefore(layout.removals, target);
    const unique = `${source.realBankId}:${offset}:${source.size}:${source.codec}`;
    if (seen.has(unique)) continue;
    seen.add(unique);
    pushMap(out, sampleHashKey(source), {
      bankId: source.realBankId, sampleOffset: offset, codec: source.codec, data: source.data,
    });
  }
  return out;
}

function tryAssignForeignRedirect(record: SampleRecord, candidates: OotCandidate[]) {
  if (!record.source || record.mixedSource || record.protected) return false;

  for (const candidate of candidates) {
    if (candidate.codec !== record.source.codec || !bytesEqual(candidate.data, record.source.data)) continue;
    for (const selector of [0, 1] as const) {
      const blocked = [...record.fonts].some(font => {
        const slot = font.slots[selector];
        return slot !== 0xff && slot !== desiredOotRawBank(font, candidate.bankId);
      });
      if (blocked) continue;

      for (const font of record.fonts)
        if (font.slots[selector] === 0xff) font.slots[selector] = desiredOotRawBank(font, candidate.bankId);
      record.foreignRedirect = { bankId: candidate.bankId, sampleOffset: candidate.sampleOffset, selector };
      return true;
    }
  }
  return false;
}

function finalizeForeignRedirects(collection: LocalCollection) {
  const keep = new Set<string>();
  for (const [key, group] of groupByPhysical(collection.records)) {
    const source = group[0].source!;
    if (!collection.pinnedBanks.has(source.realBankId) && group.every(r => r.foreignRedirect)) keep.add(key);
  }

  for (const font of collection.fonts) font.slots = [...font.originalSlots];
  for (const record of collection.records) {
    if (!record.source || !record.foreignRedirect || !keep.has(sourceKey(record.source))) {
      record.foreignRedirect = null;
      continue;
    }
    const { selector, bankId } = record.foreignRedirect;
    for (const font of record.fonts) {
      const desired = desiredOotRawBank(font, bankId), current = font.slots[selector];
      if (current !== 0xff && current !== desired)
        throw new Error(`Audio dedupe slot reassignment conflict in ${font.key}`);
      font.slots[selector] = desired;
    }
  }
}function patchForeignSampleHeaders(collection: LocalCollection) {
  for (const record of collection.records) {
    if (!record.foreignRedirect) continue;
    const { selector, sampleOffset } = record.foreignRedirect;
    record.data[record.sampleOffsetInFont] = (record.data[record.sampleOffsetInFont] & ~0x0c) | (selector << 2);
    bufWriteU32BE(record.data, record.sampleOffsetInFont + 4, sampleOffset);
  }
}

function patchSampleTable(game: Game, builder: RomBuilder, entries: AudioEntry[], layouts: Map<number, BankLayout>) {
  const runtime = builder.fileByNameRequired(`${game}/audio_table`).data;
  const file = builder.fileByNameRequired(`${game}/Audiotable`);
  if (file.paddr === undefined) throw new Error(`${game}/Audiotable must be injected before audio-table finalization`);

  for (let i = 0; i < entries.length; ++i) {
    const entry = entries[i], off = i * AUDIO_ENTRY_SIZE;
    if (!entry.size) {
      bufWriteU32BE(runtime, off, entry.romAddr);
      bufWriteU32BE(runtime, off + 4, 0);
      continue;
    }
    const layout = layouts.get(i);
    if (!layout) throw new Error(`Missing compact ${game} sample bank layout ${i}`);
    bufWriteU32BE(runtime, off, (file.paddr + layout.newStart) >>> 0);
    bufWriteU32BE(runtime, off + 4, layout.newSize);
  }
}

export function applyAudioDedupe(roms: DecompressedRoms, builder: RomBuilder): AudioDedupeResult {
  const oot = collectLocalSamples('oot', builder, roms);
  const ootIndex = buildOotCanonicalIndex(oot, identityBankLayouts(oot));
  const mm = collectLocalSamples('mm', builder, roms);
  const eligible = mm.records
      .filter(r => r.source && !r.mixedSource && !mm.pinnedBanks.has(r.source.realBankId))
      .sort((a, b) => (b.source?.size ?? 0) - (a.source?.size ?? 0));

  for (const record of eligible) {
    const source = record.source!;
    if (record.protected) continue;
    const exact = (ootIndex.get(sampleHashKey(source)) ?? []).filter(c => bytesEqual(c.data, source.data));
    if (exact.length) tryAssignForeignRedirect(record, exact);
  }

  finalizeForeignRedirects(mm);
  assignIntraGameDedupe(mm);
  const compact = compactAudiotable(mm);
  patchForeignSampleHeaders(mm);
  patchLocalSampleOffsets(mm, compact.layouts);
  syncFontSlots(mm);
  builder.replaceFileData('mm/Audiotable', compact.data);
  return { finalizePhysicalTables: () => patchSampleTable('mm', builder, mm.sampleEntries, compact.layouts) };
}

type Blob = { oldOffset: number; size: number; data: Uint8Array; protected: boolean };
type Target = { game: Game; offset: number };

type FilePlan = {
  filename: 'Audioseq' | 'Audiobank';
  runtimeTable: 'seq_table' | 'bank_table';
  mmEntries: AudioEntry[];
  mmMap: Map<string, Target>;
};

export type AudioFileDedupeResult = { finalizePhysicalTables(): void };

function collectBlobs(entries: AudioEntry[], file: Uint8Array, protectedId = 0): Blob[] {
  const blobs = new Map<string, Blob>();
  for (let id = 0; id < entries.length; ++id) {
    const real = resolveAudioEntry(entries, id).entry;
    if (real.romAddr + real.size > file.length)
      throw new Error(`Audio blob ${id} 0x${real.romAddr.toString(16)}+0x${real.size.toString(16)} out of range`);

    const key = audioBlobKey(real.romAddr, real.size);
    const existing = blobs.get(key);
    if (existing) {
      existing.protected ||= id === protectedId;
      continue;
    }
    blobs.set(key, {
      oldOffset: real.romAddr,
      size: real.size,
      data: file.subarray(real.romAddr, real.romAddr + real.size),
      protected: id === protectedId,
    });
  }
  return [...blobs.values()].sort((a, b) => a.oldOffset - b.oldOffset || a.size - b.size);
}

function sourceEntries(roms: DecompressedRoms, game: Game, kind: 'seq' | 'bank') {
  const spec = audioSpec(game);
  return parseAudioEntries(
      getDecompressedGameFile(roms, game, 'code'),
      kind === 'seq' ? spec.seqOffset : spec.fontOffset,
      kind === 'seq' ? spec.seqCount : spec.fontCount,
  );
}

function buildFilePlan(roms: DecompressedRoms, builder: RomBuilder, kind: 'seq' | 'bank'): FilePlan {
  const filename = kind === 'seq' ? 'Audioseq' : 'Audiobank';
  const runtimeTable = kind === 'seq' ? 'seq_table' : 'bank_table';
  const ootEntries = sourceEntries(roms, 'oot', kind);
  const mmEntries = sourceEntries(roms, 'mm', kind);
  const ootFile = builder.fileByNameRequired(`oot/${filename}`).data;
  const mmFile = builder.fileByNameRequired(`mm/${filename}`).data;

  const ootCanonical = new Map<string, { data: Uint8Array; offset: number }[]>();
  for (const blob of collectBlobs(ootEntries, ootFile)) {
    if (blob.protected) continue;
    const key = `${blob.size}:${hashBytes(blob.data).toString(16)}`;
    const list = ootCanonical.get(key) ?? [];
    if (!list.some(x => x.offset === blob.oldOffset)) list.push({ data: blob.data, offset: blob.oldOffset });
    ootCanonical.set(key, list);
  }

  const pool = new ExactPool();
  const mmMap = new Map<string, Target>();

  for (const blob of collectBlobs(mmEntries, mmFile)) {
    let target: Target | undefined;
    if (!blob.protected) {
      const key = `${blob.size}:${hashBytes(blob.data).toString(16)}`;
      const match = (ootCanonical.get(key) ?? []).find(c => bytesEqual(c.data, blob.data));
      if (match) {
        target = { game: 'oot', offset: match.offset };
      }
    }

    if (!target) {
      const result = pool.append(blob.data, true, blob.protected);
      target = { game: 'mm', offset: result.offset };
    }
    mmMap.set(audioBlobKey(blob.oldOffset, blob.size), target);
  }

  const compact = pool.finish();
  builder.replaceFileData(`mm/${filename}`, compact);

  return {
    filename,
    runtimeTable,
    mmEntries,
    mmMap,
  };
}

function deriveOldPhysicalBase(runtime: Uint8Array, entries: AudioEntry[]) {
  const counts = new Map<number, number>();
  const count = Math.min(entries.length, Math.floor(runtime.length / AUDIO_ENTRY_SIZE));

  for (let id = 0; id < count; ++id) {
    const off = id * AUDIO_ENTRY_SIZE;
    const addr = bufReadU32BE(runtime, off);
    const size = bufReadU32BE(runtime, off + 4);
    if (!size || addr >= 0x08000000) continue;

    const real = resolveAudioEntry(entries, id).entry;
    if (size !== real.size || addr < real.romAddr) continue;
    const base = (addr - real.romAddr) >>> 0;
    counts.set(base, (counts.get(base) ?? 0) + 1);
  }

  let best: number | undefined, bestCount = 0;
  for (const [base, count] of counts) if (count > bestCount) { best = base; bestCount = count; }
  if (best === undefined || bestCount < 2)
    throw new Error('Unable to infer original physical audio-file base from runtime table');
  return best;
}

function finalizePlan(builder: RomBuilder, plan: FilePlan) {
  const ootPaddr = builder.fileByNameRequired(`oot/${plan.filename}`).paddr;
  const mmPaddr = builder.fileByNameRequired(`mm/${plan.filename}`).paddr;
  if (ootPaddr === undefined || mmPaddr === undefined)
    throw new Error(`${plan.filename} files must be injected before audio table finalization`);

  const runtime = builder.fileByNameRequired(`mm/${plan.runtimeTable}`).data;
  const oldBase = deriveOldPhysicalBase(runtime, plan.mmEntries);
  const count = Math.floor(runtime.length / AUDIO_ENTRY_SIZE);

  for (let id = 0; id < count; ++id) {
    const off = id * AUDIO_ENTRY_SIZE;
    const addr = bufReadU32BE(runtime, off);
    const size = bufReadU32BE(runtime, off + 4);
    if (!size || addr >= 0x08000000 || addr < oldBase) continue;

    const target = plan.mmMap.get(audioBlobKey(addr - oldBase, size));
    if (!target) continue;
    bufWriteU32BE(runtime, off, ((target.game === 'oot' ? ootPaddr : mmPaddr) + target.offset) >>> 0);
  }
}

export function applyAudioFileDedupe(roms: DecompressedRoms, builder: RomBuilder): AudioFileDedupeResult {
  const sequences = buildFilePlan(roms, builder, 'seq');
  const soundfonts = buildFilePlan(roms, builder, 'bank');
  return {
    finalizePhysicalTables() {
      finalizePlan(builder, soundfonts);
      finalizePlan(builder, sequences);
    },
  };
}

const MUTATED_OR_LAYOUT_SENSITIVE = new Set([
  'oot/makerom', 'oot/boot', 'oot/dmadata', 'oot/Audiobank', 'oot/Audioseq', 'oot/Audiotable',
  'mm/makerom', 'mm/boot', 'mm/dmadata', 'mm/Audiobank', 'mm/Audioseq', 'mm/Audiotable',
  'oot/seq_table', 'oot/bank_table', 'oot/audio_table', 'oot/seq_banks',
  'mm/seq_table', 'mm/bank_table', 'mm/audio_table', 'mm/seq_banks',
  'custom/bank_table',
  'oot/payload', 'mm/payload',
]);

function eligible(file: RomFile): boolean {
  if (file.injected || file.alias || file.type === 'dummy' || file.data.length === 0) return false;
  if (file.name && MUTATED_OR_LAYOUT_SENSITIVE.has(file.name)) return false;
  if ((file.game === 'oot' || file.game === 'mm') && file.index !== undefined && file.index < 6) return false;
  return true;
}

export function applyWholeFileDedupe(builder: RomBuilder) {
  const buckets = new Map<string, RomFile[]>();
  for (const file of builder.allFiles()) {
    if (!eligible(file)) continue;
    const key = `${file.type}:${file.data.length}:${hashBytes(file.data).toString(16)}`;
    const list = buckets.get(key) || [];
    list.push(file);
    buckets.set(key, list);
  }

  for (const group of buckets.values()) {
    if (group.length < 2) continue;
    const canonical = group[0];
    for (let i = 1; i < group.length; ++i) {
      const duplicate = group[i];
      if (!bytesEqual(canonical.data, duplicate.data)) continue;
      duplicate.alias = canonical;
      duplicate.data = canonical.data;
    }
  }
}

export type AssetDedupeResult = { finalizePhysicalTables(): void };

export function applyAssetDedupe(
    roms: DecompressedRoms,
    builder: RomBuilder,
    _monitor: Monitor,
): AssetDedupeResult {
  const playerPlan = buildPlayerAnimationDedupePlan(roms);
  applyPlayerAnimationDedupe(builder, playerPlan);
  const audio = applyAudioDedupe(roms, builder);
  const audioFiles = applyAudioFileDedupe(roms, builder);
  applyWholeFileDedupe(builder);
  return {
    finalizePhysicalTables() {
      audio.finalizePhysicalTables();
      audioFiles.finalizePhysicalTables();
    },
  };
}
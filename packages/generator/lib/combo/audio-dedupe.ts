import type { Game } from '@ootmm/data';
import type { DecompressedRoms } from './decompress';
import type { RomBuilder, RomFile } from './rom-builder';
import { getDecompressedGameFile } from './compact';
import { bufReadU16BE, bufReadU32BE, bufWriteU32BE } from './util/buffer';

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
  key: string; kind: FontKind; homeGame: Game; id: number; data: Uint8Array;
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
    sampleAddr: bufReadU32BE(font, off + 4) | 0,
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
    if (vrom >= start && vrom + size <= start + file.data.length) {
      if (file.type !== 'uncompressed' || file.alias || file.injected)
        throw new Error(`Audio source is not an independent raw file: ${file.name ?? vrom}`);
      return { file, offset: vrom - start };
    }
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
    kind, homeGame: game, id, data, backingFile, backingBase, metaData: table, metaOffset,
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
      const oldPhysicalBase = (game === 'oot' ? 0 : 0x4d9f40) + audiobank.vaddr!;
      if (addr !== oldPhysicalBase + base || size !== source.fonts[id].size)
        throw new Error(`${game}: unexpected native soundfont address or size for ${id}`);
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
  if (game === font.homeGame) return raw < 8 ? raw : null;
  return raw >= 8 && raw < 16 ? raw ^ 8 : null;
}

const desiredOotRawBank = (font: FontInstance, bankId: number) => font.homeGame === 'oot' ? bankId : bankId ^ 8;
const desiredLocalRawBank = (game: Game, font: FontInstance, bankId: number) =>
    game === font.homeGame ? bankId : bankId ^ 8;
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
  const fonts = allRuntimeFonts(builder, roms);
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
    if (logicalBank === null) { record.mixedSource = true; return; }
    if (logicalBank >= native.sampleEntries.length)
      throw new Error(`${font.key}: ${game} sample bank ${logicalBank} out of range`);

    const bank = resolveAudioEntry(native.sampleEntries, logicalBank);
    const absolute = bank.entry.romAddr + info.sampleAddr;
    if (absolute < 0 || absolute + info.size > native.audiotable.length) {
      if (font.kind === 'custom') {
        record.mixedSource = true;
        return;
      }
      throw new Error(`${font.key}: sample outside ${game} Audiotable`);
    }
    if (font.kind !== 'custom' && (info.sampleAddr < 0 || info.sampleAddr + info.size > bank.entry.size))
      throw new Error(`${font.key}: sample outside ${game} sample bank ${logicalBank}`);
    const source: LocalSource = {
      realBankId: bank.index, sampleOffset: info.sampleAddr, size: info.size, codec: info.codec,
      data: native.audiotable.subarray(absolute, absolute + info.size),
    };
    record.localSources.set(sourceKey(source), source);
  });

  const pinnedBanks = new Set<number>();
  for (const record of records.values()) {
    const sources = [...record.localSources.values()];
    if (sources.length === 1 && !record.mixedSource) record.source = sources[0];
    else if (sources.length) {
      record.mixedSource = true;
      for (const source of sources) {
        const base = native.sampleEntries[source.realBankId].romAddr;
        const start = Math.min(base, base + source.sampleOffset);
        const end = Math.max(base, base + source.sampleOffset + source.size);
        for (const id of realBankIds(native.sampleEntries)) {
          const entry = native.sampleEntries[id];
          if (entry.romAddr < end && entry.romAddr + entry.size > start)
            pinnedBanks.add(id);
        }
      }
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
    return Number(!fitsSampleBank(collection, sa)) - Number(!fitsSampleBank(collection, sb)) ||
        sa.realBankId - sb.realBankId || sa.sampleOffset - sb.sampleOffset;
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

    if (!redirected && fitsSampleBank(collection, source)) {
      list.push({ source, group });
      candidates.set(key, list);
    }
  }
}

function fitsSampleBank(collection: LocalCollection, source: LocalSource) {
  return source.sampleOffset >= 0 &&
      source.sampleOffset + source.size <= resolvedBank(collection, source.realBankId).size;
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
  const ids = realBankIds(collection.sampleEntries);
  const addSample = (bankId: number, offset: number, size: number) => {
    const origin = resolvedBank(collection, bankId);
    const start = origin.romAddr + offset, end = start + size;
    for (const id of ids) {
      const bank = collection.sampleEntries[id];
      const lo = Math.max(start, bank.romAddr) - bank.romAddr;
      const hi = Math.min(end, bank.romAddr + bank.size) - bank.romAddr;
      if (lo < hi)
        pushMap(live, id, { start: lo & ~0xf, end: Math.min(align16(hi), bank.size) });
    }
  };

  for (const record of collection.records) {
    if (record.mixedSource) {
      for (const source of record.localSources.values())
        addSample(source.realBankId, source.sampleOffset, source.size);
      continue;
    }
    if (!record.source || record.foreignRedirect) continue;
    const source = record.source;
    const redirect = record.localBankRedirect;
    addSample(redirect?.bankId ?? source.realBankId,
        redirect?.sampleOffset ?? record.localRedirectOffset ?? source.sampleOffset, source.size);
  }

  for (const real of ids) {
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
    const absolute = layout.oldStart + target;
    let removed = 0;
    for (const other of layouts.values())
      removed += removedBefore(other.removals, absolute - other.oldStart);
    const relocated = absolute - removed - layout.newStart;
    if (relocated < -0x80000000 || relocated > 0x7fffffff)
      throw new Error(`${collection.game}: relocated sample offset out of range`);
    bufWriteU32BE(record.data, record.sampleOffsetInFont + 4, relocated >>> 0);
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
    if (!layout || !fitsSampleBank(collection, source)) continue;

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
  for (const game of ['oot', 'mm'] as const) {
    const name = `${game}/Audiobank`;
    builder.replaceFileData(name, builder.fileByNameRequired(name).data.slice());
  }
  const verification = snapshotSamples(builder, roms);
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
  verifySamples(verification, builder, compact.layouts);
  return { finalizePhysicalTables: () => patchSampleTable('mm', builder, mm.sampleEntries, compact.layouts) };
}

type Blob = { oldOffset: number; size: number; data: Uint8Array; protected: boolean };
type Target = { game: Game; offset: number };

type FilePlan = {
  filename: 'Audioseq' | 'Audiobank';
  runtimeTable: 'seq_table' | 'bank_table';
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
    mmMap,
  };
}

function finalizePlan(builder: RomBuilder, plan: FilePlan) {
  const ootPaddr = builder.fileByNameRequired(`oot/${plan.filename}`).paddr;
  const mmPaddr = builder.fileByNameRequired(`mm/${plan.filename}`).paddr;
  if (ootPaddr === undefined || mmPaddr === undefined)
    throw new Error(`${plan.filename} files must be injected before audio table finalization`);

  const runtime = builder.fileByNameRequired(`mm/${plan.runtimeTable}`).data;
  const oldBase = 0x4d9f40 + builder.fileByNameRequired(`mm/${plan.filename}`).vaddr!;
  const count = Math.floor(runtime.length / AUDIO_ENTRY_SIZE);

  for (let id = 0; id < count; ++id) {
    const off = id * AUDIO_ENTRY_SIZE;
    const addr = bufReadU32BE(runtime, off);
    const size = bufReadU32BE(runtime, off + 4);
    if (!size || addr >= 0x08000000) continue;

    const target = plan.mmMap.get(audioBlobKey(addr - oldBase, size));
    if (!target) throw new Error(`Unmapped MM ${plan.filename} entry ${id}: 0x${addr.toString(16)}`);
    const targetFile = builder.fileByNameRequired(`${target.game}/${plan.filename}`);
    if (target.offset + size > targetFile.data.length || targetFile.type !== 'uncompressed')
      throw new Error(`Invalid relocated MM ${plan.filename} entry ${id}`);
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


function allRuntimeFonts(builder: RomBuilder, roms: DecompressedRoms): FontInstance[] {
  return [...runtimeNativeFonts('oot', builder, roms), ...runtimeNativeFonts('mm', builder, roms),
    ...runtimeCustomFonts('oot', builder)];
}

type SampleProof = {
  font: FontInstance; offset: number; codec: number; size: number; bytes: Uint8Array;
  crossBank: boolean;
};

type UnresolvedSampleProof = {
  font: FontInstance; offset: number; header: Uint8Array; rawBank: number;
};

type SampleProofs = {
  entries: Record<Game, AudioEntry[]>; proofs: SampleProof[];
  unresolved: UnresolvedSampleProof[];
};

function resolveSample(font: FontInstance, offset: number, entries: Record<Game, AudioEntry[]>) {
  const info = sampleInfo(font.data, offset);
  if (!info || !info.size || info.selector > 1) return null;
  const raw = font.metaData[font.metaOffset + 0x0a + info.selector];
  if (raw >= 16) throw new Error(`Invalid audio bank selector in ${font.key}`);
  const game: Game = raw < 8 ? font.homeGame : font.homeGame === 'oot' ? 'mm' : 'oot';
  const bank = resolveAudioEntry(entries[game], raw & 7);
  return { ...info, game, bank };
}

function snapshotSamples(builder: RomBuilder, roms: DecompressedRoms): SampleProofs {
  const native = { oot: originalNativeTables('oot', roms), mm: originalNativeTables('mm', roms) };
  const entries = { oot: native.oot.sampleEntries, mm: native.mm.sampleEntries };
  const proofs: SampleProof[] = [];
  const unresolved: UnresolvedSampleProof[] = [];
  for (const font of allRuntimeFonts(builder, roms)) {
    const seen = new Set<number>();
    walkFontSamples(font, offset => {
      if (!offset || seen.has(offset)) return;
      seen.add(offset);
      const info = sampleInfo(font.data, offset);
      if (!info) throw new Error(`Invalid sample pointer in ${font.key}`);
      if (info.size && info.selector > 1)
        throw new Error(`Unsupported direct-address sample in ${font.key}; retry with OOTMM_DISABLE_AUDIO_DEDUPE=1`);
      const sample = resolveSample(font, offset, entries);
      if (!sample) return;
      const { entry } = sample.bank;
      const crossBank = sample.sampleAddr < 0 || sample.sampleAddr + sample.size > entry.size;
      const start = entry.romAddr + sample.sampleAddr;
      const outsideTable = start < 0 || start + sample.size > native[sample.game].audiotable.length;
      if (outsideTable && font.kind === 'custom') {
        unresolved.push({
          font, offset, header: font.data.slice(offset, offset + 0x10),
          rawBank: font.metaData[font.metaOffset + 0x0a + sample.selector],
        });
        return;
      }
      if ((crossBank && font.kind !== 'custom') || outsideTable)
        throw new Error(`Sample outside Audiotable: ${font.key} (${sample.game} bank ${sample.bank.index}, header 0x${offset.toString(16)}, offset 0x${sample.sampleAddr.toString(16)}, size 0x${sample.size.toString(16)}, bank size 0x${entry.size.toString(16)}, table size 0x${native[sample.game].audiotable.length.toString(16)})`);
      const bytes = native[sample.game].audiotable.subarray(start, start + sample.size);
      proofs.push({ font, offset, codec: sample.codec, size: sample.size, bytes, crossBank });
    });
  }
  return { entries, proofs, unresolved };
}

function verifySamples(verification: SampleProofs, builder: RomBuilder, layouts: Map<number, BankLayout>) {
  const tables = { oot: builder.fileByNameRequired('oot/Audiotable').data,
    mm: builder.fileByNameRequired('mm/Audiotable').data };
  for (const proof of verification.unresolved) {
    const info = sampleInfo(proof.font.data, proof.offset);
    if (!info || info.selector > 1 ||
        proof.font.metaData[proof.font.metaOffset + 0x0a + info.selector] !== proof.rawBank ||
        !bytesEqual(proof.header, proof.font.data.subarray(proof.offset, proof.offset + 0x10)))
      throw new Error(`Audio dedupe changed unresolved custom sample: ${proof.font.key} at 0x${proof.offset.toString(16)}`);
  }
  for (const proof of verification.proofs) {
    const sample = resolveSample(proof.font, proof.offset, verification.entries);
    if (!sample || sample.codec !== proof.codec || sample.size !== proof.size)
      throw new Error(`Audio dedupe changed sample format: ${proof.font.key}`);
    const { entry, index } = sample.bank;
    const layout = sample.game === 'mm' ? layouts.get(index) : undefined;
    if (sample.game === 'mm' && !layout) throw new Error(`Missing MM sample bank ${index}`);
    const base = layout?.newStart ?? entry.romAddr;
    const size = layout?.newSize ?? entry.size;
    const start = base + sample.sampleAddr;
    if ((!proof.crossBank && (sample.sampleAddr < 0 || sample.sampleAddr + sample.size > size)) ||
        start < 0 || start + sample.size > tables[sample.game].length ||
        !bytesEqual(proof.bytes, tables[sample.game].subarray(start, start + sample.size)))
      throw new Error(`Audio dedupe changed sample bytes: ${proof.font.key} at 0x${proof.offset.toString(16)}`);
  }
}

export function applyAudioOnlyDedupe(roms: DecompressedRoms, builder: RomBuilder) {
  const names = ['mm/Audiotable', 'mm/Audioseq', 'mm/Audiobank'];
  const before = names.map(name => builder.fileByNameRequired(name).data.length);
  const audio = applyAudioDedupe(roms, builder);
  const files = applyAudioFileDedupe(roms, builder);
  const savings = names.map((name, i) => ({ name, bytes: before[i] - builder.fileByNameRequired(name).data.length }));
  return {
    savings,
    savedBytes: savings.reduce((total, row) => total + row.bytes, 0),
    finalizePhysicalTables() {
      audio.finalizePhysicalTables();
      files.finalizePhysicalTables();
    },
  };
}

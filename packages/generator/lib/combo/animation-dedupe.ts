import type { Game } from '@ootmm/data';
import type { Patchfile } from './patch-build/patchfile';
import type { RomBuilder } from './rom-builder';
import { bufEqual } from './util/buffer';

const GAMES = ['oot', 'mm'] as const;
const ANCHOR_SIZE = 32;
const ANCHOR_STEP = 16;
const MIN_MATCH = 512;
const MAX_CANDIDATES = 8;
const MAX_RANGES_PER_BANK = 511;
const STORAGE_VROM = 0xee000000;
const STORAGE_LIMIT = 0xf0000000;

const align16 = (n: number) => (n + 15) & ~15;

type Range = { logical: number; physical: number };
type Bank = { size: number; ranges: Range[] };

type AnimationStorage = {
  data: Uint8Array;
  banks: [Bank, Bank];
  poolSize: number;
  tableSize: number;
  savedBytes: number;
};

function anchorHash(view: DataView, offset: number) {
  let hash = 0x811c9dc5;
  for (let i = 0; i < ANCHOR_SIZE; i += 4)
    hash = Math.imul(hash ^ view.getUint32(offset + i), 0x01000193);
  return hash >>> 0;
}

function addRange(ranges: Range[], logical: number, physical: number) {
  const last = ranges[ranges.length - 1];
  if (!last || last.physical + logical - last.logical !== physical)
    ranges.push({ logical, physical });
}

export function buildAnimationStorage(sources: readonly [Uint8Array, Uint8Array]): AnimationStorage {
  const pool = new Uint8Array(sources[0].length + sources[1].length);
  const poolView = new DataView(pool.buffer);
  const anchors = new Map<number, number | number[]>();
  let used = 0;
  let nextAnchor = 0;

  function indexPool() {
    while (nextAnchor + ANCHOR_SIZE <= used) {
      const hash = anchorHash(poolView, nextAnchor);
      const prior = anchors.get(hash);
      if (prior === undefined) anchors.set(hash, nextAnchor);
      else if (typeof prior === 'number') anchors.set(hash, [prior, nextAnchor]);
      else if (prior.length < MAX_CANDIDATES) prior.push(nextAnchor);
      else prior[prior.length - 1] = nextAnchor;
      nextAnchor += ANCHOR_STEP;
    }
  }

  function buildBank(source: Uint8Array): Bank {
    const ranges: Range[] = [];
    const view = new DataView(source.buffer, source.byteOffset, source.byteLength);
    let logical = 0;
    while (logical < source.length) {
      let best = -1;
      let bestLength = 0;
      if (source.length - logical >= MIN_MATCH && ranges.length + 2 <= MAX_RANGES_PER_BANK) {
        const found = anchors.get(anchorHash(view, logical));
        if (found !== undefined) {
          const candidates = typeof found === 'number' ? [found] : found;
          for (const physical of candidates) {
            const limit = Math.min(source.length - logical, used - physical);
            if (limit < MIN_MATCH || limit <= bestLength) continue;
            let length = 0;
            while (length < limit && source[logical + length] === pool[physical + length])
              ++length;
            length &= ~1;
            if (length >= MIN_MATCH && length > bestLength) {
              best = physical;
              bestLength = length;
              if (length === ((source.length - logical) & ~1)) break;
            }
          }
        }
      }
      if (best >= 0) {
        addRange(ranges, logical, best);
        logical += bestLength;
      } else {
        const size = Math.min(2, source.length - logical);
        addRange(ranges, logical, used);
        pool[used] = source[logical];
        if (size === 2) pool[used + 1] = source[logical + 1];
        used += size;
        logical += size;
        indexPool();
      }
    }
    if (!ranges.length || ranges.length > MAX_RANGES_PER_BANK)
      throw new Error('Invalid player animation storage range count');
    return { size: source.length, ranges };
  }

  const banks: [Bank, Bank] = [buildBank(sources[0]), buildBank(sources[1])];
  const tableSize = align16((banks[0].ranges.length + banks[1].ranges.length + 2) * 8);
  const data = new Uint8Array(tableSize + align16(used));
  const view = new DataView(data.buffer);
  let row = 0;
  for (const bank of banks) {
    for (const range of bank.ranges) {
      view.setUint32(row * 8, range.logical);
      view.setUint32(row * 8 + 4, range.physical);
      ++row;
    }
    view.setUint32(row * 8, bank.size);
    ++row;
  }
  data.set(pool.subarray(0, used), tableSize);

  row = 0;
  for (let i = 0; i < banks.length; ++i) {
    const bank = banks[i];
    const reconstructed = new Uint8Array(bank.size);
    let end = 0;
    for (let j = 0; j < bank.ranges.length; ++j, ++row) {
      const start = view.getUint32(row * 8);
      const physical = view.getUint32(row * 8 + 4);
      const next = view.getUint32((row + 1) * 8);
      if (start !== end || next <= start || next > bank.size || physical + next - start > used)
        throw new Error('Invalid player animation storage range');
      reconstructed.set(data.subarray(tableSize + physical, tableSize + physical + next - start), start);
      end = next;
    }
    if (view.getUint32(row * 8) !== bank.size || end !== bank.size || !bufEqual(reconstructed, sources[i]))
      throw new Error(`Player animation storage failed byte verification: ${GAMES[i]}`);
    ++row;
  }

  const savedBytes = align16(sources[0].length) + align16(sources[1].length) - data.length - 16;
  return { data, banks, poolSize: used, tableSize, savedBytes };
}

export async function applyAnimationStorageDedupe(builder: RomBuilder, patch: Patchfile) {
  if (patch.symPatches.has('COMPACT_LINK_ANIM_VROMS') || builder.fileByName('custom/mm_compact_link_anim'))
    throw new Error('Restore the native animation banks and rebuild before enabling address-preserving animation dedupe');

  const sourceFiles = GAMES.map(game => builder.fileByNameRequired(`${game}/link_animetion`));
  for (const file of sourceFiles) {
    if (file.type !== 'uncompressed' || file.alias || file.injected || file.vaddr === undefined || !file.data.length ||
      (file.vaddr & 1) || (file.data.length & 1) || file.vaddr + file.data.length > 0x04000000)
      throw new Error(`Invalid native player animation bank: ${file.name}`);
  }

  const storage = buildAnimationStorage([sourceFiles[0].data, sourceFiles[1].data]);
  if (storage.savedBytes <= 0) return null;

  const end = STORAGE_VROM + storage.data.length;
  if (end > STORAGE_LIMIT || builder.allFiles().some(file => file.vaddr !== undefined &&
    file.vaddr < end && file.vaddr + align16(file.vsize ?? file.data.length) > STORAGE_VROM))
    throw new Error('Player animation storage VROM collision');

  const mapPaddr = builder.currentPaddr();
  if (!mapPaddr || (mapPaddr & 15) || mapPaddr + storage.data.length > 0x03fff000)
    throw new Error('Invalid physical player animation storage range');
  const poolPaddr = mapPaddr + storage.tableSize;
  const table = new DataView(storage.data.buffer, storage.data.byteOffset, storage.tableSize);
  const config = new Uint8Array(24);
  const view = new DataView(config.buffer);
  view.setUint32(0, mapPaddr);
  view.setUint32(4, storage.banks[0].ranges.length + storage.banks[1].ranges.length + 2);
  let row = 0;
  sourceFiles.forEach((file, i) => {
    const start = file.vaddr! + i * 0x04000000;
    const bank = storage.banks[i];
    view.setUint32(8 + i * 8, start);
    view.setUint32(12 + i * 8, bank.size);
    for (const range of bank.ranges) {
      const logical = start + range.logical;
      const physical = poolPaddr + range.physical;
      table.setUint32(row * 8, logical);
      table.setUint32(row * 8 + 4, (physical - logical) >>> 0);
      ++row;
    }
    table.setUint32(row * 8, start + bank.size);
    ++row;
  });

  row = 0;
  sourceFiles.forEach((file, i) => {
    const start = view.getUint32(8 + i * 8);
    const bank = storage.banks[i];
    const reconstructed = new Uint8Array(bank.size);
    for (let j = 0; j < bank.ranges.length; ++j, ++row) {
      const logical = table.getUint32(row * 8);
      const physical = (logical + table.getUint32(row * 8 + 4)) >>> 0;
      const next = table.getUint32((row + 1) * 8);
      if (logical !== start + bank.ranges[j].logical || next <= logical ||
        next > start + bank.size || physical < poolPaddr || physical + next - logical > poolPaddr + storage.poolSize)
        throw new Error('Invalid physical player animation DMA map');
      reconstructed.set(storage.data.subarray(physical - mapPaddr, physical - mapPaddr + next - logical), logical - start);
    }
    if (!bufEqual(reconstructed, file.data) || table.getUint32(row * 8) !== start + bank.size)
      throw new Error(`Physical player animation map failed byte verification: ${GAMES[i]}`);
    ++row;
  });

  const destinations = GAMES.map((game: Game) => {
    const symbols = patch.symbols[game].get('ANIMATION_DMA_MAP');
    if (symbols?.length !== 1) throw new Error('Rebuild the payload for address-preserving animation dedupe');
    const file = builder.fileByVRAM(game, symbols[0]);
    const span = file?.vram?.[game];
    const offset = symbols[0] - (span?.[0] ?? 0);
    if (!file || !span || file.name !== `${game}/payload` || file.injected || file.alias ||
      offset < 0 || (offset & 3) || offset + config.length > file.data.length)
      throw new Error(`Invalid ${game} animation DMA map export`);
    return { file, offset };
  });

  builder.addFile({ game: 'custom', type: 'uncompressed', name: 'custom/player_animation_storage',
    vaddr: STORAGE_VROM, data: storage.data });
  const packed = builder.fileByNameRequired('custom/player_animation_storage');
  await builder.inject(packed);
  if (packed.paddr !== mapPaddr || packed.dma?.physEnd !== 0)
    throw new Error('Player animation storage physical address changed');
  for (const { file, offset } of destinations) file.data.set(config, offset);
  for (const file of sourceFiles) builder.removeFile(file.name!);
  return { savedBytes: storage.savedBytes, tableBytes: storage.tableSize,
    ranges: storage.banks.map(bank => bank.ranges.length) };
}

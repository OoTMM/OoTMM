import { readFileSync, writeFileSync } from 'node:fs';
import { fileURLToPath } from 'node:url';
import { resolve } from 'node:path';
import { OOT_ENEMIZER as oot_enemizer } from '@ootmm/core';

const root = fileURLToPath(new URL('../../../', import.meta.url));

const rom = readFileSync(process.argv[2] ?? resolve(root, '../oot/baseroms/ntsc-1.0/baserom-decompressed.z64'));

const mq = readFileSync(process.argv[3] ?? resolve(root, 'packages/generator/data/static/mq_rooms.bin'));

type Header = {
  actors?: {
    offset: number;
    count: number;
  };
  rooms?: {
    offset: number;
    count: number;
  };
  alternates?: number;
  setup: number;
};

function headers(start: number, end: number, segment: number): Header[] {
  const read = (at: number): Header => {
    const result: Header = { setup: 0 };
    for (let n = 0; n < 256; n++, at += 8) {
      if (at < start || at + 8 > end)
        throw Error('Header outside source room');
      const code = rom[at];
      if (code === 0x14)
        return result;
      if (![1, 4, 0x18].includes(code))
        continue;
      const ptr = rom.readUInt32BE(at + 4);
      if (code === 0x18 && !ptr)
        continue;
      if (ptr >>> 24 !== segment)
        throw Error('Unexpected source segment');
      const offset = start + (ptr & 0xffffff), count = rom[at + 1];
      if (offset < start || offset + (code === 1 ? count * 16 : code === 4 ? count * 8 : 12) > end)
        throw Error('Source list outside room');
      if (code === 1)
        result.actors = { offset, count };
      if (code === 4)
        result.rooms = { offset, count };
      if (code === 0x18)
        result.alternates = offset;
    }
    throw Error('Unterminated source header');
  };
  const base = read(start), result = [base];
  if (base.alternates !== undefined)
    for (let i = 0; i < 3; i++) {
      const ptr = rom.readUInt32BE(base.alternates + i * 4);
      if (!ptr)
        continue;
      if (ptr >>> 24 !== segment)
        throw Error('Unexpected alternate segment');
      result.push({ ...read(start + (ptr & 0xffffff)), setup: i + 1 });
    }
  return result;
}

const raw = new Map<string, Buffer>();

for (let scene = 0; scene < 101; scene++) {
  const at = 0xb71440 + scene * 0x14, start = rom.readUInt32BE(at), end = rom.readUInt32BE(at + 4);
  if (!start)
    continue;
  for (const header of headers(start, end, 2))
    if (header.rooms)
      for (let room = 0; room < header.rooms.count; room++) {
        const at = header.rooms.offset + room * 8, rs = rom.readUInt32BE(at), re = rom.readUInt32BE(at + 4);
        if (!rs)
          continue;
        for (const setup of headers(rs, re, 3))
          if (setup.actors)
            for (let slot = 0; slot < setup.actors.count; slot++) {
              const offset = setup.actors.offset + slot * 16;
              raw.set([0, scene, room, setup.setup, slot].join('/'), rom.subarray(offset, offset + 16));
            }
      }
}

for (let i = 0; i < mq.readUInt32BE(0); i++) {
  const header = 0x20 + i * 0x20, start = mq.readUInt32BE(header + 4), end = start + mq.readUInt32BE(header);
  const dungeon = mq[header + 8], scene = dungeon === 11 ? 13 : dungeon === 10 ? 11 : dungeon, room = mq[header + 9];
  const offset = start + mq.readUInt16BE(header + 12), count = mq.readUInt16BE(header + 10);
  if (offset < start || offset + count * 16 > end || end > mq.length)
    throw Error('Invalid MQ source list');
  for (let slot = 0; slot < count; slot++)
    raw.set([1, scene, room, 0, slot].join('/'), mq.subarray(offset + slot * 16, offset + (slot + 1) * 16));
}

const positions: Record<string, number[]> = {};

for (const location of oot_enemizer.locations) {
  const key = [+location.mq, location.scene, location.room, location.setup, location.slot].join('/');
  const entry = raw.get(key);
  if (!entry)
    throw Error('Missing authored enemy position: ' + key);
  const position = [2, 4, 6].map(offset => entry.readInt16BE(offset));
  for (const [axis, mode, amount] of location.position) {
    const index = axis === 'x' ? 0 : axis === 'y' ? 1 : 2;
    position[index] = mode === 'add' ? position[index] + Number(amount) : Number(amount);
  }
  positions[key] = position;
}

for (const [, aliases] of oot_enemizer.aliases)
  for (const parts of aliases) {
    const key = [0, ...parts].join('/');
    if (positions[key])
      continue;
    const entry = raw.get(key);
    if (!entry)
      throw Error('Missing authored enemy alias position: ' + key);
    positions[key] = [2, 4, 6].map(offset => entry.readInt16BE(offset));
  }

const data = JSON.stringify(positions, null, 2).replace(/"(?:\\.|[^"\\])*"/g,
  value => "'" + value.slice(1, -1).replace(/\\"/g, '"').replace(/'/g, "\\'") + "'");

const output = 'export const ENEMY_OOT_POSITIONS: Readonly<Record<string, readonly [number, number, number]>> = ' + data + ';\n';

writeFileSync(resolve(root, 'packages/core/src/enemy-position-data.ts'), output);

console.log('Verified and wrote', Object.keys(positions).length, 'OoT/MQ placement coordinates');

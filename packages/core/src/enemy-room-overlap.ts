import { MM_ROOM_OVERLAPS } from './enemy-room-overlap-data';

type MmRoomOverlaps = typeof MM_ROOM_OVERLAPS;

const EXTRA_SETUPS: Record<number, number> = {
  0x2036000: 3, 0x2249000: 1, 0x263b000: 1, 0x265e000: 1,
  0x26bf000: 1, 0x26fc000: 1, 0x2778000: 1, 0x2879000: 2,
  0x2bfe000: 1, 0x2c09000: 1, 0x2da7000: 1, 0x2e1d000: 1,
};

export function readMmRoomOverlaps(rom: Uint8Array): MmRoomOverlaps {
  const data = new DataView(rom.buffer, rom.byteOffset, rom.byteLength);
  const result: MmRoomOverlaps = {};
  for (let scene = 0; scene < 113; scene++) {
    const entry = 0xc5a1e0 + scene * 16;
    const start = data.getUint32(entry);
    const end = data.getUint32(entry + 4);
    if (!start && !end)
      continue;
    const bounds = (at: number, size: number) => {
      if (at < start || at + size > end || end > rom.length)
        throw Error(`Invalid MM scene ${scene} topology range`);
    };
    const pointer = (at: number) => {
      bounds(at, 4);
      const ptr = data.getUint32(at);
      if (ptr >>> 24 !== 2)
        throw Error(`Invalid MM scene ${scene} topology segment`);
      const address = start + (ptr & 0xffffff);
      bounds(address, 0);
      return address;
    };
    let rooms = 0;
    const pairs = new Set<string>();
    for (let setup = 0; setup <= (EXTRA_SETUPS[start] ?? 0); setup++) {
      let roomCount = 0;
      const edges: number[][] = [];
      const read = (header: number, depth = 0) => {
        if (depth > 1)
          throw Error('Recursive MM alternate scene header');
        for (let n = 0, at = header; n < 256; n++, at += 8) {
          bounds(at, 8);
          const command = rom[at];
          if (command === 0x14)
            return;
          if (command === 0x18 && setup && data.getUint32(at + 4)) {
            const alt = pointer(at + 4) + (setup - 1) * 4;
            bounds(alt, 4);
            if (data.getUint32(alt)) {
              read(pointer(alt), depth + 1);
              return;
            }
          }
          if (command === 4) {
            const list = pointer(at + 4);
            roomCount = rom[at + 1];
            bounds(list, roomCount * 8);
          }
          if (command === 0x0e) {
            const list = pointer(at + 4), count = rom[at + 1];
            bounds(list, count * 16);
            edges.length = 0;
            for (let i = 0; i < count; i++)
              edges.push([data.getInt8(list + i * 16), data.getInt8(list + i * 16 + 2)]);
          }
        }
        throw Error('Unterminated MM topology header');
      };
      read(start);
      rooms = Math.max(rooms, roomCount);
      for (const [a, b] of edges) {
        if (a < 0 || b < 0 || a === b)
          continue;
        if (a >= roomCount || b >= roomCount)
          throw Error(`MM scene ${scene}: transition outside room list`);
        pairs.add(`${Math.min(a, b)},${Math.max(a, b)}`);
      }
    }
    result[scene] = { rooms, pairs: [...pairs].sort().map(pair => pair.split(',').map(Number)) };
  }
  return result;
}

function mmRoomsOverlap(scene: number, first: number, second: number): boolean {
  if (first === second)
    return true;
  const entry = MM_ROOM_OVERLAPS[scene];
  if (!entry || first < 0 || second < 0 || first >= entry.rooms || second >= entry.rooms)
    return true;
  const low = Math.min(first, second);
  const high = Math.max(first, second);
  return entry.pairs.some(([a, b]) => a === low && b === high);
}

export function verifyMmRoomOverlaps(rom: Uint8Array): void {
  const actual = readMmRoomOverlaps(rom);
  for (const [scene, entry] of Object.entries(actual))
    for (const [a, b] of entry.pairs)
      if (!mmRoomsOverlap(+scene, a, b))
        throw Error(`MM scene ${scene}: unbudgeted room overlap ${a}/${b}`);
}

export function mmRoomOverlapStates(scene: number, maps: readonly {
  room: number;
  setup: number;
}[]): {
  setup: number;
  maps: number[];
}[] {
  const rooms = [...new Set(maps.map(map => map.room))];
  const states: {
    setup: number;
    maps: number[];
  }[] = [];
  for (const setup of new Set([0, ...maps.map(map => map.setup)])) {
    const active = rooms.map(room => {
      const alternate = maps.findIndex(map => map.room === room && map.setup === setup);
      return alternate >= 0 ? alternate : maps.findIndex(map => map.room === room && map.setup === 0);
    }).filter(index => index >= 0);
    for (let a = 0; a < active.length; a++)
      for (let b = a; b < active.length; b++)
        if (mmRoomsOverlap(scene, maps[active[a]].room, maps[active[b]].room))
          states.push({ setup, maps: a === b ? [active[a]] : [active[a], active[b]] });
  }
  return states;
}

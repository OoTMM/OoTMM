import type { Game } from '@ootmm/data';
import type { Actor, ActorHandlers, AddressingTable, Check, RawRoom, RoomActor, RoomActors } from './types';

import { promises as fs } from 'node:fs';
import { SCENES } from '@ootmm/data';
import { parseScenes, parseNpcs, parseChecks, makeOvKeyXflag } from '@ootmm/data/build';

import { CodeGen } from '../../lib/combo/util/codegen';
import { decompressGame } from '../../lib/combo/decompress';
import { ACTORS_MM, CONFIGS, MM_GENERIC_GROTTOS, MM_SCENES_WITH_EXTRA_SETUPS, OOT_FAIRY_FOUNTAINS, OOT_GENERIC_GROTTOS, OOT_SCRUBS_X2_GROTTOS, OOT_SCRUBS_X3_GROTTOS } from './data';
import { ACTORS_HANDLERS } from './handlers';

const SLICES = 12;

function scenesById(game: 'oot' | 'mm') {
  const data: {[k: number]: string} = {};
  for (const [name, id] of Object.entries(SCENES)) {
    if (name.startsWith(`${game.toUpperCase()}_`)) {
      data[id] = name;
    }
  }
  return data;
}

function sliceOverrideOot(a: Actor) {
  return -1;
}

function sliceOverrideMm(a: Actor) {
  return -1;
}

function sliceOverride(game: Game, a: Actor) {
  return game === 'oot' ? sliceOverrideOot(a) : sliceOverrideMm(a);
}

function sliceSize(game: Game, a: Actor) {
  const conf = CONFIGS[game];
  if (!conf.INTERESTING_ACTORS.includes(a.typeId))
    return 0;
  const override = sliceOverride(game, a);
  if (override !== -1)
    return override;
  return conf.SLICES[a.typeId] || 1;
}

function sortRoomActors(roomActors: RoomActors[]) {
  return roomActors.sort((a, b) => {
    if (a.sceneId !== b.sceneId)
      return a.sceneId - b.sceneId;
    if (a.setupId !== b.setupId)
      return a.setupId - b.setupId;
    if (a.roomId !== b.roomId)
      return a.roomId - b.roomId;
    return 0;
  });
}

function buildAddressingTable(game: Game, roomActors: RoomActors[]): AddressingTable {
  let sceneId = -1;
  let setupId = -1;
  let roomId = -1;
  let scenesTable: number[] = [];
  let setupsTable: number[] = [];
  let roomsTable: number[] = [];
  let bits = 0;

  for (const roomActor of roomActors) {
    /* If it's a new scene, push the offset to the setups table */
    while (sceneId < roomActor.sceneId) {
      sceneId++;
      scenesTable.push(setupsTable.length);
      setupId = -1;
      roomId = -1;
    }

    /* If it's a new setup, push the offset to the room table */
    while (setupId < roomActor.setupId) {
      setupId++;
      setupsTable.push(roomsTable.length);
      roomId = -1;
    }

    for (let slice = 0; slice < SLICES; ++slice) {
      /* We need bits starting at the first useful actor */
      const pred = (a: Actor) => sliceSize(game, a) > slice;
      let firstBit = roomActor.actors.findIndex(pred);
      if (firstBit === -1) {
        firstBit = 0;
      }
      let lastBit = roomActor.actors.findLastIndex(pred);
      if (lastBit === -1) {
        lastBit = 0;
      } else {
        lastBit += 1;
      }
      const bitCount = lastBit - firstBit;

      /* Push the bit pos */
      while (roomId < roomActor.roomId * SLICES + slice) {
        roomId++;
        roomsTable.push(bits - firstBit);
      }

      /* Allocate bits */
      bits += bitCount;
    }
  }

  return { scenesTable, setupsTable, roomsTable, bitCount: bits };
}

function findHeaderOffset(rom: Buffer, offset: number, wantedOp: number) {
  for (;;) {
    const op = rom.readUInt8(offset);
    if (op === wantedOp) {
      return offset;
    }
    if (op === 0x14) {
      return null;
    }
    offset += 8;
  }
}

function filterActors(actors: Actor[], game: Game): Actor[] {
  /* Filter the unintresting trailing actors */
  let lastInterestingActor = -1;
  for (let i = 0; i < actors.length; i++) {
    if (CONFIGS[game].INTERESTING_ACTORS.includes(actors[i].typeId)) {
      lastInterestingActor = i;
    }
  }
  return actors.slice(0, lastInterestingActor + 1);
}

function parseRoomActors(rom: Buffer, raw: RawRoom, game: Game): RoomActors[] {
  const typeIdMask = (game === 'mm' ? 0xfff : 0xffff);
  let actors: Actor[] = [];
  const actorHeaders = findHeaderOffset(rom, raw.vromHeader, 0x01);
  if (actorHeaders !== null) {
    const actorCount = (rom.readUInt32BE(actorHeaders) >> 16) & 0xff;
    const actorsVrom = raw.vromBase + (rom.readUInt32BE(actorHeaders + 4) & 0xffffff);
    for (let actorId = 0; actorId < actorCount; actorId++) {
      const actorVromBase = 0x10 * actorId + actorsVrom;
      const typeId = rom.readUInt16BE(actorVromBase + 0x00) & typeIdMask;
      const posx = rom.readInt16BE(actorVromBase + 0x02);
      const posy = rom.readInt16BE(actorVromBase + 0x04);
      const posz = rom.readInt16BE(actorVromBase + 0x06);
      const rshift = (game === 'mm') ? 7 : 0;
      const rxRaw = rom.readUInt16BE(actorVromBase + 0x08);
      const ryRaw = rom.readUInt16BE(actorVromBase + 0x0a);
      const rzRaw = rom.readUInt16BE(actorVromBase + 0x0c);
      const rx = rxRaw >>> rshift;
      const ry = ryRaw >>> rshift;
      const rz = rzRaw >>> rshift;
      const params = rom.readUInt16BE(actorVromBase + 0x0e);
      let halfDays: number;
      if (game === 'mm') {
        const hi = rxRaw & 7;
        const lo = rzRaw & 0x7f;
        halfDays = (hi << 7) | lo;
      } else {
        halfDays = 0x3ff;
      }
      actors.push({ actorId, halfDays, typeId, pos: [posx, posy, posz], rx, ry, rz, params });
    }
  }
  actors = filterActors(actors, game);

  /* OoT generic grottos */
  if (game !== 'mm' && raw.sceneId === 0x3e && raw.roomId === 0x00) {
    let genericRooms: RoomActors[] = [];
    for (const genericId of OOT_GENERIC_GROTTOS) {
      const genericRoomId = genericId | 0x20;
      const genericActors = actors.map(x => ({...x, roomId: genericRoomId }));
      genericRooms.push({ sceneId: raw.sceneId, setupId: raw.setupId, roomId: genericRoomId, actors: genericActors });
    }
    return genericRooms;
  }

  /* OoT scrub x2 grottos */
  if (game !== 'mm' && raw.sceneId === 0x3e && raw.roomId === 0x09) {
    let genericRooms: RoomActors[] = [];
    for (const genericId of OOT_SCRUBS_X2_GROTTOS) {
      const genericRoomId = genericId | 0x20;
      const genericActors = actors.map(x => ({...x, roomId: genericRoomId }));
      genericRooms.push({ sceneId: raw.sceneId, setupId: raw.setupId, roomId: genericRoomId, actors: genericActors });
    }
    return genericRooms;
  }

  /* OoT scrub x3 grottos */
  if (game !== 'mm' && raw.sceneId === 0x3e && raw.roomId === 0x0c) {
    let genericRooms: RoomActors[] = [];
    for (const genericId of OOT_SCRUBS_X3_GROTTOS) {
      const genericRoomId = genericId | 0x20;
      const genericActors = actors.map(x => ({...x, roomId: genericRoomId }));
      genericRooms.push({ sceneId: raw.sceneId, setupId: raw.setupId, roomId: genericRoomId, actors: genericActors });
    }
    return genericRooms;
  }

  /* OoT fairy fountains */
  if (game !== 'mm' && raw.sceneId === 0x3c && raw.roomId === 0x00) {
    let genericRooms: RoomActors[] = [];
    for (const genericId of OOT_FAIRY_FOUNTAINS) {
      const genericRoomId = genericId | 0x20;
      const genericActors = actors.map(x => ({...x, roomId: genericRoomId }));
      genericRooms.push({ sceneId: raw.sceneId, setupId: raw.setupId, roomId: genericRoomId, actors: genericActors });
    }
    return genericRooms;
  }

  /* MM generic grottos */
  if (game === 'mm' && raw.sceneId === 0x07 && raw.roomId === 0x04) {
    let genericRooms: RoomActors[] = [];
    for (const genericId of MM_GENERIC_GROTTOS) {
      const genericRoomId = genericId | 0x20;
      const genericActors = actors.map(x => ({...x, roomId: genericRoomId }));
      genericRooms.push({ sceneId: raw.sceneId, setupId: raw.setupId, roomId: genericRoomId, actors: genericActors });
    }
    return genericRooms;
  }

  /* MM cow grottos */
  if (game === 'mm' && raw.sceneId === 0x07 && raw.roomId === 0x0a) {
    let cowRooms: RoomActors[] = [];
    cowRooms.push({ sceneId: raw.sceneId, setupId: raw.setupId, roomId: raw.roomId, actors });
    const altRoomId = 0x0f;
    const altActors = actors.map(x => ({...x, roomId: altRoomId }));
    cowRooms.push({ sceneId: raw.sceneId, setupId: raw.setupId, roomId: altRoomId, actors: altActors });
    return cowRooms;
  }

  return [{ sceneId: raw.sceneId, setupId: raw.setupId, roomId: raw.roomId, actors }];
}

function getRawRooms(rom: Buffer, game: 'oot' | 'mq' | 'mm') {
  const rooms: RawRoom[] = [];
  const config = CONFIGS[game];
  for (let sceneId = 0; sceneId < config.SCENE_TABLE_SIZE; sceneId++) {
    const sceneVrom = rom.readUInt32BE(config.SCENE_TABLE_ADDR + sceneId * (game === 'mm' ? 0x10 : 0x14));
    if (sceneVrom === 0)
      continue;
    const roomsHeaderVrom = findHeaderOffset(rom, sceneVrom, 0x04);
    if (roomsHeaderVrom === null)
      continue;
    const roomsCount = (rom.readUInt32BE(roomsHeaderVrom) >> 16) & 0xff;
    const roomsVrom = sceneVrom + (rom.readUInt32BE(roomsHeaderVrom + 4) & 0xffffff);
    for (let roomId = 0; roomId < roomsCount; roomId++) {
      const roomFileVrom = rom.readUint32BE(roomsVrom + roomId * 8);

      /* Push the default setup */
      rooms.push({ sceneId, setupId: 0, roomId, vromBase: roomFileVrom, vromHeader: roomFileVrom });

      /* Look for alternate setups */
      if (game !== 'mm') {
        const altHeaderOffset = findHeaderOffset(rom, roomFileVrom, 0x18);
        if (altHeaderOffset === null)
          continue;
        const altHeaderListVrom = roomFileVrom + (rom.readUInt32BE(altHeaderOffset + 4) & 0xffffff);
        for (let setupId = 1; setupId < 4; ++setupId) {
          const setupAddr = rom.readUInt32BE(altHeaderListVrom + (setupId - 1) * 4);
          if (setupAddr === 0)
            continue;
          const setupVrom = roomFileVrom + (setupAddr & 0xffffff);
          rooms.push({ sceneId, setupId, roomId, vromBase: roomFileVrom, vromHeader: setupVrom });
        }
      } else {
        /* MM setups */
        const extraSetupsCount = MM_SCENES_WITH_EXTRA_SETUPS[sceneVrom];
        if (!extraSetupsCount)
          continue;
        const altHeaderOffset = findHeaderOffset(rom, roomFileVrom, 0x18)!;
        const altHeaderListVrom = roomFileVrom + (rom.readUInt32BE(altHeaderOffset + 4) & 0xffffff);
        for (let i = 0; i < extraSetupsCount; ++i) {
          const setupId = i + 1;
          const setupAddr = rom.readUInt32BE(altHeaderListVrom + i * 4);
          const setupVrom = roomFileVrom + (setupAddr & 0xffffff);
          rooms.push({ sceneId, setupId, roomId, vromBase: roomFileVrom, vromHeader: setupVrom });
        }
      }
    }
  }
  return rooms;
}

async function codegenHeader(addrTableOotMq: AddressingTable, addrTableMm: AddressingTable) {
  const byteCountOot = Math.floor((addrTableOotMq.bitCount + 7) / 8);
  const byteCountMm = Math.floor((addrTableMm.bitCount + 7) / 8);
  const cg = new CodeGen(import.meta.dirname + '/../../include/combo/xflags_data.h', 'XFLAGS_DATA');
  cg.define('XFLAGS_COUNT_OOT', byteCountOot);
  cg.define('XFLAGS_COUNT_MM', byteCountMm);
  return cg.emit();
}

function hexPad(n: number, width: number) {
  const s = n.toString(16);
  return '0x' + '0'.repeat(width - s.length) + s;
}

function binPad(n: number, width: number) {
  const s = n.toString(2);
  return '0b' + '0'.repeat(width - s.length) + s;
}

function decPad(n: number, width: number) {
  const s = n.toString();
  const count = width - s.length;
  return count > 0 ? '0'.repeat(width - s.length) + s : s;
}

async function writeAddressingTable(game: Game, addressingTable: AddressingTable) {
  const base = `${import.meta.dirname}/../../data/static`;
  const scenesTableFilename = `${base}/xflag_table_${game}_scenes.bin`;
  const setupsTableFilename = `${base}/xflag_table_${game}_setups.bin`;
  const roomsTableFilename = `${base}/xflag_table_${game}_rooms.bin`;

  const scenesTableData = Buffer.alloc(addressingTable.scenesTable.length * 2);
  const setupsTableData = Buffer.alloc(addressingTable.setupsTable.length * 2);
  const roomsTableData = Buffer.alloc(addressingTable.roomsTable.length * 2);

  for (let i = 0; i < addressingTable.scenesTable.length; ++i) {
    scenesTableData.writeUInt16BE(addressingTable.scenesTable[i], i * 2);
  }

  for (let i = 0; i < addressingTable.setupsTable.length; ++i) {
    setupsTableData.writeUInt16BE(addressingTable.setupsTable[i], i * 2);
  }

  for (let i = 0; i < addressingTable.roomsTable.length; ++i) {
    roomsTableData.writeInt16BE(addressingTable.roomsTable[i], i * 2);
  }

  return Promise.all([
    fs.writeFile(scenesTableFilename, scenesTableData),
    fs.writeFile(setupsTableFilename, setupsTableData),
    fs.writeFile(roomsTableFilename, roomsTableData),
  ]);
}

function roomActorsFromRaw(rom: Buffer, raw: RawRoom[], game: Game): RoomActors[] {
  const actorsRooms = raw.map(r => parseRoomActors(rom, r, game)).flat();

  /* Inject extra fake rooms */
  if (game === 'mm') {
    actorsRooms.push({
      sceneId: 0x71,
      roomId: 0x00,
      setupId: 0x00,
      actors: [
        { actorId: 0, typeId: ACTORS_MM.POT, halfDays: 0x3ff, pos: [0, 0, 0], params: 0x00, rx: 0, ry: 0, rz: 0x0000, },
        { actorId: 1, typeId: ACTORS_MM.POT, halfDays: 0x3ff, pos: [0, 0, 0], params: 0x00, rx: 0, ry: 0, rz: 0x0000, },
        { actorId: 2, typeId: ACTORS_MM.POT, halfDays: 0x3ff, pos: [0, 0, 0], params: 0x00, rx: 0, ry: 0, rz: 0x0000, },
        { actorId: 3, typeId: ACTORS_MM.POT, halfDays: 0x3ff, pos: [0, 0, 0], params: 0x00, rx: 0, ry: 0, rz: 0x0000, },
        { actorId: 4, typeId: ACTORS_MM.POT, halfDays: 0x3ff, pos: [0, 0, 0], params: 0x00, rx: 0, ry: 0, rz: 0x0000, },
      ]
    });
  }

  return sortRoomActors(actorsRooms);
}

function getGameRoomActor(rom: Buffer, game: 'oot' | 'mq' | 'mm'): RoomActors[] {
  const rawRooms = getRawRooms(rom, game);
  let actorRooms = roomActorsFromRaw(rom, rawRooms, game === 'mq' ? 'oot' : game);
  if (game === 'mq') {
    actorRooms = actorRooms.filter(r => r.sceneId < 0x0a || r.sceneId == 0x0b || r.sceneId == 0x0d).map(r => ({ ...r, sceneId: r.sceneId + 0x70 }));
  }
  return actorRooms;
}

function letterChecks(checks: Check[]) {
  const perScene: { [sceneId: number]: Check[] } = {};

  /* Cluster by scene */
  for (const c of checks) {
    if (!perScene[c.roomActor.sceneId]) {
      perScene[c.roomActor.sceneId] = [];
    }
    perScene[c.roomActor.sceneId].push(c);
  }

  /* Every cluster */
  for (const cluster of Object.values(perScene)) {
    let letterValue = 1;
    for (const c1 of cluster) {
      for (const c2 of cluster) {
        if (c1 === c2) continue;
        if (c1.roomActor.setupId === c2.roomActor.setupId) continue;
        if (c1.roomActor.roomId !== c2.roomActor.roomId) continue;
        if (c1.type !== c2.type) continue;
        if (c1.subtype !== c2.subtype) continue;
        if (c1.roomActor.actor.pos[0] !== c2.roomActor.actor.pos[0]) continue;
        if (c1.roomActor.actor.pos[1] !== c2.roomActor.actor.pos[1]) continue;
        if (c1.roomActor.actor.pos[2] !== c2.roomActor.actor.pos[2]) continue;

        /* We found a match */
        if (c1.letter === undefined) {
          c1.letter = letterValue.toString();
          letterValue++;
        }
        c2.letter = c1.letter;
      }
    }
  }
}

function makeChecks(rooms: RoomActors[], handlers: ActorHandlers): Check[] {
  const checks: Check[] = [];
  for (const r of rooms) {
    for (const a of r.actors) {
      const handler = handlers[a.typeId];
      if (handler) {
        const aa: RoomActor = { sceneId: r.sceneId, roomId: r.roomId, setupId: r.setupId, actor: a };
        handler(checks, aa);
      }
    }
  }

  letterChecks(checks);
  return checks;
}

function outputChecks(game: 'oot' | 'mm', checks: Check[], checkNames: Map<number, string>, filter?: string, filterSubtype?: string) {
  let lastSceneId = -1;
  let lastSetupId = -1;

  for (const check of checks) {
    if (filter && check.type !== filter)
      continue;
    if (filterSubtype && (check.subtype === undefined || check.subtype !== filterSubtype))
      continue;
    const ra = check.roomActor;

    /* Prefix */
    if (ra.sceneId != lastSceneId) {
      if (lastSceneId !== -1)
        console.log('</scene>');
      console.log(`<scene id="${scenesById(game)[ra.sceneId]}">`);
      lastSceneId = ra.sceneId;
      lastSetupId = ra.setupId;
    } else if (ra.setupId != lastSetupId) {
      console.log('');
      lastSetupId = ra.setupId;
    }

    const key = makeOvKeyXflag({ game, sceneId: ra.sceneId, setupId: ra.setupId, roomId: ra.roomId, actorId: ra.actor.actorId, sliceId: check.sliceId ?? 0 });
    let name = checkNames.get(key);

    if (!name) {
      const frags: string[] = [];
      frags.push(`Scene ${ra.sceneId.toString(16)}`);
      frags.push(`Setup ${ra.setupId}`);
      frags.push(`Room ${decPad(ra.roomId, 2)}`);
      frags.push(check.name);
      if (check.roomActor.actor.halfDays !== 0x3ff) {
        frags.push(`(HD:${binPad(check.roomActor.actor.halfDays, 10)})`);
      }
      if (check.letter) {
        frags.push(`[${check.letter.padEnd(2)}]`);
      }
      frags.push(`${decPad(ra.actor.actorId + 1, 2)}`);
      if (check.name2) {
        frags.push(check.name2);
      }
      name = frags.join(' ');
    }

    console.log(`  <xflag type="${check.type}" location="${name}" slice="0x${(check.sliceId ?? 0).toString(16)}" setup="0x${ra.setupId.toString(16)}" room="0x${ra.roomId.toString(16)}" actor="0x${ra.actor.actorId.toString(16)}" item="${check.item}"/>`);
  }
  if (lastSceneId !== -1)
    console.log('</scene>');
}

async function build() {
  /* Get OoT ROM */
  const ootRomCompressed = await fs.readFile(import.meta.dirname + '/../../../../roms/oot.z64');
  const ootDecompressed = await decompressGame('oot', ootRomCompressed);
  const ootRom = Buffer.from(ootDecompressed.rom);

  /* Get MM ROM */
  const mmRomCompressed = await fs.readFile(import.meta.dirname + '/../../../../roms/mm.z64');
  const mmDecompressed = await decompressGame('mm', mmRomCompressed);
  const mmRom = Buffer.from(mmDecompressed.rom);

  /* Get MM Rooms */
  const mmRooms = getGameRoomActor(mmRom, 'mm');

  /* Get MQ ROM */
  const mqRom = await fs.readFile(import.meta.dirname + '/../../../../roms/mq.z64');

  /* Get OoT Rooms */
  const ootRooms = getGameRoomActor(ootRom, 'oot');

  /* Get MQ Rooms */
  const mqRooms = getGameRoomActor(mqRom, 'mq');

  /* Get the merged list */
  const ootMqRooms = [...ootRooms, ...mqRooms];

  /* Build the addr tables */
  const addrTableOotMq = buildAddressingTable('oot', ootMqRooms);
  const addrTableMm = buildAddressingTable('mm', mmRooms);

  /* Codegen and write files */
  await Promise.all([
    codegenHeader(addrTableOotMq, addrTableMm),
    writeAddressingTable('oot', addrTableOotMq),
    writeAddressingTable('mm', addrTableMm),
  ]);

  return { oot: ootMqRooms, mm: mmRooms };
}

async function getCheckNames() {
  const data = new Map<number, string>();
  const [scenes, npcs] = await Promise.all([
    parseScenes(),
    parseNpcs(),
  ]);

  const checks = await parseChecks({ scenes, npcs });
  for (const c of checks) {
    data.set(c.key, c.location);
  }
  return data;
}

export async function run() {
  const [rooms, checkNames] = await Promise.all([
    build(),
    getCheckNames(),
  ]);

  const argGame = process.argv[2];
  const argFilter = process.argv[3];
  const argFilterSubtype = process.argv[4];

  let gameWithMq: Game;
  let game: 'oot' | 'mm';

  if (!argGame)
    return;
  if (['oot', 'mq', 'mm'].includes(argGame)) {
    gameWithMq = argGame as Game;
  } else {
    throw new Error(`Invalid game: ${argGame}`);
  }

  if (gameWithMq === 'mm') {
    game = 'mm';
  } else {
    game = 'oot';
  }

  const gameRooms = rooms[gameWithMq];
  const checks = makeChecks(gameRooms, ACTORS_HANDLERS[game]);
  outputChecks(game, checks, checkNames, argFilter, argFilterSubtype);
}

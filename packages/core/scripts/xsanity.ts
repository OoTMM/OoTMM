import fs from 'fs/promises';

import { CodeGen } from '../lib/combo/util/codegen';
import { decompressGame } from '../lib/combo/decompress';

const POT_ACTOR_TYPE = 0x111;

const SCENE_TABLE_ADDR = 0xb71440;
const SCENE_TABLE_SIZE = 101;

const INTERESTING_ACTORS = [
  POT_ACTOR_TYPE,
];

type RawRoom = {
  sceneId: number;
  roomId: number;
  setupId: number;
  vromBase: number;
  vromHeader: number;
}

type Actor = {
  actorId: number;
  typeId: number;
  params: number;
}

type RoomActors = {
  sceneId: number;
  roomId: number;
  setupId: number;
  actors: Actor[];
}

type AddressingTable = {
  scenesTable: number[];
  setupsTable: number[];
  roomsTable: number[];
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

function buildAddressingTable(roomActors: RoomActors[]): AddressingTable {
  let sceneId = -1;
  let setupId = -1;
  let scenesTable: number[] = [];
  let setupsTable: number[] = [];
  let roomsTable: number[] = [];
  let bits = 0;
  for (const roomActor of roomActors) {
    /* If it's a new scene, push the offset to the setups table */
    if (sceneId !== roomActor.sceneId) {
      sceneId = roomActor.sceneId;
      scenesTable.push(setupsTable.length);
      setupId = -1;
    }

    /* If it's a new setup, push the offset to the room table */
    if (setupId !== roomActor.setupId) {
      setupId = roomActor.setupId;
      setupsTable.push(roomsTable.length);
    }

    /* Push the bit pos */
    roomsTable.push(bits);

    /* Allocate bits */
    bits += roomActor.actors.length;
  }

  return { scenesTable, setupsTable, roomsTable };
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

function filterActors(actors: Actor[]): Actor[] {
  /* Filter the unintresting trailing actors */
  let lastInterestingActor = -1;
  for (let i = 0; i < actors.length; i++) {
    if (INTERESTING_ACTORS.includes(actors[i].typeId)) {
      lastInterestingActor = i;
    }
  }
  return actors.slice(0, lastInterestingActor + 1);
}

function parseRoomActors(rom: Buffer, raw: RawRoom): RoomActors {
  let actors: Actor[] = [];
  const actorHeaders = findHeaderOffset(rom, raw.vromHeader, 0x01);
  if (actorHeaders !== null) {
    const actorCount = (rom.readUInt32BE(actorHeaders) >> 16) & 0xff;
    const actorsVrom = raw.vromBase + (rom.readUInt32BE(actorHeaders + 4) & 0xffffff);
    for (let actorId = 0; actorId < actorCount; actorId++) {
      const actorVromBase = 0x10 * actorId + actorsVrom;
      const typeId = rom.readUInt16BE(actorVromBase + 0x00);
      const params = rom.readUInt16BE(actorVromBase + 0x0e);
      actors.push({ actorId, typeId, params });
    }
  }
  actors = filterActors(actors);
  return { sceneId: raw.sceneId, setupId: raw.setupId, roomId: raw.roomId, actors };
}

function getRawRooms(rom: Buffer) {
  const rooms: RawRoom[] = [];
  for (let sceneId = 0; sceneId < SCENE_TABLE_SIZE; sceneId++) {
    const sceneVrom = rom.readUInt32BE(SCENE_TABLE_ADDR + sceneId * 0x14);
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
    }
  }
  return rooms;
}

async function codegenHeader(roomActors: RoomActors[], addrTable: AddressingTable) {
  const actorsCount = roomActors.reduce((acc, room) => acc + room.actors.length, 0);
  const cg = new CodeGen(__dirname + '/../include/combo/xflags_data.h', 'XFLAGS_DATA');
  cg.define('XFLAGS_COUNT_OOT', Math.floor((actorsCount + 7) / 8));
  return cg.emit();
}

async function codegenSource(roomActors: RoomActors[], addrTable: AddressingTable) {
  const cg = new CodeGen(__dirname + '/../src/oot/xflags_data.c');
  cg.include('combo.h');
  cg.table('kXflagsTableScenes', 'u16', addrTable.scenesTable);
  cg.table('kXflagsTableSetups', 'u16', addrTable.setupsTable);
  cg.table('kXflagsTableRooms', 'u16', addrTable.roomsTable);
  return cg.emit();
}

async function codegen(roomActors: RoomActors[], addrTable: AddressingTable) {
  return Promise.all([
    codegenHeader(roomActors, addrTable),
    codegenSource(roomActors, addrTable),
  ]);
}

async function run() {
  const ootRom = await fs.readFile(__dirname + '/../../../roms/oot.z64');
  const ootDecompressed = await decompressGame('oot', ootRom);
  const { rom } = ootDecompressed;
  const rawRooms = getRawRooms(rom);
  const actorRooms = sortRoomActors(rawRooms.map(raw => parseRoomActors(rom, raw)));
  const addrTable = buildAddressingTable(actorRooms);
  await codegen(actorRooms, addrTable);
}

run().catch(e => {
  console.error(e);
  process.exit(1);
});

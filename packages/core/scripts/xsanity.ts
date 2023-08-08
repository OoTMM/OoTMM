import fs from 'fs/promises';

import { CodeGen } from '../lib/combo/util/codegen';
import { decompressGame } from '../lib/combo/decompress';

type Game = 'oot' | 'mq' | 'mm';

const MM_EXTRA_SCENES = {
  0x71: 5,
};

const MM_POTS_SET_DROPS = [
  'NOTHING',
  'RUPEE_GREEN',
  'RUPEE_BLUE',
  'NOTHING',
  'RUPEE_RED',
  'RUPEE_PURPLE',
  'NOTHING',
  'RUPEE_ORANGE',
  '???',
  '???',
  'RECOVERY_HEART',
  'RECOVERY_HEART',
  '???',
  '???',
  'MAGIC_JAR_SMALL',
  'MAGIC_JAR_LARGE',
  'FAIRY',
  'STRAY_FAIRY',
  'NOTHING',
  'NUTS_10',
  'NOTHING',
  'BOMBS_5',
  'NOTHING',
  'NOTHING',
  'NOTHING',
  'STICK',
  'NOTHING',
  'NOTHING',
  'NOTHING',
  'NOTHING',
  'ARROWS_10',
  'ARROWS_20',
];

const MM_POTS_RANDOM_DROPS = [
  'RUPEE_GREEN',
  'MAGIC_JAR_SMALL',
  'RECOVERY_HEART',
  'RUPEE_RED',
  'NOTHING',
  'NOTHING',
  'RECOVERY_HEART',
  'RECOVERY_HEART',
  'RUPEE_GREEN',
  'RUPEE_BLUE',
  'ARROWS_10',
  'BOMBS_5',
  'MAGIC_JAR_SMALL',
  'MAGIC_JAR_LARGE',
  'NUT',
  'STICK',
];

const MM_SCENES_WITH_EXTRA_SETUPS: {[k: number]: number} = {
  0x2036000: 3, /* Ikana Canyon */
  0x2249000: 1, /* Road to Mountain Village */
  0x263b000: 1, /* Goron Shrine */
  0x265e000: 1, /* Zora Hall */
  0x26bf000: 1, /* Great bay coast */
  0x26fc000: 1, /* Zora cape */
  0x2778000: 1, /* Deku Throne room */
  0x2879000: 2, /* Woodfall */
  0x2bfe000: 1, /* Road to Snowhead */
  0x2c09000: 1, /* Snowhead */
  0x2da7000: 1, /* Goron Racetrack */
  0x2e1d000: 1, /* North Clock Town */
};

const ACTORS_OOT = {
  POT: 0x111,
  FLYING_POT: 0x11d,
};

const ACTORS_MM = {
  POT: 0x82,
  FLYING_POT: 0x8d,
};

const INTERESTING_ACTORS_OOT = Object.values(ACTORS_OOT);
const INTERESTING_ACTORS_MM = Object.values(ACTORS_MM);

const CONFIGS = {
  oot: {
    SCENE_TABLE_ADDR: 0xb71440,
    SCENE_TABLE_SIZE: 101,
    INTERESTING_ACTORS: INTERESTING_ACTORS_OOT,
  },
  mq: {
    SCENE_TABLE_ADDR: 0xba0bb0,
    SCENE_TABLE_SIZE: 101,
    INTERESTING_ACTORS: INTERESTING_ACTORS_OOT,
  },
  mm: {
    SCENE_TABLE_ADDR: 0x00C5A1E0,
    SCENE_TABLE_SIZE: 113,
    INTERESTING_ACTORS: INTERESTING_ACTORS_MM,
  }
}

const ITEM00_DROPS = [
  'RUPEE_GREEN',
  'RUPEE_BLUE',
  'RUPEE_RED',
  'RECOVERY_HEART',
  'BOMB',
  '???',
  '???',
  '???',
  'ARROWS_5',
  'ARROWS_10',
  'ARROWS_30',
  'BOMBS_5',
  'NUTS_5',
  'STICK',
  'MAGIC_JAR_LARGE',
  'MAGIC_JAR_SMALL',
  'DEKU_SEEDS_5',
  'SMALL_KEY',
  'FLEXIBLE',
  'RUPEE_HUGE',
  'RUPEE_PURPLE',
  'SHIELD_DEKU',
  'SHIELD_HYLIAN',
  'TUNIC_ZORA',
  'TUNIC_GORON',
  'BOMBS_5',
];

const FLYING_POT_DROPS = [
  'RECOVERY_HEART',
  'BOMBS_5',
  'DEKU_SEEDS_5',
  'NUTS_5',
  'DEKU_SEEDS_5',
  'RUPEE_PURPLE',
  'TUNIC_GORON',
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
  rz: number;
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

function mergeRoomActors(roomActors: RoomActors[][]): RoomActors[] {
  /* For every unique room, get the longest actor list */
  const merged = new Map<string, RoomActors>();
  for (const roomActor of roomActors.flat()) {
    const key = `${roomActor.sceneId}-${roomActor.setupId}-${roomActor.roomId}`;
    const existing = merged.get(key);
    if (existing === undefined || existing.actors.length < roomActor.actors.length) {
      merged.set(key, roomActor);
    }
  }
  return sortRoomActors(Array.from(merged.values()));
}

function buildAddressingTable(roomActors: RoomActors[]): AddressingTable {
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

    /* Push the bit pos */
    while (roomId < roomActor.roomId) {
      roomId++;
      roomsTable.push(bits);
    }

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

function parseRoomActors(rom: Buffer, raw: RawRoom, game: Game): RoomActors {
  const typeIdMask = (game === 'mm' ? 0xfff : 0xffff);
  let actors: Actor[] = [];
  const actorHeaders = findHeaderOffset(rom, raw.vromHeader, 0x01);
  if (actorHeaders !== null) {
    const actorCount = (rom.readUInt32BE(actorHeaders) >> 16) & 0xff;
    const actorsVrom = raw.vromBase + (rom.readUInt32BE(actorHeaders + 4) & 0xffffff);
    for (let actorId = 0; actorId < actorCount; actorId++) {
      const actorVromBase = 0x10 * actorId + actorsVrom;
      const typeId = rom.readUInt16BE(actorVromBase + 0x00) & typeIdMask;
      const rz = rom.readUInt16BE(actorVromBase + 0x0c);
      const params = rom.readUInt16BE(actorVromBase + 0x0e);
      actors.push({ actorId, typeId, rz, params });
    }
  }
  actors = filterActors(actors, game);
  return { sceneId: raw.sceneId, setupId: raw.setupId, roomId: raw.roomId, actors };
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

async function codegenHeader(roomActorsOotMq: RoomActors[], roomActorsMm: RoomActors[], addrTableOotMq: AddressingTable, addrTableMm: AddressingTable) {
  const actorsCountOotMq = roomActorsOotMq.reduce((acc, room) => acc + room.actors.length, 0);
  const actorsCountMm = roomActorsMm.reduce((acc, room) => acc + room.actors.length, 0);
  const cg = new CodeGen(__dirname + '/../include/combo/xflags_data.h', 'XFLAGS_DATA');
  cg.define('XFLAGS_COUNT_OOT', Math.floor((actorsCountOotMq + 7) / 8));
  cg.define('XFLAGS_COUNT_MM', Math.floor((actorsCountMm + 7) / 8));
  return cg.emit();
}

async function codegenSource(roomActorsOotMq: RoomActors[], roomActorsMm: RoomActors[], addrTableOotMq: AddressingTable, addrTableMm: AddressingTable) {
  const cgOot = new CodeGen(__dirname + '/../src/oot/xflags_data.c');
  cgOot.include('combo.h');
  cgOot.table('kXflagsTableScenes', 'u16', addrTableOotMq.scenesTable);
  cgOot.table('kXflagsTableSetups', 'u16', addrTableOotMq.setupsTable);
  cgOot.table('kXflagsTableRooms', 'u16', addrTableOotMq.roomsTable);

  const cgMm = new CodeGen(__dirname + '/../src/mm/xflags_data.c');
  cgMm.include('combo.h');
  cgMm.table('kXflagsTableScenes', 'u16', addrTableMm.scenesTable);
  cgMm.table('kXflagsTableSetups', 'u16', addrTableMm.setupsTable);
  cgMm.table('kXflagsTableRooms', 'u16', addrTableMm.roomsTable);

  return Promise.all([cgOot.emit(), cgMm.emit()]);
}

function outputPotsPoolOot(roomActors: RoomActors[]) {
  for (const room of roomActors) {
    for (const actor of room.actors) {
      if (actor.typeId === ACTORS_OOT.POT) {
        const item00 = (actor.params >> 0) & 0xff;
        let item: string;
        if (item00 >= 0x1a) {
          item = 'NOTHING';
        } else {
          item = ITEM00_DROPS[item00];
        }
        const key = ((room.setupId & 0x3) << 14) | (room.roomId << 8) | actor.actorId;
        console.log(`Scene ${room.sceneId.toString(16)} Setup ${room.setupId} Room ${room.roomId} Pot ${actor.actorId}, pot,            NONE,                 SCENE_${room.sceneId.toString(16)}, 0x${key.toString(16)}, ${item}`);
      }

      if (actor.typeId === ACTORS_OOT.FLYING_POT) {
        const itemId = (actor.params >> 8) & 0xff;
        let item: string;
        if (itemId >= 0x07) {
          item = 'NOTHING';
        } else {
          item = FLYING_POT_DROPS[itemId];
        }
        const key = ((room.setupId & 0x3) << 14) | (room.roomId << 8) | actor.actorId;
        console.log(`Scene ${room.sceneId.toString(16)} Setup ${room.setupId} Room ${room.roomId} Flying Pot ${actor.actorId}, pot,            NONE,                 SCENE_${room.sceneId.toString(16)}, 0x${key.toString(16)}, ${item}`);
      }
    }
  }
}

function outputPotsPoolMm(roomActors: RoomActors[]) {
  let lastSceneId = -1;
  let lastSetupId = -1;
  for (const room of roomActors) {
    for (const actor of room.actors) {
      if (actor.typeId === ACTORS_MM.POT) {
        let item: string;
        const potType = (actor.params >> 7) & 3;
        const potEnemy = (actor.rz >> 7) & 3;
        if (potEnemy)
          continue;
        switch (potType) {
        case 0:
        case 2:
          /* Set item */
          item = MM_POTS_SET_DROPS[actor.params & 0x1f];
          break;
        case 1:
          /* Magic Pot */
          item = 'MAGIC_JAR_LARGE';
          break;
        case 3:
          /* Random item */
          if (actor.params & 0x10) {
            item = 'NOTHING';
          } else {
            item = MM_POTS_RANDOM_DROPS[actor.params & 0x1f];
          }
          break;
        default:
          item = 'DUMMY';
          break;
        }
        if (item === 'STRAY_FAIRY') {
          continue;
        }
        const key = ((room.setupId & 0x3) << 14) | (room.roomId << 8) | actor.actorId;
        if (room.sceneId != lastSceneId || room.setupId != lastSetupId) {
          console.log('');
          lastSceneId = room.sceneId;
          lastSetupId = room.setupId;
        }
        console.log(`Scene ${room.sceneId.toString(16)} Setup ${room.setupId} Room ${room.roomId} Pot ${actor.actorId}, pot,            NONE,                 SCENE_${room.sceneId.toString(16)}, 0x${key.toString(16)}, ${item}`);
      }

      /*
      if (actor.typeId === ACTORS_MM.FLYING_POT) {
        const key = ((room.setupId & 0x3) << 14) | (room.roomId << 8) | actor.actorId;
        const item = '???';
        console.log(`Scene ${room.sceneId.toString(16)} Setup ${room.setupId} Room ${room.roomId} FLYING Pot ${actor.actorId}, pot,            NONE,                 SCENE_${room.sceneId.toString(16)}, 0x${key.toString(16)}, ${item}`);
      }
      */
    }
  }
}

function roomActorsFromRaw(rom: Buffer, raw: RawRoom[], game: Game): RoomActors[] {
  const actorsRooms = raw.map(r => parseRoomActors(rom, r, game));

  /* Inject extra fake rooms */
  if (game === 'mm') {
    actorsRooms.push({
      sceneId: 0x71,
      roomId: 0x00,
      setupId: 0x00,
      actors: [
        { actorId: 0, typeId: ACTORS_MM.POT, params: 0x00, rz: 0x0000, },
        { actorId: 1, typeId: ACTORS_MM.POT, params: 0x00, rz: 0x0000, },
        { actorId: 2, typeId: ACTORS_MM.POT, params: 0x00, rz: 0x0000, },
        { actorId: 3, typeId: ACTORS_MM.POT, params: 0x00, rz: 0x0000, },
        { actorId: 4, typeId: ACTORS_MM.POT, params: 0x00, rz: 0x0000, },
      ]
    });
  }

  return sortRoomActors(actorsRooms);
}

function getGameRoomActor(rom: Buffer, game: Game) {
  const rawRooms = getRawRooms(rom, game);
  const actorRooms = roomActorsFromRaw(rom, rawRooms, game);
  return actorRooms;
}

async function run() {
  /* Get OoT ROM */
  const ootRomCompressed = await fs.readFile(__dirname + '/../../../roms/oot.z64');
  const ootDecompressed = await decompressGame('oot', ootRomCompressed);
  const ootRom = ootDecompressed.rom;

  /* Get MM ROM */
  const mmRomCompressed = await fs.readFile(__dirname + '/../../../roms/mm.z64');
  const mmDecompressed = await decompressGame('mm', mmRomCompressed);
  const mmRom = mmDecompressed.rom;

  /* Get MM Rooms */
  const mmRooms = getGameRoomActor(mmRom, 'mm');

  /* Get MQ ROM */
  const mqRom = await fs.readFile(__dirname + '/../../../roms/mq.z64');

  /* Get OoT Rooms */
  const ootRooms = getGameRoomActor(ootRom, 'oot');

  /* Get MQ Rooms */
  const mqRooms = getGameRoomActor(mqRom, 'mq');

  /* Get the merged list */
  const ootMqRooms = mergeRoomActors([ootRooms, mqRooms]);

  /* Build the addr tables */
  const addrTableOotMq = buildAddressingTable(ootMqRooms);
  const addrTableMm = buildAddressingTable(mmRooms);

  /* Codegen */
  await Promise.all([
    codegenHeader(ootMqRooms, mmRooms, addrTableOotMq, addrTableMm),
    codegenSource(ootMqRooms, mmRooms, addrTableOotMq, addrTableMm),
  ]);

  outputPotsPoolMm(mmRooms);

  /* Output */
  //outputPotsPoolOot(mqRooms);
}

run().catch(e => {
  console.error(e);
  process.exit(1);
});

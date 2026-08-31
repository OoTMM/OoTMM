import type { Actor, RawRoom, RoomActors } from './types';
import type { Game } from '@ootmm/data';

import fs from 'node:fs/promises';
import { ACTORS_MM, CONFIGS, MM_GENERIC_GROTTOS, MM_SCENES_WITH_EXTRA_SETUPS, OOT_FAIRY_FOUNTAINS, OOT_GENERIC_GROTTOS, OOT_SCRUBS_X2_GROTTOS, OOT_SCRUBS_X3_GROTTOS, ROMS_DIR } from './data';
import { decompressGame } from '../../lib/combo/decompress';

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

function getGameRoomActors(rom: Buffer, game: 'oot' | 'mq' | 'mm'): RoomActors[] {
  const rawRooms = getRawRooms(rom, game);
  let actorRooms = roomActorsFromRaw(rom, rawRooms, game === 'mq' ? 'oot' : game);
  if (game === 'mq') {
    actorRooms = actorRooms.filter(r => r.sceneId < 0x0a || r.sceneId == 0x0b || r.sceneId == 0x0d).map(r => ({ ...r, sceneId: r.sceneId + 0x70 }));
  }
  return actorRooms;
}

export async function makeRooms() {
  /* Get OoT ROM */
  const ootRomCompressed = await fs.readFile(ROMS_DIR + '/oot.z64');
  const ootDecompressed = await decompressGame('oot', ootRomCompressed);
  const ootRom = Buffer.from(ootDecompressed.rom);

  /* Get MM ROM */
  const mmRomCompressed = await fs.readFile(ROMS_DIR + '/mm.z64');
  const mmDecompressed = await decompressGame('mm', mmRomCompressed);
  const mmRom = Buffer.from(mmDecompressed.rom);

  /* Get MM Rooms */
  const mmRooms = getGameRoomActors(mmRom, 'mm');

  /* Get MQ ROM */
  const mqRom = await fs.readFile(ROMS_DIR + '/mq.z64');

  /* Get OoT Rooms */
  const ootRooms = getGameRoomActors(ootRom, 'oot');

  /* Get MQ Rooms */
  const mqRooms = getGameRoomActors(mqRom, 'mq');

  /* Get the merged list */
  const ootMqRooms = [...ootRooms, ...mqRooms];

  return { oot: ootMqRooms, mm: mmRooms };
}

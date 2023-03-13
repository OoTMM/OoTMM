import fs from 'fs';
import path from 'path';
import { argv } from 'process';

const PATHS = {
  HIDAN: 4,
  MIZUsin: 2,
  jyasinzou: 5,
  HAKAdanCH: 2,
  ice_doukutu: 5,
  ganontika: 6,
}

const DUNGEONS = {
  'ydan': 0,
  'ddan': 1,
  'bdan': 2,
  'Bmori1': 3,
  'HIDAN': 4,
  'MIZUsin': 5,
  'jyasinzou': 6,
  'HAKAdan': 7,
  'HAKAdanCH': 8,
  'ice_doukutu': 9,
  'men': 10,
  'ganontika': 11,
} as {[k: string]: number};

type RoomEntry = {
  size: number;
  offset: number;
  dungeon: number;
  room: number;
  actorCount: number;
  actorOffset: number;
  objectCount: number;
  objectOffset: number;
}

type SceneEntry = {
  size: number;
  offset: number;
  dungeon: number;
  transitionActorCount: number;
  transitionActorOffset: number;
  pathOffset: number;
}

function makeRooms() {
  let offset = 0;
  const entries: RoomEntry[] = [];
  const buffers: Buffer[] = [];

  for (const dungeon in DUNGEONS) {
    let roomId = 0;
    for (;;) {
      const roomDataPath = path.join(argv[2], `${dungeon}_room_${roomId}.bin`);
      let roomData: Buffer | null = null;
      try {
        roomData = fs.readFileSync(roomDataPath);
      } catch (e) {
      }
      if (!roomData) {
        break;
      }

      let headerCursor = 0;
      let size = 0;
      let actorCount = 0;
      let actorOffset = 0;
      let objectCount = 0;
      let objectOffset = 0;
      for (;;) {
        const headerOp = roomData.readUInt8(headerCursor * 8 + 0);
        const headerData1 = roomData.readUInt8(headerCursor * 8 + 1);
        const headerData2 = roomData.readUInt32BE(headerCursor * 8 + 4);

        if (headerOp === 0x14) {
          break;
        }

        if (headerOp === 0x01) {
          /* Actor list */
          const actorListPos = headerData2 & 0xffffff;
          actorCount = headerData1;
          actorOffset = size;
          const actorData = roomData.subarray(actorListPos, actorListPos + actorCount * 0x10);
          buffers.push(actorData);
          size += actorData.length;
        }

        if (headerOp === 0x0b) {
          const objectListPos = headerData2 & 0xffffff;
          objectCount = headerData1;
          objectOffset = size;
          const objectData = roomData.subarray(objectListPos, objectListPos + objectCount * 0x2);
          buffers.push(objectData);
          size += objectData.length;
        }

        headerCursor++;
      }

      if (size % 16 !== 0) {
        const padding = Buffer.alloc(16 - (size % 16));
        buffers.push(padding);
        size += padding.length;
      }

      entries.push({
        size,
        offset,
        dungeon: DUNGEONS[dungeon],
        room: roomId,
        actorCount,
        actorOffset,
        objectCount,
        objectOffset,
      });

      offset += size;
      roomId++;
    }
  }

  /* Make the file */
  const globalHeader = Buffer.alloc(0x20);
  globalHeader.writeInt32BE(entries.length, 0);
  const headerSize = entries.length * 0x20;
  const headerBuffer = Buffer.alloc(headerSize);
  const dataOffset = headerSize + 0x20;
  for (let i = 0; i < entries.length; i++) {
    const entry = entries[i];
    const base = i * 0x20;
    headerBuffer.writeUInt32BE(entry.size, base + 0x00);
    headerBuffer.writeUInt32BE(entry.offset + dataOffset, base + 0x04);
    headerBuffer.writeUInt8(entry.dungeon, base + 0x08);
    headerBuffer.writeUInt8(entry.room, base + 0x09);
    headerBuffer.writeUInt16BE(entry.actorCount, base + 0x0a);
    headerBuffer.writeUInt16BE(entry.actorOffset, base + 0x0c);
    headerBuffer.writeUInt16BE(entry.objectCount, base + 0x0e);
    headerBuffer.writeUInt16BE(entry.objectOffset, base + 0x10);
  }

  const fileBuffer = Buffer.concat([globalHeader, headerBuffer, ...buffers]);

  fs.writeFileSync(__dirname + '/../data/static/mq_rooms.bin', fileBuffer);
}

function makeScenes() {
  let offset = 0;
  const entries: SceneEntry[] = [];
  const buffers: Buffer[] = [];

  for (const dungeon in DUNGEONS) {
    const sceneDataPath = path.join(argv[2], `${dungeon}_scene.bin`);
    const sceneData = fs.readFileSync(sceneDataPath);

    let headerCursor = 0;
    let size = 0;
    let transitionActorCount = 0;
    let transitionActorOffset = 0;
    let pathOffset = 0xffff;
    for (;;) {
      const headerOp = sceneData.readUInt8(headerCursor * 8 + 0);
      const headerData1 = sceneData.readUInt8(headerCursor * 8 + 1);
      const headerData2 = sceneData.readUInt32BE(headerCursor * 8 + 4);

      if (headerOp === 0x14) {
        break;
      }

      if (headerOp === 0x0e) {
        /* Transition Actor list */
        const actorListPos = headerData2 & 0xffffff;
        transitionActorCount = headerData1;
        transitionActorOffset = size;
        const actorData = sceneData.subarray(actorListPos, actorListPos + transitionActorCount * 0x10);

        /* Water temple door fix */
        if (dungeon === 'MIZUsin') {
          actorData.writeUInt16BE(0x0082, 0x16 * 0x10 + 0xe);
        }

        buffers.push(actorData);
        size += actorData.length;
      }

      if (headerOp === 0x0d) {
        /* Path list */
        let pathListPos = headerData2 & 0xffffff;
        const pathBuffers: Buffer[] = [];
        let pathsCount = (PATHS as {[k: string]: number})[dungeon] || 0;
        while (pathsCount--) {
          const count = sceneData.readUInt8(pathListPos);
          const singlePathBuffer = Buffer.alloc(8);
          singlePathBuffer.writeUint8(count, 0);
          singlePathBuffer.writeUInt32BE(size, 4);
          const dataPos = sceneData.readUInt32BE(pathListPos + 4) & 0xffffff;
          const data = sceneData.subarray(dataPos, dataPos + count * 6);
          buffers.push(data);
          size += data.length;
          pathListPos += 8;
          pathBuffers.push(singlePathBuffer);
        }
        const pad = Buffer.alloc(4);
        buffers.push(pad);
        size += 4;
        if (size % 4 !== 0) {
          const padding = Buffer.alloc(4 - (size % 4));
          buffers.push(padding);
          size += padding.length;
        }
        pathOffset = size;
        const pathHeader = Buffer.concat(pathBuffers);
        buffers.push(pathHeader);
        size += pathHeader.length;
      }

      headerCursor++;
    }

    if (size % 16 !== 0) {
      const padding = Buffer.alloc(16 - (size % 16));
      buffers.push(padding);
      size += padding.length;
    }

    entries.push({
      size,
      offset,
      dungeon: DUNGEONS[dungeon],
      transitionActorCount,
      transitionActorOffset,
      pathOffset,
    });

    offset += size;
  }

  /* Make the file */
  const globalHeader = Buffer.alloc(0x20);
  globalHeader.writeInt32BE(entries.length, 0);
  const headerSize = entries.length * 0x20;
  const headerBuffer = Buffer.alloc(headerSize);
  const dataOffset = headerSize + 0x20;
  for (let i = 0; i < entries.length; i++) {
    const entry = entries[i];
    const base = i * 0x20;
    headerBuffer.writeUInt32BE(entry.size, base + 0x00);
    headerBuffer.writeUInt32BE(entry.offset + dataOffset, base + 0x04);
    headerBuffer.writeUInt8(entry.dungeon, base + 0x08);
    headerBuffer.writeUInt16BE(entry.transitionActorCount, base + 0x0a);
    headerBuffer.writeUInt16BE(entry.transitionActorOffset, base + 0x0c);
    headerBuffer.writeUInt16BE(entry.pathOffset, base + 0x0e);
  }

  const fileBuffer = Buffer.concat([globalHeader, headerBuffer, ...buffers]);

  fs.writeFileSync(__dirname + '/../data/static/mq_scenes.bin', fileBuffer);
}

function makeMaps() {
  const offset = 0x18570;
  const size = 34 * 0x1ec;
  const kaleido = fs.readFileSync(path.join(argv[2], `ovl_kaleido_scope.bin`));
  const mapData = kaleido.subarray(offset, offset + size);
  fs.writeFileSync(__dirname + '/../data/static/mq_maps.bin', mapData);
}

makeRooms();
makeScenes();
makeMaps();

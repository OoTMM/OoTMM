import fs from 'fs';
import path from 'path';
import { argv } from 'process';

import fireData from './mqdata_fire.json';

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

const MINIMAPS_OFFSET = [
  0x0000,
  0x05cc,
  0x0e44,
  0x1564,
  0x216c,
  0x3258,
  0x45f0,
  0x54a4,
  0x60ac,
  0x6520,
  0x6a78,
];

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
  minimapSize: number;
  minimapOffset: number;
  polyCount: number;
  polyOffset: number;
  polyTypeCount: number;
  polyTypeOffset: number;
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

          /* Shadow fixes */
          if (dungeon === 'HAKAdan') {
            if (roomId === 0x07) {
              actorData.writeUInt16BE(0xffff, 4 * 0x10 + 0x0e);
            }
            else if (roomId === 0x06) {
              actorData.writeUInt16BE(0xffff, 11 * 0x10 + 0x0e);
            }
          }

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
    let minimapSize = 0;
    let minimapOffset = 0;
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

    let extraOffset = size;

    /* Extract minimap if required */
    const sceneId = DUNGEONS[dungeon];
    if (sceneId < 10) {
      const minimapFile = fs.readFileSync(path.join(argv[2], `ovl_map_mark_data.bin`));
      const dataOffset = MINIMAPS_OFFSET[sceneId];
      const dataSize = MINIMAPS_OFFSET[sceneId + 1] - dataOffset;
      const data = minimapFile.subarray(dataOffset, dataOffset + dataSize);

      /* Fire Temple fix */
      if (sceneId === 4) {
        const recordAddr = 0x72 * 3;
        data.writeUint8(0x00, recordAddr + 0x00);
        data.writeUint8(0x01, recordAddr + 0x01);
        data.writeUint8(0x00, recordAddr + 0x02);
        data.writeUint8(0x48, recordAddr + 0x03);
        data.writeUint8(0x3c, recordAddr + 0x04);
        data.writeUint8(0xff, recordAddr + 0x26);
      }

      buffers.push(data);
      minimapSize = dataSize;
      minimapOffset = extraOffset;
      if (dataSize % 16 !== 0) {
        const padding = Buffer.alloc(16 - (dataSize % 16));
        buffers.push(padding);
        minimapSize += padding.length;
      }
      extraOffset += minimapSize;
    }

    /* Poly data */
    let polyCount = 0;
    let polyOffset = extraOffset;

    if (dungeon === 'HIDAN') {
      const polys = fireData.ColDelta.Polys;
      polyCount = polys.length;
      polyOffset = extraOffset;
      const polyBuffer = Buffer.alloc(polyCount * 4);
      for (let i = 0; i < polyCount; i++) {
        polyBuffer.writeUInt16BE(polys[i].Id, i * 4 + 0);
        polyBuffer.writeUInt8(polys[i].Type, i * 4 + 2);
        polyBuffer.writeUInt8(polys[i].Flags, i * 4 + 3);
      }
      extraOffset += polyCount * 4;
      buffers.push(polyBuffer);

      if (extraOffset % 16 !== 0) {
        const padding = Buffer.alloc(16 - (extraOffset % 16));
        buffers.push(padding);
        extraOffset += padding.length;
      }
    }

    /* Poly type data */
    let polyTypeCount = 0;
    let polyTypeOffset = extraOffset;

    if (dungeon === 'HIDAN') {
      const polys = fireData.ColDelta.PolyTypes;
      polyTypeCount = polys.length;
      polyTypeOffset = extraOffset;
      const polyBuffer = Buffer.alloc(polyTypeCount * 0x10);
      for (let i = 0; i < polyTypeCount; i++) {
        polyBuffer.writeUInt32BE(polys[i].Id, i * 0x10 + 0);
        polyBuffer.writeUInt32BE(polys[i].High, i * 0x10 + 4);
        polyBuffer.writeUInt32BE(polys[i].Low, i * 0x10 + 8);
      }
      extraOffset += polyTypeCount * 0x10;
      buffers.push(polyBuffer);
    }

    entries.push({
      size,
      offset,
      dungeon: DUNGEONS[dungeon],
      transitionActorCount,
      transitionActorOffset,
      pathOffset,
      minimapSize,
      minimapOffset,
      polyCount,
      polyOffset,
      polyTypeCount,
      polyTypeOffset,
    });

    offset += extraOffset;
  }

  /* Make the file */
  const globalHeader = Buffer.alloc(0x40);
  globalHeader.writeInt32BE(entries.length, 0);
  const headerSize = entries.length * 0x40;
  const headerBuffer = Buffer.alloc(headerSize);
  const dataOffset = headerSize + 0x40;
  for (let i = 0; i < entries.length; i++) {
    const entry = entries[i];
    const base = i * 0x40;
    headerBuffer.writeUInt32BE(entry.size, base + 0x00);
    headerBuffer.writeUInt32BE(entry.offset + dataOffset, base + 0x04);
    headerBuffer.writeUInt8(entry.dungeon, base + 0x08);
    headerBuffer.writeUInt16BE(entry.transitionActorCount, base + 0x0a);
    headerBuffer.writeUInt16BE(entry.transitionActorOffset, base + 0x0c);
    headerBuffer.writeUInt16BE(entry.pathOffset, base + 0x0e);
    headerBuffer.writeUInt32BE(entry.minimapSize, base + 0x10);
    headerBuffer.writeUInt32BE(entry.minimapOffset + dataOffset + entry.offset, base + 0x14);
    headerBuffer.writeUInt32BE(entry.polyCount, base + 0x18);
    headerBuffer.writeUInt32BE(entry.polyOffset + dataOffset + entry.offset, base + 0x1c);
    headerBuffer.writeUInt32BE(entry.polyTypeCount, base + 0x20);
    headerBuffer.writeUInt32BE(entry.polyTypeOffset + dataOffset + entry.offset, base + 0x24);
  }

  const fileBuffer = Buffer.concat([globalHeader, headerBuffer, ...buffers]);

  fs.writeFileSync(__dirname + '/../data/static/mq_scenes.bin', fileBuffer);
}

function makeMaps() {
  const offset = 0x17aa0;
  const size = 34 * 0x1ec;
  const kaleido = fs.readFileSync(path.join(argv[2], `ovl_kaleido_scope.bin`));
  const mapData = kaleido.subarray(offset, offset + size);

  /* PATCH FIRE TEMPLE MISSING CHEST */
  const fireMissingChestId = 5;

  const recordAddr = 0x20ac;
  mapData.writeUInt32BE(6, recordAddr + 0x10);
  const chestRecordAddr = recordAddr + 0x14 + fireMissingChestId * 0x0c;
  mapData.writeUInt16BE(0x00, chestRecordAddr + 0x00);
  mapData.writeFloatBE(46.5, chestRecordAddr + 0x04);
  mapData.writeFloatBE(-17.5, chestRecordAddr + 0x08);

  fs.writeFileSync(__dirname + '/../data/static/mq_maps.bin', mapData);
}

makeRooms();
makeScenes();
makeMaps();

import fs from 'fs';
import path from 'path';
import { argv } from 'process';

const DUNGEONS = {
  'ydan': 0,
} as {[k: string]: number};

type Entry = {
  size: number;
  offset: number;
  dungeon: number;
  room: number;
  actorCount: number;
  actorOffset: number;
  objectCount: number;
  objectOffset: number;
}

let offset = 0;
const entries: Entry[] = [];
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
      console.log(`room ${roomId} header ${headerCursor}`);
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

fs.writeFileSync(argv[3], fileBuffer);

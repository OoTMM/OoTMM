import fs from 'fs';
import path from 'path';
import { argv } from 'process';

const DUNGEONS = {
  'ydan': 0,
} as {[k: string]: number};

type Entry = {
  dungeon: number;
  room: number;
  actorCount: number;
  actorsOffset: number;
  extraSize: number;
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
    let actorCount = 0;
    let actorOffset = 0;
    let extraSize = 0;
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
        actorOffset = offset;
        const actorData = roomData.subarray(actorListPos, actorListPos + actorCount * 0x10);
        buffers.push(actorData);
        offset += actorData.length;
        extraSize += actorData.length;
      }

      headerCursor++;
    }

    entries.push({
      dungeon: DUNGEONS[dungeon],
      room: roomId,
      actorCount,
      actorsOffset: actorOffset,
      extraSize,
    });

    roomId++;
  }
}

/* Make the file */
const globalHeader = Buffer.alloc(0x10);
globalHeader.writeInt32BE(entries.length, 0);
const headerSize = entries.length * 0x10;
const headerBuffer = Buffer.alloc(headerSize);
const dataOffset = headerSize + 0x10;
for (let i = 0; i < entries.length; i++) {
  const entry = entries[i];
  const base = i * 0x10;
  headerBuffer.writeUInt8(entry.dungeon, base + 0);
  headerBuffer.writeUInt8(entry.room, base + 1);
  headerBuffer.writeUInt16BE(entry.actorCount, base + 2);
  headerBuffer.writeUInt32BE(entry.extraSize, base + 4);
  headerBuffer.writeUInt32BE(entry.actorsOffset + dataOffset, base + 8);
}

const fileBuffer = Buffer.concat([globalHeader, headerBuffer, ...buffers]);

fs.writeFileSync(argv[3], fileBuffer);

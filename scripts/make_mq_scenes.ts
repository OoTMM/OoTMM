import fs from 'fs';
import path from 'path';
import { argv } from 'process';

import { DUNGEONS } from './make_mq_common';

type Entry = {
  size: number;
  offset: number;
  dungeon: number;
  transitionActorCount: number;
  transitionActorOffset: number;
}

let offset = 0;
const entries: Entry[] = [];
const buffers: Buffer[] = [];

for (const dungeon in DUNGEONS) {
  const sceneDataPath = path.join(argv[2], `${dungeon}_scene.bin`);
  const sceneData = fs.readFileSync(sceneDataPath);

  let headerCursor = 0;
  let size = 0;
  let transitionActorCount = 0;
  let transitionActorOffset = 0;
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
      buffers.push(actorData);
      size += actorData.length;
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
}

const fileBuffer = Buffer.concat([globalHeader, headerBuffer, ...buffers]);

fs.writeFileSync(argv[3], fileBuffer);

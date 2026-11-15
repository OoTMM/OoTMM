import { readFileSync, writeFileSync } from 'node:fs';
import { fileURLToPath } from 'node:url';
import { resolve } from 'node:path';
import { readMmRoomOverlaps } from '../../core/src/enemy-room-overlap';

const root = fileURLToPath(new URL('../../../', import.meta.url));

const rom = readFileSync(process.argv[2] ?? resolve(root, '../mm/baseroms/n64-us/baserom-decompressed.z64'));

const data = JSON.stringify(readMmRoomOverlaps(rom), null, 2).replace(/"(?:\\.|[^"\\])*"/g,
  value => "'" + value.slice(1, -1).replace(/\\"/g, '"').replace(/'/g, "\\'") + "'");

writeFileSync(resolve(root, 'packages/core/src/enemy-room-overlap-data.ts'),
  'export const MM_ROOM_OVERLAPS: Record<number, { rooms: number; pairs: number[][] }> = ' + data + ';\n');

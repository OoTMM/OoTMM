import type { Game } from '@ootmm/data';
import type { Actor, ActorHandlers, AddressingTable, Check, RoomActor, RoomActors } from './types';

import { promises as fs } from 'node:fs';
import { SCENES } from '@ootmm/data';
import { parseScenes, parseNpcs, parseChecks, makeOvKeyXflag } from '@ootmm/data/build';

import { CodeGen } from '../../lib/combo/util/codegen';
import { CONFIGS } from './data';
import { ACTORS_HANDLERS } from './handlers';
import { makeRooms } from './rooms';

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
  const rooms = await makeRooms();

  /* Build the addr tables */
  const addrTableOotMq = buildAddressingTable('oot', rooms.oot);
  const addrTableMm = buildAddressingTable('mm', rooms.mm);

  /* Codegen and write files */
  await Promise.all([
    codegenHeader(addrTableOotMq, addrTableMm),
    writeAddressingTable('oot', addrTableOotMq),
    writeAddressingTable('mm', addrTableMm),
  ]);

  return rooms;
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

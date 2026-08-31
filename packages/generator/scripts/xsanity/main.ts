import type { Game } from '@ootmm/data';
import type { ActorHandlers, Check, RoomActor, RoomActors } from './types';

import { SCENES } from '@ootmm/data';
import { parseScenes, parseNpcs, parseChecks } from '@ootmm/data/build';

import { ACTORS_HANDLERS } from './handlers';
import { makeRooms } from './rooms';

function scenesById(game: 'oot' | 'mm') {
  const data: {[k: number]: string} = {};
  for (const [name, id] of Object.entries(SCENES)) {
    if (name.startsWith(`${game.toUpperCase()}_`)) {
      data[id] = name;
    }
  }
  return data;
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

    /* TODO: Repair this eventually */
    //const key = makeOvKeyXflag({ game, sceneId: ra.sceneId, setupId: ra.setupId, roomId: ra.roomId, actorId: ra.actor.actorId, sliceId: check.sliceId ?? 0 });
    //let name = checkNames.get(key);
    let name = null;

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

async function getCheckNames() {
  const data = new Map<number, string>();
  const [scenes, npcs] = await Promise.all([
    parseScenes(),
    parseNpcs(),
  ]);

  const { checks } = await parseChecks({ scenes, npcs });
  for (const c of checks) {
    data.set(c.key, c.location);
  }
  return data;
}

export async function run() {
  const [rooms, checkNames] = await Promise.all([
    makeRooms(),
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

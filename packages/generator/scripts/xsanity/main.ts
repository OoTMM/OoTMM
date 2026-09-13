import type { Game } from '@ootmm/data';
import type { Check, RoomActor, RoomActors } from './types';

import { SCENES } from '@ootmm/data';
import { parseScenes, parseNpcs, parseChecks, makeXflagMatchKey, makeXflagSliceKey, parseXflagMatchKey, xflagIdFromCheckKey } from '@ootmm/data/build';

import { ACTORS_HANDLERS } from './handlers';
import { makeRooms } from './rooms';
import type { HandlerMap } from './handlers/types';

function scenesById(game: Game) {
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

function makeChecks(game: Game, rooms: RoomActors[], handlers: HandlerMap): Check[] {
  const checks: Check[] = [];
  for (const r of rooms) {
    for (const a of r.actors) {
      const handler = handlers[a.typeId];
      if (handler) {
        const aa: RoomActor = { sceneId: r.sceneId, roomId: r.roomId, setupId: r.setupId, actor: a };
        const params = { game, checks, ra: aa };
        handler(params);
      }
    }
  }

  letterChecks(checks);
  return checks;
}

type Identity = {
  sceneId: number;
  setupId: number;
  roomId: number;
  actorId: number;
};

class XflagLookup {
  private locations = new Map<number, string>();
  private identities = new Map<number, Identity[]>();

  constructor(private game: 'oot' | 'mm', private matches: Record<number, number>, checks: any[]) {
    for (const c of checks) {
      if (c.ov === 'xflag') {
        this.locations.set(xflagIdFromCheckKey(c.key), c.location);
      }
    }

    for (const k of Object.keys(matches)) {
      const parsed = parseXflagMatchKey(Number(k));
      if (parsed.kind !== 'actor' || parsed.game !== game) {
        continue;
      }
      const baseId = matches[Number(k)];
      let list = this.identities.get(baseId);
      if (list === undefined) {
        list = [];
        this.identities.set(baseId, list);
      }
      list.push({ sceneId: parsed.sceneId, setupId: parsed.setupId, roomId: parsed.roomId, actorId: parsed.actorId });
    }
  }

  baseId(ident: Identity): number | undefined {
    return this.matches[makeXflagMatchKey(this.game, ident)];
  }

  sliceOf(baseId: number, sliceId: number): number | undefined {
    return sliceId === 0 ? baseId : this.matches[makeXflagSliceKey(baseId, sliceId)];
  }

  /* Every identity the data resolves to this xflag, the aliases included */
  aliases(baseId: number): Identity[] {
    return this.identities.get(baseId) ?? [];
  }

  location(id: number | undefined): string | undefined {
    if (id === undefined) {
      return undefined;
    }
    const location = this.locations.get(id);

    return location?.replace(/^(OOT|MM) /, '');
  }
}

type ActorGroup = {
  sceneId: number;
  identities: Identity[];
  checks: Check[];
  baseId?: number;
};

function identityOf(check: Check): Identity {
  const ra = check.roomActor;
  return { sceneId: ra.sceneId, setupId: ra.setupId, roomId: ra.roomId, actorId: ra.actor.actorId };
}

function groupChecks(checks: Check[], lookup: XflagLookup): ActorGroup[] {
  const groups = new Map<string, ActorGroup>();
  const order: ActorGroup[] = [];

  for (const check of checks) {
    const ident = identityOf(check);
    const baseId = lookup.baseId(ident);
    const groupKey = baseId !== undefined ? `id:${baseId}` : `raw:${ident.sceneId}:${ident.setupId}:${ident.roomId}:${ident.actorId}`;

    let group = groups.get(groupKey);
    if (group === undefined) {
      group = {
        sceneId: ident.sceneId,
        identities: baseId !== undefined ? lookup.aliases(baseId) : [ident],
        checks: [],
        baseId,
      };
      groups.set(groupKey, group);
      order.push(group);
    }

    /* An aliased actor is discovered once per identity, keep one copy */
    if (!group.checks.some(c => (c.sliceId ?? 0) === (check.sliceId ?? 0))) {
      group.checks.push(check);
    }
  }

  return order;
}

function fallbackName(check: Check) {
  const ra = check.roomActor;
  const frags: string[] = [];

  frags.push(`Scene ${ra.sceneId.toString(16)}`);
  frags.push(`Setup ${ra.setupId}`);
  frags.push(`Room ${decPad(ra.roomId, 2)}`);
  frags.push(check.name);
  if (ra.actor.halfDays !== 0x3ff) {
    frags.push(`(HD:${binPad(ra.actor.halfDays, 10)})`);
  }
  if (check.letter) {
    frags.push(`[${check.letter.padEnd(2)}]`);
  }
  frags.push(`${decPad(ra.actor.actorId + 1, 2)}`);
  if (check.name2) {
    frags.push(check.name2);
  }
  return frags.join(' ');
}

function xflagName(check: Check, group: ActorGroup, lookup: XflagLookup) {
  if (group.baseId !== undefined) {
    const known = lookup.location(lookup.sliceOf(group.baseId, check.sliceId ?? 0));
    if (known) {
      return known;
    }
  }
  return fallbackName(check);
}

function hex(n: number) {
  return `0x${n.toString(16)}`;
}

function outputChecks(game: 'oot' | 'mm', checks: Check[], lookup: XflagLookup, filter?: string, filterSubtype?: string) {
  const filtered = checks.filter(c => {
    if (filter && c.type !== filter)
      return false;
    if (filterSubtype && (c.subtype === undefined || c.subtype !== filterSubtype))
      return false;
    return true;
  });

  const scenes = scenesById(game);
  let lastSceneId = -1;

  for (const group of groupChecks(filtered, lookup)) {
    if (group.sceneId !== lastSceneId) {
      if (lastSceneId !== -1)
        console.log('</scene>');
      console.log(`<scene id="${scenes[group.sceneId]}">`);
      lastSceneId = group.sceneId;
    }

    /* A lone unaliased slice stays on one line */
    if (group.identities.length === 1 && group.checks.length === 1) {
      const check = group.checks[0];
      const ident = group.identities[0];
      const name = xflagName(check, group, lookup);
      console.log(`  <xflag type="${check.type}" location="${name}" slice="${check.sliceId ?? 0}" setup="${ident.setupId}" room="${hex(ident.roomId)}" actor="${hex(ident.actorId)}" item="${check.item}"/>`);
      continue;
    }

    console.log('  <actor>');
    for (const ident of group.identities) {
      console.log(`    <match setup="${ident.setupId}" room="${hex(ident.roomId)}" actor="${hex(ident.actorId)}"/>`);
    }
    for (const check of group.checks) {
      const name = xflagName(check, group, lookup);
      console.log(`    <xflag type="${check.type}" location="${name}" slice="${check.sliceId ?? 0}" item="${check.item}"/>`);
    }
    console.log('  </actor>');
  }

  if (lastSceneId !== -1)
    console.log('</scene>');
}

async function getXflagData() {
  const [scenes, npcs] = await Promise.all([
    parseScenes(),
    parseNpcs(),
  ]);

  return parseChecks({ scenes, npcs });
}

export async function run() {
  const [rooms, xflagData] = await Promise.all([
    makeRooms(),
    getXflagData(),
  ]);

  const game = process.argv[2];
  const argFilter = process.argv[3];
  const argFilterSubtype = process.argv[4];

  if (!game)
    return;
  if (game !== 'oot' && game !== 'mm') {
    throw new Error(`Invalid game: ${game}`);
  }

  const gameRooms = rooms[game];
  const checks = makeChecks(game, gameRooms, ACTORS_HANDLERS[game]);
  const lookup = new XflagLookup(game, xflagData.matches, xflagData.checks);
  outputChecks(game, checks, lookup, argFilter, argFilterSubtype);
}

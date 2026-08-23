//import type { Game } from '@ootmm/core';
//
//import { gameId, CHECKS } from '@ootmm/core';
//import { checkKey } from '../combo/randomizer/checks';
//
///* TODO: very ugly, will fix */
//async function buildManifestChecks(game: Game) {
//  const pool = POOL[game];
//  const set = new Set<number>();
//  for (const entry of pool) {
//    const location = gameId(game, String(entry.location), ' ');
//    const type = String(entry.type);
//    const scene = gameId(game, String(entry.scene), '_');
//    let id = null;
//    if (type === 'npc') {
//      id = gameId(game, String(entry.id), '_');
//    } else {
//      id = Number(entry.id);
//    }
//    const key = checkKey({ game, scene, location, type, id } as any);
//    if (set.has(key)) {
//      console.warn(`Duplicate check key 0x${key.toString(16)} for ${location}`);
//    }
//    set.add(key);
//  }
//}
//
//export async function buildManifests() {
//  await Promise.all([
//    //buildManifestChecks('oot'),
//    //buildManifestChecks('mm'),
//  ]);
//}

export async function buildManifests() {}

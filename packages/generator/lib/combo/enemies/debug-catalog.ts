import { ENEMIES, enemyShuffleTargets } from '@ootmm/core';
import type { Patchfile } from '../patch-build/patchfile';

type Game = 'oot' | 'mm';

export function debugEnemyForms(game: Game) {
  const club = ENEMIES.find(enemy => enemy.key === 'oot-club-moblin')!;
  const targets = enemyShuffleTargets(game, true);
  if (game === 'oot') targets.push({ ...club, spawnParams: 0 });
  return targets.map(enemy => ({ enemy, foreign: enemy.game !== game, params: enemy.spawnParams }));
}

export function installDebugEnemyCatalog(patch: Patchfile) {
  const habitats = { ground: 0, air: 1, 'water-surface': 2, underwater: 3 };
  for (const game of ['oot', 'mm'] as const) {
    const symbol = `DEBUG_ENEMY_TABLE_${game.toUpperCase()}`;
    if (!patch.symbols[game].has(symbol)) continue;
    const entries = new Map<number, string>();
    for (const { enemy, foreign, params } of debugEnemyForms(game)) {
      const id = enemy.actorId + (foreign ? 0xc00 : 0);
      const key = ((habitats[enemy.habitat] << 28) | (id << 16) | (params & 0xffff)) >>> 0;
      const name = enemy.name.replace(/[^ -~]/g, '').slice(0, 31);
      if (!entries.has(key)) entries.set(key, foreign ? `${enemy.game === 'oot' ? 'OoT' : 'MM'} ${name}` : name);
    }
    const counts = new Map<string, number>();
    for (const label of entries.values()) counts.set(label, (counts.get(label) ?? 0) + 1);
    const rows = [...entries].sort((a, b) => a[1].localeCompare(b[1])).map(([key, label]) => ({
      key,
      name: counts.get(label) === 1 ? label : `${label} [${(key & 0xffff).toString(16)}]`,
    }));
    let offset = (rows.length + 1) * 8;
    const data = new Uint8Array(offset + rows.reduce((n, row) => n + row.name.length + 1, 0));
    const view = new DataView(data.buffer);
    const encoder = new TextEncoder();
    rows.forEach((row, i) => {
      view.setUint32(i * 8, offset);
      view.setUint32(i * 8 + 4, row.key);
      data.set(encoder.encode(row.name), offset);
      offset += row.name.length + 1;
    });
    const vrom = game === 'oot' ? 0xd9000000 : 0xd9004000;
    if (data.length > 0x4000) throw new Error('Debug enemy catalog exceeds its ROM allocation');
    patch.addNewFile({ name: `debug/${game}/enemies`, vrom, data, compressed: false });
    const pointer = new Uint8Array(4);
    new DataView(pointer.buffer).setUint32(0, vrom);
    patch.addSymbolPatch(symbol, pointer);
  }
}

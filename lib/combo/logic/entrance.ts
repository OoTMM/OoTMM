import { Random, sample } from "../random";
import { EntranceOverrides, pathfind, Reachable } from "./pathfind";
import { World, WorldEntrance } from "./world";

class EntranceShuffler {
  constructor(
    private world: World,
    private random: Random,
  ){
  }

  private isBossAssignable(src: WorldEntrance, dst: WorldEntrance, overrides: EntranceOverrides, reachable: Reachable) {
    /* Pathfind with the override */
    overrides = { ...overrides, [src.from]: { [src.to]: dst.to } };
    reachable = pathfind(this.world, {}, false, undefined, { entranceOverrides: overrides, ignoreItems: true });

    /* Check if every location is reachable */
    const dungeon = this.world.areas[dst.to].dungeon!;
    const locations = [...this.world.dungeons[dungeon]];
    if (locations.some(l => !reachable.locations.has(l))) {
      return false;
    }
    return true;
  }

  private fixBosses() {
    const bossEntrances = this.world.entrances.filter(e => this.world.areas[e.to].boss);
    const bossEntrancesByDungeon: {[k: string]: WorldEntrance} = {};
    const overrides: EntranceOverrides = {};
    const combinations: {[k: string]: string[]} = {};
    const placed: {[k: string]: string} = {};

    /* Set up null overrides */
    for (const e of bossEntrances) {
      const override = overrides[e.from] || {};
      override[e.to] = null;
      overrides[e.from] = override;
      const dungeon = this.world.areas[e.to].dungeon!;
      bossEntrancesByDungeon[dungeon] = e;
    }

    const dungeons = Object.keys(bossEntrancesByDungeon);

    /* Set up base reachability */
    const reachable = pathfind(this.world, {}, false, undefined, { entranceOverrides: overrides, ignoreItems: true });
    for (const dungeonSrc of dungeons) {
      for (const dungeonDst of dungeons) {
        const src = bossEntrancesByDungeon[dungeonSrc];
        const dst = bossEntrancesByDungeon[dungeonDst];
        if (this.isBossAssignable(src, dst, overrides, reachable)) {
          const combination = combinations[dungeonDst] || [];
          combination.push(dungeonSrc);
          combinations[dungeonDst] = combination;
        }
      }
    }

    for (;;) {
      const keys = Object.keys(combinations);
      if (keys.length === 0) {
        break;
      }
      const sorted = keys.sort((a, b) => combinations[a].length - combinations[b].length);
      const boss = sorted[0];
      const src = sample(this.random, combinations[boss]);
      placed[src] = boss;
      delete combinations[boss];
      for (const k of Object.keys(combinations)) {
        combinations[k] = combinations[k].filter(s => s !== src);
      }
    }

    console.log(placed);
  }

  run() {
    this.fixBosses();
  }
};

export function shuffleEntrances(world: World, random: Random) {
  const shuffler = new EntranceShuffler(world, random);
  shuffler.run();
}

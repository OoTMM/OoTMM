import { HINTS, ENTRANCES, REGIONS, SCENES, NPC } from '@ootmm/data';
import { Game, Settings, Random, sample } from '@ootmm/core';

import { LogicResult } from '../logic';
import { DATA_HINTS_POOL } from '../data';
import { BOSS_INDEX_BY_DUNGEON, World, WorldCheck } from '../logic/world';
import { HintGossip } from '../logic/hints';
import { padBuffer16, toU32Buffer } from '../util';
import { Patchfile } from '../patch-build/patchfile';
import { makeLocation, isLocationFullyShuffled } from '../logic/locations';
import { regionData } from '../logic/regions';
import { ItemGroups, ItemHelpers, ItemsCount } from '../items';
import { bufReadU32BE, bufWriteI8, bufWriteU16BE, bufWriteU32BE, bufWriteU8 } from '../util/buffer';
import { concatUint8Arrays } from 'uint8array-extras';
import { END_BOSS_METADATA } from '../logic/boss';
import { PATH_EVENT_DATA } from '../logic/analysis-path';
import { DUNGEONS_BY_KEY } from '../logic/dungeons';
import { Options } from '../options';
import { RandomizerPatcherConfig } from './config';
import { gi } from './gi';
import { RandomizerPatcherStartingItems } from './starting-items';

const HINT_OFFSETS = {
  KEY: 0,
  TYPE: 1,
  REGION: 2,
  WORLD: 3,
  ITEM: 4,
  ITEM2: 6,
  ITEM3: 8,
  PLAYER: 10,
  PLAYER2: 11,
  PLAYER3: 12,
  IMPORTANCE: 13,
  IMPORTANCE2: 14,
  IMPORTANCE3: 15,
};

const entrance2 = (srcGame: Game, dstGame: Game, name: string) => {
  const entr = ENTRANCES[name as keyof typeof ENTRANCES];
  if (entr === undefined) {
    throw new Error(`Unknown entrance ${name}`);
  }
  let data = entr.id;
  if (srcGame !== dstGame) {
    data = (data | 0x80000000) >>> 0;
  }
  return data;
}

const checkId = (check: WorldCheck) => {
  if (check.type === 'npc') {
    if (!NPC.hasOwnProperty(check.id)) {
      throw new Error(`Unknown NPC ${check.id}`);
    }
    return (NPC as any)[check.id];
  }
  return check.id;
}

function checkKey(check: WorldCheck): number {
  /* Extract the ID */
  const id = checkId(check);

  /* Extract the type */
  let typeId: number;
  switch (check.type) {
    case 'chest':
      typeId = 0x01;
      break;
    case 'collectible':
      typeId = 0x02;
      break;
    case 'npc':
      typeId = 0x03;
      break;
    case 'gs':
      typeId = 0x04;
      break;
    case 'sf':
      typeId = 0x05;
      break;
    case 'cow':
      typeId = 0x06;
      break;
    case 'shop':
      typeId = 0x07;
      break;
    case 'scrub':
      typeId = 0x08;
      break;
    case 'sr':
      typeId = 0x09;
      break;
    case 'fish':
      typeId = 0x0A;
      break;
    case 'pot':
    case 'crate':
    case 'barrel':
    case 'grass':
    case 'tree':
    case 'bush':
    case 'rock':
    case 'soil':
    case 'fairy':
    case 'snowball':
    case 'hive':
    case 'rupee':
    case 'heart':
    case 'fairy_spot':
    case 'wonder':
    case 'butterfly':
    case 'redboulder':
    case 'icicle':
    case 'redice':
      /* xflag */
      typeId = 0x10 + ((id >> 16) & 0xf);
      break;
  }

  /* Extract the scene ID */
  let sceneId = 0;
  switch (check.type) {
    case 'chest':
    case 'collectible':
    case 'sf':
    case 'pot':
    case 'crate':
    case 'barrel':
    case 'grass':
    case 'tree':
    case 'bush':
    case 'rock':
    case 'soil':
    case 'fairy':
    case 'snowball':
    case 'hive':
    case 'rupee':
    case 'heart':
    case 'fairy_spot':
    case 'wonder':
    case 'butterfly':
    case 'redboulder':
    case 'icicle':
    case 'redice':
      sceneId = (SCENES as any)[check.scene];
      if (sceneId === undefined) {
        throw new Error(`Unknown scene ${check.scene}`);
      }
      break;
    default:
      break;
  }

  /* Build the key */
  let key = 0;
  key = (key | ((typeId & 0xff) << 24)) >>> 0;
  key = (key | ((sceneId & 0xff) << 16)) >>> 0;
  key = (key | (id & 0xffff)) >>> 0;

  return key;
}

async function makeCloakGi(key: number, seed: string, settings: Settings, logic: LogicResult): Promise<number> {
  const random = new Random();
  await random.seed(key.toString(16) + '\x00' + seed);

  for (; ;) {
    const locs = [...logic.items.keys()];
    const loc = sample(random, locs);
    const item = logic.items.get(loc)!;

    if (!ItemHelpers.isItemMajor(item.item) && !ItemHelpers.isSilverRupee(item.item) && !ItemHelpers.isKey(item.item) && !ItemHelpers.isBossKey(item.item)) continue;
    if (ItemGroups.JUNK.has(item.item)) continue;
    if (!isLocationFullyShuffled(settings, logic.fixedLocations, logic.items, logic.plandoLocations, loc, { songs: true, noPlando: true })) continue;

    return gi(settings, 'oot', item.item, false);
  }
}

function playerId(player: number | 'all'): number {
  if (player === 'all')
    return 0xff;
  return player + 1;
}

const hintBuffer = (settings: Settings, game: Game, gossip: string, hint: HintGossip): Uint8Array => {
  const data = new Uint8Array(0x10);
  data.fill(0xff);
  let gossipData = DATA_HINTS_POOL[game][gossip];
  if (!gossipData) {
    throw new Error(`Unknown gossip ${gossip} for game ${game}`);
  }
  let id = null;
  switch (gossipData.type) {
    case 'gossip':
      id = gossipData.id;
      break;
    case 'gossip-grotto':
      id = gossipData.id | 0x20;
      break;
    case 'gossip-moon':
      id = gossipData.id | 0x40;
      break;
  }
  switch (hint.type) {
    case 'path':
      {
        const { path } = hint;
        const player = path.player === 'all' ? 0xff : path.player + 1;
        let pathId: number;
        let pathSubId: number;
        const regionD = regionData(hint.region);
        const region = (REGIONS as any)[regionD.id];
        if (region === undefined) {
          throw new Error(`Unknown region ${hint.region}`);
        }
        switch (path.type) {
          case 'woth':
            pathId = 0;
            pathSubId = 0;
            break;
          case 'triforce':
            pathId = 1;
            pathSubId = { 'Power': 0, 'Courage': 1, 'Wisdom': 2 }[path.triforce];
            break;
          case 'dungeon':
            pathId = 2;
            pathSubId = DUNGEONS_BY_KEY.get(path.dungeon)!.id;
            break;
          case 'boss':
            pathId = 3;
            pathSubId = BOSS_INDEX_BY_DUNGEON[path.boss];
            break;
          case 'end-boss':
            pathId = 4;
            pathSubId = END_BOSS_METADATA.findIndex((e) => e.name === path.boss);
            break;
          case 'event':
            pathId = 5;
            pathSubId = PATH_EVENT_DATA.findIndex((e) => e.key === path.event);
            break;
        }

        bufWriteU8(data, HINT_OFFSETS.KEY, id);
        bufWriteU8(data, HINT_OFFSETS.TYPE, 0x00);
        bufWriteU8(data, HINT_OFFSETS.REGION, region);
        bufWriteU8(data, HINT_OFFSETS.WORLD, regionD.world + 1);
        bufWriteU16BE(data, HINT_OFFSETS.ITEM, pathId);
        bufWriteU16BE(data, HINT_OFFSETS.ITEM2, pathSubId);
        bufWriteU8(data, HINT_OFFSETS.PLAYER, player);
      }
      break;
    case 'foolish':
      {
        const regionD = regionData(hint.region);
        const region = (REGIONS as any)[regionD.id];
        if (region === undefined) {
          throw new Error(`Unknown region ${hint.region}`);
        }
        bufWriteU8(data, HINT_OFFSETS.KEY, id);
        bufWriteU8(data, HINT_OFFSETS.TYPE, 0x01);
        bufWriteU8(data, HINT_OFFSETS.REGION, region);
        bufWriteU8(data, HINT_OFFSETS.WORLD, regionD.world + 1);
      }
      break;
    case 'item-exact':
      {
        const check = (HINTS as any)[hint.check];
        if (check === undefined) {
          throw new Error(`Unknown named check: ${hint.check}`);
        }
        const items = hint.items;
        const itemsGI = hint.items.map((item) => gi(settings, 'oot', item.item, false));
        bufWriteU8(data, HINT_OFFSETS.KEY, id);
        bufWriteU8(data, HINT_OFFSETS.TYPE, 0x02);
        bufWriteU8(data, HINT_OFFSETS.REGION, check);
        bufWriteU8(data, HINT_OFFSETS.WORLD, hint.world + 1);
        bufWriteU16BE(data, HINT_OFFSETS.ITEM, itemsGI[0]);
        bufWriteU8(data, HINT_OFFSETS.PLAYER, playerId(items[0].player));
        bufWriteI8(data, HINT_OFFSETS.IMPORTANCE, hint.importances[0]);
        if (items.length > 1) {
          bufWriteU16BE(data, HINT_OFFSETS.ITEM2, itemsGI[1]);
          bufWriteU8(data, HINT_OFFSETS.PLAYER2, playerId(items[1].player));
          bufWriteI8(data, HINT_OFFSETS.IMPORTANCE2, hint.importances[1]);
        }
        if (items.length > 2) {
          bufWriteU16BE(data, HINT_OFFSETS.ITEM3, itemsGI[2]);
          bufWriteU8(data, HINT_OFFSETS.PLAYER3, playerId(items[2].player));
          bufWriteI8(data, HINT_OFFSETS.IMPORTANCE3, hint.importances[2]);
        }
      }
      break;
    case 'item-region':
      {
        const regionD = regionData(hint.region);
        const region = (REGIONS as any)[regionD.id];
        const item = hint.item;
        if (region === undefined) {
          throw new Error(`Unknown region ${hint.region}`);
        }
        const itemGI = gi(settings, 'oot', item.item, false);
        bufWriteU8(data, HINT_OFFSETS.KEY, id);
        bufWriteU8(data, HINT_OFFSETS.TYPE, 0x03);
        bufWriteU8(data, HINT_OFFSETS.REGION, region);
        bufWriteU8(data, HINT_OFFSETS.WORLD, regionD.world + 1);
        bufWriteU16BE(data, HINT_OFFSETS.ITEM, itemGI);
        bufWriteU8(data, HINT_OFFSETS.PLAYER, playerId(item.player));
        bufWriteI8(data, HINT_OFFSETS.IMPORTANCE, hint.importance);
      }
      break;
    case 'junk':
      {
        bufWriteU8(data, HINT_OFFSETS.KEY, id);
        bufWriteU8(data, HINT_OFFSETS.TYPE, 0x04);
        bufWriteU16BE(data, HINT_OFFSETS.ITEM, hint.id);
      }
      break;
  }
  return data;
}

class PatchRandomizer {
  private world: World;

  constructor(
    private worldId: number,
    private logic: LogicResult,
    private options: Options,
    private settings: Settings,
    private patchfile: Patchfile,
  ) {
    this.world = logic.worlds[worldId];
  }

  async run() {
    const { worldId, logic, settings } = this;

    const bufferConfig = RandomizerPatcherConfig.run({ worldId, logic, settings });
    const startingItems = RandomizerPatcherStartingItems.run({ worldId, logic, settings });

    this.patchfile.addNewFile({ vrom: 0xf0200000, data: bufferConfig, compressed: true });
    this.patchfile.addNewFile({ vrom: 0xf0300000, data: startingItems, compressed: false });
    this.patchfile.addNewFile({ vrom: 0xf0400000, data: await this.gameChecks('oot'), compressed: false });
    this.patchfile.addNewFile({ vrom: 0xf0500000, data: await this.gameChecks('mm'), compressed: false });
    this.patchfile.addNewFile({ vrom: 0xf0600000, data: this.gameHints('oot'), compressed: true });
    this.patchfile.addNewFile({ vrom: 0xf0700000, data: this.gameHints('mm'), compressed: true });
    this.patchfile.addNewFile({ vrom: 0xf0800000, data: this.gameEntrances('oot'), compressed: true });
    this.patchfile.addNewFile({ vrom: 0xf0900000, data: this.gameEntrances('mm'), compressed: true });
  }

  private async gameChecks(game: Game): Promise<Uint8Array> {
    const buffers: Uint8Array[] = [];
    for (const locId in this.world.checks) {
      const loc = makeLocation(locId, this.worldId);
      const c = this.world.checks[locId];
      const item = this.logic.items.get(loc)!;

      if (c.game !== game) {
        continue;
      }

      /* Skip cows if not shuffled */
      if (c.game === 'oot' && c.type === 'cow' && !this.settings.cowShuffleOot) continue;
      if (c.game === 'mm' && c.type === 'cow' && !this.settings.cowShuffleMm) continue;

      const key = checkKey(c);
      const itemGi = gi(this.settings, game, item.item, true);
      const b = new Uint8Array(16);
      bufWriteU32BE(b, 0, key);
      bufWriteU16BE(b, 4, playerId(item.player));
      bufWriteU16BE(b, 6, itemGi);
      let cloakGi = 0;
      if (this.settings.cloakTraps && ItemGroups.TRAPS.has(item.item)) {
        cloakGi = await makeCloakGi(key, this.options.seed, this.settings, this.logic);
      }
      bufWriteU16BE(b, 8, cloakGi);
      buffers.push(b);
    }

    /* Sort by key ascending */
    buffers.sort((a, b) => bufReadU32BE(a, 0) < bufReadU32BE(b, 0) ? -1 : 1);
    const end = new Uint8Array(16);
    end.fill(0xff);
    buffers.push(end);
    return concatUint8Arrays(buffers);
  }

  private gameHints(game: Game): Uint8Array {
    const buffers: Uint8Array[] = [];
    const hints = this.logic.hints[this.worldId];
    for (const gossip in hints.gossip) {
      const h = hints.gossip[gossip];
      if (h.game !== game) {
        continue;
      }
      buffers.push(hintBuffer(this.settings, game, gossip, h));
    }
    const b = new Uint8Array(0x10);
    b.fill(0xff);
    buffers.push(b);
    return padBuffer16(concatUint8Arrays(buffers));
  }

  private gameEntrances(game: Game) {
    const data: number[] = [];
    for (const [src, dst] of this.world.entranceOverrides) {
      const srcEntrance = ENTRANCES[src as keyof typeof ENTRANCES];
      const dstEntrance = ENTRANCES[dst as keyof typeof ENTRANCES];
      if (srcEntrance.game !== game)
        continue;
      const srcId = entrance2(srcEntrance.game, srcEntrance.game as Game, src);
      const dstId = entrance2(srcEntrance.game, dstEntrance.game as Game, dst);
      data.push(srcId, dstId);
    }
    data.push(0xffffffff);
    data.push(0xffffffff);
    return padBuffer16(toU32Buffer(data));
  }
}

export async function patchRandomizer(worldId: number, logic: LogicResult, options: Options, settings: Settings, patchfile: Patchfile) {
  const pr = new PatchRandomizer(worldId, logic, options, settings, patchfile);
  await pr.run();
}

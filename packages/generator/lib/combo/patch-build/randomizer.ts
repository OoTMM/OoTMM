import path from 'path';
import { HINTS, ENTRANCES, REGIONS, SCENES, NPC } from '@ootmm/data';
import { Game, SETTINGS, Settings, SPECIAL_CONDS, SPECIAL_CONDS_FIELDS, Random, sample } from '@ootmm/core';

import { LogicResult } from '../logic';
import { GI, DATA_HINTS_POOL } from '../data';
import { BOSS_INDEX_BY_DUNGEON, World, WorldCheck } from '../logic/world';
import { HintGossip, WorldHints } from '../logic/hints';
import { countMapAdd, gameId, padBuffer16, toI8Buffer, toU16Buffer, toU32Buffer, toU8Buffer } from '../util';
import { Patchfile } from './patchfile';
import { locationsZelda, makeLocation, makePlayerLocations, getPreActivatedOwlsLocations, isLocationFullyShuffled } from '../logic/locations';
import { CONFVARS_VALUES } from '../confvars';
import { Region, regionData } from '../logic/regions';
import { Item, ItemGroups, ItemHelpers, Items, ItemsCount } from '../items';
import { SharedItemGroups } from '../logic/shared';
import { bufReadU32BE, bufWriteI8, bufWriteU16BE, bufWriteU32BE, bufWriteU8 } from '../util/buffer';
import { concatUint8Arrays } from 'uint8array-extras';
import { DUNGEON_ENTRANCES } from '../logic/entrance';
import { END_BOSS_METADATA } from '../logic/boss';
import { PATH_EVENT_DATA } from '../logic/analysis-path';
import { DUNGEONS, DUNGEONS_BY_KEY } from '../logic/dungeons';
import { Options } from '../options';
import { ITEMS_SUBSTITUTIONS, SHARED_ITEMS } from './items';
import { worldConfig } from './world-config';

const DUNGEON_REWARD_LOCATIONS = [
  'OOT Deku Tree Boss',
  'OOT Dodongo Cavern Boss',
  'OOT Jabu-Jabu Boss',
  'OOT Forest Temple Boss',
  'OOT Fire Temple Boss',
  'OOT Water Temple Boss',
  'OOT Shadow Temple Boss',
  'OOT Spirit Temple Boss',
  'MM Woodfall Temple Boss',
  'MM Snowhead Temple Boss',
  'MM Great Bay Temple Boss',
  'MM Stone Tower Temple Inverted Boss',
];

const gi = (settings: Settings, game: Game, item: Item, generic: boolean) => {
  let itemId = item.id;
  if (generic) {
    if (ItemHelpers.isSmallKeyHideout(item) && settings.smallKeyShuffleHideout !== 'anywhere') {
      itemId = gameId(game, 'SMALL_KEY', '_');
    } else if (ItemHelpers.isKeyRingHideout(item) && settings.smallKeyShuffleHideout !== 'anywhere') {
      itemId = gameId(game, 'KEY_RING', '_');
    } else if (ItemHelpers.isSmallKeyTCG(item) && settings.smallKeyShuffleChestGame !== 'anywhere') {
      itemId = gameId(game, 'SMALL_KEY', '_');
    } else if (ItemHelpers.isKeyRingTCG(item) && settings.smallKeyShuffleChestGame !== 'anywhere') {
      itemId = gameId(game, 'KEY_RING', '_');
    } else if (ItemHelpers.isSmallKeyRegularOot(item) && settings.smallKeyShuffleOot === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'SMALL_KEY', '_');
    } else if (ItemHelpers.isKeyRingRegularOot(item) && settings.smallKeyShuffleOot === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'KEY_RING', '_');
    } else if (ItemHelpers.isSmallKeyRegularMm(item) && settings.smallKeyShuffleMm === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'SMALL_KEY', '_');
    } else if (ItemHelpers.isKeyRingRegularMm(item) && settings.smallKeyShuffleMm === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'KEY_RING', '_');
    } else if (ItemHelpers.isGanonBossKey(item) && settings.ganonBossKey !== 'anywhere') {
      itemId = gameId(game, 'BOSS_KEY', '_');
    } else if (ItemHelpers.isRegularBossKeyOot(item) && settings.bossKeyShuffleOot === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'BOSS_KEY', '_');
    } else if (ItemHelpers.isRegularBossKeyMm(item) && settings.bossKeyShuffleMm === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'BOSS_KEY', '_');
    } else if (ItemHelpers.isTownStrayFairy(item) && settings.townFairyShuffle === 'vanilla') {
      itemId = gameId(game, 'STRAY_FAIRY', '_');
    } else if (ItemHelpers.isDungeonStrayFairy(item) && settings.strayFairyChestShuffle !== 'anywhere' && settings.strayFairyOtherShuffle !== 'anywhere' && settings.erBoss === 'none') {
      itemId = gameId(game, 'STRAY_FAIRY', '_');
    } else if (ItemHelpers.isMap(item) && settings.mapCompassShuffle === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'MAP', '_');
    } else if (ItemHelpers.isCompass(item) && settings.mapCompassShuffle === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'COMPASS', '_');
    }
  }

  /* Resolve shared item */
  if (itemId === 'SHARED_OCARINA' && settings.fairyOcarinaMm && game === 'mm') {
    itemId = 'MM_OCARINA';
  } else {
    const sharedItems = SHARED_ITEMS[game];
    const sharedItem = sharedItems.get(itemId);
    if (sharedItem) {
      itemId = sharedItem;
    }
  }

  /* Resolve shared items - new system */
  for (const group of Object.values(SharedItemGroups)) {
    for (const def of group) {
      if (def.shared.id === itemId) {
        itemId = def[game].id;
        break;
      }
    }
  }

  /* Resolve substitutions */
  if (itemId === 'MM_OCARINA' && settings.fairyOcarinaMm) {
    itemId = 'MM_OCARINA_FAIRY';
  } else if (itemId === 'MM_HOOKSHOT' && settings.shortHookshotMm) {
    itemId = 'MM_HOOKSHOT_SHORT';
  } else {
    const subst = ITEMS_SUBSTITUTIONS[itemId];
    if (subst) {
      itemId = subst;
    }
  }

  if (!GI.hasOwnProperty(itemId)) {
    throw new Error(`Unknown item ${itemId}`);
  }
  let value = GI[itemId].index;

  return value;
};

function entrance(srcName: string, world: World) {
  const dstName = world.entranceOverrides.get(srcName) || srcName;
  const srcGame: Game = (/^OOT_/.test(srcName) ? 'oot' : 'mm');
  const dstGame: Game = (/^OOT_/.test(dstName) ? 'oot' : 'mm');
  const entr = ENTRANCES[dstName as keyof typeof ENTRANCES];
  if (entr === undefined) {
    throw new Error(`Unknown entrance ${dstName}`);
  }
  let data = entr.id;
  if (srcGame !== dstGame) {
    data = (data | 0x80000000) >>> 0;
  }
  return data;
}

function entranceAbs(world: World, name: string) {
  const dstName = world.entranceOverrides.get(name) || name;
  const dstGame: Game = (/^OOT_/.test(dstName) ? 'oot' : 'mm');
  const entr = ENTRANCES[dstName as keyof typeof ENTRANCES];
  if (entr === undefined) {
    throw new Error(`Unknown entrance ${dstName}`);
  }
  let data = entr.id;
  if (dstGame === 'mm') {
    data = (data | 0x80000000) >>> 0;
  }
  return data;
}

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

function zoraSapphireGI(worldId: number, logic: LogicResult): number | null {
  /* Find the dungeon holding the Zora Sapphire */
  const world = logic.worlds[worldId];
  const dungeonId = world.bossIds.indexOf(0x02);
  if (dungeonId === -1)
    return null;

  /* Find the location */
  const locId = DUNGEON_REWARD_LOCATIONS[dungeonId];
  if (!locId)
    return null;
  const loc = makeLocation(locId, worldId);
  const item = logic.items.get(loc);
  if (!item)
    return null;
  return gi(logic.settings, 'oot', item.item, false);
}

function zoraSapphireBuffer(world: number, logic: LogicResult): Uint8Array {
  let value = zoraSapphireGI(world, logic);
  if (value === null)
    value = gi(logic.settings, 'oot', Items.OOT_STONE_SAPPHIRE, false);
  return toU16Buffer([value]);
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

const gameChecks = async (worldId: number, opts: Options, settings: Settings, game: Game, logic: LogicResult): Promise<Uint8Array> => {
  const buffers: Uint8Array[] = [];
  const world = logic.worlds[worldId];
  for (const locId in world.checks) {
    const loc = makeLocation(locId, worldId);
    const c = world.checks[locId];
    const item = logic.items.get(loc)!;

    if (c.game !== game) {
      continue;
    }

    /* Skip cows if not shuffled */
    if (c.game === 'oot' && c.type === 'cow' && !settings.cowShuffleOot) continue;
    if (c.game === 'mm' && c.type === 'cow' && !settings.cowShuffleMm) continue;

    const key = checkKey(c);
    const itemGi = gi(settings, game, item.item, true);
    const b = new Uint8Array(16);
    bufWriteU32BE(b, 0, key);
    bufWriteU16BE(b, 4, playerId(item.player));
    bufWriteU16BE(b, 6, itemGi);
    let cloakGi = 0;
    if (settings.cloakTraps && ItemGroups.TRAPS.has(item.item)) {
      cloakGi = await makeCloakGi(key, opts.seed, settings, logic);
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
};

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

const gameHints = (settings: Settings, game: Game, hints: WorldHints): Uint8Array => {
  const buffers: Uint8Array[] = [];
  for (const gossip in hints.gossip) {
    const h = hints.gossip[gossip];
    if (h.game !== game) {
      continue;
    }
    buffers.push(hintBuffer(settings, game, gossip, h));
  }
  const b = new Uint8Array(0x10);
  b.fill(0xff);
  buffers.push(b);
  return padBuffer16(concatUint8Arrays(buffers));
}

function dungeonWarpsBuffer(world: World) {
  const defaultWarps = [
    'OOT_KOKIRI_FOREST_FROM_DEKU_TREE',
    'OOT_MOUNTAIN_TRAIL_FROM_DODONGO_CAVERN',
    'OOT_ZORA_FOUNTAIN_FROM_JABU_JABU',
    'OOT_SACRED_MEADOW_FROM_TEMPLE_FOREST',
    'OOT_DEATH_CRATER_FROM_TEMPLE_FIRE',
    'OOT_LAKE_HYLIA_FROM_TEMPLE_WATER',
    'OOT_GRAVEYARD_FROM_TEMPLE_SHADOW',
    'OOT_DESERT_COLOSSUS_FROM_TEMPLE_SPIRIT',
    'MM_WOODFALL_FROM_TEMPLE',
    'MM_SNOWHEAD_FROM_TEMPLE',
    'MM_GREAT_BAY_FROM_TEMPLE',
    'MM_STONE_TOWER_INVERTED_FROM_TEMPLE',
  ];

  const entrances = defaultWarps.map((e) => entranceAbs(world, e));
  return toU32Buffer(entrances);
}

function dungeonEntrancesBuffer(world: World) {
  const buffer = new Uint8Array(DUNGEON_ENTRANCES.length * 4);

  for (let i = 0; i < DUNGEON_ENTRANCES.length; ++i) {
    const entranceName = DUNGEON_ENTRANCES[i];
    const entranceData = world.dungeonsEntrances.get(entranceName) || { type: 'region', region: 'NONE' as Region };
    let data: number;

    if (entranceData.type === 'replace') {
      data = DUNGEON_ENTRANCES.indexOf(entranceData.entrance);
      data = (data | 0x80000000) >>> 0;
    } else {
      data = (REGIONS as any)[entranceData.region];
    }

    bufWriteU32BE(buffer, i * 4, data);
  }

  return buffer;
}

const regionsBuffer = (regions: Region[]) => {
  const data = regions.map((region) => {
    const regionId = (REGIONS as any)[regionData(region).id];
    if (regionId === undefined) {
      throw new Error(`Unknown region ${region}`);
    }
    const world = regionData(region).world + 1;
    return [regionId, world];
  });
  return toU8Buffer(data.flat());
};

const gameEntrances = (worldId: number, game: Game, logic: LogicResult) => {
  const data: number[] = [];
  const world = logic.worlds[worldId];
  for (const [src, dst] of world.entranceOverrides) {
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
};

const randomizerDungeonsBits = (worldId: number, logic: LogicResult): Uint8Array => {
  let mq = 0;
  let preCompleted = 0;
  const world = logic.worlds[worldId];
  const settingDataMQ = SETTINGS.find(x => x.key === 'mqDungeons')!;
  for (let i = 0; i < settingDataMQ.values.length; ++i) {
    const dungeon = settingDataMQ.values[i].value;
    if (world.resolvedFlags.mqDungeons.has(dungeon)) {
      mq |= 1 << i;
    }
  }

  for (let i = 0; i < DUNGEONS.length; ++i) {
    const dungeon = DUNGEONS[i];
    if (world.preCompleted.has(dungeon.key)) {
      preCompleted |= 1 << i;
    }
  }

  const buffer = new Uint8Array(8);
  bufWriteU32BE(buffer, 0, mq);
  bufWriteU32BE(buffer, 4, preCompleted);
  return buffer;
}

function randomizerWarps(worldId: number, logic: LogicResult): Uint8Array {
  const songs = [
    'OOT_WARP_SONG_MEADOW',
    'OOT_WARP_SONG_CRATER',
    'OOT_WARP_SONG_LAKE',
    'OOT_WARP_SONG_DESERT',
    'OOT_WARP_SONG_GRAVE',
    'OOT_WARP_SONG_TEMPLE',
  ];
  const warpSongs = toU32Buffer(songs.map(e => entrance(e, logic.worlds[worldId])));

  const owlStatues = [
    'MM_WARP_OWL_GREAT_BAY',
    'MM_WARP_OWL_ZORA_CAPE',
    'MM_WARP_OWL_SNOWHEAD',
    'MM_WARP_OWL_MOUNTAIN_VILLAGE',
    'MM_WARP_OWL_CLOCK_TOWN',
    'MM_WARP_OWL_MILK_ROAD',
    'MM_WARP_OWL_WOODFALL',
    'MM_WARP_OWL_SOUTHERN_SWAMP',
    'MM_WARP_OWL_IKANA_CANYON',
    'MM_WARP_OWL_STONE_TOWER',
  ];
  const owlStatuesBuffer = toU32Buffer(owlStatues.map(e => entrance(e, logic.worlds[worldId])));

  const spawns = [
    'OOT_SPAWN_ADULT',
    'OOT_SPAWN_CHILD'
  ];
  const spawnsBuffer = toU32Buffer(spawns.map(e => entrance(e, logic.worlds[worldId])));

  return concatUint8Arrays([warpSongs, owlStatuesBuffer, spawnsBuffer]);
}

export const randomizerConfig = (world: World, settings: Settings): Uint8Array => {
  const config = worldConfig(world, settings);
  const bits = Array.from(config).map((c) => {
    const bit = CONFVARS_VALUES[c];
    if (bit === undefined) {
      throw new Error(`Unknown config ${c}`);
    }
    return bit;
  });
  const block = new Uint8Array(0x40);
  for (const bit of bits) {
    const byte = Math.floor(bit / 8);
    const mask = 1 << (bit % 8);
    block[byte] |= mask;
  }
  return block;
};

export const randomizerHints = (world: number, logic: LogicResult): Uint8Array => {
  const buffers: Uint8Array[] = [];
  const h = logic.hints[world];
  buffers.push(regionsBuffer(h.dungeonRewards));
  buffers.push(regionsBuffer([h.lightArrow]));
  buffers.push(regionsBuffer(h.oathToOrder));
  buffers.push(regionsBuffer([h.ganonBossKey]));
  return concatUint8Arrays(buffers);
};

const randomizerBoss = (worldId: number, logic: LogicResult): Uint8Array => toU8Buffer(logic.worlds[worldId].bossIds);
const randomizerTriforce = (logic: LogicResult): Uint8Array => toU16Buffer([logic.settings.triforcePieces, logic.settings.triforceGoal]);

function specialConds(settings: Settings) {
  const buffers: Uint8Array[] = [];
  const flagsKeys: keyof typeof SPECIAL_CONDS_FIELDS = Object.keys(SPECIAL_CONDS_FIELDS) as any;
  for (const special in SPECIAL_CONDS) {
    const cond = settings.specialConds[special as keyof typeof SPECIAL_CONDS];
    let flags = 0;
    for (let i = 0; i < flagsKeys.length; ++i) {
      const key = flagsKeys[i];
      if ((cond as any)[key]) {
        flags = (flags | (1 << i)) >>> 0;
      }
    }
    const buffer = new Uint8Array(8);
    bufWriteU32BE(buffer, 0, flags);
    bufWriteU16BE(buffer, 4, cond.count);
    buffers.push(buffer);
  }
  return concatUint8Arrays(buffers);
}

export const prices = (worldId: number, logic: LogicResult): Uint8Array => {
  return toU16Buffer(logic.worlds[worldId].prices);
};

const BOMBCHU_BEHAVIORS = {
  free: 0,
  bombBag: 1,
  bagFirst: 2,
  bagSeparate: 3,
};

function configBombchuBehavior(behavior: keyof typeof BOMBCHU_BEHAVIORS): Uint8Array {
  return new Uint8Array([BOMBCHU_BEHAVIORS[behavior]]);
}

export const randomizerData = (worldId: number, logic: LogicResult): Uint8Array => {
  const buffers = [];
  buffers.push(toU8Buffer([worldId + 1, 0, 0, 0]));
  buffers.push(dungeonWarpsBuffer(logic.worlds[worldId]));
  buffers.push(dungeonEntrancesBuffer(logic.worlds[worldId]));
  buffers.push(randomizerDungeonsBits(worldId, logic));
  buffers.push(randomizerWarps(worldId, logic));
  buffers.push(randomizerConfig(logic.worlds[worldId], logic.settings));
  buffers.push(specialConds(logic.settings));
  buffers.push(toU16Buffer([logic.settings.coinsRed, logic.settings.coinsGreen, logic.settings.coinsBlue, logic.settings.coinsYellow]));
  buffers.push(prices(worldId, logic));
  buffers.push(randomizerTriforce(logic));
  buffers.push(randomizerHints(worldId, logic));
  buffers.push(toI8Buffer(logic.hints[worldId].staticHintsImportances));
  buffers.push(zoraSapphireBuffer(worldId, logic));
  buffers.push(randomizerBoss(worldId, logic));
  buffers.push(toU8Buffer([logic.settings.strayFairyRewardCount]));
  buffers.push(configBombchuBehavior(logic.settings.bombchuBehaviorOot));
  buffers.push(configBombchuBehavior(logic.settings.bombchuBehaviorMm));
  buffers.push(toU8Buffer(logic.worlds[worldId].songEvents));
  return concatUint8Arrays(buffers);
};

function addStartingItemLocsWorld(world: number, logic: LogicResult, locs: string[], items: ItemsCount) {
  const l = makePlayerLocations(logic.settings, locs);
  const i = l.map(x => logic.items.get(x)!);

  for (const item of i) {
    if (item.player === world) {
      countMapAdd(items, item.item);
    }
  }
}

const effectiveStartingItems = (worldId: number, logic: LogicResult): ItemsCount => {
  const { settings } = logic;
  const itemsCount: ItemsCount = new Map;
  for (const [pi, count] of logic.startingItems) {
    if (pi.player === worldId) {
      itemsCount.set(pi.item, count);
    }
  }

  if (settings.tingleShuffle === 'starting') {
    for (const item of ItemGroups.TINGLE_MAPS) {
      itemsCount.set(item, 1);
    }
  }

  if (settings.mapCompassShuffle === 'starting') {
    for (const item of [...ItemGroups.MAPS, ...ItemGroups.COMPASSES]) {
      itemsCount.set(item, 1);
    }
  }

  if (settings.skipZelda) addStartingItemLocsWorld(worldId, logic, locationsZelda(settings), itemsCount);
  if (settings.gerudoFortress === 'open') addStartingItemLocsWorld(worldId, logic, ['OOT Gerudo Member Card'], itemsCount);
  if (settings.mmPreActivatedOwls.type !== 'none') addStartingItemLocsWorld(worldId, logic, getPreActivatedOwlsLocations(logic.worlds[worldId]), itemsCount);

  return itemsCount;
}

const randomizerStartingItems = (world: number, logic: LogicResult): Uint8Array => {
  const { settings } = logic;
  const ids: number[] = [];
  const ids2: number[] = [];
  const items = effectiveStartingItems(world, logic);
  for (const [item, count] of items.entries()) {
    const id = gi(settings, 'oot', item, false);
    if (gi === undefined) {
      throw new Error(`Unknown item ${item.id}`);
    }
    /* Consumables need to be added late */
    if (ItemHelpers.isItemUnlimitedStarting(item)) {
      ids2.push(id);
      ids2.push(count);
    } else {
      ids.push(id);
      ids.push(count);
    }
  }
  return toU16Buffer([...ids, ...ids2, 0xffff, 0xffff]);
};

export async function patchRandomizer(worldId: number, logic: LogicResult, options: Options, settings: Settings, patchfile: Patchfile) {
  patchfile.addNewFile({ vrom: 0xf0200000, data: randomizerData(worldId, logic), compressed: true });
  patchfile.addNewFile({ vrom: 0xf0300000, data: randomizerStartingItems(worldId, logic), compressed: false });
  patchfile.addNewFile({ vrom: 0xf0400000, data: await gameChecks(worldId, options, settings, 'oot', logic), compressed: false });
  patchfile.addNewFile({ vrom: 0xf0500000, data: await gameChecks(worldId, options, settings, 'mm', logic), compressed: false });
  patchfile.addNewFile({ vrom: 0xf0600000, data: gameHints(settings, 'oot', logic.hints[worldId]), compressed: true });
  patchfile.addNewFile({ vrom: 0xf0700000, data: gameHints(settings, 'mm', logic.hints[worldId]), compressed: true });
  patchfile.addNewFile({ vrom: 0xf0800000, data: gameEntrances(worldId, 'oot', logic), compressed: true });
  patchfile.addNewFile({ vrom: 0xf0900000, data: gameEntrances(worldId, 'mm', logic), compressed: true });
}

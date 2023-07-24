import { Buffer } from 'buffer';

import { LogicResult } from '../logic';
import { DATA_GI, DATA_NPC, DATA_SCENES, DATA_REGIONS, DATA_HINTS_POOL, DATA_HINTS, DATA_ENTRANCES } from '../data';
import { Game, GAMES } from "../config";
import { WorldCheck } from '../logic/world';
import { DUNGEONS, Settings, SPECIAL_CONDS, SPECIAL_CONDS_KEYS } from '../settings';
import { HintGossip, WorldHints } from '../logic/hints';
import { CountMap, countMapAdd, gameId, toU16Buffer, toU32Buffer, toU8Buffer } from '../util';
import { Patchfile } from './patchfile';
import { LOCATIONS_ZELDA, makeLocation, makePlayerLocations } from '../logic/locations';
import { CONFVARS_VALUES, Confvar } from '../confvars';
import { Region, regionData } from '../logic/regions';
import { Item, ItemGroups, ItemHelpers, Items, ItemsCount, itemByID } from '../items';
import { SharedItemGroups } from '../logic/shared';

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
  'MM Stone Tower Boss',
];

const HINTS_DATA_OFFSETS = {
  oot: 0x11000,
  mm: 0x12000,
};

const STARTING_ITEMS_DATA_OFFSET = 0x13000;

const ENTRANCE_DATA_OFFSETS = {
  oot: 0x14000,
  mm: 0x15000,
};

const SHARED_ITEMS_OOT = new Map([
  ['SHARED_BOW',              'OOT_BOW'],
  ['SHARED_BOMB_BAG',         'OOT_BOMB_BAG'],
  ['SHARED_ARROWS_5',         'OOT_ARROWS_5'],
  ['SHARED_ARROWS_10',        'OOT_ARROWS_10'],
  ['SHARED_ARROWS_30',        'OOT_ARROWS_30'],
  ['SHARED_ARROWS_40',        'MM_ARROWS_40'], /* OoT lacks 40 pack */
  ['SHARED_BOMB',             'OOT_BOMB'],
  ['SHARED_BOMBS_5',          'OOT_BOMBS_5'],
  ['SHARED_BOMBS_10',         'OOT_BOMBS_10'],
  ['SHARED_BOMBS_20',         'OOT_BOMBS_20'],
  ['SHARED_BOMBS_30',         'OOT_BOMBS_30'],
  ['SHARED_MAGIC_UPGRADE',    'OOT_MAGIC_UPGRADE'],
  ['SHARED_ARROW_FIRE',       'OOT_ARROW_FIRE'],
  ['SHARED_ARROW_ICE',        'OOT_ARROW_ICE'],
  ['SHARED_ARROW_LIGHT',      'OOT_ARROW_LIGHT'],
  ['SHARED_SONG_TIME',        'OOT_SONG_TIME'],
  ['SHARED_SONG_EPONA',       'OOT_SONG_EPONA'],
  ['SHARED_SONG_STORMS',      'OOT_SONG_STORMS'],
  ['SHARED_SONG_SUN',         'OOT_SONG_SUN'],
  ['SHARED_NUT',              'MM_NUT'] /* OoT lacks single nut */,
  ['SHARED_NUTS_5',           'OOT_NUTS_5'],
  ['SHARED_NUTS_10',          'OOT_NUTS_10'],
  ['SHARED_STICK',            'OOT_STICK'],
  ['SHARED_STICKS_5',         'OOT_STICKS_5'],
  ['SHARED_STICKS_10',        'OOT_STICKS_10'],
  ['SHARED_HOOKSHOT',         'OOT_HOOKSHOT'],
  ['SHARED_LENS',             'OOT_LENS'],
  ['SHARED_OCARINA',          'OOT_OCARINA'],
  ['SHARED_MASK_KEATON',      'OOT_MASK_KEATON'],
  ['SHARED_MASK_BUNNY',       'OOT_MASK_BUNNY'],
  ['SHARED_MASK_TRUTH',       'OOT_MASK_TRUTH'],
  ['SHARED_MASK_GORON',       'OOT_MASK_GORON'],
  ['SHARED_MASK_ZORA',        'OOT_MASK_ZORA'],
  ['SHARED_WALLET',           'OOT_WALLET'],
  ['SHARED_RUPEE_GREEN',      'OOT_RUPEE_GREEN'],
  ['SHARED_RUPEE_BLUE',       'OOT_RUPEE_BLUE'],
  ['SHARED_RUPEE_RED',        'OOT_RUPEE_RED'],
  ['SHARED_RUPEE_PURPLE',     'OOT_RUPEE_PURPLE'],
  ['SHARED_RUPEE_SILVER',     'MM_RUPEE_SILVER'], /* OoT lacks silver rupee */
  ['SHARED_RUPEE_GOLD',       'OOT_RUPEE_HUGE'], /* Equivalent */
  ['SHARED_HEART_PIECE',      'OOT_HEART_PIECE'],
  ['SHARED_HEART_CONTAINER',  'OOT_HEART_CONTAINER'],
  ['SHARED_RECOVERY_HEART',   'OOT_RECOVERY_HEART'],
  ['SHARED_DEFENSE_UPGRADE',  'OOT_DEFENSE_UPGRADE'],
]);

const SHARED_ITEMS_MM = new Map([
  ['SHARED_BOW',              'MM_BOW'],
  ['SHARED_BOMB_BAG',         'MM_BOMB_BAG'],
  ['SHARED_ARROWS_5',         'OOT_ARROWS_5'], /* MM lacks 5 pack */
  ['SHARED_ARROWS_10',        'MM_ARROWS_10'],
  ['SHARED_ARROWS_30',        'MM_ARROWS_30'],
  ['SHARED_ARROWS_40',        'MM_ARROWS_40'],
  ['SHARED_BOMB',             'MM_BOMB'],
  ['SHARED_BOMBS_5',          'MM_BOMBS_5'],
  ['SHARED_BOMBS_10',         'MM_BOMBS_10'],
  ['SHARED_BOMBS_20',         'MM_BOMBS_20'],
  ['SHARED_BOMBS_30',         'MM_BOMBS_30'],
  ['SHARED_MAGIC_UPGRADE',    'MM_MAGIC_UPGRADE'],
  ['SHARED_ARROW_FIRE',       'MM_ARROW_FIRE'],
  ['SHARED_ARROW_ICE',        'MM_ARROW_ICE'],
  ['SHARED_ARROW_LIGHT',      'MM_ARROW_LIGHT'],
  ['SHARED_SONG_TIME',        'MM_SONG_TIME'],
  ['SHARED_SONG_EPONA',       'MM_SONG_EPONA'],
  ['SHARED_SONG_STORMS',      'MM_SONG_STORMS'],
  ['SHARED_SONG_SUN',         'MM_SONG_SUN'],
  ['SHARED_NUT',              'MM_NUT'],
  ['SHARED_NUTS_5',           'MM_NUTS_5'],
  ['SHARED_NUTS_10',          'MM_NUTS_10'],
  ['SHARED_STICK',            'MM_STICK'],
  ['SHARED_STICKS_5',         'OOT_STICKS_5'],  /* MM lacks 5 pack */
  ['SHARED_STICKS_10',        'OOT_STICKS_10'], /* MM lacks 10 pack */
  ['SHARED_HOOKSHOT',         'MM_HOOKSHOT'],
  ['SHARED_LENS',             'MM_LENS'],
  ['SHARED_OCARINA',          'OOT_OCARINA'], /* Progressive */
  ['SHARED_MASK_KEATON',      'MM_MASK_KEATON'],
  ['SHARED_MASK_BUNNY',       'MM_MASK_BUNNY'],
  ['SHARED_MASK_TRUTH',       'MM_MASK_TRUTH'],
  ['SHARED_MASK_GORON',       'MM_MASK_GORON'],
  ['SHARED_MASK_ZORA',        'MM_MASK_ZORA'],
  ['SHARED_WALLET',           'MM_WALLET'],
  ['SHARED_RUPEE_GREEN',      'MM_RUPEE_GREEN'],
  ['SHARED_RUPEE_BLUE',       'MM_RUPEE_BLUE'],
  ['SHARED_RUPEE_RED',        'MM_RUPEE_RED'],
  ['SHARED_RUPEE_PURPLE',     'MM_RUPEE_PURPLE'],
  ['SHARED_RUPEE_SILVER',     'MM_RUPEE_SILVER'],
  ['SHARED_RUPEE_GOLD',       'MM_RUPEE_GOLD'],
  ['SHARED_HEART_PIECE',      'MM_HEART_PIECE'],
  ['SHARED_HEART_CONTAINER',  'MM_HEART_CONTAINER'],
  ['SHARED_RECOVERY_HEART',   'MM_RECOVERY_HEART'],
  ['SHARED_DEFENSE_UPGRADE',  'MM_DEFENSE_UPGRADE'],
]);

const SHARED_ITEMS = {
  oot: SHARED_ITEMS_OOT,
  mm: SHARED_ITEMS_MM,
};

const SUBSTITUTIONS: {[k: string]: string} = {
  OOT_SWORD: "OOT_SWORD_KOKIRI",
  OOT_SWORD_GORON: "OOT_SWORD_KNIFE",
  OOT_OCARINA: "OOT_OCARINA_FAIRY",
  OOT_STRENGTH: "OOT_GORON_BRACELET",
  OOT_SCALE: "OOT_SCALE_SILVER",
  OOT_SHIELD: "OOT_PROGRESSIVE_SHIELD_DEKU",
  OOT_ICE_TRAP: "OOT_RUPEE_BLUE",
  MM_SWORD: "MM_SWORD_KOKIRI",
  MM_SHIELD: "MM_PROGRESSIVE_SHIELD_HERO",
  MM_OCARINA: "MM_OCARINA_OF_TIME",
  SHARED_TRIFORCE: "OOT_TRIFORCE",
};

const gi = (settings: Settings, game: Game, item: Item, generic: boolean) => {
  let itemId = item.id;
  if (generic) {
    if (ItemHelpers.isSmallKeyHideout(item) && settings.smallKeyShuffleHideout !== 'anywhere') {
      itemId = gameId(game, 'SMALL_KEY', '_');
    } else if (ItemHelpers.isSmallKeyRegularOot(item) && settings.smallKeyShuffleOot === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'SMALL_KEY', '_');
    } else if (ItemHelpers.isSmallKeyRegularMm(item) && settings.smallKeyShuffleMm === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'SMALL_KEY', '_');
    } else if (ItemHelpers.isGanonBossKey(item) && settings.ganonBossKey !== 'anywhere') {
      itemId = gameId(game, 'BOSS_KEY', '_');
    } else if (ItemHelpers.isRegularBossKeyOot(item) && settings.bossKeyShuffleOot === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'BOSS_KEY', '_');
    } else if (ItemHelpers.isRegularBossKeyMm(item) && settings.bossKeyShuffleMm === 'ownDungeon' && settings.erBoss === 'none') {
      itemId = gameId(game, 'BOSS_KEY', '_');
    } else if (ItemHelpers.isTownStrayFairy(item) && settings.townFairyShuffle === 'vanilla') {
      itemId = gameId(game, 'STRAY_FAIRY', '_');
    } else if (ItemHelpers.isDungeonStrayFairy(item) && settings.strayFairyShuffle !== 'anywhere' && settings.erBoss === 'none') {
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
    const subst = SUBSTITUTIONS[itemId];
    if (subst) {
      itemId = subst;
    }
  }

  if (!DATA_GI.hasOwnProperty(itemId)) {
    throw new Error(`Unknown item ${itemId}`);
  }
  let value = DATA_GI[itemId] as number;

  if ((/^OOT_/.test(itemId) && game === 'mm') || (/^MM_/.test(itemId) && game === 'oot')) {
    value |= 0x200;
  }

  return value;
};

const entrance = (srcGame: Game, dstGame: Game, name: string) => {
  let data = DATA_ENTRANCES[name] as number;
  if (data === undefined) {
    throw new Error(`Unknown entrance ${name}`);
  }
  if (srcGame !== dstGame) {
    data = (data | 0x80000000) >>> 0;
  }
  return data;
}

const checkId = (check: WorldCheck) => {
  if (check.type === 'npc') {
    if (!DATA_NPC.hasOwnProperty(check.id)) {
      throw new Error(`Unknown NPC ${check.id}`);
    }
    return DATA_NPC[check.id];
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

function zoraSapphireBuffer(world: number, logic: LogicResult): Buffer {
  let value = zoraSapphireGI(world, logic);
  if (value === null)
    value = gi(logic.settings, 'oot', Items.OOT_STONE_SAPPHIRE, false);
  return toU16Buffer([value]);
}

const gameChecks = (worldId: number, settings: Settings, game: Game, logic: LogicResult): Buffer => {
  const buf: number[] = [];
  const world = logic.worlds[worldId];
  for (const locId in world.checks) {
    const loc = makeLocation(locId, worldId);
    const c = world.checks[locId];
    const item = logic.items.get(loc)!;

    if (c.game !== game) {
      continue;
    }
    let { scene } = c;
    let id = checkId(c);
    if (!DATA_SCENES.hasOwnProperty(scene)) {
      throw new Error(`Unknown scene ${scene}`);
    }
    let sceneId = DATA_SCENES[scene];
    switch (c.type) {
    case 'npc':
      sceneId = 0xf0;
      break;
    case 'gs':
      sceneId = 0xf1;
      break;
    case 'cow':
      sceneId = 0xf2;
      break;
    case 'shop':
      sceneId = 0xf3;
      break;
    case 'scrub':
      sceneId = 0xf4;
      break;
    case 'sr':
      sceneId = 0xf5;
      break;
    case 'collectible':
      id |= 0x40;
      break;
    case 'sf':
      id |= 0x80;
      break;
    }
    const key = (sceneId << 8) | id;
    const itemGi = gi(settings, game, item.item, true);
    buf.push(item.player + 1, 0, key, itemGi);
  }
  return toU16Buffer(buf);
};

const hintBuffer = (settings: Settings, game: Game, gossip: string, hint: HintGossip): Buffer => {
  const data = Buffer.alloc(10, 0xff);
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
  case 'hero':
    {
      const regionD = regionData(hint.region);
      const region = DATA_REGIONS[regionD.id];
      if (region === undefined) {
        throw new Error(`Unknown region ${hint.region}`);
      }
      data.writeUInt8(id, 0);
      data.writeUInt8(0x00, 1);
      data.writeUInt8(region, 2);
      data.writeUInt8(regionD.world + 1, 3);
    }
    break;
  case 'foolish':
    {
      const regionD = regionData(hint.region);
      const region = DATA_REGIONS[regionD.id];
      if (region === undefined) {
        throw new Error(`Unknown region ${hint.region}`);
      }
      data.writeUInt8(id, 0);
      data.writeUInt8(0x01, 1);
      data.writeUInt8(region, 2);
      data.writeUInt8(regionD.world + 1, 3);
    }
    break;
  case 'item-exact':
    {
      const check = DATA_HINTS[hint.check];
      if (check === undefined) {
        throw new Error(`Unknown named check: ${hint.check}`);
      }
      const items = hint.items;
      const itemsGI = hint.items.map((item) => gi(settings, 'oot', item.item, true));
      data.writeUInt8(id, 0);
      data.writeUInt8(0x02, 1);
      data.writeUInt8(check, 2);
      data.writeUInt8(hint.world + 1, 3);
      data.writeUInt16BE(itemsGI[0], 4);
      data.writeUint8(items[0].player + 1, 8);
      if (items.length > 1) {
        data.writeUInt16BE(itemsGI[1], 6);
        data.writeUint8(items[1].player + 1, 9);
      }
    }
    break;
  case 'item-region':
      {
        const regionD = regionData(hint.region);
        const region = DATA_REGIONS[regionD.id];
        const item = hint.item;
        if (region === undefined) {
          throw new Error(`Unknown region ${hint.region}`);
        }
        const itemGI = gi(settings, 'oot', item.item, true);
        data.writeUInt8(id, 0);
        data.writeUInt8(0x03, 1);
        data.writeUInt8(region, 2);
        data.writeUInt8(regionD.world + 1, 3);
        data.writeUInt16BE(itemGI, 4);
        data.writeUint8(item.player + 1, 8);
      }
      break;
  case 'junk':
    {
      data.writeUInt8(id, 0);
      data.writeUInt8(0x04, 1);
      data.writeUInt16BE(hint.id, 4);
    }
    break;
  }
  return data;
}

const gameHints = (settings: Settings, game: Game, hints: WorldHints): Buffer => {
  const buffers: Buffer[] = [];
  for (const gossip in hints.gossip) {
    const h = hints.gossip[gossip];
    if (h.game !== game) {
      continue;
    }
    buffers.push(hintBuffer(settings, game, gossip, h));
  }
  return Buffer.concat(buffers);
}

const regionsBuffer = (regions: Region[]) => {
  const data = regions.map((region) => {
    const regionId = DATA_REGIONS[regionData(region).id];
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
    const srcEntrance = world.entrances.get(src)!;
    const dstEntrance = world.entrances.get(dst)!;
    if (srcEntrance.game !== game)
      continue;
    const srcId = entrance(srcEntrance.game, srcEntrance.game, src);
    const dstId = entrance(srcEntrance.game, dstEntrance.game, dst);
    data.push(srcId, dstId);
  }
  return toU32Buffer(data);
};

export const randomizerMq = (worldId: number, logic: LogicResult): Buffer => {
  let mq = 0;
  const dungeons = Object.keys(DUNGEONS);
  const world = logic.worlds[worldId];
  for (let i = 0; i < dungeons.length; ++i) {
    const dungeon = dungeons[i];
    if (world.mq.has(dungeon)) {
      mq |= 1 << i;
    }
  }
  const buffer = Buffer.alloc(4);
  buffer.writeUInt32BE(mq);
  return buffer;
}

export const randomizerConfig = (config: Set<Confvar>): Buffer => {
  const bits = Array.from(config).map((c) => {
    const bit = CONFVARS_VALUES[c];
    if (bit === undefined) {
      throw new Error(`Unknown config ${c}`);
    }
    return bit;
  });
  const block = Buffer.alloc(0x40, 0);
  for (const bit of bits) {
    const byte = Math.floor(bit / 8);
    const mask = 1 << (bit % 8);
    block[byte] |= mask;
  }
  return block;
};

export const randomizerHints = (world: number, logic: LogicResult): Buffer => {
  const buffers: Buffer[] = [];
  const h = logic.hints[world];
  buffers.push(regionsBuffer(h.dungeonRewards));
  buffers.push(regionsBuffer([h.lightArrow]));
  buffers.push(regionsBuffer([h.oathToOrder]));
  buffers.push(regionsBuffer([h.ganonBossKey]));
  return Buffer.concat(buffers);
};

const randomizerBoss = (worldId: number, logic: LogicResult): Buffer => toU8Buffer(logic.worlds[worldId].bossIds);
const randomizerDungeons = (worldId: number, logic: LogicResult): Buffer => toU8Buffer(logic.worlds[worldId].dungeonIds);
const randomizerTriforce = (logic: LogicResult): Buffer => toU8Buffer([logic.settings.triforcePieces, logic.settings.triforceGoal]);

function specialConds(settings: Settings) {
  const buffers: Buffer[] = [];
  const flagsKeys: keyof typeof SPECIAL_CONDS_KEYS = Object.keys(SPECIAL_CONDS_KEYS) as any;
  for (const special in SPECIAL_CONDS) {
    const cond = settings.specialConds[special as keyof typeof SPECIAL_CONDS];
    let flags = 0;
    for (let i = 0; i < flagsKeys.length; ++i) {
      const key = flagsKeys[i];
      if ((cond as any)[key]) {
        flags |= 1 << i;
      }
    }
    const buffer = Buffer.alloc(4);
    buffer.writeUInt16BE(flags, 0);
    buffer.writeUInt16BE(cond.count, 2);
    buffers.push(buffer);
  }
  return Buffer.concat(buffers);
}

export const prices = (worldId: number, logic: LogicResult): Buffer => {
  return toU16Buffer(logic.worlds[worldId].prices);
};

export const randomizerData = (worldId: number, logic: LogicResult): Buffer => {
  const buffers = [];
  buffers.push(logic.uuid);
  buffers.push(toU8Buffer([worldId + 1, 0, 0, 0]));
  buffers.push(randomizerMq(worldId, logic));
  buffers.push(randomizerConfig(logic.config));
  buffers.push(specialConds(logic.settings));
  buffers.push(prices(worldId, logic));
  buffers.push(randomizerHints(worldId, logic));
  buffers.push(zoraSapphireBuffer(worldId, logic));
  buffers.push(randomizerBoss(worldId, logic));
  buffers.push(randomizerDungeons(worldId, logic));
  buffers.push(randomizerTriforce(logic));
  return Buffer.concat(buffers);
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

const effectiveStartingItems = (world: number, logic: LogicResult): ItemsCount => {
  const { settings } = logic;
  const startingItems: ItemsCount = new Map;
  for (const [itemId, count] of Object.entries(logic.settings.startingItems)) {
    const item = itemByID(itemId);
    startingItems.set(item, count);
  }

  if (settings.tingleShuffle === 'starting') {
    for (const item of ItemGroups.TINGLE_MAPS) {
      startingItems.set(item, 1);
    }
  }

  if (settings.mapCompassShuffle === 'starting') {
    for (const item of [...ItemGroups.MAPS, ...ItemGroups.COMPASSES]) {
      startingItems.set(item, 1);
    }
  }

  if (settings.skipZelda) addStartingItemLocsWorld(world, logic, LOCATIONS_ZELDA, startingItems);
  if (settings.gerudoFortress === 'open') addStartingItemLocsWorld(world, logic, ['OOT Gerudo Member Card'], startingItems);

  return startingItems;
}

const randomizerStartingItems = (world: number, logic: LogicResult): Buffer => {
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

export function patchRandomizer(worldId: number, logic: LogicResult, settings: Settings, patchfile: Patchfile) {
  patchfile.addNewFile(0xf0200000, randomizerData(worldId, logic), true);
  patchfile.addNewFile(0xf0300000, randomizerStartingItems(worldId, logic), false);
  patchfile.addNewFile(0xf0400000, gameChecks(worldId, settings, 'oot', logic), true);
  patchfile.addNewFile(0xf0500000, gameChecks(worldId, settings, 'mm', logic), true);
  patchfile.addNewFile(0xf0600000, gameHints(settings, 'oot', logic.hints[worldId]), true);
  patchfile.addNewFile(0xf0700000, gameHints(settings, 'mm', logic.hints[worldId]), true);
  patchfile.addNewFile(0xf0800000, gameEntrances(worldId, 'oot', logic), true);
  patchfile.addNewFile(0xf0900000, gameEntrances(worldId, 'mm', logic), true);
}

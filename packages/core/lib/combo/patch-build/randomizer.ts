import { Buffer } from 'buffer';

import { LogicResult } from '../logic';
import { DATA_GI, DATA_NPC, DATA_SCENES, DATA_REGIONS, DATA_CONFIG, DATA_HINTS_POOL, DATA_HINTS, DATA_ENTRANCES } from '../data';
import { Game, GAMES } from "../config";
import { WorldCheck } from '../logic/world';
import { DUNGEONS, Settings, SPECIAL_CONDS, SPECIAL_CONDS_KEYS } from '../settings';
import { HintGossip, Hints } from '../logic/hints';
import { isDungeonStrayFairy, isGanonBossKey, isMap, isCompass, isRegularBossKey, isSmallKeyRegular, isTownStrayFairy, isSmallKeyHideout, isItemUnlimitedStarting, ITEMS_MAPS, ITEMS_COMPASSES, addItem, ITEMS_TINGLE_MAPS } from '../logic/items';
import { gameId } from '../util';
import { EntranceShuffleResult } from '../logic/entrance';
import { Patchfile } from './patchfile';
import { LOCATIONS_ZELDA } from '../logic/locations';

const GAME_DATA_OFFSETS = {
  oot: 0x1000,
  mm: 0x5000,
};

const HINTS_DATA_OFFSETS = {
  oot: 0x9000,
  mm: 0xa000,
};

const STARTING_ITEMS_DATA_OFFSET = 0xb000;

const ENTRANCE_DATA_OFFSETS = {
  oot: 0xc000,
  mm: 0xd000,
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
};

const gi = (settings: Settings, game: Game, item: string, generic: boolean) => {
  if (generic) {
    if (isSmallKeyHideout(item) && settings.smallKeyShuffleHideout !== 'anywhere') {
      item = gameId(game, 'SMALL_KEY', '_');
    } else if (isSmallKeyRegular(item) && settings.smallKeyShuffle === 'ownDungeon' && settings.erBoss === 'none') {
      item = gameId(game, 'SMALL_KEY', '_');
    } else if (isGanonBossKey(item) && settings.ganonBossKey !== 'anywhere') {
      item = gameId(game, 'BOSS_KEY', '_');
    } else if (isRegularBossKey(item) && settings.bossKeyShuffle === 'ownDungeon' && settings.erBoss === 'none') {
      item = gameId(game, 'BOSS_KEY', '_');
    } else if (isTownStrayFairy(item) && settings.townFairyShuffle === 'vanilla') {
      item = gameId(game, 'STRAY_FAIRY', '_');
    } else if (isDungeonStrayFairy(item) && settings.strayFairyShuffle !== 'anywhere' && settings.erBoss === 'none') {
      item = gameId(game, 'STRAY_FAIRY', '_');
    } else if (isMap(item) && settings.mapCompassShuffle === 'ownDungeon' && settings.erBoss === 'none') {
      item = gameId(game, 'MAP', '_');
    } else if (isCompass(item) && settings.mapCompassShuffle === 'ownDungeon' && settings.erBoss === 'none') {
      item = gameId(game, 'COMPASS', '_');
    }
  }

  /* Resolve shared item */
  if (item === 'SHARED_OCARINA' && settings.fairyOcarinaMm && game === 'mm') {
    item = 'MM_OCARINA';
  } else {
    const sharedItems = SHARED_ITEMS[game];
    const sharedItem = sharedItems.get(item);
    if (sharedItem) {
      item = sharedItem;
    }
  }

  /* Resolve substitutions */
  if (item === 'MM_OCARINA' && settings.fairyOcarinaMm) {
    item = 'MM_OCARINA_FAIRY';
  } else if (item === 'MM_HOOKSHOT' && settings.shortHookshotMm) {
    item = 'MM_HOOKSHOT_SHORT';
  } else {
    const subst = SUBSTITUTIONS[item];
    if (subst) {
      item = subst;
    }
  }

  if (!DATA_GI.hasOwnProperty(item)) {
    throw new Error(`Unknown item ${item}`);
  }
  let value = DATA_GI[item];

  if ((/^OOT_/.test(item) && game === 'mm') || (/^MM_/.test(item) && game === 'oot')) {
    value |= 0x200;
  }

  return value;
};

const entrance = (game: Game, from: string, to: string) => {
  let entrGame: Game;

  if (from.startsWith('MM ')) {
    from = from.substring(3);
    to = to.substring(3);
    entrGame = 'mm';
  } else {
    from = from.substring(4);
    to = to.substring(4);
    entrGame = 'oot';
  }
  const entrances = DATA_ENTRANCES[entrGame];
  const e = entrances.find((e: any) => e.from === from && e.to === to);
  if (!e) {
    throw new Error(`Unknown ${entrGame} entrance ${from} -> ${to}`);
  }
  let id = Number(e.id);
  if (game !== entrGame) {
    id = (id | 0x80000000) >>> 0;
  }
  return id;
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

const toU8Buffer = (data: number[]) => {
  const buf = Buffer.alloc(data.length);
  for (let i = 0; i < data.length; ++i) {
    buf.writeUInt8(data[i], i);
  }
  return buf;
};

const toU16Buffer = (data: number[]) => {
  const buf = Buffer.alloc(data.length * 2);
  for (let i = 0; i < data.length; ++i) {
    buf.writeUInt16BE(data[i], i * 2);
  }
  return buf;
};

const toU32Buffer = (data: number[]) => {
  const buf = Buffer.alloc(data.length * 4);
  for (let i = 0; i < data.length; ++i) {
    buf.writeUInt32BE(data[i], i * 4);
  }
  return buf;
};

const gameChecks = (settings: Settings, game: Game, logic: LogicResult): Buffer => {
  const buf: number[] = [];
  for (const loc in logic.world.checks) {
    const c = logic.world.checks[loc];
    const item = logic.items[loc];

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
    case 'collectible':
      id |= 0x40;
      break;
    case 'sf':
      id |= 0x80;
      break;
    }
    const key = (sceneId << 8) | id;
    const itemGi = gi(settings, game, item, true);
    buf.push(key, itemGi);
  }
  return toU16Buffer(buf);
};

const hintBuffer = (settings: Settings, game: Game, gossip: string, hint: HintGossip): Buffer => {
  const data = Buffer.alloc(8, 0xff);
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
      const region = DATA_REGIONS[hint.region];
      if (region === undefined) {
        throw new Error(`Unknown region ${hint.region}`);
      }
      data.writeUInt8(id, 0);
      data.writeUInt8(0x00, 1);
      data.writeUInt8(region, 2);
    }
    break;
  case 'foolish':
    {
      const region = DATA_REGIONS[hint.region];
      if (region === undefined) {
        throw new Error(`Unknown region ${hint.region}`);
      }
      data.writeUInt8(id, 0);
      data.writeUInt8(0x01, 1);
      data.writeUInt8(region, 2);
    }
    break;
  case 'item-exact':
    {
      const check = DATA_HINTS[hint.check];
      if (check === undefined) {
        throw new Error(`Unknown named check: ${hint.check}`);
      }
      const items = hint.items.map((item) => gi(settings, 'oot', item, true));
      data.writeUInt8(id, 0);
      data.writeUInt8(0x02, 1);
      data.writeUInt8(check, 2);
      data.writeUInt16BE(items[0], 4);
      if (items.length > 1) {
        data.writeUInt16BE(items[1], 6);
      }
    }
    break;
  case 'item-region':
      {
        const region = DATA_REGIONS[hint.region];
        if (region === undefined) {
          throw new Error(`Unknown region ${hint.region}`);
        }
        const item = gi(settings, 'oot', hint.item, true);
        data.writeUInt8(id, 0);
        data.writeUInt8(0x03, 1);
        data.writeUInt8(region, 2);
        data.writeUInt16BE(item, 4);
      }
      break;
  }
  return data;
}

const gameHints = (settings: Settings, game: Game, hints: Hints): Buffer => {
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

const regionsBuffer = (regions: string[]) => {
  const data = regions.map((region) => {
    const regionId = DATA_REGIONS[region];
    if (regionId === undefined) {
      throw new Error(`Unknown region ${region}`);
    }
    return regionId;
  });
  return toU8Buffer(data);
};

const gameEntrances = (game: Game, entrances: EntranceShuffleResult) => {
  const data: number[] = [];
  const gamePrefix = game === 'oot' ? 'OOT ' : 'MM ';
  for (const srcFrom in entrances.overrides) {
    if (!srcFrom.startsWith(gamePrefix)) {
      continue;
    }
    const src = entrances.overrides[srcFrom];
    for (const srcTo in src) {
      const dst = src[srcTo];
      const srcId = entrance(game, srcFrom, srcTo);
      const dstId = entrance(game, dst.from, dst.to);
      data.push(srcId, dstId);
    }
  }
  return toU32Buffer(data);
};

export const randomizerMq = (logic: LogicResult): Buffer => {
  let mq = 0;
  const dungeons = Object.keys(DUNGEONS);
  for (let i = 0; i < dungeons.length; ++i) {
    const dungeon = dungeons[i];
    if (logic.mq.has(dungeon)) {
      mq |= 1 << i;
    }
  }
  const buffer = Buffer.alloc(4);
  buffer.writeUInt32BE(mq);
  return buffer;
}

export const randomizerConfig = (config: Set<string>): Buffer => {
  const bits = Array.from(config).map((c) => {
    const bit = DATA_CONFIG[c];
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

export const randomizerHints = (logic: LogicResult): Buffer => {
  const buffers: Buffer[] = [];
  buffers.push(regionsBuffer(logic.hints.dungeonRewards));
  buffers.push(regionsBuffer([logic.hints.lightArrow]));
  buffers.push(regionsBuffer([logic.hints.oathToOrder]));
  buffers.push(regionsBuffer([logic.hints.ganonBossKey]));
  return Buffer.concat(buffers);
};

const randomizerBoss = (logic: LogicResult): Buffer => toU8Buffer(logic.entrances.boss);
const randomizerDungeons = (logic: LogicResult): Buffer => toU8Buffer(logic.entrances.dungeons);

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

export const randomizerData = (logic: LogicResult): Buffer => {
  const buffers = [];
  buffers.push(randomizerMq(logic));
  buffers.push(randomizerConfig(logic.config));
  buffers.push(specialConds(logic.settings));
  buffers.push(randomizerHints(logic));
  buffers.push(randomizerBoss(logic));
  buffers.push(randomizerDungeons(logic));
  return Buffer.concat(buffers);
};

const effectiveStartingItems = (logic: LogicResult): {[k: string]: number} => {
  const { settings, items } = logic;
  const startingItems = {...settings.startingItems};

  if (settings.tingleShuffle === 'starting') {
    for (const item of ITEMS_TINGLE_MAPS) {
      startingItems[item] = 1;
    }
  }

  if (settings.mapCompassShuffle === 'starting') {
    for (const item of [...ITEMS_MAPS, ...ITEMS_COMPASSES]) {
      startingItems[item] = 1;
    }
  }

  if (settings.skipZelda) {
    for (const loc of LOCATIONS_ZELDA) {
      addItem(startingItems, items[loc]);
    }
  }

  return startingItems;
}

const randomizerStartingItems = (logic: LogicResult): Buffer => {
  const { settings } = logic;
  const buffer = Buffer.alloc(0x1000, 0xff);
  const ids: number[] = [];
  const ids2: number[] = [];
  const items = effectiveStartingItems(logic);
  for (const item in items) {
    const count = items[item];
    const id = gi(settings, 'oot', item, false);
    if (gi === undefined) {
      throw new Error(`Unknown item ${item}`);
    }
    /* Consumables need to be added late */
    if (isItemUnlimitedStarting(item)) {
      ids2.push(id);
      ids2.push(count);
    } else {
      ids.push(id);
      ids.push(count);
    }
  }
  const data = toU16Buffer([...ids, ...ids2]);
  data.copy(buffer, 0);
  return buffer;
};

export function patchRandomizer(logic: LogicResult, settings: Settings, patchfile: Patchfile) {
  const buffer = Buffer.alloc(0x20000, 0xff);
  for (const g of GAMES) {
    const checksBuffer = gameChecks(settings, g, logic);
    const hintsBuffer = gameHints(settings, g, logic.hints);
    const entrancesBuffer = gameEntrances(g, logic.entrances);
    checksBuffer.copy(buffer, GAME_DATA_OFFSETS[g]);
    hintsBuffer.copy(buffer, HINTS_DATA_OFFSETS[g]);
    entrancesBuffer.copy(buffer, ENTRANCE_DATA_OFFSETS[g]);
  }
  const data = randomizerData(logic);
  data.copy(buffer, 0);
  const startingItems = randomizerStartingItems(logic);
  startingItems.copy(buffer, STARTING_ITEMS_DATA_OFFSET);
  patchfile.addPatch('global', 0x03fe0000, buffer);
}

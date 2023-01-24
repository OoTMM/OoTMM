import { Buffer } from 'buffer';

import { logic, LogicResult } from './logic';
import { DATA_GI, DATA_NPC, DATA_SCENES, DATA_REGIONS, DATA_CONFIG, DATA_HINTS_POOL, DATA_HINTS } from './data';
import { Game, GAMES } from "./config";
import { WorldCheck } from './logic/world';
import { Options } from './options';
import { Settings } from './settings';
import { HintGossip, Hints } from './logic/hints';
import { Monitor } from './monitor';
import { isDungeonStrayFairy, isGanonBossKey, isMap, isCompass, isRegularBossKey, isSmallKey, isTownStrayFairy } from './logic/items';
import { gameId } from './util';

const GAME_DATA_OFFSETS = {
  oot: 0x1000,
  mm: 0x3000,
};

const HINTS_DATA_OFFSETS = {
  oot: 0x5000,
  mm: 0x6000,
};

const STARTING_ITEMS_DATA_OFFSET = 0x7000;

const SUBSTITUTIONS: {[k: string]: string} = {
  OOT_SWORD: "OOT_SWORD_KOKIRI",
  OOT_SWORD_GORON: "OOT_SWORD_KNIFE",
  OOT_OCARINA: "OOT_OCARINA_FAIRY",
  OOT_STRENGTH: "OOT_GORON_BRACELET",
  OOT_SCALE: "OOT_SCALE_SILVER",
  OOT_SHIELD: "OOT_PROGRESSIVE_SHIELD_DEKU",
  OOT_ICE_TRAP: "OOT_RUPEE_BLUE",
  OOT_WALLET: "OOT_WALLET2",
  MM_SWORD: "MM_SWORD_KOKIRI",
  MM_SHIELD: "MM_PROGRESSIVE_SHIELD_HERO",
  MM_OCARINA: "MM_OCARINA_OF_TIME",
  MM_WALLET: "MM_WALLET2",
};

const gi = (settings: Settings, game: Game, item: string, generic: boolean) => {
  if (generic) {
    if (isSmallKey(item) && settings.smallKeyShuffle === 'ownDungeon') {
      item = gameId(game, 'SMALL_KEY', '_');
    } else if (isGanonBossKey(item) && settings.ganonBossKey !== 'anywhere') {
      item = gameId(game, 'BOSS_KEY', '_');
    } else if (isRegularBossKey(item) && settings.bossKeyShuffle === 'ownDungeon') {
      item = gameId(game, 'BOSS_KEY', '_');
    } else if (isTownStrayFairy(item) && settings.townFairyShuffle === 'vanilla') {
      item = gameId(game, 'STRAY_FAIRY', '_');
    } else if (isDungeonStrayFairy(item) && settings.strayFairyShuffle !== 'anywhere') {
      item = gameId(game, 'STRAY_FAIRY', '_');
    } else if (isMap(item) && settings.mapCompassShuffle === 'ownDungeon') {
      item = gameId(game, 'MAP', '_');
    } else if (isCompass(item) && settings.mapCompassShuffle === 'ownDungeon') {
      item = gameId(game, 'COMPASS', '_');
    }
  }

  const subst = SUBSTITUTIONS[item];
  if (subst) {
    item = subst;
  }

  if (!DATA_GI.hasOwnProperty(item)) {
    throw new Error(`Unknown item ${item}`);
  }
  let value = DATA_GI[item];

  if ((/^OOT_/.test(item) && game === 'mm') || (/^MM_/.test(item) && game === 'oot')) {
    value |= 0x100;
  }

  return value;
};

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

const gameChecks = (settings: Settings, game: Game, logic: LogicResult): Buffer => {
  const buf: number[] = [];
  for (const c of logic.items) {
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
    case 'collectible':
      id |= 0x40;
      break;
    case 'sf':
      id |= 0x80;
      break;
    }
    const key = (sceneId << 8) | id;
    const item = gi(settings, game, c.item, true);
    buf.push(key, item);
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
  return Buffer.concat(buffers);
};

export const randomizerData = (logic: LogicResult, options: Options): Buffer => {
  const buffers = [];
  buffers.push(randomizerConfig(logic.config));
  buffers.push(randomizerHints(logic));
  return Buffer.concat(buffers);
};

const effectiveStartingItems = (settings: Settings): {[k: string]: number} => {
  const MAPS_AND_COMPASSES = [
    'OOT_MAP_DT',
    'OOT_MAP_DC',
    'OOT_MAP_JJ',
    'OOT_MAP_FOREST',
    'OOT_MAP_FIRE',
    'OOT_MAP_WATER',
    'OOT_MAP_SPIRIT',
    'OOT_MAP_SHADOW',
    'OOT_MAP_BOTW',
    'OOT_MAP_IC',
    'OOT_COMPASS_DT',
    'OOT_COMPASS_DC',
    'OOT_COMPASS_JJ',
    'OOT_COMPASS_FOREST',
    'OOT_COMPASS_FIRE',
    'OOT_COMPASS_WATER',
    'OOT_COMPASS_SPIRIT',
    'OOT_COMPASS_SHADOW',
    'OOT_COMPASS_BOTW',
    'OOT_COMPASS_IC',
    'MM_MAP_WF',
    'MM_MAP_SH',
    'MM_MAP_GB',
    'MM_MAP_ST',
    'MM_COMPASS_WF',
    'MM_COMPASS_SH',
    'MM_COMPASS_GB',
    'MM_COMPASS_ST',
  ];

  const items = {...settings.startingItems};
  if (settings.mapCompassShuffle === 'starting') {
    for (const item of MAPS_AND_COMPASSES) {
      items[item] = 1;
    }
  }

  if (settings.progressiveShieldsMm === 'start') {
    items['MM_SHIELD_HERO'] = 1;
  }

  return items;
}

const randomizerStartingItems = (settings: Settings): Buffer => {
  const buffer = Buffer.alloc(0x1000, 0xff);
  const ids: number[] = [];
  const items = effectiveStartingItems(settings);
  for (const item in items) {
    const count = items[item];
    const id = gi(settings, 'oot', item, false);
    if (gi === undefined) {
      throw new Error(`Unknown item ${item}`);
    }
    ids.push(id);
    ids.push(count);
  }
  const data = toU16Buffer(ids);
  data.copy(buffer, 0);
  return buffer;
};

export const randomize = (monitor: Monitor, rom: Buffer, opts: Options) => {
  monitor.log("Randomizing...");
  const res = logic(monitor, opts);
  const buffer = Buffer.alloc(0x20000, 0xff);
  for (const g of GAMES) {
    const checksBuffer = gameChecks(opts.settings, g, res);
    const hintsBuffer = gameHints(opts.settings, g, res.hints);
    checksBuffer.copy(buffer, GAME_DATA_OFFSETS[g]);
    hintsBuffer.copy(buffer, HINTS_DATA_OFFSETS[g]);
  }
  const data = randomizerData(res, opts);
  data.copy(buffer, 0);
  const startingItems = randomizerStartingItems(opts.settings);
  startingItems.copy(buffer, STARTING_ITEMS_DATA_OFFSET);
  buffer.copy(rom, 0x03fe0000);
  return { log: res.log , hash: res.hash };
}

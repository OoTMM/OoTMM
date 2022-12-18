import { Buffer } from 'buffer';

import { logic, LogicResult } from './logic';
import { DATA_GI, DATA_NPC, DATA_SCENES, DATA_REGIONS, DATA_CONFIG, DATA_HINTS } from './data';
import { Game, GAMES } from "./config";
import { WorldCheck } from './logic/world';
import { Options } from './options';
import { Settings } from './settings';
import { HintGossip, Hints } from './logic/hints';
import { Monitor } from './monitor';

const GAME_DATA_OFFSETS = {
  oot: 0x1000,
  mm: 0x3000,
};

const HINTS_DATA_OFFSETS = {
  oot: 0x5000,
  mm: 0x6000,
};

const SUBSTITUTIONS: {[k: string]: string} = {
  OOT_SWORD: "OOT_SWORD_KOKIRI",
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

const gi = (game: Game, item: string) => {
  /* Dungeon Items */
  /* TODO: Refactor this horror */
  if (/^OOT_MAP/.test(item)) {
    item = "OOT_MAP";
  } else if (/^OOT_COMPASS/.test(item)) {
    item = "OOT_COMPASS";
  } else if (/^OOT_SMALL_KEY/.test(item)) {
    item = "OOT_SMALL_KEY";
  } else if (/^OOT_BOSS_KEY/.test(item)) {
    item = "OOT_BOSS_KEY";
  } else if (/^MM_MAP/.test(item)) {
    item = "MM_MAP";
  } else if (/^MM_COMPASS/.test(item)) {
    item = "MM_COMPASS";
  } else if (/^MM_SMALL_KEY/.test(item)) {
    item = "MM_SMALL_KEY";
  } else if (/^MM_BOSS_KEY/.test(item)) {
    item = "MM_BOSS_KEY";
  } else if (/^MM_STRAY_FAIRY/.test(item)) {
    item = "MM_STRAY_FAIRY";
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
    if (game === 'oot' && c.type === 'gs' && settings.goldSkulltulaTokens === 'none') {
      continue;
    }
    if (c.type === 'sf') {
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
    }
    const key = (sceneId << 8) | id;
    const item = gi(game, c.item);
    buf.push(key, item);
  }
  return toU16Buffer(buf);
};

const hintBuffer = (game: Game, gossip: string, hint: HintGossip): Buffer => {
  const data = Buffer.alloc(8, 0xff);
  let gossipData = DATA_HINTS[game][gossip];
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
  }
  return data;
}

const gameHints = (game: Game, hints: Hints): Buffer => {
  const buffers: Buffer[] = [];
  for (const gossip in hints.gossip) {
    const h = hints.gossip[gossip];
    if (h.game !== game) {
      continue;
    }
    buffers.push(hintBuffer(game, gossip, h));
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

export const randomize = (monitor: Monitor, rom: Buffer, opts: Options) => {
  monitor.log("Randomizing...");
  const res = logic(opts);
  const buffer = Buffer.alloc(0x20000, 0xff);
  for (const g of GAMES) {
    const checksBuffer = gameChecks(opts.settings, g, res);
    const hintsBuffer = gameHints(g, res.hints);
    checksBuffer.copy(buffer, GAME_DATA_OFFSETS[g]);
    hintsBuffer.copy(buffer, HINTS_DATA_OFFSETS[g]);
  }
  const data = randomizerData(res, opts);
  data.copy(buffer, 0);
  buffer.copy(rom, 0x03fe0000);
  return res.log;
}

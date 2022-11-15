import { logic, LogicResult } from './logic';
import { DATA_GI, DATA_NPC, DATA_SCENES } from './data';
import { Game, GAMES } from "./config";
import { WorldCheck } from './logic/world';
import { Options } from './options';

const OFFSETS = {
  oot: 0x1000,
  mm: 0x3000,
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
  MM_SHIELD: "MM_SHIELD_HYLIAN",
  MM_OCARINA: "MM_OCARINA_OF_TIME",
  MM_WALLET: "MM_WALLET2",
};

const gi = async (game: Game, item: string) => {
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

  const data = await DATA_GI;
  if (!data.hasOwnProperty(item)) {
    throw new Error(`Unknown item ${item}`);
  }
  let value = data[item];

  if ((/^OOT_/.test(item) && game === 'mm') || (/^MM_/.test(item) && game === 'oot')) {
    value |= 0x100;
  }

  return value;
};

const checkId = async (check: WorldCheck) => {
  if (check.type === 'npc') {
    const data = await DATA_NPC;
    if (!data.hasOwnProperty(check.id)) {
      throw new Error(`Unknown NPC ${check.id}`);
    }
    return data[check.id];
  }
  return check.id;
}

const toU16Buffer = (data: number[]) => {
  const buf = Buffer.alloc(data.length * 2);
  for (let i = 0; i < data.length; ++i) {
    buf.writeUInt16BE(data[i], i * 2);
  }
  return buf;
};

export const randomizeGame = async (game: Game, logic: LogicResult): Promise<Buffer> => {
  const scenes = await DATA_SCENES;
  const buf: number[] = [];
  for (const c of logic.items) {
    if (c.game !== game) {
      continue;
    }
    if (c.type === 'gs' || c.type === 'sf') {
      continue;
    }
    let { scene } = c;
    let id = await checkId(c);
    if (!scenes.hasOwnProperty(scene)) {
      throw new Error(`Unknown scene ${scene}`);
    }
    let sceneId = scenes[scene];
    switch (c.type) {
    case 'npc':
      sceneId = 0xf0;
      break;
    case 'collectible':
      id |= 0x40;
      break;
    }
    const key = (sceneId << 8) | id;
    const item = await gi(game, c.item);
    buf.push(key, item);
  }
  return toU16Buffer(buf);
};

export const randomize = async (rom: Buffer, opts: Options) => {
  console.log("Randomizing...");
  const res = await logic(opts);
  const buffer = Buffer.alloc(0x20000, 0xff);
  for (const g of GAMES) {
    const gameBuffer = await randomizeGame(g, res);
    gameBuffer.copy(buffer, OFFSETS[g]);
  }
  buffer.copy(rom, 0x03fe0000);
  return res.log;
}

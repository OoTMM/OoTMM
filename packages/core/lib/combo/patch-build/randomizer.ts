import { HINTS, ENTRANCES, REGIONS, SCENES, NPC } from '@ootmm/data';

import { LogicResult } from '../logic';
import { isEntranceShuffle } from '../logic/helpers';
import { GI, DATA_HINTS_POOL } from '../data';
import { Game } from "../config";
import { World, WorldCheck } from '../logic/world';
import { DUNGEONS, Settings, SPECIAL_CONDS, SPECIAL_CONDS_FIELDS } from '../settings';
import { HINTS_PATHS, HintGossip, WorldHints } from '../logic/hints';
import { countMapAdd, gameId, padBuffer16, toI8Buffer, toU16Buffer, toU32Buffer, toU8Buffer } from '../util';
import { Patchfile } from './patchfile';
import { LOCATIONS_ZELDA, makeLocation, makePlayerLocations } from '../logic/locations';
import { CONFVARS_VALUES, Confvar } from '../confvars';
import { Region, regionData } from '../logic/regions';
import { Item, ItemGroups, ItemHelpers, Items, ItemsCount } from '../items';
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
  ['SHARED_SHIELD_HYLIAN',    'OOT_SHIELD_HYLIAN'],
  ['SHARED_SHIELD_MIRROR',    'OOT_SHIELD_MIRROR'],
  ['SHARED_SHIELD',           'OOT_SHIELD'],
  ['SHARED_MAGIC_JAR_SMALL',  'OOT_MAGIC_JAR_SMALL'],
  ['SHARED_MAGIC_JAR_LARGE',  'OOT_MAGIC_JAR_LARGE'],
  ['SHARED_BOMBCHU',          'MM_BOMBCHU'], /* OoT lacks single bombchu */
  ['SHARED_BOMBCHU_5',        'OOT_BOMBCHU_5'],
  ['SHARED_BOMBCHU_10',       'OOT_BOMBCHU_10'],
  ['SHARED_BOMBCHU_20',       'OOT_BOMBCHU_20'],
  ['SHARED_FAIRY_BIG',        'OOT_FAIRY_BIG'],
  ['SHARED_SPELL_FIRE',       'OOT_SPELL_FIRE'],
  ['SHARED_SPELL_WIND',       'OOT_SPELL_WIND'],
  ['SHARED_SPELL_LOVE',       'OOT_SPELL_LOVE'],
  ['SHARED_BOOTS_IRON',       'OOT_BOOTS_IRON'],
  ['SHARED_BOOTS_HOVER',      'OOT_BOOTS_HOVER'],
  ['SHARED_TUNIC_GORON',      'OOT_TUNIC_GORON'],
  ['SHARED_TUNIC_ZORA',       'OOT_TUNIC_ZORA'],
  ['SHARED_MASK_BLAST',       'OOT_MASK_BLAST'],
  ['SHARED_MASK_STONE',       'OOT_MASK_STONE'],
  ['SHARED_SCALE',            'OOT_SCALE'],
  ['SHARED_STRENGTH',         'OOT_STRENGTH'],
  ['SHARED_TRAP_RUPOOR',      'OOT_TRAP_RUPOOR'],
  ['SHARED_SONG_EMPTINESS',   'OOT_SONG_EMPTINESS'],
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
  ['SHARED_SHIELD_HYLIAN',    'MM_SHIELD_HERO'],
  ['SHARED_SHIELD_MIRROR',    'MM_SHIELD_MIRROR'],
  ['SHARED_SHIELD',           'OOT_SHIELD'], /* Progressive */
  ['SHARED_MAGIC_JAR_SMALL',  'MM_MAGIC_JAR_SMALL'],
  ['SHARED_MAGIC_JAR_LARGE',  'MM_MAGIC_JAR_LARGE'],
  ['SHARED_BOMBCHU',          'MM_BOMBCHU'],
  ['SHARED_BOMBCHU_5',        'MM_BOMBCHU_5'],
  ['SHARED_BOMBCHU_10',       'MM_BOMBCHU_10'],
  ['SHARED_BOMBCHU_20',       'MM_BOMBCHU_20'],
  ['SHARED_FAIRY_BIG',        'MM_FAIRY_BIG'],
  ['SHARED_SPELL_FIRE',       'MM_SPELL_FIRE'],
  ['SHARED_SPELL_WIND',       'MM_SPELL_WIND'],
  ['SHARED_SPELL_LOVE',       'MM_SPELL_LOVE'],
  ['SHARED_BOOTS_IRON',       'MM_BOOTS_IRON'],
  ['SHARED_BOOTS_HOVER',      'MM_BOOTS_HOVER'],
  ['SHARED_TUNIC_GORON',      'MM_TUNIC_GORON'],
  ['SHARED_TUNIC_ZORA',       'MM_TUNIC_ZORA'],
  ['SHARED_MASK_BLAST',       'MM_MASK_BLAST'],
  ['SHARED_MASK_STONE',       'MM_MASK_STONE'],
  ['SHARED_SCALE',            'MM_SCALE'],
  ['SHARED_STRENGTH',         'MM_STRENGTH'],
  ['SHARED_TRAP_RUPOOR',      'MM_TRAP_RUPOOR'],
  ['SHARED_SONG_EMPTINESS',   'MM_SONG_EMPTINESS'],
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
  MM_SCALE: "MM_SCALE_SILVER",
  MM_STRENGTH: "MM_GORON_BRACELET",
  MM_CLOCK: "MM_CLOCK1",
  SHARED_TRIFORCE: "OOT_TRIFORCE",
  SHARED_TRIFORCE_POWER: "OOT_TRIFORCE_POWER",
  SHARED_TRIFORCE_COURAGE: "OOT_TRIFORCE_COURAGE",
  SHARED_TRIFORCE_WISDOM: "OOT_TRIFORCE_WISDOM",
};

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
    const subst = SUBSTITUTIONS[itemId];
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

function zoraSapphireBuffer(world: number, logic: LogicResult): Buffer {
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
  case 'grass':
  case 'fairy':
  case 'rupee':
  case 'heart':
  case 'fairy_spot':
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
  case 'grass':
  case 'fairy':
  case 'rupee':
  case 'heart':
  case 'fairy_spot':
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

const gameChecks = (worldId: number, settings: Settings, game: Game, logic: LogicResult): Buffer => {
  const buffers: Buffer[] = [];
  const world = logic.worlds[worldId];
  for (const locId in world.checks) {
    const loc = makeLocation(locId, worldId);
    const c = world.checks[locId];
    const item = logic.items.get(loc)!;

    if (c.game !== game) {
      continue;
    }
    const key = checkKey(c);
    const itemGi = gi(settings, game, item.item, true);
    const b = Buffer.alloc(8, 0xff);
    b.writeUInt32BE(key, 0);
    b.writeUint16BE(item.player + 1, 4);
    b.writeUInt16BE(itemGi, 6);
    buffers.push(b);
  }
  /* Sort by key ascending */
  buffers.sort((a, b) => a.readUInt32BE(0) < b.readUInt32BE(0) ? -1 : 1);
  return padBuffer16(Buffer.concat(buffers));
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

const hintBuffer = (settings: Settings, game: Game, gossip: string, hint: HintGossip): Buffer => {
  const data = Buffer.alloc(0x10, 0xff);
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
      const regionD = regionData(hint.region);
      const region = (REGIONS as any)[regionD.id];
      const pathId = HINTS_PATHS[hint.path].id;
      if (region === undefined) {
        throw new Error(`Unknown region ${hint.region}`);
      }
      data.writeUInt8(id, HINT_OFFSETS.KEY);
      data.writeUInt8(0x00, HINT_OFFSETS.TYPE);
      data.writeUInt8(region, HINT_OFFSETS.REGION);
      data.writeUInt8(regionD.world + 1, HINT_OFFSETS.WORLD);
      data.writeUInt16BE(pathId, HINT_OFFSETS.ITEM);
    }
    break;
  case 'foolish':
    {
      const regionD = regionData(hint.region);
      const region = (REGIONS as any)[regionD.id];
      if (region === undefined) {
        throw new Error(`Unknown region ${hint.region}`);
      }
      data.writeUInt8(id, HINT_OFFSETS.KEY);
      data.writeUInt8(0x01, HINT_OFFSETS.TYPE);
      data.writeUInt8(region, HINT_OFFSETS.REGION);
      data.writeUInt8(regionD.world + 1, HINT_OFFSETS.WORLD);
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
      data.writeUInt8(id, HINT_OFFSETS.KEY);
      data.writeUInt8(0x02, HINT_OFFSETS.TYPE);
      data.writeUInt8(check, HINT_OFFSETS.REGION);
      data.writeUInt8(hint.world + 1, HINT_OFFSETS.WORLD);
      data.writeUInt16BE(itemsGI[0], HINT_OFFSETS.ITEM);
      data.writeUint8(items[0].player + 1, HINT_OFFSETS.PLAYER);
      data.writeInt8(hint.importances[0], HINT_OFFSETS.IMPORTANCE);
      if (items.length > 1) {
        data.writeUInt16BE(itemsGI[1], HINT_OFFSETS.ITEM2);
        data.writeUint8(items[1].player + 1, HINT_OFFSETS.PLAYER2);
        data.writeInt8(hint.importances[1], HINT_OFFSETS.IMPORTANCE2);
      }
      if (items.length > 2) {
        data.writeUInt16BE(itemsGI[2], HINT_OFFSETS.ITEM3);
        data.writeUint8(items[2].player + 1, HINT_OFFSETS.PLAYER3);
        data.writeInt8(hint.importances[2], HINT_OFFSETS.IMPORTANCE3);
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
        data.writeUInt8(id, HINT_OFFSETS.KEY);
        data.writeUInt8(0x03, HINT_OFFSETS.TYPE);
        data.writeUInt8(region, HINT_OFFSETS.REGION);
        data.writeUInt8(regionD.world + 1, HINT_OFFSETS.WORLD);
        data.writeUInt16BE(itemGI, HINT_OFFSETS.ITEM);
        data.writeUint8(item.player + 1, HINT_OFFSETS.PLAYER);
        data.writeInt8(hint.importance, HINT_OFFSETS.IMPORTANCE);
      }
      break;
  case 'junk':
    {
      data.writeUInt8(id, HINT_OFFSETS.KEY);
      data.writeUInt8(0x04, HINT_OFFSETS.TYPE);
      data.writeUInt16BE(hint.id, HINT_OFFSETS.ITEM);
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
  buffers.push(Buffer.alloc(0x10, 0xff));
  return padBuffer16(Buffer.concat(buffers));
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

const randomizerDungeonsBits = (worldId: number, logic: LogicResult): Buffer => {
  const DUNGEONS_PRECOMPLETED = [
    'DT',
    'DC',
    'JJ',
    'Forest',
    'Fire',
    'Water',
    'Shadow',
    'Spirit',
    'WF',
    'SH',
    'GB',
    'IST',
    'ST',
    'SSH',
    'OSH',
    'BotW',
    'IC',
    'GTG',
    'BtW',
    'ACoI',
    'SS',
    'BtWE',
    'PF',
    'Ganon',
    'Tower',
  ]
  let mq = 0;
  let preCompleted = 0;
  const dungeons = Object.keys(DUNGEONS);
  const world = logic.worlds[worldId];
  for (let i = 0; i < dungeons.length; ++i) {
    const dungeon = dungeons[i];
    if (world.mq.has(dungeon)) {
      mq |= 1 << i;
    }
  }

  for (let i = 0; i < DUNGEONS_PRECOMPLETED.length; ++i) {
    const dungeon = DUNGEONS_PRECOMPLETED[i];
    if (world.preCompleted.has(dungeon)) {
      preCompleted |= 1 << i;
    }
  }

  const buffer = Buffer.alloc(8);
  buffer.writeUInt32BE(mq, 0);
  buffer.writeUInt32BE(preCompleted, 4);
  return buffer;
}

function randomizerWarps(worldId: number, logic: LogicResult): Buffer {
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

  return Buffer.concat([warpSongs, owlStatuesBuffer]);
}

function worldConfig(world: World, settings: Settings): Set<Confvar> {
  const config = new Set<Confvar>;

  const exprs: {[k in Confvar]: boolean} = {
    GANON_NO_BOSS_KEY: settings.ganonBossKey === 'removed',
    SMALL_KEY_SHUFFLE: settings.smallKeyShuffleOot === 'anywhere',
    CSMC: settings.csmc === 'always',
    CSMC_EXTRA: settings.csmcExtra,
    CSMC_AGONY: settings.csmc === 'agony',
    OOT_PROGRESSIVE_SHIELDS: settings.progressiveShieldsOot === 'progressive',
    OOT_PROGRESSIVE_SWORDS: settings.progressiveSwordsOot === 'progressive',
    OOT_PROGRESSIVE_SWORDS_GORON: settings.progressiveSwordsOot === 'goron',
    MM_PROGRESSIVE_SHIELDS: settings.progressiveShieldsMm === 'progressive',
    MM_PROGRESSIVE_LULLABY: settings.progressiveGoronLullaby === 'progressive',
    DOOR_OF_TIME_OPEN: settings.doorOfTime === 'open',
    OOT_OPEN_DEKU: settings.dekuTree === 'open',
    ER_DUNGEONS: settings.erDungeons !== 'none',
    ER_MAJOR_DUNGEONS: settings.erMajorDungeons,
    ER_BOSS: settings.erBoss !== 'none',
    ER_ANY: isEntranceShuffle(settings),
    SHARED_BOWS: settings.sharedBows,
    SHARED_BOMB_BAGS: settings.sharedBombBags,
    SHARED_MAGIC: settings.sharedMagic,
    SHARED_MAGIC_ARROW_FIRE: settings.sharedMagicArrowFire,
    SHARED_MAGIC_ARROW_ICE: settings.sharedMagicArrowIce,
    SHARED_MAGIC_ARROW_LIGHT: settings.sharedMagicArrowLight,
    SHARED_SONG_EPONA: settings.sharedSongEpona,
    SHARED_SONG_STORMS: settings.sharedSongStorms,
    SHARED_SONG_TIME: settings.sharedSongTime,
    SHARED_SONG_SUN: settings.sharedSongSun,
    SHARED_NUTS_STICKS: settings.sharedNutsSticks,
    SHARED_HOOKSHOT: settings.sharedHookshot,
    SHARED_LENS: settings.sharedLens,
    SHARED_OCARINA: settings.sharedOcarina,
    SHARED_MASK_GORON: settings.sharedMaskGoron,
    SHARED_MASK_ZORA: settings.sharedMaskZora,
    SHARED_MASK_BUNNY: settings.sharedMaskBunny,
    SHARED_MASK_TRUTH: settings.sharedMaskTruth,
    SHARED_MASK_KEATON: settings.sharedMaskKeaton,
    SHARED_WALLETS: settings.sharedWallets,
    SHARED_HEALTH: settings.sharedHealth,
    SHARED_SOULS_ENEMY: settings.sharedSoulsEnemy,
    SHARED_OCARINA_BUTTONS: settings.sharedOcarinaButtons,
    SHARED_SHIELDS: settings.sharedShields,
    SHARED_SPELL_FIRE: settings.sharedSpellFire,
    SHARED_SPELL_WIND: settings.sharedSpellWind,
    SHARED_SPELL_LOVE: settings.sharedSpellLove,
    SHARED_BOOTS_IRON: settings.sharedBootsIron,
    SHARED_BOOTS_HOVER: settings.sharedBootsHover,
    SHARED_TUNIC_GORON: settings.sharedTunicGoron,
    SHARED_TUNIC_ZORA: settings.sharedTunicZora,
    SHARED_SCALES: settings.sharedScales,
    SHARED_STRENGTH: settings.sharedStrength,
    OOT_CROSS_WARP: settings.crossWarpOot,
    MM_CROSS_WARP: settings.crossWarpMm !== 'none',
    MM_CROSS_WARP_ADULT: settings.crossWarpMm === 'full',
    MM_OCARINA_FAIRY: settings.fairyOcarinaMm,
    MM_HOOKSHOT_SHORT: settings.shortHookshotMm,
    MM_SONG_SUN: settings.sunSongMm,
    OOT_SKIP_ZELDA: settings.skipZelda,
    OOT_OPEN_KAKARIKO_GATE: settings.kakarikoGate === 'open',
    OOT_LACS_CUSTOM: settings.lacs === 'custom',
    OOT_GANON_BK_CUSTOM: settings.ganonBossKey === 'custom',
    OOT_KZ_OPEN: settings.zoraKing === 'open',
    OOT_KZ_OPEN_ADULT: settings.zoraKing === 'adult',
    GOAL_GANON: settings.goal === 'ganon' || settings.goal === 'both',
    GOAL_MAJORA: settings.goal === 'majora' || settings.goal === 'both',
    GOAL_TRIFORCE: settings.goal === 'triforce',
    GOAL_TRIFORCE3: settings.goal === 'triforce3',
    MM_MAJORA_CHILD_CUSTOM: settings.majoraChild === 'custom',
    FILL_WALLETS: settings.fillWallets,
    CHILD_WALLET: settings.childWallets,
    OOT_ADULT_WELL: settings.wellAdult,
    COLOSSAL_WALLET: settings.colossalWallets,
    BOTTOMLESS_WALLET: settings.bottomlessWallets,
    OOT_AGELESS_BOOTS: settings.agelessBoots,
    MM_OWL_SHUFFLE: settings.owlShuffle === 'anywhere',
    OOT_CARPENTERS_ONE: settings.gerudoFortress === 'single',
    OOT_CARPENTERS_NONE: settings.gerudoFortress === 'open',
    OOT_NO_BOSS_KEY: settings.bossKeyShuffleOot === 'removed',
    OOT_NO_SMALL_KEY: settings.smallKeyShuffleOot === 'removed',
    MM_NO_BOSS_KEY: settings.bossKeyShuffleMm === 'removed',
    MM_NO_SMALL_KEY: settings.smallKeyShuffleMm === 'removed',
    CSMC_HEARTS: settings.csmcHearts,
    OOT_BLUE_FIRE_ARROWS: settings.blueFireArrows,
    OOT_SUNLIGHT_ARROWS: settings.sunlightArrows,
    OOT_SILVER_RUPEE_SHUFFLE: settings.silverRupeeShuffle !== 'vanilla',
    OOT_FREE_SCARECROW: settings.freeScarecrowOot,
    OOT_SOULS_ENEMY: settings.soulsEnemyOot,
    MM_SOULS_ENEMY: settings.soulsEnemyMm,
    OOT_SOULS_BOSS: settings.soulsBossOot,
    MM_SOULS_BOSS: settings.soulsBossMm,
    OOT_SOULS_NPC: settings.soulsNpcOot,
    MM_REMOVED_FAIRIES: settings.strayFairyOtherShuffle === 'removed',
    SHARED_SKELETON_KEY: settings.sharedSkeletonKey,
    OOT_SHUFFLE_GRASS: settings.shuffleGrassOot,
    MM_SHUFFLE_GRASS: settings.shuffleGrassMm,
    MENU_NOTEBOOK: settings.menuNotebook,
    OOT_AGELESS_CHILD_TRADE: settings.agelessChildTrade,
    OOT_START_ADULT: settings.startingAge === 'adult',
    HINT_IMPORTANCE: settings.hintImportance,
    OOT_OCARINA_BUTTONS: settings.ocarinaButtonsShuffleOot,
    MM_OCARINA_BUTTONS: settings.ocarinaButtonsShuffleMm,
    OOT_AGE_CHANGE: settings.ageChange !== 'none',
    OOT_AGE_CHANGE_NEEDS_OOT: settings.ageChange === 'oot',
    MM_PROGRESSIVE_GFS: settings.progressiveGFS === 'progressive',
    OOT_CHEST_GAME_SHUFFLE: settings.smallKeyShuffleChestGame !== 'vanilla',
    MM_CLIMB_MOST_SURFACES: settings.climbMostSurfacesMm,
    OOT_TRIAL_LIGHT: world.resolvedFlags.ganonTrials.has('Light'),
    OOT_TRIAL_FOREST: world.resolvedFlags.ganonTrials.has('Forest'),
    OOT_TRIAL_FIRE: world.resolvedFlags.ganonTrials.has('Fire'),
    OOT_TRIAL_WATER: world.resolvedFlags.ganonTrials.has('Water'),
    OOT_TRIAL_SHADOW: world.resolvedFlags.ganonTrials.has('Shadow'),
    OOT_TRIAL_SPIRIT: world.resolvedFlags.ganonTrials.has('Spirit'),
    ER_MOON: settings.erMoon,
    MM_OPEN_MOON: settings.openMoon,
    NO_BROKEN_ACTORS: !settings.restoreBrokenActors,
    OOT_BOMBCHU_BAG: settings.bombchuBagOot,
    MM_BOMBCHU_BAG: settings.bombchuBagMm,
    SHARED_BOMBCHU: settings.sharedBombchuBags,
    ER_WALLMASTERS: settings.erWallmasters !== 'none',
    OOT_OPEN_MASK_SHOP: settings.openMaskShop,
    OOT_BRIDGE_VANILLA: settings.rainbowBridge === 'vanilla',
    OOT_BRIDGE_MEDALLIONS: settings.rainbowBridge === 'medallions',
    OOT_BRIDGE_CUSTOM: settings.rainbowBridge === 'custom',
    MULTIPLAYER: settings.mode !== 'single',
    MM_OPEN_WF: world.resolvedFlags.openDungeonsMm.has('WF'),
    MM_OPEN_SH: world.resolvedFlags.openDungeonsMm.has('SH'),
    MM_OPEN_GB: world.resolvedFlags.openDungeonsMm.has('GB'),
    MM_OPEN_ST: world.resolvedFlags.openDungeonsMm.has('ST'),
    MM_CLEAR_OPEN_WF: settings.clearStateDungeonsMm === 'WF' || settings.clearStateDungeonsMm === 'both',
    MM_CLEAR_OPEN_GB: settings.clearStateDungeonsMm === 'GB' || settings.clearStateDungeonsMm === 'both',
    MM_SPELL_FIRE: settings.spellFireMm,
    MM_SPELL_WIND: settings.spellWindMm,
    MM_SPELL_LOVE: settings.spellLoveMm,
    MM_BOOTS_IRON: settings.bootsIronMm,
    MM_BOOTS_HOVER: settings.bootsHoverMm,
    MM_TUNIC_GORON: settings.tunicGoronMm,
    MM_TUNIC_ZORA: settings.tunicZoraMm,
    MM_SCALES: settings.scalesMm,
    MM_STRENGTH: settings.strengthMm,
    OOT_GANON_BOSS_KEY_HINT: settings.ganonBossKey === 'anywhere',
    BLAST_MASK_DELAY_INSTANT: settings.blastMaskCooldown === 'instant',
    BLAST_MASK_DELAY_VERYSHORT: settings.blastMaskCooldown === 'veryshort',
    BLAST_MASK_DELAY_SHORT: settings.blastMaskCooldown === 'short',
    BLAST_MASK_DELAY_LONG: settings.blastMaskCooldown === 'long',
    BLAST_MASK_DELAY_VERYLONG: settings.blastMaskCooldown === 'verylong',
    OOT_MASK_BLAST: settings.blastMaskOot,
    SHARED_MASK_BLAST: settings.sharedMaskBlast,
    OOT_MASK_STONE: settings.stoneMaskOot,
    SHARED_MASK_STONE: settings.sharedMaskStone,
    OOT_SONG_EMPTINESS: settings.elegyOot,
    SHARED_SONG_EMPTINESS: settings.sharedSongElegy,
    MM_FD_ANYWHERE: settings.fierceDeityAnywhere,
    MM_CLOCK_SPEED_VERYSLOW: settings.clockSpeed === 'veryslow',
    MM_CLOCK_SPEED_SLOW: settings.clockSpeed === 'slow',
    MM_CLOCK_SPEED_FAST: settings.clockSpeed === 'fast',
    MM_CLOCK_SPEED_VERYFAST: settings.clockSpeed === 'veryfast',
    MM_CLOCK_SPEED_SUPERFAST: settings.clockSpeed === 'superfast',
    MM_AUTO_INVERT_ALWAYS: settings.autoInvert === 'always',
    MM_AUTO_INVERT_FIRST_CYCLE: settings.autoInvert === 'firstCycle',
    MM_MOON_CRASH_CYCLE: settings.moonCrash === 'cycle',
    OOT_OPEN_ZD_SHORTCUT: settings.openZdShortcut,
    MM_CLOCKS: settings.clocks,
    MM_CLOCKS_PROGRESSIVE: settings.progressiveClocks !== 'separate',
    MM_CLOCKS_PROGRESSIVE_REVERSE: settings.progressiveClocks === 'descending',
  };

  for (const v in exprs) {
    if (exprs[v as Confvar]) {
      config.add(v as Confvar);
    }
  }

  return config;
}

export const randomizerConfig = (world: World, settings: Settings): Buffer => {
  const config = worldConfig(world, settings);
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
const randomizerTriforce = (logic: LogicResult): Buffer => toU16Buffer([logic.settings.triforcePieces, logic.settings.triforceGoal]);

function specialConds(settings: Settings) {
  const buffers: Buffer[] = [];
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
    const buffer = Buffer.alloc(8);
    buffer.writeUInt32BE(flags, 0);
    buffer.writeUInt16BE(cond.count, 4);
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
  buffers.push(randomizerDungeons(worldId, logic));
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

  if (settings.skipZelda) addStartingItemLocsWorld(worldId, logic, LOCATIONS_ZELDA, itemsCount);
  if (settings.gerudoFortress === 'open') addStartingItemLocsWorld(worldId, logic, ['OOT Gerudo Member Card'], itemsCount);

  return itemsCount;
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
  patchfile.addNewFile(null, 0xf0200000, randomizerData(worldId, logic), true);
  patchfile.addNewFile(null, 0xf0300000, randomizerStartingItems(worldId, logic), false);
  patchfile.addNewFile(null, 0xf0400000, gameChecks(worldId, settings, 'oot', logic), false);
  patchfile.addNewFile(null, 0xf0500000, gameChecks(worldId, settings, 'mm', logic), false);
  patchfile.addNewFile(null, 0xf0600000, gameHints(settings, 'oot', logic.hints[worldId]), true);
  patchfile.addNewFile(null, 0xf0700000, gameHints(settings, 'mm', logic.hints[worldId]), true);
  patchfile.addNewFile(null, 0xf0800000, gameEntrances(worldId, 'oot', logic), true);
  patchfile.addNewFile(null, 0xf0900000, gameEntrances(worldId, 'mm', logic), true);
}

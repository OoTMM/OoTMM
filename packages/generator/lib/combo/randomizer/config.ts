import type { Game } from '@ootmm/data';
import type { Settings, Region } from '@ootmm/core';
import type { LogicResult, LogicResultWorld } from '@ootmm/logic';

import { concatUint8Arrays } from 'uint8array-extras';
import { ENTRANCES, REGIONS } from '@ootmm/data';
import { SETTINGS, SPECIAL_CONDS, SPECIAL_CONDS_FIELDS, MM_AGE_REQ_ITEMS, regionData } from '@ootmm/core';
import { DUNGEONS, DUNGEON_ENTRANCES, makeLocation, Items } from '@ootmm/logic';

import { toI8Buffer, toU16Buffer, toU32Buffer, toU8Buffer } from '../util';
import { bufWriteU16BE, bufWriteU32BE } from '../util/buffer';
import { worldConfig } from './world-config';
import { CONFVARS_VALUES } from '../confvars';
import { gi } from './util';

const MM_AGE_REQ_VALUES = { none: 0, child: 1, adult: 2 } as const;

const MM_AGE_REQ_ITEM_IDS: Record<string, readonly number[]> = {
  hookshot: [0x0f],
  hookshotShort: [0x11],
  bow: [0x01],
  ocarinaOfTime: [0x00],
  ocarinaFairy: [0x05],
  bomb: [0x06],
  bombchu: [0x07],
  arrowFire: [0x02],
  arrowIce: [0x03],
  arrowLight: [0x04],
  hammer: [0xb5],
  spellWind: [0xae],
  spellLove: [0xaf],
  spellFire: [0xb0],
  bootsIron: [0xb1],
  bootsHover: [0xb2],
  tunicGoron: [0xb3],
  tunicZora: [0xb4],
  greatFairySword: [0x10],
  boomerang: [0xb6],
  slingshot: [0xb7],
  stick: [0x08],
  maskDeku: [0x32],
  nut: [0x09],
  magicBean: [0x0a],
  powderKeg: [0x0c],
  pictographBox: [0x0d],
  lensOfTruth: [0x0e],
  maskGoron: [0x33],
  maskZora: [0x34],
  maskFierceDeity: [0x35],
  maskPostman: [0x3e],
  maskAllNight: [0x38],
  maskBlast: [0x47],
  maskStone: [0x45],
  maskGreatFairy: [0x40],
  maskKeaton: [0x3a],
  maskBremen: [0x46],
  maskBunny: [0x39],
  maskDonGero: [0x42],
  maskScents: [0x48],
  maskRomani: [0x3c],
  maskTroupeLeader: [0x3d],
  maskKafei: [0x37],
  maskCouple: [0x3f],
  maskTruth: [0x36],
  maskKamaro: [0x43],
  maskGibdo: [0x41],
  maskGaro: [0x3b],
  maskCaptain: [0x44],
  maskGiant: [0x49],
  maskGerudo: [0xb9],
  maskSkull: [0xba],
  maskSpooky: [0xbb],
  moonTear: [0x28],
  deedLand: [0x29],
  deedSwamp: [0x2a],
  deedMountain: [0x2b],
  deedOcean: [0x2c],
  letterToMama: [0x2e],
  letterToKafei: [0x2f],
  pendantOfMemories: [0x30],
  swordKokiri: [0x4d],
  swordRazor: [0x4e],
  swordGilded: [0x4f],
  swordMaster: [0xc1],
  swordGoron: [0xc2, 0xc3],
  shieldDeku: [0xc5],
  shieldHero: [0x51],
  shieldHylian: [0xc4],
  shieldMirror: [0x52],
};

const BOMBCHU_BEHAVIORS = {
  free: 0,
  bombBag: 1,
  bagFirst: 2,
  bagSeparate: 3,
};

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

function entrance(srcName: string, world: LogicResultWorld) {
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

function entranceAbs(world: LogicResultWorld, name: string) {
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

function regionsBuffer(regions: Region[]) {
  const data = regions.map((region) => {
    const regionId = (REGIONS as any)[regionData(region).id];
    if (regionId === undefined) {
      throw new Error(`Unknown region ${region}`);
    }
    const world = regionData(region).world + 1;
    return [regionId, world];
  });
  return toU8Buffer(data.flat());
}

type RandomizerPatcherConfigContext = {
  worldId: number;
  logic: LogicResult;
  settings: Settings;
};

export class RandomizerPatcherConfig {
  private world: LogicResultWorld;
  private ctx: RandomizerPatcherConfigContext;

  constructor(ctx: RandomizerPatcherConfigContext) {
    this.ctx = ctx;
    this.world = ctx.logic.worlds[ctx.worldId];
  }

  public static run(ctx: RandomizerPatcherConfigContext) {
    return (new RandomizerPatcherConfig(ctx)).execute();
  }

  private execute() {
    const buffers = [];
    buffers.push(this.dungeonWarpsBuffer());
    buffers.push(this.dungeonEntrancesBuffer());
    buffers.push(this.randomizerDungeonsBits());
    buffers.push(this.randomizerWarps());
    buffers.push(this.randomizerConfig());
    buffers.push(this.specialConds());
    buffers.push(toU16Buffer([this.ctx.settings.coinsRed, this.ctx.settings.coinsGreen, this.ctx.settings.coinsBlue, this.ctx.settings.coinsYellow]));
    buffers.push(toU16Buffer(this.world.prices));
    buffers.push(toU16Buffer([this.ctx.settings.triforcePieces, this.ctx.settings.triforceGoal]));
    buffers.push(this.randomizerHints());
    buffers.push(this.zoraSapphireBuffer());
    buffers.push(toI8Buffer(this.ctx.logic.hints[this.ctx.worldId].staticHintsImportances));
    buffers.push(toU8Buffer(this.world.bossIds));
    buffers.push(toU8Buffer([this.ctx.settings.strayFairyRewardCount]));
    buffers.push(toU8Buffer([BOMBCHU_BEHAVIORS[this.ctx.settings.bombchuBehaviorOot]]));
    buffers.push(toU8Buffer([BOMBCHU_BEHAVIORS[this.ctx.settings.bombchuBehaviorMm]]));
    buffers.push(toU8Buffer(this.world.songEventsOot));
    buffers.push(toU8Buffer(this.world.songEventsMm));
    buffers.push(toU8Buffer([
      this.ctx.settings.giantsKnifeDurabilityOot,
      this.ctx.settings.GiantsKnifeDurabilityMm,
    ]));
    buffers.push(toU8Buffer([MM_AGE_REQ_VALUES[this.ctx.settings.mmAgeReqStrength]]));
    buffers.push(this.mmAgeRequirementsBuffer());
    return concatUint8Arrays(buffers);
  }

  private dungeonWarpsBuffer() {
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

    const entrances = defaultWarps.map((e) => entranceAbs(this.world, e));
    return toU32Buffer(entrances);
  }

  private dungeonEntrancesBuffer() {
    const buffer = new Uint8Array(DUNGEON_ENTRANCES.length * 4);

    for (let i = 0; i < DUNGEON_ENTRANCES.length; ++i) {
      const entranceName = DUNGEON_ENTRANCES[i];
      const entranceData = this.world.dungeonsEntrances.get(entranceName) || { type: 'region', region: 'NONE' as Region };
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

  private randomizerDungeonsBits(): Uint8Array {
    let mq = 0;
    let preCompleted = 0;
    const settingDataMQ = SETTINGS.find(x => x.key === 'mqDungeons')!;
    for (let i = 0; i < settingDataMQ.values.length; ++i) {
      const dungeon = settingDataMQ.values[i].value;
      if (this.world.resolvedFlags.mqDungeons.has(dungeon)) {
        mq |= 1 << i;
      }
    }

    for (let i = 0; i < DUNGEONS.length; ++i) {
      const dungeon = DUNGEONS[i];
      if (this.world.preCompleted.has(dungeon.key)) {
        preCompleted |= 1 << i;
      }
    }

    const buffer = new Uint8Array(8);
    bufWriteU32BE(buffer, 0, mq);
    bufWriteU32BE(buffer, 4, preCompleted);
    return buffer;
  }

  private randomizerWarps(): Uint8Array {
    const songs = [
      'OOT_WARP_SONG_MEADOW',
      'OOT_WARP_SONG_CRATER',
      'OOT_WARP_SONG_LAKE',
      'OOT_WARP_SONG_DESERT',
      'OOT_WARP_SONG_GRAVE',
      'OOT_WARP_SONG_TEMPLE',
    ];
    const warpSongs = toU32Buffer(songs.map(e => entrance(e, this.world)));

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
    const owlStatuesBuffer = toU32Buffer(owlStatues.map(e => entrance(e, this.world)));

    const spawns = [
      'OOT_SPAWN_ADULT',
      'OOT_SPAWN_CHILD'
    ];
    const spawnsBuffer = toU32Buffer(spawns.map(e => entrance(e, this.world)));

    return concatUint8Arrays([warpSongs, owlStatuesBuffer, spawnsBuffer]);
  }

  private randomizerConfig(): Uint8Array {
    const config = worldConfig(this.world, this.ctx.settings);
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
  }

  private mmAgeRequirementsBuffer(): Uint8Array {
    const result = new Uint8Array(0x40);
    for (const item of MM_AGE_REQ_ITEMS) {
      if (item.id === 'strength') {
        continue;
      }
      const age = this.ctx.settings[item.setting];
      const value = MM_AGE_REQ_VALUES[age];
      if (value === undefined) {
        throw new Error(`Unknown age requirement ${age} for ${item.id}`);
      }
      const ids = MM_AGE_REQ_ITEM_IDS[item.id];
      if (!ids || ids.length === 0) {
        throw new Error(`Missing numeric MM item IDs for ${item.id}`);
      }
      for (const id of ids) {
        result[id >> 2] |= value << ((id & 3) * 2);
      }
    }
    return result;
  }

  private specialConds() {
    const buffers: Uint8Array[] = [];
    const flagsKeys: keyof typeof SPECIAL_CONDS_FIELDS = Object.keys(SPECIAL_CONDS_FIELDS) as any;
    for (const special in SPECIAL_CONDS) {
      const cond = this.ctx.settings.specialConds[special as keyof typeof SPECIAL_CONDS];
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

  private randomizerHints(): Uint8Array {
    const buffers: Uint8Array[] = [];
    const h = this.ctx.logic.hints[this.ctx.worldId];
    buffers.push(regionsBuffer(h.dungeonRewards));
    buffers.push(regionsBuffer([h.lightArrow]));
    buffers.push(regionsBuffer(h.oathToOrder));
    buffers.push(regionsBuffer([h.ganonBossKey]));
    return concatUint8Arrays(buffers);
  }

  private zoraSapphireGI(): number | null {
    /* Find the dungeon holding the Zora Sapphire */
    const dungeonId = this.world.bossIds.indexOf(0x02);
    if (dungeonId === -1)
      return null;

    /* Find the location */
    const locId = DUNGEON_REWARD_LOCATIONS[dungeonId];
    if (!locId)
      return null;
    const loc = makeLocation(locId, this.ctx.worldId);
    const item = this.ctx.logic.items.get(loc);
    if (!item)
      return null;
    return gi(this.ctx.settings, 'oot', item.item, false);
  }

  private zoraSapphireBuffer(): Uint8Array {
    let value = this.zoraSapphireGI();
    if (value === null)
      value = gi(this.ctx.settings, 'oot', Items.OOT_STONE_SAPPHIRE, false);
    return toU16Buffer([value]);
  }
}

import { concatUint8Arrays } from 'uint8array-extras';
import { Game, SETTINGS, Settings, SPECIAL_CONDS, SPECIAL_CONDS_FIELDS } from '@ootmm/core';

import { toI8Buffer, toU16Buffer, toU32Buffer, toU8Buffer } from '../util';
import { World } from '../logic/world';
import { LogicResult } from '../logic';
import { bufWriteU16BE, bufWriteU32BE } from '../util/buffer';
import { DUNGEON_ENTRANCES } from '../logic/entrance';
import { ENTRANCES, REGIONS } from '@ootmm/data';
import { DUNGEONS } from '../logic/dungeons';
import { worldConfig } from './world-config';
import { CONFVARS_VALUES } from '../confvars';
import { makeLocation } from '../logic/locations';
import { Items } from '../items';
import { gi } from './gi';
import { Region, regionData } from '../logic/regions';

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
  private world: World;
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
    buffers.push(toU8Buffer([this.ctx.worldId + 1, 0, 0, 0]));
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
    buffers.push(toI8Buffer(this.ctx.logic.hints[this.ctx.worldId].staticHintsImportances));
    buffers.push(this.zoraSapphireBuffer());
    buffers.push(toU8Buffer(this.world.bossIds));
    buffers.push(toU8Buffer([this.ctx.settings.strayFairyRewardCount]));
    buffers.push(toU8Buffer([BOMBCHU_BEHAVIORS[this.ctx.settings.bombchuBehaviorOot]]));
    buffers.push(toU8Buffer([BOMBCHU_BEHAVIORS[this.ctx.settings.bombchuBehaviorMm]]));
    buffers.push(toU8Buffer(this.world.songEvents));
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

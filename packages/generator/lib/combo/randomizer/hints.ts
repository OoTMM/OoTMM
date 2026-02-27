import { concatUint8Arrays } from 'uint8array-extras';
import { Game, Settings } from '@ootmm/core';
import { HINTS, REGIONS } from '@ootmm/data';

import { LogicResult } from '../logic';
import { BOSS_INDEX_BY_DUNGEON } from '../logic/world';
import { padBuffer16 } from '../util';
import { DATA_HINTS_POOL } from '../data';
import { HintGossip } from '../logic/hints';
import { regionData } from '../logic/regions';
import { DUNGEONS_BY_KEY } from '../logic/dungeons';
import { END_BOSS_METADATA } from '../logic/boss';
import { PATH_EVENT_DATA } from '../logic/analysis-path';
import { bufWriteI8, bufWriteU16BE, bufWriteU8 } from '../util/buffer';
import { gi, playerId } from './util';

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

type RandomizerPatcherHintsContext = {
  game: Game;
  worldId: number;
  logic: LogicResult;
  settings: Settings;
};


export class RandomizerPatcherHints {
  private ctx: RandomizerPatcherHintsContext;

  constructor(ctx: RandomizerPatcherHintsContext) {
    this.ctx = ctx;
  }

  public static run(ctx: RandomizerPatcherHintsContext) {
    return (new RandomizerPatcherHints(ctx)).execute();
  }

  private execute(): Uint8Array {
    const buffers: Uint8Array[] = [];
    const hints = this.ctx.logic.hints[this.ctx.worldId];
    for (const gossip in hints.gossip) {
      const h = hints.gossip[gossip];
      if (h.game !== this.ctx.game) {
        continue;
      }
      buffers.push(this.hintBuffer(gossip, h));
    }
    const b = new Uint8Array(0x10);
    b.fill(0xff);
    buffers.push(b);
    return padBuffer16(concatUint8Arrays(buffers));
  }

  private hintBuffer(gossip: string, hint: HintGossip): Uint8Array {
    const data = new Uint8Array(0x10);
    data.fill(0xff);
    let gossipData = DATA_HINTS_POOL[this.ctx.game][gossip];
    if (!gossipData) {
      throw new Error(`Unknown gossip ${gossip} for game ${this.ctx.game}`);
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
          const itemsGI = hint.items.map((item) => gi(this.ctx.settings, 'oot', item.item, false));
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
          const itemGI = gi(this.ctx.settings, 'oot', item.item, false);
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
}

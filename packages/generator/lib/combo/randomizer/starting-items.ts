import { Settings } from '@ootmm/core';

import { LogicResult } from '../logic';
import { World } from '../logic/world';
import { countMapAdd, toU16Buffer } from '../util';
import { ItemGroups, ItemHelpers, ItemsCount } from '../items';
import { gi } from './gi';
import { getPreActivatedOwlsLocations, locationsZelda, makePlayerLocations } from '../logic/locations';

type RandomizerPatcherStartingItemsContext = {
  worldId: number;
  logic: LogicResult;
  settings: Settings;
};

export class RandomizerPatcherStartingItems {
  private world: World;
  private ctx: RandomizerPatcherStartingItemsContext;

  constructor(ctx: RandomizerPatcherStartingItemsContext) {
    this.ctx = ctx;
    this.world = ctx.logic.worlds[ctx.worldId];
  }

  public static run(ctx: RandomizerPatcherStartingItemsContext) {
    return (new RandomizerPatcherStartingItems(ctx)).execute();
  }

  private execute(): Uint8Array {
    const ids: number[] = [];
    const ids2: number[] = [];
    const items = this.effectiveStartingItems();
    for (const [item, count] of items.entries()) {
      const id = gi(this.ctx.settings, 'oot', item, false);
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
  }

  private effectiveStartingItems(): ItemsCount {
    const itemsCount: ItemsCount = new Map;
    for (const [pi, count] of this.ctx.logic.startingItems) {
      if (pi.player === this.ctx.worldId) {
        itemsCount.set(pi.item, count);
      }
    }

    if (this.ctx.settings.tingleShuffle === 'starting') {
      for (const item of ItemGroups.TINGLE_MAPS) {
        itemsCount.set(item, 1);
      }
    }

    if (this.ctx.settings.mapCompassShuffle === 'starting') {
      for (const item of [...ItemGroups.MAPS, ...ItemGroups.COMPASSES]) {
        itemsCount.set(item, 1);
      }
    }

    if (this.ctx.settings.skipZelda) this.addStartingItemLocsWorld(locationsZelda(this.ctx.settings), itemsCount);
    if (this.ctx.settings.gerudoFortress === 'open') this.addStartingItemLocsWorld(['OOT Gerudo Member Card'], itemsCount);
    if (this.ctx.settings.mmPreActivatedOwls.type !== 'none') this.addStartingItemLocsWorld(getPreActivatedOwlsLocations(this.world), itemsCount);

    return itemsCount;
  }

  private addStartingItemLocsWorld(locs: string[], items: ItemsCount) {
    const l = makePlayerLocations(this.ctx.settings, locs);
    const i = l.map(x => this.ctx.logic.items.get(x)!);

    for (const item of i) {
      if (item.player === this.ctx.worldId) {
        countMapAdd(items, item.item);
      }
    }
  }
}

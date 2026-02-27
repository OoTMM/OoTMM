import { Random, sample, Settings } from '@ootmm/core';

import { Item, ItemGroups, ItemHelpers, PlayerItem } from '../items';
import { ItemPlacement } from './solve';
import { isLocationFullyShuffled, Location } from './locations';

export class LogicPassCloak {
  private itemCloaks: Map<Location, Item>;

  constructor(
    private readonly state: {
      settings: Settings;
      items: ItemPlacement;
      random: Random;
      fixedLocations: Set<Location>;
      plandoLocations: Map<Location, PlayerItem>;
    }
  ) {
    this.itemCloaks = new Map();
  }

  run() {
    if (this.state.settings.cloakTraps) {
      this.cloakTraps();
    }
    return { itemCloaks: this.itemCloaks };
  }

  private cloakTraps() {
    for (const [location, pi] of this.state.items.entries()) {
      if (!ItemGroups.TRAPS.has(pi.item)) {
        continue;
      }
      const cloakItem = this.makeCloakItem();
      this.itemCloaks.set(location, cloakItem);
    }
  }

  private makeCloakItem() {
    for (;;) {
      const locs = [...this.state.items.keys()];
      const loc = sample(this.state.random, locs);
      const item = this.state.items.get(loc)!;

      if (!ItemHelpers.isItemMajor(item.item) && !ItemHelpers.isSilverRupee(item.item) && !ItemHelpers.isKey(item.item) && !ItemHelpers.isBossKey(item.item)) continue;
      if (ItemGroups.JUNK.has(item.item)) continue;
      if (!isLocationFullyShuffled(this.state.settings, this.state.fixedLocations, this.state.items, this.state.plandoLocations, loc, { songs: true, noPlando: true })) continue;

      return item.item;
    }
  }
}

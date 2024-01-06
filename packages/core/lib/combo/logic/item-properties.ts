import { Item, ItemGroups } from '../items';
import { Settings } from '../settings';

export type ItemProperties = {
  junk: Set<Item>;
  license: Set<Item>;
  important: Set<Item>;
};

export class LogicPassItemProperties {
  private properties: ItemProperties;
  constructor(
    private readonly state: {
      settings: Settings;
    }
  ) {
    this.properties = {
      junk: new Set(),
      license: new Set(),
      important: new Set(),
    };
  }

  run() {
    this.properties.junk = new Set(ItemGroups.JUNK);
    this.properties.license = new Set(ItemGroups.LICENSES);

    if (this.state.settings.bombchuBagOot) {
      this.properties.license = new Set([...this.properties.license, ...ItemGroups.OOT_BOMBCHU, ...ItemGroups.SHARED_BOMBCHU]);
    } else {
      this.properties.junk = new Set([...this.properties.junk, ...ItemGroups.OOT_BOMBCHU, ...ItemGroups.SHARED_BOMBCHU]);
    }

    if (this.state.settings.bombchuBagMm) {
      this.properties.license = new Set([...this.properties.license, ...ItemGroups.MM_BOMBCHU, ...ItemGroups.SHARED_BOMBCHU]);
    } else {
      this.properties.junk = new Set([...this.properties.junk, ...ItemGroups.MM_BOMBCHU, ...ItemGroups.SHARED_BOMBCHU]);
    }

    this.properties.important = new Set([...ItemGroups.REQUIRED, ...ItemGroups.CONSUMABLES, ...this.properties.license]);

    return { itemProperties: this.properties };
  }
};

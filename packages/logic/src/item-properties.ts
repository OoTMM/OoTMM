import type { Settings, Item } from '@ootmm/core';

import { ItemGroups } from '@ootmm/core';

export type ItemProperties = {
  junk: Set<Item>;
  license: Set<Item>;
  important: Set<Item>;
};

type LogicPassItemPropertiesState = {
  settings: Settings;
};

class LogicPassItemProperties {
  private properties: ItemProperties;
  constructor(
    private readonly state: LogicPassItemPropertiesState,
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

    if (this.state.settings.bombchuBehaviorOot === 'bagFirst') {
      this.properties.license = new Set([...this.properties.license, ...ItemGroups.OOT_BOMBCHU, ...ItemGroups.SHARED_BOMBCHU]);
    } else {
      this.properties.junk = new Set([...this.properties.junk, ...ItemGroups.OOT_BOMBCHU, ...ItemGroups.SHARED_BOMBCHU]);
    }

    if (this.state.settings.bombchuBehaviorMm === 'bagFirst') {
      this.properties.license = new Set([...this.properties.license, ...ItemGroups.MM_BOMBCHU, ...ItemGroups.SHARED_BOMBCHU]);
    } else {
      this.properties.junk = new Set([...this.properties.junk, ...ItemGroups.MM_BOMBCHU, ...ItemGroups.SHARED_BOMBCHU]);
    }

    this.properties.important = new Set([...ItemGroups.REQUIRED, ...ItemGroups.CONSUMABLES, ...this.properties.license]);

    return { itemProperties: this.properties };
  }
};

export function logicPassItemProperties(state: LogicPassItemPropertiesState) {
  return new LogicPassItemProperties(state).run();
}

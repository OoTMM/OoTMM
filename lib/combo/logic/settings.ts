import { Settings } from "../settings";
import { isMapCompass } from "./items";
import { World } from "./world";

export const configFromSettings = (settings: Settings) => {
  const config = new Set<string>;

  if (settings.ganonBossKey === 'removed') {
    config.add('GANON_NO_BOSS_KEY');
  }

  if (settings.mapCompassShuffle === 'removed') {
    config.add('STARTING_MAP_COMPASS');
  }

  if (settings.smallKeyShuffle === 'anywhere') {
    config.add('SMALL_KEY_SHUFFLE');
  }

  if (settings.csmc) {
    config.add('CSMC');
  }

  return config;
};

export const alterWorld = (world: World, settings: Settings, config: Set<string>) => {
  if (config.has('GANON_NO_BOSS_KEY')) {
    world.checks['OOT Ganon Castle Boss Key'].item = 'OOT_RUPEE_BLUE';
  }

  if (!config.has('SMALL_KEY_SHUFFLE')) {
    world.checks['OOT Fire Temple Ring Jail'].item = 'OOT_RUPEE_BLUE';
  }

  if (config.has('STARTING_MAP_COMPASS')) {
    for (const loc in world.checks) {
      const check = world.checks[loc];
      const item = check.item;
      if (isMapCompass(item)) {
        if (check.game === 'oot') {
          check.item = 'OOT_RUPEE_BLUE';
        } else {
          check.item = 'MM_RUPEE_BLUE';
        }
      }
    }
  }
};

import { Settings } from "../settings";
import { isMapCompass } from "./items";
import { World } from "./world";

export const configFromSettings = (settings: Settings) => {
  const config = new Set<string>;

  if (settings.ganonBossKey === 'removed') {
    config.add('GANON_NO_BOSS_KEY');
  }

  if (settings.smallKeyShuffle === 'anywhere') {
    config.add('SMALL_KEY_SHUFFLE');
  }

  if (settings.csmc) {
    config.add('CSMC');
  }

  if (settings.progressiveShieldsOot === 'progressive') {
    config.add('OOT_PROGRESSIVE_SHIELDS');
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

  if (['starting', 'removed'].includes(settings.mapCompassShuffle)) {
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

  if (config.has('OOT_PROGRESSIVE_SHIELDS')) {
    let count = 3;
    for (const loc in world.checks) {
      const check = world.checks[loc];
      let item = check.item;
      if (['OOT_SHIELD_DEKU', 'OOT_SHIELD_HYLIAN', 'OOT_SHIELD_MIRROR'].includes(item)) {
        if (count > 0) {
          count -= 1;
          item = 'OOT_SHIELD';
        } else {
          item = 'OOT_RUPEE_BLUE';
        }
        check.item = item;
      }
    }
  }
};

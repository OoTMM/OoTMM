import { Settings } from "../settings";
import { isGanonBossKey, isJunk, isMapCompass } from "./items";
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

  if (settings.progressiveSwordsOot === 'progressive') {
    config.add('OOT_PROGRESSIVE_SWORDS');
  }

  if (settings.progressiveSwordsOot === 'goron') {
    config.add('OOT_PROGRESSIVE_SWORDS_GORON');
  }

  if (settings.progressiveShieldsMm === 'progressive') {
    config.add('MM_PROGRESSIVE_SHIELDS');
  }

  if (settings.progressiveGoronLullaby === 'progressive') {
    config.add('MM_PROGRESSIVE_LULLABY');
  }

  if (settings.doorOfTime === 'open') {
    config.add('DOOR_OF_TIME_OPEN');
  }

  return config;
};

export const alterWorld = (world: World, settings: Settings, config: Set<string>) => {
  let fireTempleKeyRemoved = false;
  let mmExtraShield = false;
  let ootShields = 3;

  for (const loc in world.checks) {
    const check = world.checks[loc];
    let item = check.item;

    /* Maps/Compass */
    if (isMapCompass(item) && ['starting', 'removed'].includes(settings.mapCompassShuffle)) {
      if (check.game === 'oot') {
        item = 'OOT_RUPEE_BLUE';
      } else {
        item = 'MM_RUPEE_BLUE';
      }
    }

    /* Ganon BK */
    if (isGanonBossKey(item) && config.has('GANON_NO_BOSS_KEY')) {
      item = 'OOT_RUPEE_BLUE';
    }

    /* Fire temple key */
    if (item === 'OOT_SMALL_KEY_FIRE' && !config.has('SMALL_KEY_SHUFFLE') && !fireTempleKeyRemoved) {
      fireTempleKeyRemoved = true;
      item = 'OOT_RUPEE_BLUE';
    }

    /* OoT shields */
    if (['OOT_SHIELD_DEKU', 'OOT_SHIELD_HYLIAN', 'OOT_SHIELD_MIRROR'].includes(item) && config.has('OOT_PROGRESSIVE_SHIELDS')) {
      if (ootShields > 0) {
        ootShields -= 1;
        item = 'OOT_SHIELD';
      } else {
        item = 'OOT_RUPEE_BLUE';
      }
    }

    /* MM shields */
    if (item === 'MM_SHIELD_MIRROR' && config.has('MM_PROGRESSIVE_SHIELDS')) {
      item = 'MM_SHIELD';
    }

    if (isJunk(item) && config.has('MM_PROGRESSIVE_SHIELDS') && !mmExtraShield) {
      mmExtraShield = true;
      item = 'MM_SHIELD';
    }

    /* OoT swords */
    if (['OOT_SWORD_KOKIRI', 'OOT_SWORD_MASTER', 'OOT_SWORD_KNIFE', 'OOT_SWORD_BIGGORON'].includes(item) && config.has('OOT_PROGRESSIVE_SWORDS')) {
      item = 'OOT_SWORD';
    }

    /* OoT swords (Goron) */
    if (['OOT_SWORD_KNIFE', 'OOT_SWORD_BIGGORON'].includes(item) && config.has('OOT_PROGRESSIVE_SWORDS_GORON')) {
      item = 'OOT_SWORD_GORON';
    }

    /* MM lullaby */
    if (item === 'MM_SONG_GORON') {
      if (config.has('MM_PROGRESSIVE_LULLABY')) {
        item = 'MM_SONG_GORON_HALF';
      } else {
        item = 'MM_RUPEE_BLUE';
      }
    } else if (item === 'MM_SONG_GORON_HALF' && !config.has('MM_PROGRESSIVE_LULLABY')) {
      item = 'MM_SONG_GORON';
    }

    check.item = item;
  }
};

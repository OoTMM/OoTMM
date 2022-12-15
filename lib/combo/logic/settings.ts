import { Settings } from "../settings";
import { World } from "./world";

export const configFromSettings = (settings: Settings) => {
  const config = new Set<string>;

  if (settings.ganonBossKey === 'removed') {
    config.add('GANON_NO_BOSS_KEY');
  }

  return config;
};

export const alterWorld = (world: World, settings: Settings, config: Set<string>) => {
  if (config.has('GANON_NO_BOSS_KEY')) {
    world.checks['OOT Ganon Castle Boss Key'].item = 'OOT_RUPEE_BLUE';
  }
};

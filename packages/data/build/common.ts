import { loadYaml } from './helpers';

export const parseScenes = () => loadYaml('defs/scenes.yml');
export const parseNpcs = () => loadYaml('defs/npc.yml');
export const parseRegions = () => loadYaml('defs/regions.yml');
export const parseHints = () => loadYaml('defs/hints.yml');
export const parseEntrances = () => loadYaml('defs/entrances.yml');
export const parseGi = () => loadYaml('defs/gi.yml');
export const parseDrawGi = () => loadYaml('defs/drawgi.yml');
export const parseLinkAnimations = () => loadYaml('defs/link-animations.yml');

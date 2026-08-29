import { loadYaml } from './helpers';

export const parseScenes = () => loadYaml('defs/scenes.yml');
export const parseNpcs = () => loadYaml('defs/npc.yml');

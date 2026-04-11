import type { Game } from './defines';

export type GossipDefinition = {
  game: Game;
  location: string;
  type: 'gossip' | 'gossip-grotto' | 'gossip-moon';
  id: number;
};

import DATA from '../dist/data-gossips.json';

export const GOSSIPS = DATA as GossipDefinition[];
export const GOSSIPS_BY_LOCATION = Object.fromEntries(GOSSIPS.map(h => [h.location, h]));

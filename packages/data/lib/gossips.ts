import type { Game } from './game';
import type { GossipDefinition } from './types';

import DATA from '../dist/data-gossips.json';

export const GOSSIPS = DATA as GossipDefinition[];
export const GOSSIPS_BY_LOCATION = Object.fromEntries(GOSSIPS.map(h => [h.location, h]));

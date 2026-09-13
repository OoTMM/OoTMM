import type { GossipDefinition } from './types';

import DATA from '../dist/lib/data-gossips.json';

export const GOSSIPS = DATA as GossipDefinition[];
export const GOSSIPS_BY_LOCATION = Object.fromEntries(GOSSIPS.map(h => [h.location, h]));

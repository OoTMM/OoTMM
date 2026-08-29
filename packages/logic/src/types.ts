import type { PlayerItem } from './items';

export type Location = string & { __brand: 'Location' };
export type ItemPlacement = Map<Location, PlayerItem>;

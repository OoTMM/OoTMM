import type { PlayerItem } from '@ootmm/core';

export type Location = string & { __brand: 'Location' };
export type ItemPlacement = Map<Location, PlayerItem>;

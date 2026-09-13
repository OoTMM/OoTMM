import type { Game } from '../game';

export type GossipDefinition = {
  game: Game;
  location: string;
  type: 'gossip' | 'gossip-grotto' | 'gossip-moon';
  id: number;
};

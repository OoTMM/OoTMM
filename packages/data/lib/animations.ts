import { default as DATA } from '../dist/lib/data-link-animations.json';
import type { Game } from './game';

export type CustomAnimation = {
  name: string;
  home_game: Game;
  header_offset: number;
  frame_data_offset: number;
};

export const DATA_ANIMATIONS = DATA.animations as CustomAnimation[];

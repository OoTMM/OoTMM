import type { Game } from '@ootmm/data';
import type { Check, RoomActor } from '../types';

export type HandlerParams = {
  game: Game;
  checks: Check[];
  ra: RoomActor;
};

export type Handler = (params: HandlerParams) => void;
export type HandlerMap = { [actorId: number]: Handler };

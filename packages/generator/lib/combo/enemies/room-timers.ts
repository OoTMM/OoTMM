export type OotTimerLocation = { mq: boolean; scene: number; room: number; setup: number };
export type OotTimerRoom = OotTimerLocation & {
  actors: readonly { actorId: number; params: number }[];
};

/* Obj_Roomtimer normally waits for a temporary-clear flag. Only rooms with
 * shuffled combat and no silver-rupee objective may substitute an enemy
 * completion check. Keep uncertain layouts/setups on their vanilla path. */
export function ootCombatTimerRooms(placements: readonly OotTimerLocation[], rooms: readonly OotTimerRoom[]): number[] {
  const masks = Array<number>(101).fill(0);
  const blocked = Array<number>(101).fill(0);
  const layouts = new Map<number, Set<boolean>>();
  for (const placement of placements) {
    const scene = layouts.get(placement.scene) ?? new Set<boolean>();
    scene.add(placement.mq);
    layouts.set(placement.scene, scene);
  }
  for (const room of rooms) {
    const layout = layouts.get(room.scene);
    if (!layout || layout.size !== 1 || !layout.has(room.mq)) continue;
    if (room.scene < 0 || room.scene >= masks.length || room.room < 0 || room.room >= 32)
      throw Error('Invalid OoT room timer location');
    const bit = 1 << room.room;
    const puzzle = room.actors.some(actor => actor.actorId === 0x117 && (actor.params >>> 12) <= 1);
    if (puzzle) blocked[room.scene] |= bit;
    if (!room.actors.some(actor => actor.actorId === 0x187)) continue;
    const shuffled = placements.some(placement => placement.mq === room.mq && placement.scene === room.scene &&
      placement.room === room.room && placement.setup === room.setup);
    if (shuffled && !puzzle) masks[room.scene] |= bit;
    else blocked[room.scene] |= bit;
  }
  return masks.map((mask, scene) => (mask & ~blocked[scene]) >>> 0);
}

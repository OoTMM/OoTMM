import { enemySpawnMacroMatches } from '@ootmm/core';
import { bufReadI16BE, bufReadU16BE } from '../util/buffer';

/* The type-0 Bg_Hidan_Curtain has an 81-unit collision radius. Keep it in
 * place and start ordinary replacements 160 units from its center, putting
 * their origin 79 units outside that radius. Native Flare Dancers still
 * jump out on their own. Evaluate each chaotic choice separately so native
 * rolls stay centered. */
export function ootFlareReplacementPosition(source: Uint8Array, targetActorId: number,
  location: { scene: number; room: number }): { x: number; z: number } | undefined {
  if (source.length !== 16 || !enemySpawnMacroMatches('oot-flare-ring-offset', {
    location: { game: 'oot', ...location },
    source: { actorId: bufReadU16BE(source, 0) },
    target: { actorId: targetActorId },
  })) return undefined;
  const yaw = bufReadI16BE(source, 10) * Math.PI / 0x8000;
  return {
    x: bufReadI16BE(source, 2) + Math.round(Math.sin(yaw) * 160),
    z: bufReadI16BE(source, 6) + Math.round(Math.cos(yaw) * 160),
  };
}

import { Options } from '../options';
import { Hints } from './hints';
import { ItemPlacement } from './solve';
import { World } from './world';

const spoilerHints = (buffer: string[], hints: Hints, placement: ItemPlacement) => {
  buffer.push('Hints');
  for (const gossip in hints.gossip) {
    const h = hints.gossip[gossip];
    if (h.type === 'hero') {
      buffer.push(`  ${gossip}: Hero, ${h.region} (${h.location}: ${placement[h.location]})`);
    }
  }
  buffer.push('');
};

const spoilerRaw = (buffer: string[], placement: ItemPlacement) => {
  for (const loc in placement) {
    buffer.push(`${loc}: ${placement[loc]}`);
  }
  buffer.push('');
};

const spoilerSpheres = (buffer: string[], world: World, placement: ItemPlacement, spheres: string[][]) => {
  for (const i in spheres) {
    buffer.push(`Sphere ${i}`);
    const sphere = spheres[i];
    for (const loc of sphere) {
      buffer.push(`  ${loc}: ${placement[loc]}`);
    }
    buffer.push('');
  }
};

export const spoiler = (world: World, placement: ItemPlacement, spheres: string[][], opts: Options, hints: Hints) => {
  const buffer: string[] = [];
  buffer.push(`Seed: ${opts.seed}`);
  buffer.push('');
  spoilerHints(buffer, hints, placement);
  if (!opts.settings.noLogic) {
    spoilerSpheres(buffer, world, placement, spheres);
  }
  spoilerRaw(buffer, placement);
  return buffer.join("\n");
};

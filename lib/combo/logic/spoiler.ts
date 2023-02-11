import { Options } from '../options';
import { Settings, Trick, Tricks } from '../settings';
import { EntranceShuffleResult } from './entrance';
import { Hints } from './hints';
import { ItemPlacement } from './solve';
import { World } from './world';
import { itemName } from '../names';

const VERSION = process.env.VERSION || 'XXX';

const spoilerHeader = (buffer: string[], seed: string) => {
  buffer.push(`Seed: ${seed}`);
  buffer.push(`Version: ${VERSION}`);
  buffer.push('');
};

const spoilerSettings = (buffer: string[], settings: Settings) => {
  buffer.push('Settings');
  for (const s in settings) {
    if (s === 'startingItems' || s === 'tricks') {
      continue;
    }
    const v = (settings as any)[s];
    buffer.push(`  ${s}: ${v}`);
  }
  buffer.push('');
};

const spoilerTricks = (buffer: string[], tricks: Tricks) => {
  const tt = Object.keys(tricks).filter(t => tricks[t as Trick]);
  if (tt.length === 0) {
    return;
  }
  buffer.push('Tricks');
  for (const t of tt) {
    buffer.push(`  ${t}`);
  }
  buffer.push('');
};

const spoilerStartingItems = (buffer: string[], startingItems: {[k: string]: number}) => {
  if (Object.keys(startingItems).length === 0) {
    return;
  }

  buffer.push('Starting Items');
  for (const item in startingItems) {
    const count = startingItems[item];
    buffer.push(`  ${item}: ${count}`);
  }
  buffer.push('');
};

const spoilerEntrances = (buffer: string[], entrances: EntranceShuffleResult) => {
  if (Object.keys(entrances).length === 0) {
    return;
  }

  buffer.push('Entrances');
  for (const srcFrom in entrances.overrides) {
    const e = entrances.overrides[srcFrom];
    for (const srcTo in e) {
      const dest = e[srcTo];
      buffer.push(`  ${srcFrom}/${srcTo} -> ${dest.from}/${dest.to}`);
    }
  }
  buffer.push('');
};

const spoilerFoolish = (buffer: string[], foolish: {[k: string]: number}) => {
  buffer.push('Foolish Regions');
  for (const region in foolish) {
    const weight = foolish[region];
    buffer.push(`  ${region}: ${weight}`);
  }
  buffer.push('');
};

const spoilerHints = (buffer: string[], hints: Hints, placement: ItemPlacement) => {
  buffer.push('Hints');
  for (const gossip in hints.gossip) {
    const h = hints.gossip[gossip];
    if (h.type === 'hero') {
      buffer.push(`  ${gossip}: Hero, ${h.region} (${h.location}: ${itemName(placement[h.location])})`);
    }
    if (h.type === 'foolish') {
      buffer.push(`  ${gossip}: Foolish, ${h.region}`);
    }
    if (h.type === 'item-exact') {
      const newNames = h.items.map(itemName);
      buffer.push(`  ${gossip}: Item-Exact, ${h.check} (${newNames.join(', ')})`);
    }
    if (h.type === 'item-region') {
      buffer.push(`  ${gossip}: Item-Region, ${h.region} (${itemName(h.item)})`);
    }
  }
  buffer.push('');
};

const spoilerRaw = (buffer: string[], placement: ItemPlacement) => {
  for (const loc in placement) {
    buffer.push(`${loc}: ${itemName(placement[loc])}`);
  }
  buffer.push('');
};

const spoilerSpheres = (buffer: string[], world: World, placement: ItemPlacement, spheres: string[][]) => {
  for (const i in spheres) {
    buffer.push(`Sphere ${i}`);
    const sphere = spheres[i];
    for (const loc of sphere) {
      buffer.push(`  ${loc}: ${itemName(placement[loc])}`);
    }
    buffer.push('');
  }
};

export const spoiler = (world: World, placement: ItemPlacement, spheres: string[][], opts: Options, hints: Hints, entrances: EntranceShuffleResult) => {
  const buffer: string[] = [];
  spoilerHeader(buffer, opts.seed);
  spoilerSettings(buffer, opts.settings);
  spoilerTricks(buffer, opts.settings.tricks);
  spoilerStartingItems(buffer, opts.settings.startingItems);
  spoilerEntrances(buffer, entrances);
  spoilerFoolish(buffer, hints.foolish);
  spoilerHints(buffer, hints, placement);
  if (!opts.settings.noLogic) {
    spoilerSpheres(buffer, world, placement, spheres);
  }
  spoilerRaw(buffer, placement);
  return buffer.join("\n");
};

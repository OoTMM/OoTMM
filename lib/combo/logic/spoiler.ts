import { Options } from '../options';
import { Settings, Trick, Tricks } from '../settings';
import { EntranceShuffleResult } from './entrance';
import { Hints } from './hints';
import { ItemPlacement } from './solve';
import { World } from './world';
import { itemName } from '../names';
import { Monitor } from '../monitor';
import { Analysis } from './analysis';
import { regionName } from '../regions';
import { isShuffled } from './is-shuffled'

const VERSION = process.env.VERSION || 'XXX';

const spoilerSectionHeader = (buffer: string[]) => {
  const lineLength = 75;
  const delimiter = '=';
  let line = '';
  for (let x=0; x<lineLength; x++) {
    line += delimiter;
  }
  buffer.push(line);
}

const spoilerHeader = (buffer: string[], seed: string) => {
  buffer.push(`Seed: ${seed}`);
  buffer.push(`Version: ${VERSION}`);
  buffer.push('');
};

const spoilerSettings = (buffer: string[], settings: Settings) => {
  buffer.push('Settings');
  for (const s in settings) {
    if (s === 'startingItems' || s === 'tricks' || s === 'junkLocations') {
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

const spoilerJunkLocations = (buffer: string[], junkLocations: string[]) => {
  if (junkLocations.length === 0) {
    return;
  }

  buffer.push('Junk Locations');
  for (const location of junkLocations) {
    buffer.push(`  ${location}`);
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

const spoilerRaw = (buffer: string[], settings: Settings, world: World, placement: ItemPlacement) => {
  spoilerSectionHeader(buffer);
  buffer.push('Location List');
  const regionNames = new Set(Object.values(world.regions));
  const dungeonLocations = Object.values(world.dungeons).reduce((acc, x) => new Set([...acc, ...x]));
  regionNames.forEach(region => {
    const regionalLocations = Object.keys(world.regions)
      .filter(location => world.regions[location] === region)
      .filter(location => isShuffled(settings, world, location, dungeonLocations))
      .map(loc => `    ${loc}: ${itemName(placement[loc])}`);
    buffer.push(`  ${regionName(region)}:`);
    buffer.push(regionalLocations.join('\n'));
    buffer.push('')
    }
  )
};

const spoilerSpheres = (buffer: string[], world: World, placement: ItemPlacement, spheres: string[][]) => {
  spoilerSectionHeader(buffer);
  buffer.push('Spheres');
  for (const i in spheres) {
    buffer.push(`  Sphere ${i}`);
    const sphere = spheres[i];
    for (const loc of sphere) {
      buffer.push(`    ${loc}: ${itemName(placement[loc])}`);
    }
    buffer.push('');
  }
};

export class LogicPassSpoiler {
  constructor(
    private readonly state: {
      world: World,
      items: ItemPlacement,
      analysis: Analysis,
      opts: Options,
      hints: Hints,
      entrances: EntranceShuffleResult,
      monitor: Monitor,
    }
  ) {
  }

  run() {
    this.state.monitor.log('Logic: Spoiler');

    const buffer: string[] = [];
    spoilerHeader(buffer, this.state.opts.seed);
    spoilerSettings(buffer, this.state.opts.settings);
    spoilerTricks(buffer, this.state.opts.settings.tricks);
    spoilerStartingItems(buffer, this.state.opts.settings.startingItems);
    spoilerJunkLocations(buffer, this.state.opts.settings.junkLocations);
    spoilerEntrances(buffer, this.state.entrances);
    spoilerFoolish(buffer, this.state.hints.foolish);
    spoilerHints(buffer, this.state.hints, this.state.items);
    if (this.state.opts.settings.logic !== 'none') {
      spoilerSpheres(buffer, this.state.world, this.state.items, this.state.analysis.spheres);
    }
    spoilerRaw(buffer, this.state.opts.settings, this.state.world, this.state.items);
    const log = buffer.join("\n");
    return { log };
  };
}

import { sortBy } from 'lodash';

import { Options } from '../options';
import { Glitch, GLITCHES, Settings, Trick, TRICKS } from '../settings';
import { HintGossipFoolish, HintGossipHero, HintGossipItemExact, HintGossipItemRegion, Hints } from './hints';
import { World } from './world';
import { itemName } from '../names';
import { Monitor } from '../monitor';
import { Analysis } from './analysis';
import { regionName } from '../regions';
import { isShuffled } from './is-shuffled'
import { ItemPlacement } from './solve';
import { Location, locationData, makeLocation } from './locations';
import { Region, regionData } from './regions';
import { PlayerItem } from '../items';
import { worldState } from '.';

const VERSION = process.env.VERSION || 'XXX';

export class LogicPassSpoiler {
  private buffer: string[];
  private indentLevel: number;
  private isMulti: boolean;
  private worlds: number;

  constructor(
    private readonly state: {
      worlds: World[],
      items: ItemPlacement,
      analysis: Analysis,
      opts: Options,
      settings: Settings,
      hints: Hints,
      monitor: Monitor,
    }
  ) {
    this.buffer = [];
    this.indentLevel = 0;
    this.isMulti = this.state.settings.mode === 'multi';
    this.worlds = this.isMulti ? this.state.settings.players : 1;
  }

  private indent(str?: string) {
    if (str !== undefined) {
      this.write(str);
    }
    this.indentLevel++;
  }

  private unindent(str?: string) {
    this.indentLevel--;
    if (this.indentLevel < 0) {
      throw new Error('unindent() called too many times');
    }
    if (str !== undefined) {
      this.write(str);
    }
  }

  private write(str: string) {
    if (str === '') {
      this.buffer.push('');
      return;
    }
    this.buffer.push('  '.repeat(this.indentLevel) + str);
  }

  private writeSectionHeader() {
    this.buffer.push('='.repeat(75));
  }

  private writeHeader() {
    this.write(`Seed: ${this.state.opts.seed}`);
    this.write(`Version: ${VERSION}`);
    this.write('');
  }

  private writeSettings() {
    this.indent('Settings');
    for (const s in this.state.settings) {
      if (s === 'startingItems' || s === 'tricks' || s === 'glitches' || s === 'junkLocations' || s === 'dungeon' || s === 'specialConds' || s === 'plando' || s === 'hints') {
        continue;
      }
      if (this.state.settings.goal !== 'triforce' && (s === 'triforceGoal' || s === 'triforcePieces')) {
        continue;
      }
      const v = (this.state.settings as any)[s];
      this.write(`${s}: ${v}`);
    }
    this.unindent('');
  }

  private writeSpecialConds() {
    this.indent('Special Conditions');
    for (const s in this.state.settings.specialConds) {
      const cond = this.state.settings.specialConds[s as keyof typeof this.state.settings.specialConds];
      this.indent(`${s}:`);
      for (const key in cond) {
        this.write(`${key}: ${cond[key as keyof typeof cond]}`);
      }
      this.unindent();
    }
    this.unindent('');
  }

  private writeTricks() {
    const { tricks } = this.state.settings;
    if (tricks.length === 0) {
      return;
    }
    this.indent('Tricks');
    for (const trick of tricks) {
      this.write(`${TRICKS[trick as Trick]}`);
    }
    this.unindent('');
  }

  private writeGlitches() {
    const { glitches } = this.state.settings;
    if (glitches.length === 0) {
      return;
    }
    this.indent('Glitches');
    for (const glitch of glitches) {
      this.write(`${GLITCHES[glitch as Glitch]}`);
    }
    this.unindent('');
  }

  private writeStartingItems() {
    const { startingItems } = this.state.settings;
    if (Object.keys(startingItems).length === 0) {
      return;
    }

    this.indent('Starting Items');
    for (const item in startingItems) {
      const count = startingItems[item];
      this.write(`${itemName(item)}: ${count}`);
    }
    this.unindent('');
  }

  private writeJunkLocations() {
    const { junkLocations } = this.state.settings;
    if (junkLocations.length === 0) {
      return;
    }

    this.indent('Junk Locations');
    for (const location of junkLocations) {
      this.write(`${location}`);
    }
    this.unindent('');
  }

  private writeMQ() {
    let worlds = this.state.worlds;
    if (!this.state.settings.distinctWorlds) {
      worlds = [this.state.worlds[0]];
    }
    if (!worlds.some(world => world.mq.size > 0)) {
      return;
    }

    this.indent('MQ Dungeons');
    for (let i = 0; i < worlds.length; ++i) {
      if (worlds.length > 1) {
        this.indent(`World ${i + 1}`);
      }

      for (const d of worlds[i].mq) {
        this.write(`${d}`);
      }

      if (worlds.length > 1) {
        this.unindent('');
      }
    }
    this.unindent('');
  }

  private writeEntrances() {
    let worlds = this.state.worlds;
    if (!this.state.settings.distinctWorlds) {
      worlds = [this.state.worlds[0]];
    }
    if (!worlds.some(world => world.entranceOverrides.size > 0)) {
      return;
    }

    this.indent('Entrances');
    for (let i = 0; i < worlds.length; ++i) {
      const world = worlds[i];
      if (worlds.length > 1) {
        this.indent(`World ${i + 1}`);
      }

      for (const [src, dst] of world.entranceOverrides) {
        this.write(`${src} -> ${dst}`);
      }

      if (worlds.length > 1) {
        this.unindent('');
      }
    }
    this.unindent('');
  }

  private writeHints() {
    const globalHints = this.state.hints;
    this.indent('Hints');
    for (let worldId = 0; worldId < this.worlds; ++worldId) {
      if (this.isMulti) this.indent(`World ${worldId + 1}:`);
      const hints = globalHints[worldId];
      const gossipStones = Object.entries(hints.gossip);
      const gossipsHero = gossipStones.filter(stone => stone[1].type === 'hero').sort() as [string, HintGossipHero][];
      const gossipsFoolish = gossipStones.filter(stone => stone[1].type === 'foolish').sort() as [string, HintGossipFoolish][];
      const gossipsItemExact = gossipStones.filter(stone => stone[1].type === 'item-exact').sort() as [string, HintGossipItemExact][];
      const gossipsItemRegion = gossipStones.filter(stone => stone[1].type === 'item-region').sort() as [string, HintGossipItemRegion][];

      if (gossipsHero.length > 0) {
        this.indent('Way of the Hero:');
        for (const [stone, hint] of gossipsHero) {
          this.write(stone);
          this.write(`  ${this.regionName(hint.region)}    ${this.locationName(hint.location)} - ${this.itemName(this.state.items.get(hint.location)!)}`);
        }
        this.unindent('');
      }

      if (gossipsFoolish.length > 0) {
        this.indent('Foolish:');
        for (const [stone, hint] of gossipsFoolish) {
          this.write(stone);
          this.write(`  ${this.regionName(hint.region)}`);
        }
        this.unindent('');
      }

      if (gossipsItemExact.length > 0) {
        this.indent('Specific Hints:');
        for (const [stone, hint] of gossipsItemExact) {
          const world = this.state.worlds[hint.world];
          this.write(stone);
          this.write(`  ${world.checkHints[hint.check].join(', ')} (${hint.items.map(x => this.itemName(x)).join(', ')})`);
        }
        this.unindent('');
      }

      if (gossipsItemRegion.length > 0) {
        this.indent('Regional Hints:');
        for (const [stone, hint] of gossipsItemRegion) {
          this.write(stone);
          this.write(`  ${this.regionName(hint.region)} (${this.itemName(hint.item)})`);
        }
        this.unindent('');
      }
      this.write('');
      this.indent('Foolish Regions:');
      const foolish = sortBy([...hints.foolish.keys()], x => -hints.foolish.get(x)!);
      for (const f of foolish) {
        const weight = hints.foolish.get(f);
        this.write(`${regionName(f)}: ${weight}`);
      }
      this.unindent();
      if (this.isMulti) this.unindent('');
    }
    this.unindent('');
  }

  private locationName(location: Location) {
    const data = locationData(location);
    if (this.isMulti) {
      return `World ${data.world as number + 1} ${data.id}`;
    } else {
      return data.id;
    }
  }

  private itemName(item: PlayerItem) {
    if (this.isMulti) {
      return `Player ${item.player + 1} ${itemName(item.item.id)}`;
    } else {
      return itemName(item.item.id);
    }
  }

  private regionName(region: Region) {
    const data = regionData(region);
    if (this.isMulti) {
      return `World ${data.world + 1} ${regionName(data.id)}`;
    } else {
      return regionName(data.id);
    }
  }

  private writeRaw() {
    const { worlds, items: placement, settings } = this.state;
    const allLocsCount = worlds.map(x => x.locations.size).reduce((a, b) => a + b, 0);
    this.writeSectionHeader();
    this.indent(`Location List (${allLocsCount})`);
    for (let i = 0; i < this.state.settings.players; ++i) {
      if (this.isMulti) this.indent(`World ${i+1}`);
      const world = worlds[i];
      const regionNames = new Set(Object.values(world.regions));
      const dungeonLocations = Object.values(world.dungeons).reduce((acc, x) => new Set([...acc, ...x]));
      for (const region of regionNames) {
        const regionalLocations = Object.keys(world.regions)
          .filter(location => world.regions[location] === region)
          .filter(location => isShuffled(settings, world, location, dungeonLocations))
          .map(loc => `${loc}: ${this.itemName(placement.get(makeLocation(loc, i))!)}`);
        this.indent(`${regionName(region)} (${regionalLocations.length}):`);
        for (const loc of regionalLocations) {
          this.write(loc);
        }
        this.unindent('');
      }
      if (this.isMulti) this.unindent('');
    }
    this.unindent('');
  }

  private writeSpheres() {
    const { spheres } = this.state.analysis;
    this.writeSectionHeader();
    this.indent('Spheres');
    for (const i in spheres) {
      this.indent(`Sphere ${i}`);
      const sphere = spheres[i];
      for (const loc of sphere) {
        this.write(`${this.locationName(loc)}: ${this.itemName(this.state.items.get(loc)!)}`);
      }
      this.unindent('');
    }
    this.unindent();
  }

  run() {
    this.state.monitor.log('Logic: Spoiler');

    if (!this.state.opts.settings.generateSpoilerLog) {
      return { log: null };
    }

    this.writeHeader();
    this.writeSettings();
    this.writeSpecialConds();
    this.writeTricks();
    this.writeGlitches();
    this.writeStartingItems();
    this.writeJunkLocations();
    this.writeMQ();
    this.writeEntrances();
    this.writeHints();
    if (this.state.opts.settings.logic !== 'none') {
      this.writeSpheres();
    }
    this.writeRaw();
    const log = this.buffer.join("\n");
    return { log };
  };
}

import { sortBy } from 'lodash';

import { Options } from '../options';
import { SETTINGS, Settings, TrickKey, TRICKS } from '../settings';
import { HintGossipFoolish, HintGossipPath, HintGossipItemExact, HintGossipItemRegion, Hints, HINTS_PATHS } from './hints';
import { World, WORLD_FLAGS } from './world';
import { itemName } from '../names';
import { Monitor } from '../monitor';
import { Analysis } from './analysis';
import { regionName } from '../regions';
import { isShuffled } from './is-shuffled'
import { ItemPlacement } from './solve';
import { Location, locationData, makeLocation } from './locations';
import { Region, regionData } from './regions';
import { PlayerItem, PlayerItems } from '../items';
import { exportSettings } from '../settings/string';
import { ENTRANCES } from '@ootmm/data';
import { LogWriter } from '../util/log-writer';

const VERSION = process.env.VERSION || 'XXX';

export class LogicPassSpoiler {
  private writer: LogWriter;
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
      startingItems: PlayerItems;
    }
  ) {
    this.writer = new LogWriter();
    this.isMulti = this.state.settings.mode === 'multi';
    this.worlds = this.isMulti ? this.state.settings.players : 1;
  }

  private writeHeader() {
    this.writer.write(`Seed: ${this.state.opts.seed}`);
    this.writer.write(`Version: ${VERSION}`);
    this.writer.write(`SettingsString: ${exportSettings(this.state.settings)}`);
    this.writer.write('');
  }

  private writeSettings() {
    this.writer.indent('Settings');
    for (const s in this.state.settings) {
      if (s === 'startingItems' || s === 'tricks' || s === 'junkLocations' || s === 'dungeon' || s === 'specialConds' || s === 'plando' || s === 'hints') {
        continue;
      }
      if (this.state.settings.goal !== 'triforce' && (s === 'triforceGoal' || s === 'triforcePieces')) {
        continue;
      }
      if (WORLD_FLAGS.includes(s as any)) {
        continue;
      }
      const v = (this.state.settings as any)[s];
      const data = SETTINGS.find(x => x.key === s)! as any;
      if (data && data.cond && !data.cond(this.state.settings)) {
        continue;
      }
      this.writer.write(`${s}: ${v}`);
    }
    this.writer.unindent('');
  }

  private writeSpecialConds() {
    this.writer.indent('Special Conditions');
    for (const s in this.state.settings.specialConds) {
      const cond = this.state.settings.specialConds[s as keyof typeof this.state.settings.specialConds];
      this.writer.indent(`${s}:`);
      for (const key in cond) {
        this.writer.write(`${key}: ${cond[key as keyof typeof cond]}`);
      }
      this.writer.unindent();
    }
    this.writer.unindent('');
  }

  private writeTricks() {
    let { tricks } = this.state.settings;
    tricks = tricks.filter(x => !TRICKS[x].glitch);
    if (tricks.length === 0) {
      return;
    }
    this.writer.indent('Tricks');
    for (const trick of tricks) {
      this.writer.write(`${TRICKS[trick as TrickKey].name}`);
    }
    this.writer.unindent('');
  }

  private writeGlitches() {
    const { tricks } = this.state.settings;
    const glitches = tricks.filter(x => TRICKS[x].glitch);
    if (glitches.length === 0) {
      return;
    }
    this.writer.indent('Glitches');
    for (const glitch of glitches) {
      this.writer.write(`${TRICKS[glitch].name}`);
    }
    this.writer.unindent('');
  }

  private writeStartingItems() {
    const { startingItems } = this.state;
    if (startingItems.size === 0) {
      return;
    }

    this.writer.indent('Starting Items');
    for (let playerId = 0; playerId < this.state.settings.players; ++playerId) {
      if (this.isMulti) this.writer.indent(`Player ${playerId + 1}`);
      const items = new Map(Array.from(startingItems.entries()).filter(([x, _]) => x.player === playerId));
      for (const [item, count] of items) {
        this.writer.write(`${itemName(item.item.id)}: ${count}`);
      }
      if (this.isMulti) this.writer.unindent('');
    }
    this.writer.unindent('');
  }

  private writeJunkLocations() {
    const { junkLocations } = this.state.settings;
    if (junkLocations.length === 0) {
      return;
    }

    this.writer.indent('Junk Locations');
    for (const location of junkLocations) {
      this.writer.write(`${location}`);
    }
    this.writer.unindent('');
  }

  private writeMQ() {
    let worlds = this.state.worlds;
    if (!this.state.settings.distinctWorlds) {
      worlds = [this.state.worlds[0]];
    }
    if (!worlds.some(world => world.mq.size > 0)) {
      return;
    }

    this.writer.indent('MQ Dungeons');
    for (let i = 0; i < worlds.length; ++i) {
      if (worlds.length > 1) {
        this.writer.indent(`World ${i + 1}`);
      }

      for (const d of worlds[i].mq) {
        this.writer.write(`${d}`);
      }

      if (worlds.length > 1) {
        this.writer.unindent('');
      }
    }
    this.writer.unindent('');
  }

  private writeWorldFlags() {
    let worlds = this.state.worlds;
    if (!this.state.settings.distinctWorlds) {
      worlds = [this.state.worlds[0]];
    }

    this.writer.indent('World Flags');
    for (let i = 0; i < worlds.length; ++i) {
      const world = worlds[i];
      if (worlds.length > 1) {
        this.writer.indent(`World ${i + 1}`);
      }

      for (const s of WORLD_FLAGS) {
        const v = world.resolvedFlags[s as keyof typeof world.resolvedFlags];
        const setting_name = SETTINGS.find(x => x.key === s)!.name;
        if (v.value === 'specific') {
          this.writer.indent(`${setting_name}:`);
          const setting = SETTINGS.find(x => x.key === v.setting)!;
          const values = ((setting as any).values as any[]).filter(x => v.has(x.value));
          for (const value of values) {
            this.writer.write(`- ${value.name}`);
          }
          this.writer.unindent('');
        } else {
          this.writer.write(`${setting_name}: ${v.value}`);
        }
      }

      if (worlds.length > 1) {
        this.writer.unindent('');
      }
    }
    this.writer.unindent('');
  }

  private writePreCompleted() {
    let worlds = this.state.worlds;
    if (!this.state.settings.distinctWorlds) {
      worlds = [this.state.worlds[0]];
    }
    if (!worlds.some(world => world.preCompleted.size > 0)) {
      return;
    }

    this.writer.indent('Pre-Completed Dungeons');
    for (let i = 0; i < worlds.length; ++i) {
      if (worlds.length > 1) {
        this.writer.indent(`World ${i + 1}`);
      }

      for (const d of worlds[i].preCompleted) {
        this.writer.write(`${d}`);
      }

      if (worlds.length > 1) {
        this.writer.unindent('');
      }
    }
    this.writer.unindent('');
  }

  private writeEntrances() {
    let worlds = this.state.worlds;
    if (!this.state.settings.distinctWorlds) {
      worlds = [this.state.worlds[0]];
    }
    if (!worlds.some(world => world.entranceOverrides.size > 0)) {
      return;
    }

    this.writer.indent('Entrances');
    for (let i = 0; i < worlds.length; ++i) {
      const world = worlds[i];
      if (worlds.length > 1) {
        this.writer.indent(`World ${i + 1}`);
      }

      const niceEntrances = new Array();
      for (const [src, dst] of world.entranceOverrides) {
        const srcEntry = ENTRANCES[src as keyof typeof ENTRANCES];
        const dstEntry = ENTRANCES[dst as keyof typeof ENTRANCES];
        niceEntrances.push(`${srcEntry.from} to ${srcEntry.to} (${src}) -> ${dstEntry.to} from ${dstEntry.from} (${dst})`);
      }
      for (const niceE of niceEntrances.sort()) {
        this.writer.write(`${niceE}`);
      }

      if (worlds.length > 1) {
        this.writer.unindent('');
      }
    }
    this.writer.unindent('');
  }

  private writePlando() {
    const { plando } = this.state.settings;
    if (Object.keys(plando.locations).length >= 1) {
      this.writer.indent('Plando');
      for (let loc in plando.locations) {
        let item = plando.locations[loc];
        if (item)
          this.writer.write(`${loc}: ${itemName(item)}`)
      }
      this.writer.unindent('');
    }
  }

  private writeHints() {
    const globalHints = this.state.hints;
    this.writer.indent('Hints');
    for (let worldId = 0; worldId < this.worlds; ++worldId) {
      if (this.isMulti) this.writer.indent(`World ${worldId + 1}:`);
      const hints = globalHints[worldId];
      const gossipStones = Object.entries(hints.gossip);
      const gossipsPaths = gossipStones.filter(stone => stone[1].type === 'path').sort() as [string, HintGossipPath][];
      const gossipsFoolish = gossipStones.filter(stone => stone[1].type === 'foolish').sort() as [string, HintGossipFoolish][];
      const gossipsItemExact = gossipStones.filter(stone => stone[1].type === 'item-exact').sort() as [string, HintGossipItemExact][];
      const gossipsItemRegion = gossipStones.filter(stone => stone[1].type === 'item-region').sort() as [string, HintGossipItemRegion][];

      if (gossipsPaths.length > 0) {
        for (const type of Object.keys(HINTS_PATHS)) {
          const hints = gossipsPaths.filter(x => x[1].path === type);
          if (hints.length === 0) continue;
          const name = (HINTS_PATHS as any)[type].name;
          this.writer.indent(`${name}:`);
          for (const [stone, hint] of hints) {
            this.writer.write(stone);
            this.writer.write(`  ${this.regionName(hint.region)}    ${this.locationName(hint.location)} - ${this.itemName(this.state.items.get(hint.location)!)}`);
          }
          this.writer.unindent('');
        }
      }

      if (gossipsFoolish.length > 0) {
        this.writer.indent('Foolish:');
        for (const [stone, hint] of gossipsFoolish) {
          this.writer.write(stone);
          this.writer.write(`  ${this.regionName(hint.region)}`);
        }
        this.writer.unindent('');
      }

      if (gossipsItemExact.length > 0) {
        this.writer.indent('Specific Hints:');
        for (const [stone, hint] of gossipsItemExact) {
          const world = this.state.worlds[hint.world];
          this.writer.write(stone);
          this.writer.write(`  ${world.checkHints[hint.check].join(', ')} (${hint.items.map(x => this.itemName(x)).join(', ')})`);
        }
        this.writer.unindent('');
      }

      if (gossipsItemRegion.length > 0) {
        this.writer.indent('Regional Hints:');
        for (const [stone, hint] of gossipsItemRegion) {
          this.writer.write(stone);
          this.writer.write(`  ${this.regionName(hint.region)} (${this.itemName(hint.item)})`);
        }
        this.writer.unindent('');
      }
      this.writer.write('');
      this.writer.indent('Foolish Regions:');
      const foolish = sortBy([...hints.foolish.keys()], x => -hints.foolish.get(x)!);
      for (const f of foolish) {
        const weight = hints.foolish.get(f);
        this.writer.write(`${regionName(f)}: ${weight}`);
      }
      this.writer.unindent();
      if (this.isMulti) this.writer.unindent('');
    }
    this.writer.unindent('');
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
    this.writer.writeSectionHeader();
    this.writer.indent(`Location List (${allLocsCount})`);
    for (let i = 0; i < this.state.settings.players; ++i) {
      if (this.isMulti) this.writer.indent(`World ${i+1} (${worlds[i].locations.size})`);
      const world = worlds[i];
      const regionNames = new Set(Object.values(world.regions));
      const dungeonLocations = Object.values(world.dungeons).reduce((acc, x) => new Set([...acc, ...x]));
      for (const region of regionNames) {
        const regionalLocations = Object.keys(world.regions)
          .filter(location => world.regions[location] === region)
          .filter(location => isShuffled(settings, world, location, dungeonLocations))
          .map(loc => `${loc}: ${this.itemName(placement.get(makeLocation(loc, i))!)}`);
        this.writer.indent(`${regionName(region)} (${regionalLocations.length}):`);
        for (const loc of regionalLocations) {
          this.writer.write(loc);
        }
        this.writer.unindent('');
      }
      if (this.isMulti) this.writer.unindent('');
    }
    this.writer.unindent('');
  }

  private writeSpheres() {
    const { spheres } = this.state.analysis;
    this.writer.writeSectionHeader();
    this.writer.indent('Spheres');
    for (const i in spheres) {
      this.writer.indent(`Sphere ${i}`);
      const sphere = spheres[i].map(x => `${this.locationName(x)}: ${this.itemName(this.state.items.get(x)!)}`).sort();
      for (const loc of sphere) {
        this.writer.write(loc);
      }
      this.writer.unindent('');
    }
    this.writer.unindent();
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
    this.writeWorldFlags();
    this.writeMQ();
    this.writePreCompleted();
    this.writeEntrances();
    this.writeHints();
    this.writePlando();
    if (this.state.opts.settings.logic !== 'none') {
      this.writeSpheres();
    }
    this.writeRaw();

    return { log: this.writer.emit() };
  };
}

import { Options } from '../options';
import { Settings, Trick, TRICKS } from '../settings';
import { EntranceShuffleResult } from './entrance';
import { HintGossipFoolish, HintGossipHero, HintGossipItemExact, HintGossipItemRegion, Hints } from './hints';
import { ItemPlacement } from './solve';
import { World } from './world';
import { itemName } from '../names';
import { Monitor } from '../monitor';
import { Analysis } from './analysis';
import { regionName } from '../regions';
import { isShuffled } from './is-shuffled'

const VERSION = process.env.VERSION || 'XXX';

export class LogicPassSpoiler {
  private buffer: string[] = [];

  constructor(
    private readonly state: {
      world: World,
      items: ItemPlacement,
      analysis: Analysis,
      opts: Options,
      settings: Settings,
      hints: Hints,
      entrances: EntranceShuffleResult,
      monitor: Monitor,
      mq: Set<string>,
    }
  ) {
  }

  private writeSectionHeader() {
    this.buffer.push('='.repeat(75));
  }

  private writeHeader() {
    this.buffer.push(`Seed: ${this.state.opts.seed}`);
    this.buffer.push(`Version: ${VERSION}`);
    this.buffer.push('');
  }

  private writeSettings() {
    this.buffer.push('Settings');
    for (const s in this.state.settings) {
      if (s === 'startingItems' || s === 'tricks' || s === 'junkLocations' || s === 'dungeon' || s === 'specialConds' || s === 'plando') {
        continue;
      }
      const v = (this.state.settings as any)[s];
      this.buffer.push(`  ${s}: ${v}`);
    }
    this.buffer.push('');
  }

  private writeSpecialConds() {
    this.buffer.push('Special Conditions');
    for (const s in this.state.settings.specialConds) {
      const cond = this.state.settings.specialConds[s as keyof typeof this.state.settings.specialConds];
      this.buffer.push(`  ${s}:`);
      for (const key in cond) {
        this.buffer.push(`    ${key}: ${cond[key as keyof typeof cond]}`);
      }
    }
    this.buffer.push('');
  }

  private writeTricks() {
    const { tricks } = this.state.settings;
    if (tricks.length === 0) {
      return;
    }
    this.buffer.push('Tricks');
    for (const trick of tricks) {
      this.buffer.push(`  ${TRICKS[trick as Trick]}`);
    }
    this.buffer.push('');
  }

  private writeStartingItems() {
    const { startingItems } = this.state.settings;
    if (Object.keys(startingItems).length === 0) {
      return;
    }

    this.buffer.push('Starting Items');
    for (const item in startingItems) {
      const count = startingItems[item];
      this.buffer.push(`  ${itemName(item)}: ${count}`);
    }
    this.buffer.push('');
  }

  private writeJunkLocations() {
    const { junkLocations } = this.state.settings;
    if (junkLocations.length === 0) {
      return;
    }

    this.buffer.push('Junk Locations');
    for (const location of junkLocations) {
      this.buffer.push(`  ${location}`);
    }
    this.buffer.push('');
  }

  private writeMQ() {
    const { mq } = this.state;
    if (mq.size === 0) {
      return;
    }

    this.buffer.push('MQ Dungeons');
    for (const d of mq) {
      this.buffer.push(`  ${d}`);
    }
    this.buffer.push('');
  }

  private writeEntrances() {
    const { entrances } = this.state;
    if (Object.keys(entrances.overrides).length === 0) {
      return;
    }

    this.buffer.push('Entrances');
    for (const srcFrom in entrances.overrides) {
      const e = entrances.overrides[srcFrom];
      for (const srcTo in e) {
        const dest = e[srcTo];
        this.buffer.push(`  ${srcFrom}/${srcTo} -> ${dest.from}/${dest.to}`);
      }
    }
    this.buffer.push('');
  }

  private writeFoolish() {
    const { foolish } = this.state.hints;
    this.buffer.push('Foolish Regions');
    for (const region in foolish) {
      const weight = foolish[region];
      this.buffer.push(`  ${regionName(region)}: ${weight}`);
    }
    this.buffer.push('');
  };

  private writeHints() {
    const { hints } = this.state;
    this.buffer.push('Hints');
    const gossipStones = Object.entries(hints.gossip)
    let sortedHints = {
      hero: <[string, HintGossipHero][]><unknown>gossipStones.filter(stone => stone[1].type === 'hero').sort(),
      foolish: <[string, HintGossipFoolish][]><unknown>gossipStones.filter(stone => stone[1].type === 'foolish').sort(),
      exact: <[string, HintGossipItemExact][]><unknown>gossipStones.filter(stone => stone[1].type === 'item-exact').sort(),
      region: <[string, HintGossipItemRegion][]><unknown>gossipStones.filter(stone => stone[1].type === 'item-region').sort()
    }
    let type: keyof typeof sortedHints
    for (type in sortedHints) {
      sortedHints[type].forEach(gossipStone => {
        const stone = gossipStone[0]
        const hint = gossipStone[1]
        if (hint.type === 'hero') {
          if(sortedHints.hero[0][0] === stone)this.buffer.push('  Way of the Hero:')
          this.buffer.push(`    ${stone}\n      ${regionName(hint.region)}    ${hint.location} - ${itemName(this.state.items[hint.location])}`)
        }
        if (hint.type === 'foolish') {
          if(sortedHints.foolish[0][0] === stone) this.buffer.push('\n  Foolish:')
          this.buffer.push(`    ${stone}\n      ${regionName(hint.region)}`)
        }
        if (hint.type === 'item-exact') {
          if(sortedHints.exact[0][0] === stone) this.buffer.push('\n  Specific Hints:')
          this.buffer.push(`    ${stone}\n      ${this.state.world.checkHints[hint.check].join(', ')} (${hint.items.map(itemName).join(', ')})`)
        }
        if (hint.type === 'item-region') {
          if(sortedHints.region[0][0] === stone) this.buffer.push('\n  Regional Hints:')
          this.buffer.push(`    ${stone}\n      ${regionName(hint.region)} (${itemName(hint.item)})`)
        }
      })
    }
    this.buffer.push('');
  }

  private writeRaw() {
    const { world, items: placement, settings } = this.state;
    this.writeSectionHeader();
    this.buffer.push('Location List');
    const regionNames = new Set(Object.values(world.regions));
    const dungeonLocations = Object.values(world.dungeons).reduce((acc, x) => new Set([...acc, ...x]));
    regionNames.forEach(region => {
      const regionalLocations = Object.keys(world.regions)
        .filter(location => world.regions[location] === region)
        .filter(location => isShuffled(settings, world, location, dungeonLocations))
        .map(loc => `    ${loc}: ${itemName(placement[loc])}`);
      this.buffer.push(`  ${regionName(region)}:`);
      this.buffer.push(regionalLocations.join('\n'));
      this.buffer.push('')
      }
    )
  }

  private writeSpheres() {
    const { spheres } = this.state.analysis;
    this.writeSectionHeader();
    this.buffer.push('Spheres');
    for (const i in spheres) {
      this.buffer.push(`  Sphere ${i}`);
      const sphere = spheres[i];
      for (const loc of sphere) {
        this.buffer.push(`    ${loc}: ${itemName(this.state.items[loc])}`);
      }
      this.buffer.push('');
    }
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
    this.writeStartingItems();
    this.writeJunkLocations();
    this.writeMQ();
    this.writeEntrances();
    this.writeFoolish();
    this.writeHints();
    if (this.state.opts.settings.logic !== 'none') {
      this.writeSpheres();
    }
    this.writeRaw();
    const log = this.buffer.join("\n");
    return { log };
  };
}

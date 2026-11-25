import type { Options, TrickKey, Region, SongEventSongs } from '@ootmm/core';
import type { PlayerItem, LogicResult, HintGossipFoolish, HintGossipPath, HintGossipItemExact, HintGossipItemRegion, AnalysisPath, SphereEntryEvent, Location } from '@ootmm/logic';

import { sortBy } from 'lodash-es';
import { ENTRANCES, hintLocations, SCENES, ENEMY_LOGIC_MM_ROOMS } from '@ootmm/data';
import { SETTINGS, TRICKS, MM_GENERIC_GROTTOS, exportSettings, regionData, SONG_EVENT_LOCATIONS_OOT, SONG_EVENT_LOCATIONS_MM, SONG_EVENT_SONGS, enemyDefinition, ENEMIES, ENEMY_GOLD_SOURCES, OOT_DESERT_LEEVER_SLOTS, OOT_FIELD_STALCHILD_SLOTS, OOT_FOREST_BOW_SLOTS, OOT_BURIAL_ENEMY_SLOTS, OOT_ENCOUNTER_MEMORY_ROOMS } from '@ootmm/core';
import { DUNGEONS_BY_KEY, PATH_EVENT_DATA, BOSS_METADATA_BY_DUNGEON, ANALYSIS_EVENTS, WORLD_FLAGS, locationData, makeLocation } from '@ootmm/logic';

import { itemName } from './names';
import { regionName } from './regions';
import { LogWriter } from './util/log-writer';

const VERSION = process.env.VERSION || 'XXX';

const SONG_EVENT_SONG_NAMES: Record<SongEventSongs, string> = {
  ZELDAS_LULLABY: 'Zelda\'s Lullaby',
  EPONAS: 'Epona\'s Song',
  SARIAS: 'Saria\'s Song',
  SUNS: 'Sun\'s Song',
  TIME: 'Song of Time',
  STORMS: 'Song of Storms',
  MINUET: 'Minuet of Forest',
  BOLERO: 'Bolero of Fire',
  SERENADE: 'Serenade of Water',
  REQUIEM: 'Requiem of Spirit',
  NOCTURNE: 'Nocturne of Shadow',
  PRELUDE: 'Prelude of Light',
  HEALING: 'Song of Healing',
  SOARING: 'Song of Soaring',
  SONATA: 'Sonata of Awakening',
  GORON_LULLABY: 'Goron Lullaby',
  GORON_LULLABY_INTRO: 'Goron Lullaby Intro',
  NEW_WAVE: 'New Wave Bossa Nova',
  ELEGY: 'Elegy of Emptiness',
  OATH: 'Oath to Order',
};

class SpoilerWriter {
  private writer: LogWriter;
  private isMulti: boolean;
  private worlds: number;

  constructor(
    private readonly logic: LogicResult,
    private readonly opts: Options,
  ) {
    this.writer = new LogWriter();
    this.isMulti = this.opts.settings.mode === 'multi';
    this.worlds = this.isMulti ? this.opts.settings.players : 1;
  }

  private getMaxKeyLength(map: [string, any][]): number {
    let maxLength = 0;
    for (const key of map) {
      maxLength = Math.max(maxLength, key[0].length);
    }
    return maxLength+1;
  }

  private getImportanceSuffix(importance: number): string {
    if (this.opts.settings.logic === 'none' || !this.opts.settings.hintImportance) return ""; // No need for importance in no-logic
    switch (importance) {
      case  0: return "(unreachable)";
      case  1: return "(not required)";
      case  2: return "(sometimes required)";
      case  3: return "(required)";
      default: return "";
    }
  }

  private plandoSongEventSongName(song: SongEventSongs | 'random') {
    return song === 'random'
        ? 'Random'
        : this.songEventSongName(song);
  }

  private writeHeader() {
    this.writer.write(`Seed: ${this.opts.seed}`);
    this.writer.write(`Version: ${VERSION}`);
    this.writer.write(`SettingsString: ${exportSettings(this.opts.settings)}`);
    this.writer.write('');
  }

  private writeSettings() {
    this.writer.indent('Settings');
    for (const s in this.opts.settings) {
      if (s === 'startingItems' || s === 'tricks' || s === 'junkLocations' || s === 'specialConds' || s === 'plando' || s === 'hints') {
        continue;
      }
      if (this.opts.settings.goal !== 'triforce' && (s === 'triforceGoal' || s === 'triforcePieces')) {
        continue;
      }
      if (WORLD_FLAGS.includes(s as any)) {
        continue;
      }
      const v = (this.opts.settings as any)[s];
      const data = SETTINGS.find(x => x.key === s)! as any;
      if (data && data.cond && !data.cond(this.opts.settings)) {
        continue;
      }
      this.writer.write(`${s}: ${v}`);
    }
    this.writer.unindent('');
  }

  private writeSpecialConds() {
    this.writer.indent('Special Conditions');
    for (const s in this.opts.settings.specialConds) {
      const cond = this.opts.settings.specialConds[s as keyof typeof this.opts.settings.specialConds];
      this.writer.indent(`${s}:`);
      for (const key in cond) {
        this.writer.write(`${key}: ${cond[key as keyof typeof cond]}`);
      }
      this.writer.unindent();
    }
    this.writer.unindent('');
  }

  private writeTricks() {
    let { tricks } = this.opts.settings;
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
    const { tricks } = this.opts.settings;
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
    const { startingItems } = this.logic;
    if (startingItems.size === 0) {
      return;
    }

    this.writer.indent('Starting Items');
    for (let playerId = 0; playerId < this.opts.settings.players; ++playerId) {
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
    const { junkLocations } = this.opts.settings;
    if (junkLocations.length === 0) {
      return;
    }

    this.writer.indent('Junk Locations');
    for (const location of junkLocations) {
      this.writer.write(`${location}`);
    }
    this.writer.unindent('');
  }

  private writeWorldFlags() {
    let worlds = this.logic.worlds;

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
    let worlds = this.logic.worlds;
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
    let worlds = this.logic.worlds;
    if (!worlds.some(world => world.entranceOverrides.size > 0)) {
      return;
    }

    this.writer.indent('Entrances');
    for (let i = 0; i < worlds.length; ++i) {
      const world = worlds[i];
      if (worlds.length > 1) {
        this.writer.indent(`World ${i + 1}`);
      }

      const niceEntrances: {[key: string]: string} = {};
      let longestEntranceEntry = 0;
      for (const [src, dst] of world.entranceOverrides) {
        const srcEntry = ENTRANCES[src as keyof typeof ENTRANCES];
        const dstEntry = ENTRANCES[dst as keyof typeof ENTRANCES];
        let key = `${srcEntry.from} to ${srcEntry.to} (${src})`;
        let value = `${dstEntry.to} from ${dstEntry.from} (${dst})`;
        niceEntrances[key] = value;
        longestEntranceEntry = Math.max(longestEntranceEntry, key.length);
      }
      Object.keys(niceEntrances).sort().forEach(key => {
        let value = niceEntrances[key];
        this.writer.write(`${key.padEnd(longestEntranceEntry)} -> ${value}`);
      });

      if (worlds.length > 1) {
        this.writer.unindent('');
      }
    }
    this.writer.unindent('');
  }

  private writePlando() {
    const plandoSongEventsOot = this.opts.settings.plando.songEvents?.oot || {};
    const plandoSongEventsMm = this.opts.settings.plando.songEvents?.mm || {};
    const hasPlandoLocations = this.logic.plandoLocations.size >= 1;
    const hasPlandoSongEvents =
        Object.values(plandoSongEventsOot).some(Boolean) ||
        Object.values(plandoSongEventsMm).some(Boolean);

    if (hasPlandoLocations || hasPlandoSongEvents) {
      this.writer.indent('Plando');
      for (let i = 0; i < this.logic.worlds.length; ++i) {
        const world = this.logic.worlds[i];
        if (this.logic.worlds.length > 1) {
          this.writer.indent(`World ${i + 1}`);
        }
        const plandoLocations = Array.from(this.logic.plandoLocations.entries())
            .filter(([loc]) => locationData(loc).world === i);
        if (plandoLocations.length > 0) {
          this.writer.indent('Locations');

          for (const [loc, pi] of plandoLocations) {
            const locD = locationData(loc);
            this.writer.write(`${locD.id}: ${itemName(pi.item.id)}`);
          }
          this.writer.unindent('');
        }
        if (this.opts.settings.songEventsShuffleOot && Object.values(plandoSongEventsOot).some(Boolean)) {
          this.writer.indent('Song Events - Ocarina of Time');
          this.writePlandoSongEventGame(SONG_EVENT_LOCATIONS_OOT, world.songEventsOot, plandoSongEventsOot);
          this.writer.unindent('');
        }

        if (this.opts.settings.songEventsShuffleMm && Object.values(plandoSongEventsMm).some(Boolean)) {
          this.writer.indent('Song Events - Majora\'s Mask');
          this.writePlandoSongEventGame(SONG_EVENT_LOCATIONS_MM, world.songEventsMm, plandoSongEventsMm);
          this.writer.unindent('');
        }
        if (this.logic.worlds.length > 1) {
          this.writer.unindent('');
        }
      }
      this.writer.unindent('');
    }
  }

  private writeSongEvents() {
    let worlds = this.logic.worlds;

    const shouldWriteOot = this.opts.settings.songEventsShuffleOot;
    const shouldWriteMm = this.opts.settings.songEventsShuffleMm;

    if (!shouldWriteOot && !shouldWriteMm) {
      return;
    }

    this.writer.indent('Song Events');

    for (let i = 0; i < worlds.length; ++i) {
      const world = worlds[i];

      if (worlds.length > 1) {
        this.writer.indent(`World ${i + 1}`);
      }

      if (shouldWriteOot) {
        this.writer.indent('Ocarina of Time');
        this.writeSongEventGame(SONG_EVENT_LOCATIONS_OOT, world.songEventsOot);
        this.writer.unindent('');
      }

      if (shouldWriteMm) {
        this.writer.indent('Majora\'s Mask');
        this.writeSongEventGame(SONG_EVENT_LOCATIONS_MM, world.songEventsMm);
        this.writer.unindent('');
      }

      if (worlds.length > 1) {
        this.writer.unindent('');
      }
    }

    this.writer.unindent('');
  }

  private writeSongEventGame(locations: readonly string[], resolvedSongs: readonly number[]) {
    const entries = locations
      .map((event, index) => ({ event, song: resolvedSongs[index] }))
      .filter(({ song }) => song !== undefined);

    if (entries.length === 0) {
      return;
    }

    const longestEventName = Math.max(...entries.map(({ event }) => event.length));

    for (const { event, song } of entries) {
      this.writer.write(`${event.padEnd(longestEventName + 1)}: ${this.songEventSongName(SONG_EVENT_SONGS[song])}`);
    }
  }

  private writePlandoSongEventGame(
      locations: readonly string[],
      resolvedSongs: readonly number[],
      plandoSongs: Record<string, { song: SongEventSongs | 'random'; group?: string } | null | undefined>,
  ) {
    const entries = Object.entries(plandoSongs)
        .flatMap(([event, plando]) => {
          const resolvedSong = resolvedSongs[locations.indexOf(event)];
          return plando && resolvedSong !== undefined
              ? [{ event, plando, resolvedSong }]
              : [];
        });
    if (!entries.length) return;
    const longestEventName = Math.max(...entries.map(({ event }) => event.length));
    const longestSongName = Math.max(
        ...entries.map(({ plando }) => this.plandoSongEventSongName(plando.song).length),
    );
    for (const { event, plando, resolvedSong } of entries) {
      const songName = this.plandoSongEventSongName(plando.song);
      const group = plando.group ? ` (${plando.group})` : '';
      const eventName = `${event.padEnd(longestEventName + 1)}: `;
      this.writer.write(
          plando.song === 'random'
              ? `${eventName}${songName} - ${this.songEventSongName(SONG_EVENT_SONGS[resolvedSong])}${group}`
              : `${eventName}${songName}${group}`,
      );
    }
  }

  private songEventSongName(song: SongEventSongs) {
    return SONG_EVENT_SONG_NAMES[song] ?? `Unknown Song (${song})`;
  }

  private pathName(path: AnalysisPath): string {
    switch (path.type) {
    case 'woth': return 'Way of the Hero';
    case 'dungeon': return `Path to ${DUNGEONS_BY_KEY.get(path.dungeon)!.name}`;
    case 'triforce': return `Path to ${path.triforce}`;
    case 'boss': return `Path to ${BOSS_METADATA_BY_DUNGEON.get(path.boss)!.name}`;
    case 'end-boss': return `Path to ${path.boss}`;
    case 'event': return `Path to ${PATH_EVENT_DATA.find(x => x.key === path.event)!.name}`;
    }
  }

  private writeHints() {
    const globalHints = this.logic.hints;
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
        const paths = new Set(gossipsPaths.map(x => x[1].path));
        for (const path of paths) {
          const hints = gossipsPaths.filter(x => x[1].path === path);
          const name = this.pathName(path);
          this.writer.indent(`${name}:`);
          const longestRegionNameLength = [...hints].reduce( function (a, b) { return regionName(a[1].location).length > regionName(b[1].location).length ? a : b; } )[1].location.length;
          const longestGossipNameLength = this.getMaxKeyLength(hints);
          for (const [stone, hint] of hints) {
            this.writer.write(`${stone.padEnd(longestGossipNameLength)} ${this.locationName(hint.location).padEnd(longestRegionNameLength)} ${this.itemName(this.logic.items.get(hint.location)!)}`);
          }
          this.writer.unindent('');
        }
      }

      if (gossipsFoolish.length > 0) {
        this.writer.indent('Foolish:');
        const longestGossipNameLength = this.getMaxKeyLength(gossipsFoolish);
        for (const [stone, hint] of gossipsFoolish) {
          this.writer.write(`${stone.padEnd(longestGossipNameLength)} ${this.regionName(hint.region)}`);
        }
        this.writer.unindent('');
      }

      if (gossipsItemExact.length > 0) {
        this.writer.indent('Specific Hints:');
        let longestLocationNameLength = 0;
        for (const [stone, hint] of gossipsItemExact) {
          const world = this.logic.worlds[hint.world];
          const locations = hintLocations(hint.check).filter(x => world.locations.has(x));
          const longestLocationName = Math.max(...locations.map(x => x.length));
          longestLocationNameLength = Math.max(longestLocationNameLength, longestLocationName);
        }
        for (const [stone, hint] of gossipsItemExact) {
          const world = this.logic.worlds[hint.world];
          const longestGossipNameLength = this.getMaxKeyLength(gossipsItemExact);
          const locations = hintLocations(hint.check).filter(x => world.locations.has(x));
          for(let i = 0; i < hint.items.length; i++) {
            this.writer.write(`${(i ? " ": stone).padEnd(longestGossipNameLength)} ${locations[i].padEnd(longestLocationNameLength + 1)} ${this.itemName(hint.items[i])} ${this.getImportanceSuffix(hint.importances[i])}`);
          }
        }
        this.writer.unindent('');
      }

      if (gossipsItemRegion.length > 0) {
        this.writer.indent('Regional Hints:');
        const longestRegionNameLength = this.regionName([...gossipsItemRegion.values()].reduce((a, b) => { return this.regionName(a[1].region).length > this.regionName(b[1].region).length ? a : b; } )[1].region).length;
        const longestGossipNameLength = this.getMaxKeyLength(gossipsItemRegion);
        for (const [stone, hint] of gossipsItemRegion) {
          this.writer.write(`${stone.padEnd(longestGossipNameLength)} ${this.regionName(hint.region).padEnd(longestRegionNameLength + 1)} ${this.itemName(hint.item)} ${this.getImportanceSuffix(hint.importance)}`);
        }
        this.writer.unindent('');
      }

      if(hints.foolish.size > 0) {
        this.writer.indent('Foolish Regions:');
        const foolish = sortBy([...hints.foolish.keys()], x => -hints.foolish.get(x)!);
        const longestRegionName = foolish.reduce( function (a, b) { return regionName(a).length > regionName(b).length ? regionName(a) : regionName(b); } ).length;
        for (const f of foolish) {
          const weight = hints.foolish.get(f);
          this.writer.write(`${regionName(f).padEnd(longestRegionName + 1)}: ${weight}`);
        }
        this.writer.unindent();
      }

      if (this.isMulti) this.writer.unindent('');
    }
    this.writer.unindent('');
  }

  private writePaths() {
    const { paths } = this.logic.analysis;
    if (paths.length === 0) {
      return;
    }
    this.writer.indent('Paths');
    for (const path of paths) {
      this.writer.indent(this.pathName(path));
      const locations = [...path.locations].map(x => `${this.locationName(x)}: ${this.itemName(this.logic.items.get(x)!)}`).sort();
      for (const loc of locations) {
        this.writer.write(loc);
      }
      this.writer.unindent('');
    }
    this.writer.unindent('');
  }

  private decPad(n: number, width: number) {
    const s = n.toString();
    const count = width - s.length;
    return count > 0 ? '0'.repeat(width - s.length) + s : s;
  }

  private strPad(s: string, number: number) {
    return `${s} ${this.decPad(number, this.logic.worlds.length.toString().length)}`;
  }

  private locationName(location: Location) {
    const data = locationData(location);
    if (this.isMulti) {
      return `${this.strPad("World", data.world! + 1)} ${data.id}`;
    } else {
      return data.id;
    }
  }

  private itemName(item: PlayerItem) {
    if (this.isMulti && item.player !== 'all') {
      return `${this.strPad("Player", item.player + 1)} ${itemName(item.item.id)}`;
    } else {
      return itemName(item.item.id);
    }
  }

  private itemNameAt(loc: Location) {
    const pi = this.logic.items.get(loc)!;
    const cloakItem = this.logic.itemCloaks.get(loc);

    let str = this.itemName(pi);
    if (cloakItem) {
      str += ` (cloaked as ${itemName(cloakItem.id)})`;
    }
    return str;
  }

  private regionName(region: Region) {
    const data = regionData(region);
    if (this.isMulti) {
      return `${this.strPad("World", data.world + 1)} ${regionName(data.id)}`;
    } else {
      return regionName(data.id);
    }
  }

  private eventName(entry: SphereEntryEvent) {
    const event = ANALYSIS_EVENTS.get(entry.event)!;
    if (this.isMulti) {
      return `${this.strPad("World", entry.playerId + 1)} ${event}`;
    } else {
      return event;
    }
  }

  private writeRaw() {
    const { worlds } = this.logic;
    const allLocsCount = worlds.map(x => x.locations.size).reduce((a, b) => a + b, 0);
    this.writer.writeSectionHeader();
    this.writer.indent(`Location List (${allLocsCount})`);
    for (let i = 0; i < this.opts.settings.players; ++i) {
      if (this.isMulti) this.writer.indent(`World ${i+1} (${worlds[i].locations.size})`);
      const world = worlds[i];
      const regionNames = new Set(Object.values(world.regions));
      for (const region of regionNames) {
        const regionalLocations = Object.keys(world.regions)
          .filter(location => world.regions[location] === region)
          .map(loc => `${loc}: ${this.itemNameAt(makeLocation(loc, i)!)}`);
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
    const { spheres } = this.logic.analysis;
    this.writer.writeSectionHeader();
    this.writer.indent('Spheres');
    for (const i in spheres) {
      this.writer.indent(`Sphere ${i}`);
      const text: string[] = [];
      const sphere = spheres[i];
      for (const entry of sphere) {
        switch (entry.type) {
          case 'location': text.push(`Location - ${this.locationName(entry.location)}: ${this.itemName(this.logic.items.get(entry.location)!)}`); break;
          case 'event': text.push(`Event - ${this.eventName(entry)}`); break;
        }
      }

      for (const t of text.sort()) {
        this.writer.write(t);
      }
      this.writer.unindent('');
    }
    this.writer.unindent();
  }

  run() {
    this.writeHeader();
    this.writeSettings();
    this.writeSpecialConds();
    this.writeTricks();
    this.writeGlitches();
    this.writeStartingItems();
    this.writeJunkLocations();
    this.writeWorldFlags();
    this.writePreCompleted();
    this.writeEntrances();
    this.writeSongEvents();
    this.writeHints();
    this.writePaths();
    this.writePlando();
    if (this.opts.settings.logic !== 'none') {
      this.writeSpheres();
    }
    this.writeRaw();

    return this.writer.emit();
  };
}

export function makeSpoilerLog(result: LogicResult, options: Options): string {
  const spoilerWriter = new SpoilerWriter(result, options);
  return spoilerWriter.run();
}

const ENEMY_SCENE_NAMES = new Map(Object.entries(SCENES).map(([key, id]) => [
  `${key.slice(0, key.indexOf('_')).toLowerCase()}/${id}`,
  key.slice(key.indexOf('_') + 1).toLowerCase().replaceAll('_', ' ').replace(/\b\w/g, ch => ch.toUpperCase()),
]));

ENEMY_SCENE_NAMES.set('oot/65', 'Royal Family’s Tomb');
ENEMY_SCENE_NAMES.set('mm/86', 'Ikana Castle Throne Room');

function enemyRoomLabel(game: string, scene: number, room: number, setup?: number, slot?: number) {
  const name = ENEMY_SCENE_NAMES.get(`${game}/${scene}`);
  const id = `0x${scene.toString(16).padStart(2, '0')}`;
  return [name ? `${name} (${id})` : `Scene ${id}`, `room ${room}`,
    setup === undefined ? undefined : `setup ${setup}`,
    slot === undefined ? undefined : `slot ${slot}`].filter(Boolean).join(', ');
}

const ENEMY_SCRIPTED_ROOMS = new Map(Object.values(ENEMY_LOGIC_MM_ROOMS)
  .flatMap(area => 'encounters' in area ? area.encounters ?? [] : [])
  .map(encounter => [encounter.source, encounter.room]));

const ENEMY_SPAWNER_SLOTS = new Map([
  ...OOT_DESERT_LEEVER_SLOTS, ...OOT_FIELD_STALCHILD_SLOTS, ...OOT_FOREST_BOW_SLOTS, ...OOT_BURIAL_ENEMY_SLOTS,
].map(slot => [slot.key, slot]));

function enemySpoilerHex(value: number): string {
  return `0x${(value & 0xffff).toString(16).padStart(4, '0')}`;
}

function enemySpoilerParams(game: string, actorId: number, params: number | undefined,
  fallback: string, isSource: boolean): { name: string; detail: string } {
  if (params === undefined) return { name: fallback, detail: 'authored parameters unavailable' };
  const p = params & 0xffff;
  const lo = p & 0xff, hi = p >>> 8;
  const fields: string[] = [];
  let name: string | undefined;
  const forms = (names: Readonly<Record<number, string>>, value: number, field = 'form') => {
    name = names[value];
    fields.push(`${field}: ${name ?? 'unrecognized'} (${enemySpoilerHex(value)})`);
  };
  if (game === 'oot' && actorId === 0x69) {
    forms({ 0xff: 'Blue Bubble', 0xfe: 'Red Bubble', 0xfd: 'White Bubble', 0xfc: 'Green Bubble', 0xfb: 'Big Green Bubble' }, lo);
    fields.push(`upper-byte configuration: ${enemySpoilerHex(hi)}`);
  } else if (game === 'oot' && actorId === 0x13 || game === 'mm' && actorId === 0x0c) {
    forms({ 0: 'Fire Keese', 1: 'Fire Keese', 2: 'Keese', 3: 'Keese', 4: 'Ice Keese' }, lo);
    if (name && (p & 0x8000)) name = `Invisible ${name}`;
    fields.push(`invisible: ${p & 0x8000 ? 'yes' : 'no'}`);
    if (p & 0x7f00) fields.push(`other upper bits: ${enemySpoilerHex(p & 0x7f00)}`);
  } else if (game === 'oot' && actorId === 0x1b || game === 'mm' && actorId === 0x12) {
    forms(game === 'oot' ? { 0xfffe: 'Blue Tektite', 0xffff: 'Red Tektite', 0: 'Red Tektite' } :
      { 0xfffd: 'Blue Tektite', 0xfffe: 'Blue Tektite', 0xffff: 'Red Tektite', 0: 'Red Tektite' }, p);
  } else if (game === 'oot' && actorId === 0x1af || game === 'mm' && actorId === 0xec) {
    forms({ 0: 'Wolfos', 1: 'White Wolfos' }, lo);
    fields.push(`upper-byte configuration: ${enemySpoilerHex(hi)}`);
  } else if (game === 'oot' && actorId === 0x113 || game === 'mm' && actorId === 0x84) {
    forms({ 1: 'Silver Iron Knuckle', 2: 'Black Iron Knuckle', 3: 'White Iron Knuckle' }, lo & 0x7f, 'armor');
    fields.push(`other configuration bits: ${enemySpoilerHex(p & 0xff80)}`);
  } else if (game === 'oot' && actorId === 0x90 || game === 'mm' && actorId === 0x4c) {
    forms({ 0: 'ReDead (mourns when stationary)', 1: 'ReDead', 2: 'ReDead', 3: 'Invisible ReDead',
      4: 'Frozen ReDead', 5: 'Dancing ReDead', 6: 'Dancing ReDead', 7: 'Dancing ReDead' }, lo);
    fields.push(`upper-byte configuration: ${enemySpoilerHex(hi)}`);
  } else if (game === 'oot' && actorId === 0x8a) {
    forms({ 0: 'Beamos (Large)', 1: 'Beamos (Small)' }, lo, 'size');
    fields.push(`upper-byte configuration: ${enemySpoilerHex(hi)}`);
  } else if (game === 'oot' && actorId === 0x25) {
    if (p === 0xfffe || p === 0xff80) forms({ 0xfffe: 'Dinolfos', 0xff80: 'Dinolfos' }, p);
    else if (p === 0xffff) forms({ 0xffff: 'Lizalfos' }, p);
  } else if (game === 'mm' && actorId === 0x14a) {
    forms({ 0: 'Blue ChuChu', 1: 'Green ChuChu', 2: 'Yellow ChuChu', 3: 'Red ChuChu' }, lo, 'color');
    fields.push(`upper-byte configuration: ${enemySpoilerHex(hi)}`);
  } else if (game === 'mm' && actorId === 0x12d) {
    forms({ 0: 'Bio Deku Baba', 1: 'Leafless Bio Deku Baba', 2: 'Bio Deku Baba Lily Pad', 4: 'Walking Bio Deku Baba' }, lo);
    if (hi) fields.push(`upper-byte configuration: ${enemySpoilerHex(hi)}`);
  } else if (game === 'mm' && actorId === 0x1e9) {
    forms({ 0: 'Masked Hiploop', 0x0101: 'Hiploop' }, p);
  } else if (game === 'mm' && actorId === 0x1e6) {
    forms({ 0: 'Small Eeno', 1: 'Large Eeno' }, p & 1, 'size');
    if (p & 0xfffe) fields.push(`other configuration bits: ${enemySpoilerHex(p & 0xfffe)}`);
  } else if (game === 'mm' && actorId === 0x109) {
    forms({ 0: 'Dragonfly', 1: 'Dragonfly (grows when spawned)', 2: 'Woodfall Temple final-room Dragonfly', 3: 'Woodfall Dragonfly', 0xffff: 'Dragonfly (unused normal)' }, p, 'type');
  } else if (game === 'mm' && actorId === 0x15b) {
    const type = p & 0x1f, flags = (p >>> 5) & 7;
    name = flags & 4 ? 'Perching Bad Bat' : type > 1 && type !== 0x1f ? 'Bad Bat swarm' : 'Bad Bat';
    fields.push(`type/count: ${type}`, `switch flag: ${hi}`, `perch: ${flags & 4 ? 'yes' : 'no'}`,
      `check relative height: ${flags & 2 ? 'yes' : 'no'}`);
    if (flags & 1) fields.push('other flag: 0x1');
  } else if (game === 'mm' && actorId === 0x13a) {
    fields.push(`type: ${p >>> 12}`, `revive timer field: ${(p >>> 7) & 0x1f}`, `switch flag: ${p & 0x7f}`);
  }

  /* Cover the entire roster using exact authored/catalog forms. A unique
   * catalog label is useful even where individual bit fields are undocumented.
   * Do not infer meanings from another actor or from a conflicting form. */
  const candidates = ENEMIES.filter(e => e.game === game && e.actorId === actorId);
  const exactForms = candidates.filter(e => e.spawnParams === p);
  const exact = [...new Set((exactForms.length ? exactForms : candidates.filter(e => e.params.includes(p))).map(e => e.name))];
  if (!name && isSource && exact.length === 1) name = exact[0];
  if (!fields.length) {
    if (!isSource || exact.length === 1) fields.push(`catalog form: ${name ?? fallback}; field meanings not decoded`);
    else if (exact.length > 1) fields.push(`ambiguous catalog forms: ${exact.join(' / ')}; field meanings not decoded`);
    else fields.push('unmapped authored parameters');
  }
  return { name: name ?? fallback, detail: fields.join('; ') };
}

export function makeEnemySpoilerLog(result: LogicResult, options: Options): string | undefined {
  if (!result.worlds.some(world => world.enemyPlan.length)) return;
  const writer = new LogWriter();
  writer.write(`Seed: ${options.seed}`);
  writer.write(`Version: ${VERSION}`);
  writer.write('');
  writer.indent('Enemy Replacement Plans');

  result.worlds.forEach((world, index) => {
    writer.indent(`World ${index + 1} (${world.enemyPlan.length} placements)`);
    for (const game of ['oot', 'mm'] as const) {
      const rows = world.enemyPlan.filter(row => enemyDefinition(row.source).game === game);
      if (!rows.length) continue;
      writer.indent(game === 'oot' ? 'Ocarina of Time' : 'Majora’s Mask');
      for (const row of rows) {
        const source = enemyDefinition(row.source), target = enemyDefinition(row.target);
        const exact = row.ootr?.location ?? row.placement;
        const spawned = ENEMY_SPAWNER_SLOTS.get(row.source);
        const gold = ENEMY_GOLD_SOURCES.find(slot => slot.key === row.source);
        const scripted = ENEMY_SCRIPTED_ROOMS.get(row.source);
        const arena = OOT_ENCOUNTER_MEMORY_ROOMS[row.source];
        const location = exact ? enemyRoomLabel(game, exact.scene, exact.room, exact.setup, exact.slot) :
          spawned ? enemyRoomLabel(game, spawned.scene, spawned.room, undefined, spawned.slot) :
          gold ? `${gold.location} — ${gold.slots.map(slot => enemyRoomLabel(game, gold.scene, slot.room, slot.setup, slot.slot)).join(' / ')}` :
          scripted ? enemyRoomLabel(game, scripted.scene, scripted.room, scripted.setup, scripted.slots.length === 1 ? scripted.slots[0] : undefined) :
          arena ? arena.map(([scene, room]) => enemyRoomLabel(game, scene, room)).join(' / ') :
          row.source;
        const layout = row.ootr?.location.mq ? ' [MQ]' : row.placement?.grotto !== undefined ? ' [' + MM_GENERIC_GROTTOS.find(grotto => grotto.id === row.placement!.grotto)!.name + ' Generic Grotto]' : '';
        const params = row.ootr?.targetParams ?? row.placement?.targetParams ?? row.field?.targetParams ?? target.spawnParams;
        const sourceActorId = row.ootr?.location.actorId ?? row.placement?.sourceActorId ?? source.actorId;
        const sourceParams = row.ootr?.sourceParams ?? row.placement?.sourceParams;
        const sourceLabel = enemySpoilerParams(game, sourceActorId, sourceParams,
          row.source === 'mm-ikana-throne-scripted' ? 'Igos du Ikana' : source.name, true);
        const targetLabel = enemySpoilerParams(target.game, target.actorId, params, target.name, false);
        const sourceInfo = ` [${game.toUpperCase()} actor ${enemySpoilerHex(sourceActorId)}, params ${sourceParams === undefined ? 'unavailable' : enemySpoilerHex(sourceParams)}; ${sourceLabel.detail}]`;
        const targetInfo = ` [${target.game.toUpperCase()} actor ${enemySpoilerHex(target.actorId)}, params ${enemySpoilerHex(params)}; ${targetLabel.detail}]`;
        writer.write(`${location}${layout}: ${sourceLabel.name}${sourceInfo} → ${targetLabel.name}${targetInfo}`);
        for (const [scene, room, setup, slot] of row.ootr?.aliases ?? [])
          writer.write(`  Also applies to ${enemyRoomLabel(game, scene, room, setup, slot)}`);
        if (row.splitChildren?.length)
          writer.write(`  Split spawns: ${row.splitChildren.map(key => enemyDefinition(key).name).join(', ')}`);
        const choices = row.chaoticOotChoices?.length ?? row.chaoticMmChoices?.length;
        if (choices)
          writer.write(`  Runtime alternatives: ${choices}`);
      }
      writer.unindent('');
    }
    writer.unindent('');
  });
  writer.unindent('');
  return writer.emit();
}

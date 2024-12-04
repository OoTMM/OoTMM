import { Random, shuffle } from '../random';
import { Settings } from '../settings';
import { cloneWorld, World } from './world';
import { Pathfinder, PathfinderState } from './pathfind';
import { Monitor } from '../monitor';
import { isLocationRenewable, Location, makeLocation, locationData } from './locations';
import { ItemPlacement } from './solve';
import { ItemHelpers, PlayerItem, PlayerItems } from '../items';
import { ItemProperties } from './item-properties';
import { AnalysisPath } from './analysis-path';

type ImportantEvent = {
  event: string;
  name: string;
};

type SphereEntryLocation = {
  type: 'location';
  location: Location;
};

type SphereEntryEvent = {
  type: 'event';
  event: string;
  playerId: number;
};

type SphereEntry = SphereEntryLocation | SphereEntryEvent;

/* The following events are considered entries in spheres */
export const ANALYSIS_EVENTS = new Map([
  /* Bosses */
  ['OOT_BOSS_GOHMA',          'Beat Gohma'],
  ['OOT_BOSS_KING_DODONGO',   'Beat King Dodongo'],
  ['OOT_BOSS_BARINADE',       'Beat Barinade'],
  ['OOT_BOSS_PHANTOM_GANON',  'Beat Phantom Ganon'],
  ['OOT_BOSS_VOLVAGIA',       'Beat Volvagia'],
  ['OOT_BOSS_MORPHA',         'Beat Morpha'],
  ['OOT_BOSS_BONGO_BONGO',    'Beat Bongo-Bongo'],
  ['OOT_BOSS_TWINROVA',       'Beat Twinrova'],
  ['OOT_GANON',               'Beat Ganon'],
  ['MM_BOSS_ODOLWA',          'Beat Odolwa'],
  ['MM_BOSS_GOHT',            'Beat Goht'],
  ['MM_BOSS_GYORG',           'Beat Gyorg'],
  ['MM_BOSS_TWINMOLD',        'Beat Twinmold'],
  ['MM_MAJORA',               'Beat Majora'],

  /* Major events */
  ['OOT_TIME_TRAVEL' ,  'Time Travel'],
  ['OOT_BRIDGE_OPEN',   'Rainbow Bridge'],
  ['MM_ACCESS',         'Termina Access'],
  ['MM_MOON_OPEN',      'Moon Access'],
]);

export class LogicPassAnalysis {
  private pathfinder: Pathfinder;
  private spheres: SphereEntry[][] = [];
  private requiredLocs = new Set<Location>();
  private requiredlocsByItem: Map<PlayerItem, Set<Location>> = new Map();
  private uselessLocs = new Set<Location>();
  private unreachableLocs = new Set<Location>();
  private locations: Location[];

  constructor(
    private readonly state: {
      settings: Settings;
      random: Random;
      worlds: World[];
      items: ItemPlacement;
      monitor: Monitor;
      startingItems: PlayerItems;
      itemProperties: ItemProperties;
    },
  ){
    this.pathfinder = new Pathfinder(this.state.worlds, this.state.settings, this.state.startingItems);
    this.locations = this.state.worlds.map((x, i) => [...x.locations].map(l => makeLocation(l, i))).flat();
  }

  private worldFromEntries(worlds: World[], entries: SphereEntry[]) {
    const newWorlds = worlds.map(x => cloneWorld(x));

    for (let i = 0; i < newWorlds.length; ++i) {
      const world = newWorlds[i];
      const events = new Set(entries.filter(x => x.type === 'event').filter(x => x.playerId === i).map(x => x.event));
      const locationNames = new Set(entries.filter(x => x.type === 'location').filter(x => locationData(x.location).world === i).map(x => locationData(x.location).id));

      for (const area of Object.values(world.areas)) {
        for (const location of Object.keys(area.locations)) {
          if (!locationNames.has(location)) {
            delete area.locations[location];
          }
        }

        for (const event of Object.keys(area.events)) {
          if (ANALYSIS_EVENTS.has(event) && !events.has(event)) {
            delete area.events[event];
          }
        }
      }
    }

    return newWorlds;
  }

  private makeSpheresRaw(entries?: SphereEntry[]) {
    let worlds: World[];
    let pathfinder: Pathfinder;

    if (entries) {
      worlds = this.worldFromEntries(this.state.worlds, entries);
      pathfinder = new Pathfinder(worlds, this.state.settings, this.state.startingItems);
    } else {
      worlds = this.state.worlds;
      pathfinder = this.pathfinder;
    }

    const spheres: SphereEntry[][] = [];
    let pathfinderState: PathfinderState | null = null;
    let count = 0;

    do {
      this.progress(count++, 10);
      pathfinderState = pathfinder.run(pathfinderState, { inPlace: true, items: this.state.items, stopAtGoal: true });
      const sphere: SphereEntry[] = [];
      const locs = Array.from(pathfinderState.newLocations).filter(x => this.state.itemProperties.important.has(this.state.items.get(x)!.item));
      for (const loc of locs) {
        sphere.push({ type: 'location', location: loc });
      }
      for (let i = 0; i < pathfinderState.ws.length; ++i) {
        const ws = pathfinderState.ws[i];
        const { newEvents } = ws;
        for (const event of newEvents) {
          if (ANALYSIS_EVENTS.has(event)) {
            sphere.push({ type: 'event', event, playerId: i });
          }
        }
      }
      if (sphere.length !== 0) {
        spheres.push(shuffle(this.state.random, sphere));
      }
    } while (!pathfinderState.goal);

    return spheres;
  }

  private makeSpheres() {
    this.state.monitor.log('Analysis - Spheres (Raw)');

    const entries = this.makeSpheresRaw().reverse().flat();
    const entriesCopy = [...entries];

    this.state.monitor.log('Analysis - Spheres (Playthrough)');
    let count = 0;
    for (const e of entriesCopy) {
      this.state.monitor.setProgress(count++, entriesCopy.length);
      const entriesIndex = entries.indexOf(e);
      entries.splice(entriesIndex, 1);
      const worlds = this.worldFromEntries(this.state.worlds, entries);
      const pathfinder = new Pathfinder(worlds, this.state.settings, this.state.startingItems);
      const pathfinderState = pathfinder.run(null, { recursive: true, items: this.state.items, stopAtGoal: true });
      if (!pathfinderState.goal) {
        entries.push(e);
      }
    }

    /* Recompute spheres to ensure correct order */
    this.state.monitor.log('Analysis - Spheres (Reorder)');
    this.spheres = this.makeSpheresRaw(entries);
  }

  private makeRequiredLocs() {
    this.state.monitor.log('Analysis - WotH');
    let count = 0;
    const locations = new Set(this.spheres.flat().filter(x => x.type === 'location').map(x => x.location));
    for (const loc of locations) {
      this.state.monitor.setProgress(count++, locations.size);
      const pathfinderState = this.pathfinder.run(null, { items: this.state.items, forbiddenLocations: new Set([loc]), recursive: true, stopAtGoal: true });
      if (!pathfinderState.goal) {
        this.requiredLocs.add(loc);
        const item = this.state.items.get(loc)!;
        if (!this.requiredlocsByItem.has(item)) {
          this.requiredlocsByItem.set(item, new Set());
        }
        this.requiredlocsByItem.get(item)!.add(loc);
      }
    }
  }

  private isLocUselessNonRenewable(loc: Location) {
    const pi = this.state.items.get(loc)!;
    const locD = locationData(loc);
    return (ItemHelpers.isItemConsumable(pi.item) && !isLocationRenewable(this.state.worlds[locD.world as number], loc) && !this.state.itemProperties.license.has(pi.item));
  }

  private makeUselessLocs() {
    for (const loc of this.locations) {
      if (this.requiredLocs.has(loc) || this.uselessLocs.has(loc) || this.unreachableLocs.has(loc)) {
        continue;
      }
      const item = this.state.items.get(loc)!;
      if (!this.state.itemProperties.important.has(item.item) || this.isLocUselessNonRenewable(loc)) {
        this.uselessLocs.add(loc);
      }
    }
  }

  private makeUnreachable() {
    const pathfinderState = this.pathfinder.run(null, { items: this.state.items, recursive: true });
    for (const loc of this.locations) {
      const item = this.state.items.get(loc)!;
      if (this.state.itemProperties.important.has(item.item) && !pathfinderState.locations.has(loc)) {
        this.state.monitor.debug(`Analysis - makeUnreachable: ${item.item.id}@${item.player}`);
        this.unreachableLocs.add(loc);
      }
    }
  }

  private progress(x: number, slope: number) {
    this.state.monitor.setProgress(x, x + slope);
  }

  run() {
    this.state.monitor.log('Logic: Analysis');
    if (this.state.settings.logic !== 'none') {
      this.makeSpheres();
      this.makeRequiredLocs();
    }
    if (this.state.settings.logic === 'beatable') {
      this.makeUnreachable();
    }
    this.makeUselessLocs();

    const analysis = {
      spheres: this.spheres,
      required: this.requiredLocs,
      useless: this.uselessLocs,
      unreachable: this.unreachableLocs,
      paths: [] as AnalysisPath[],
    }

    return { analysis };
  }
}

export type Analysis = ReturnType<LogicPassAnalysis['run']>['analysis'];

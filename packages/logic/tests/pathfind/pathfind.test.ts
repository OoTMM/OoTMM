import { describe, it, expect } from 'vitest';
import type { ItemPlacement, World, WorldArea } from '../../src';

import { makeSettings } from '@ootmm/core';
import { Items, makePlayerItem, Pathfinder, makeLocation, exprTrue, exprFalse, exprHas, exprEvent } from '../../src';

/*
 * Check metadata (type, item) lives in the global CHECKS_BY_LOCATION table rather
 * than on the world, so locations used here have to be real ids — the pathfinder
 * resolves them whenever it collects an item.
 */
const LOC_A = 'OOT Kokiri Forest Kokiri Sword Chest';
const LOC_B = 'OOT Kokiri Forest Storms Grotto';
const LOC_LOCKED = 'OOT Lost Woods Grotto Generic';
const LOC_REWARD = 'OOT Sacred Meadow Grotto';

const SETTINGS = makeSettings({
  goal: 'ganon',
  games: 'oot',
  triforceGoal: 1,
});

function makeArea(overrides: Partial<WorldArea> = {}): WorldArea {
  return {
    game: 'oot',
    boss: false,
    ageChange: false,
    dungeon: null,
    time: 'still',
    region: 'NONE',
    exits: {},
    events: {},
    locations: {},
    gossip: {},
    stay: null,
    ...overrides,
  };
}

function makeWorld(areas: World['areas'], locations: string[] = []): World {
  return {
    areas,
    dungeons: {},
    dungeonsBossAreas: {},
    regions: {},
    gossip: {},
    locations: new Set(locations),
    checkItems: new Map(locations.map(l => [l, Items.NOTHING])),
    songLocations: new Set(),
    warpLocations: new Set(),
    prices: [],
    songEventsOot: [],
    songEventsMm: [],
    bossIds: [],
    preCompleted: new Set(),
    entranceOverrides: new Map(),
    entranceOverridesRev: new Map(),
    dungeonsEntrances: new Map(),
    resolvedFlags: {} as any,
    exprParsers: {} as any,
  };
}

describe('Pathfinder', () => {
  describe('basic reachability', () => {
    it('finds no locations when the world has none', () => {
      const world = makeWorld({ 'OOT SPAWN': makeArea() });
      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null);
      expect(state.locations.size).toBe(0);
    });

    it('reaches a free location in the spawn area', () => {
      const world = makeWorld(
        { 'OOT SPAWN': makeArea({ locations: { [LOC_A]: exprTrue() } }) },
        [LOC_A],
      );
      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null);
      expect(state.locations.has(makeLocation(LOC_A, 0))).toBe(true);
    });

    it('does not reach a location whose condition is false', () => {
      const world = makeWorld(
        { 'OOT SPAWN': makeArea({ locations: { [LOC_A]: exprFalse() } }) },
        [LOC_A],
      );
      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null);
      expect(state.locations.has(makeLocation(LOC_A, 0))).toBe(false);
    });
  });

  describe('connected areas', () => {
    it('reaches a location in a directly connected area', () => {
      const world = makeWorld({
        'OOT SPAWN': makeArea({ exits: { 'OOT Area A': exprTrue() } }),
        'OOT Area A': makeArea({ locations: { [LOC_A]: exprTrue() } }),
      }, [LOC_A]);
      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null);
      expect(state.locations.has(makeLocation(LOC_A, 0))).toBe(true);
    });

    it('cannot reach a location behind a false exit', () => {
      const world = makeWorld({
        'OOT SPAWN': makeArea({ exits: { 'OOT Area A': exprFalse() } }),
        'OOT Area A': makeArea({ locations: { [LOC_A]: exprTrue() } }),
      }, [LOC_A]);
      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null);
      expect(state.locations.has(makeLocation(LOC_A, 0))).toBe(false);
    });
  });

  describe('item gating', () => {
    it('cannot reach a location that requires an item when the item is absent', () => {
      const world = makeWorld(
        { 'OOT SPAWN': makeArea({ locations: { [LOC_A]: exprHas(Items.OOT_SWORD, 1) } }) },
        [LOC_A],
      );
      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null);
      expect(state.locations.has(makeLocation(LOC_A, 0))).toBe(false);
    });

    it('reaches an item-gated location when the item is provided as a starting item', () => {
      const world = makeWorld(
        { 'OOT SPAWN': makeArea({ locations: { [LOC_A]: exprHas(Items.OOT_SWORD, 1) } }) },
        [LOC_A],
      );
      const startingItems = new Map([
        [makePlayerItem(Items.OOT_SWORD, 0), 1],
      ]);
      const pf = new Pathfinder([world], SETTINGS, startingItems);
      const state = pf.run(null);
      expect(state.locations.has(makeLocation(LOC_A, 0))).toBe(true);
    });

    it('collects an item from one location and uses it to reach another (recursive)', () => {
      const world = makeWorld({
        'OOT SPAWN': makeArea({
          locations: {
            [LOC_A]: exprTrue(),
            [LOC_B]: exprHas(Items.OOT_SWORD, 1),
          },
        }),
      }, [LOC_A, LOC_B]);

      const loc1 = makeLocation(LOC_A, 0);
      const items: ItemPlacement = new Map([
        [loc1, makePlayerItem(Items.OOT_SWORD, 0)],
      ]);

      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null, { recursive: true, items });
      expect(state.locations.has(makeLocation(LOC_A, 0))).toBe(true);
      expect(state.locations.has(makeLocation(LOC_B, 0))).toBe(true);
    });

    it('item in a locked area can only unlock things once the area itself is reachable', () => {
      const world = makeWorld({
        'OOT SPAWN': makeArea({
          exits: { 'OOT Area A': exprHas(Items.OOT_SWORD, 1) },
          locations: { [LOC_LOCKED]: exprTrue() },
        }),
        'OOT Area A': makeArea({
          locations: { [LOC_REWARD]: exprTrue() },
        }),
      }, [LOC_LOCKED, LOC_REWARD]);

      /* loc in locked area has the sword — but you need the sword to enter */
      const lockedLoc = makeLocation(LOC_LOCKED, 0);
      const items: ItemPlacement = new Map([
        [lockedLoc, makePlayerItem(Items.OOT_SWORD, 0)],
      ]);

      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null, { recursive: true, items });
      /* locked_loc is freely reachable from spawn */
      expect(state.locations.has(makeLocation(LOC_LOCKED, 0))).toBe(true);
      /* reward_loc requires the sword from locked_loc — it should be reached recursively */
      expect(state.locations.has(makeLocation(LOC_REWARD, 0))).toBe(true);
    });
  });

  describe('event gating', () => {
    it('event from one area unlocks an exit in the same area', () => {
      const world = makeWorld({
        'OOT SPAWN': makeArea({
          events: { 'MY_EVENT': exprTrue() },
          exits: { 'OOT Area A': exprEvent('MY_EVENT') },
        }),
        'OOT Area A': makeArea({ locations: { [LOC_A]: exprTrue() } }),
      }, [LOC_A]);
      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null, { recursive: true });
      expect(state.locations.has(makeLocation(LOC_A, 0))).toBe(true);
    });

    it('cannot reach an area behind an event that is never set', () => {
      const world = makeWorld({
        'OOT SPAWN': makeArea({
          exits: { 'OOT Area A': exprEvent('MISSING_EVENT') },
        }),
        'OOT Area A': makeArea({ locations: { [LOC_A]: exprTrue() } }),
      }, [LOC_A]);
      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null, { recursive: true });
      expect(state.locations.has(makeLocation(LOC_A, 0))).toBe(false);
    });
  });

  describe('no-logic mode', () => {
    it('makes all check locations reachable regardless of conditions', () => {
      const noLogicSettings = makeSettings({ ...SETTINGS, logic: 'none' });
      const world = makeWorld({
        'OOT SPAWN': makeArea({ exits: { 'OOT Area A': exprFalse() } }),
        'OOT Area A': makeArea({ locations: { [LOC_A]: exprFalse() } }),
      }, [LOC_A]);
      const pf = new Pathfinder([world], noLogicSettings, new Map());
      const state = pf.run(null);
      expect(state.locations.has(makeLocation(LOC_A, 0))).toBe(true);
      expect(state.goal).toBe(true);
    });
  });
});

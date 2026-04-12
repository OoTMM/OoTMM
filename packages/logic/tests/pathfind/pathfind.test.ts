import { describe, it, expect } from 'vitest';
import type { Settings } from '@ootmm/core';
import type { World, WorldArea, WorldCheck } from '../../src/world/types';
import type { ItemPlacement } from '../../src/types';

import { Items, makePlayerItem, makeSettings } from '@ootmm/core';
import { exprTrue, exprFalse, exprHas, exprEvent } from '../../src';
import { Pathfinder } from '../../src/pathfind/pathfind';
import { makeLocation } from '../../src/locations';

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

function makeCheck(): WorldCheck {
  return {
    game: 'oot',
    type: 'chest',
    scene: 'TEST',
    id: 0,
    item: Items.NOTHING,
    hint: 'NONE',
  };
}

function makeWorld(areas: World['areas'], checks: World['checks'] = {}): World {
  return {
    areas,
    checks,
    dungeons: {},
    dungeonsBossAreas: {},
    regions: {},
    gossip: {},
    locations: new Set(Object.keys(checks)),
    songLocations: new Set(),
    warpLocations: new Set(),
    prices: [],
    songEvents: [],
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
        { 'OOT SPAWN': makeArea({ locations: { 'loc1': exprTrue() } }) },
        { 'loc1': makeCheck() },
      );
      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null);
      expect(state.locations.has(makeLocation('loc1', 0))).toBe(true);
    });

    it('does not reach a location whose condition is false', () => {
      const world = makeWorld(
        { 'OOT SPAWN': makeArea({ locations: { 'loc1': exprFalse() } }) },
        { 'loc1': makeCheck() },
      );
      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null);
      expect(state.locations.has(makeLocation('loc1', 0))).toBe(false);
    });
  });

  describe('connected areas', () => {
    it('reaches a location in a directly connected area', () => {
      const world = makeWorld({
        'OOT SPAWN': makeArea({ exits: { 'OOT Area A': exprTrue() } }),
        'OOT Area A': makeArea({ locations: { 'loc1': exprTrue() } }),
      }, { 'loc1': makeCheck() });
      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null);
      expect(state.locations.has(makeLocation('loc1', 0))).toBe(true);
    });

    it('cannot reach a location behind a false exit', () => {
      const world = makeWorld({
        'OOT SPAWN': makeArea({ exits: { 'OOT Area A': exprFalse() } }),
        'OOT Area A': makeArea({ locations: { 'loc1': exprTrue() } }),
      }, { 'loc1': makeCheck() });
      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null);
      expect(state.locations.has(makeLocation('loc1', 0))).toBe(false);
    });
  });

  describe('item gating', () => {
    it('cannot reach a location that requires an item when the item is absent', () => {
      const world = makeWorld(
        { 'OOT SPAWN': makeArea({ locations: { 'loc1': exprHas(Items.OOT_SWORD, 1) } }) },
        { 'loc1': makeCheck() },
      );
      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null);
      expect(state.locations.has(makeLocation('loc1', 0))).toBe(false);
    });

    it('reaches an item-gated location when the item is provided as a starting item', () => {
      const world = makeWorld(
        { 'OOT SPAWN': makeArea({ locations: { 'loc1': exprHas(Items.OOT_SWORD, 1) } }) },
        { 'loc1': makeCheck() },
      );
      const startingItems = new Map([
        [makePlayerItem(Items.OOT_SWORD, 0), 1],
      ]);
      const pf = new Pathfinder([world], SETTINGS, startingItems);
      const state = pf.run(null);
      expect(state.locations.has(makeLocation('loc1', 0))).toBe(true);
    });

    it('collects an item from one location and uses it to reach another (recursive)', () => {
      const world = makeWorld({
        'OOT SPAWN': makeArea({
          locations: {
            'loc1': exprTrue(),
            'loc2': exprHas(Items.OOT_SWORD, 1),
          },
        }),
      }, {
        'loc1': makeCheck(),
        'loc2': makeCheck(),
      });

      const loc1 = makeLocation('loc1', 0);
      const items: ItemPlacement = new Map([
        [loc1, makePlayerItem(Items.OOT_SWORD, 0)],
      ]);

      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null, { recursive: true, items });
      expect(state.locations.has(makeLocation('loc1', 0))).toBe(true);
      expect(state.locations.has(makeLocation('loc2', 0))).toBe(true);
    });

    it('item in a locked area can only unlock things once the area itself is reachable', () => {
      const world = makeWorld({
        'OOT SPAWN': makeArea({
          exits: { 'OOT Area A': exprHas(Items.OOT_SWORD, 1) },
          locations: { 'locked_loc': exprTrue() },
        }),
        'OOT Area A': makeArea({
          locations: { 'reward_loc': exprTrue() },
        }),
      }, {
        'locked_loc': makeCheck(),
        'reward_loc': makeCheck(),
      });

      /* loc in locked area has the sword — but you need the sword to enter */
      const lockedLoc = makeLocation('locked_loc', 0);
      const items: ItemPlacement = new Map([
        [lockedLoc, makePlayerItem(Items.OOT_SWORD, 0)],
      ]);

      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null, { recursive: true, items });
      /* locked_loc is freely reachable from spawn */
      expect(state.locations.has(makeLocation('locked_loc', 0))).toBe(true);
      /* reward_loc requires the sword from locked_loc — it should be reached recursively */
      expect(state.locations.has(makeLocation('reward_loc', 0))).toBe(true);
    });
  });

  describe('event gating', () => {
    it('event from one area unlocks an exit in the same area', () => {
      const world = makeWorld({
        'OOT SPAWN': makeArea({
          events: { 'MY_EVENT': exprTrue() },
          exits: { 'OOT Area A': exprEvent('MY_EVENT') },
        }),
        'OOT Area A': makeArea({ locations: { 'loc1': exprTrue() } }),
      }, { 'loc1': makeCheck() });
      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null, { recursive: true });
      expect(state.locations.has(makeLocation('loc1', 0))).toBe(true);
    });

    it('cannot reach an area behind an event that is never set', () => {
      const world = makeWorld({
        'OOT SPAWN': makeArea({
          exits: { 'OOT Area A': exprEvent('MISSING_EVENT') },
        }),
        'OOT Area A': makeArea({ locations: { 'loc1': exprTrue() } }),
      }, { 'loc1': makeCheck() });
      const pf = new Pathfinder([world], SETTINGS, new Map());
      const state = pf.run(null, { recursive: true });
      expect(state.locations.has(makeLocation('loc1', 0))).toBe(false);
    });
  });

  describe('no-logic mode', () => {
    it('makes all check locations reachable regardless of conditions', () => {
      const noLogicSettings = makeSettings({ ...SETTINGS, logic: 'none' });
      const world = makeWorld({
        'OOT SPAWN': makeArea({ exits: { 'OOT Area A': exprFalse() } }),
        'OOT Area A': makeArea({ locations: { 'loc1': exprFalse() } }),
      }, { 'loc1': makeCheck() });
      const pf = new Pathfinder([world], noLogicSettings, new Map());
      const state = pf.run(null);
      expect(state.locations.has(makeLocation('loc1', 0))).toBe(true);
      expect(state.goal).toBe(true);
    });
  });
});

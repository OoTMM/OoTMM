import type { TypeDef } from './Components';
import { TypeCard, TypeIndex, TypeRef, t } from './Components';

const TYPES: TypeDef[] = [
  {
    name: 'Event',
    kind: 'union',
    description: <p>An event that has occurred in-game.</p>,
    discriminator: 'type',
    of: ['EventGameStart', 'EventGameEnd', 'EventInfoItem', 'EventInfoEntrance'],
  },
  {
    name: 'EventType',
    kind: 'enum',
    description: <p>Uniquely identifies the type of an <TypeRef name="Event"/>, and which structure it has.</p>,
    values: [
      { value: 'GAME_START', ref: 'EventGameStart' },
      { value: 'GAME_END', ref: 'EventGameEnd' },
      { value: 'INFO_ITEM', ref: 'EventInfoItem' },
      { value: 'INFO_ENTRANCE', ref: 'EventInfoEntrance' },
    ],
  },
  {
    name: 'EventGameStart',
    kind: 'struct',
    description: <p>Indicates that a game has started, that is, the player has loaded a save file and gained control of link.</p>,
    fields: [
      { name: 'type', type: t.lit('GAME_START') },
      { name: 'sessionId', type: t.string },
      { name: 'playerId', type: t.string },
      { name: 'playerName', type: t.string },
      { name: 'worldId', type: t.number },
    ],
    example: {
      type: 'GAME_START',
      sessionId: '0123456789abcdef0123456789abcdef',
      playerId: 'fedcba9876543210fedcba9876543210',
      playerName: 'Link',
      worldId: 1,
    },
  },
  {
    name: 'EventGameEnd',
    kind: 'struct',
    description: <p>Indicates that a game has ended, that is, the player has lost control of link and went back to the title screen or file select menu (or closed the game altogether).</p>,
    fields: [
      { name: 'type', type: t.lit('GAME_END') },
    ],
  },
  {
    name: 'EventInfoItem',
    kind: 'struct',
    description: <p>
      Sent when the player collects an item (for themselves).<br/>
      The <code>location</code> field will not be present for some items that don't map to any check (for example, the ganon boss key received through special conditions).<br/>
      <code>self</code> will be true if the collected item belong to the same player, false otherwise.<br/><br/>
      Valid item values can be found <a href="https://github.com/OoTMM/OoTMM/blob/master/data/defs/gi.yml">here</a>.
    </p>,
    fields: [
      { name: 'type', type: t.lit('INFO_ITEM') },
      { name: 'item', type: t.string },
      { name: 'location', type: t.string, optional: true },
      { name: 'self', type: t.boolean }
    ],
    example: {
      type: 'INFO_ITEM',
      item: 'OOT_BOMBS_5',
      location: 'OOT Deku Tree Map Chest',
      self: true,
    },
  },
  {
    name: 'EventInfoEntrance',
    kind: 'struct',
    description:
        <p>Sent when the player enters a new area.<br/>The <code>original</code> field will not be present on respawns, age swaps, etc. <code>entrance</code> and <code>original</code> will be identical if the entrance is not shuffled.</p>,
    fields: [
      { name: 'type', type: t.lit('INFO_ENTRANCE') },
      { name: 'entrance', type: t.string },
      { name: 'original', type: t.string, optional: true },
      { name: 'age', type: t.union(t.lit('CHILD'), t.lit('ADULT')) },
    ],
    example: {
      type: 'INFO_ENTRANCE',
      entrance: 'OOT_DEKU_TREE',
      original: 'OOT_DODONGO_CAVERN',
      age: 'CHILD',
    },
  },
];

export const ClientAPI = () => {
  return (
    <div className="pb-16">
      <h1 className="text-3xl font-bold mb-2">Client API</h1>
      <p className="mb-6 text-gray-600 dark:text-gray-400">
        Types are shown as annotated JSON. Fields marked with <code className="font-mono">?</code> may be omitted.
        Click a type name to jump to its definition.
      </p>

      <h2 className="text-xl font-semibold mb-3">Types</h2>
      <TypeIndex defs={TYPES}/>
      {TYPES.map(def => <TypeCard key={def.name} def={def}/>)}
    </div>
  );
};

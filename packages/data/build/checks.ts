import fs from 'node:fs/promises';
import path from 'node:path';
import { XMLParser } from 'fast-xml-parser';
import { gameId } from '../lib/game';
import { DATA_DIR } from './helpers';

type BuildChecksState = {
  scenes: any;
  npcs: any;
};

const OV_VALUES = {
  chest: 0x01,
  collectible: 0x02,
  npc: 0x03,
  gs: 0x04,
  sf: 0x05,
  cow: 0x06,
  shop: 0x07,
  scrub: 0x08,
  sr: 0x09,
  fish: 0x0a,
  xflag: 0x10,
};

function makeOvKey(game: 'oot' | 'mm', ov: number, sceneId: number, value: number): number {
  const gameMask = game === 'mm' ? 0x80000000 : 0;
  return (((ov & 0x7f) << 24) | ((sceneId & 0xff) << 16) | (value & 0xffff) | gameMask) >>> 0;
}

type OvKeyXflagParams = {
  game: 'oot' | 'mm';
  sceneId: number;
  sliceId: number;
  roomId: number;
  setupId: number;
  actorId: number;
};
export function makeOvKeyXflag(params: OvKeyXflagParams) {
  const roomSetup = (params.roomId | ((params.setupId & 3) << 6)) & 0xff;
  const ovValue = OV_VALUES.xflag + params.sliceId;
  return makeOvKey(params.game, ovValue, params.sceneId, (roomSetup << 8) | params.actorId);
}

function sceneLookup(scene: string, state: BuildChecksState): number {
  const id = state.scenes[scene];
  if (id === undefined) {
    throw new Error(`Unknown scene ${scene}`);
  }
  return id;
}

function npcLookup(npc: string, state: BuildChecksState): number {
  const id = state.npcs[npc];
  if (id === undefined) {
    throw new Error(`Unknown npc ${npc}`);
  }
  return id;
}

export async function extractEntries(filepath: string, state: BuildChecksState) {
  /* Parse the XML file */
  const entries: any = [];
  const data = await fs.readFile(filepath, 'utf-8');
  const parser = new XMLParser({
    ignoreAttributes: false,
    preserveOrder: true,
    attributeNamePrefix: '',
  });
  const xml = parser.parse(data);
  const xmlRoot = xml.find((e: any) => e['checks']);
  const game = xmlRoot[':@'].game;
  for (const xmlScene of xmlRoot.checks) {
    let scene = xmlScene[':@'].id;
    if (scene !== 'NONE') {
      scene = gameId(game, scene, '_');
    }
    const children = xmlScene['scene'];
    for (const xmlCheck of children) {
      const ov = Object.keys(xmlCheck).find(k => k !== ':@')!;
      const attrs = xmlCheck[':@'];
      const location = gameId(game, attrs['location'], ' ');
      const type = attrs['type'] ?? ov;
      let item = attrs['item'];
      if (item !== 'NOTHING') {
        item = gameId(game, item, '_');
      }
      let hint = attrs['hint'];
      if (hint) {
        hint = gameId(game, hint, '_');
      }
      let key: number;

      switch (ov) {
      case 'chest':
      case 'collectible':
      case 'sf':
        {
          const sceneId = sceneLookup(scene, state);
          const value = parseInt(attrs['flag']);
          key = makeOvKey(game, OV_VALUES[ov], sceneId, value);
        }
        break;
      case 'npc':
        {
          const value = npcLookup(gameId(game, attrs['npc'], '_'), state);
          key = makeOvKey(game, OV_VALUES[ov], 0, value);
        }
        break;
      case 'gs':
      case 'cow':
      case 'shop':
      case 'scrub':
      case 'sr':
      case 'fish':
        key = makeOvKey(game, OV_VALUES[ov], 0, parseInt(attrs['flag']));
        break;
      case 'xflag':
        {
          const sceneId = sceneLookup(scene, state);
          const sliceId = parseInt(attrs['slice']);
          const roomId = parseInt(attrs['room']);
          const setupId = parseInt(attrs['setup']);
          const actorId = parseInt(attrs['actor']);
          key = makeOvKeyXflag({ game, sceneId, sliceId, roomId, setupId, actorId });
        }
        break;
      default:
        throw new Error(`Unknown ov type ${ov}`);
      }

      entries.push({
        game,
        ov,
        type,
        location,
        key,
        item,
        hint,
        scene,
      });
    }
  }

  return entries;
}

export async function parseChecks(state: BuildChecksState): Promise<any> {
  /* Detect XML files */
  const inputDir = path.join(DATA_DIR, 'checks');
  const filepaths: string[] = [];
  for await (const entry of fs.glob('**/*.xml', { cwd: inputDir })) {
    filepaths.push(entry);
  }
  filepaths.sort();

  /* Collect entries */
  const entries: any = [];
  for (const f of filepaths) {
    const filepath = path.resolve(inputDir, f);
    const newEntries = await extractEntries(filepath, state);
    entries.push(...newEntries);
  }

  /* Detect duplicates */
  const set = new Set<number>();
  for (const entry of entries) {
    if (set.has(entry.key)) {
      const matching = entries.filter((e: any) => e.key === entry.key).map((e: any) => e.location);
      console.error(`Duplicate check key ${entry.key} for locations ${matching.join(', ')}`);
      process.exit(1);
    }
    set.add(entry.key);
  }

  return entries;
}


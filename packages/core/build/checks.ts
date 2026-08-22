import { XMLParser } from 'fast-xml-parser';
import { readFile } from 'node:fs/promises';
import { gameId } from '../src/util';

type BuildChecksState = {
  scenes: any;
  npcs: any;
};

function makeOvKey(ov: number, sceneId: number, value: number): number {
  return ((ov & 0xff) << 24) | ((sceneId & 0xff) << 16) | (value & 0xffff);
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

export async function buildGameChecks(game: 'oot' | 'mm', state: BuildChecksState) {
  /* Parse the XML file */
  const entries: any = [];
  const file = __dirname + `/../../../data/checks/checks_${game}.xml`;
  const data = await readFile(file, 'utf-8');
  const parser = new XMLParser({
    ignoreAttributes: false,
    preserveOrder: true,
    attributeNamePrefix: '',
  });
  const [_, xmlRoot] = parser.parse(data);
  for (const xmlScene of xmlRoot.checks) {
    const scene = gameId(game, xmlScene[':@'].id, '_');
    const children = xmlScene['scene'];
    for (const xmlCheck of children) {
      const ov = Object.keys(xmlCheck)[0];
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
        {
          const sceneId = sceneLookup(scene, state);
          const value = parseInt(attrs['flag']);
          key = makeOvKey(OV_VALUES[ov], sceneId, value);
        }
        break;
      case 'npc':
        {
          const value = npcLookup(gameId(game, attrs['npc'], '_'), state);
          key = makeOvKey(OV_VALUES[ov], 0, value);
        }
        break;
      case 'gs':
      case 'sf':
      case 'cow':
      case 'shop':
      case 'scrub':
      case 'sr':
      case 'fish':
        key = makeOvKey(OV_VALUES[ov], 0, parseInt(attrs['flag']));
        break;
      case 'xflag':
        {
          const sceneId = sceneLookup(scene, state);
          const sliceId = parseInt(attrs['slice']);
          const roomId = parseInt(attrs['room']);
          const actorId = parseInt(attrs['actor']);
          const ovValue = OV_VALUES[ov] + sliceId;
          key = makeOvKey(ovValue, sceneId, (roomId << 8) | actorId);
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

export async function buildChecks(state: BuildChecksState): Promise<any> {
  const [oot, mm] = await Promise.all([
    buildGameChecks('oot', state),
    buildGameChecks('mm', state),
  ]);

  return [...oot, ...mm];
}


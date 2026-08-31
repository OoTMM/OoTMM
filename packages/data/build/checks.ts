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

class ChecksBuilder {
  private nextXflagId: number;
  private checks: any[];
  private matches: Record<number, number>;

  constructor(private state: BuildChecksState) {
    this.nextXflagId = 0;
    this.checks = [];
    this.matches = {};
  }

  private async process(filepath: string) {
    /* Parse the XML file */
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
            const sceneId = sceneLookup(scene, this.state);
            const value = parseInt(attrs['flag']);
            key = makeOvKey(game, OV_VALUES[ov], sceneId, value);
          }
          break;
        case 'npc':
          {
            const value = npcLookup(gameId(game, attrs['npc'], '_'), this.state);
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
            const xflagId = this.nextXflagId++;
            key = makeOvKey(game, OV_VALUES.xflag, 0, xflagId);
            const sceneId = sceneLookup(scene, this.state);
            const sliceId = parseInt(attrs['slice']);
            const roomId = parseInt(attrs['room']);
            const setupId = parseInt(attrs['setup']);
            const actorId = parseInt(attrs['actor']);
            let matchId = (actorId & 0xff) | ((sliceId & 0xf) << 8) | ((roomId & 0x3f) << 12) | ((setupId & 0x3) << 18) | ((sceneId & 0xff) << 20);
            if (game === 'mm') {
              matchId = (matchId | 0x80000000) >>> 0;
            }
            if (this.matches[matchId] !== undefined) {
              console.error(`Duplicate xflag match for scene ${scene} slice ${sliceId} room ${roomId} setup ${setupId} actor ${actorId}`);
              process.exit(1);
            }
            this.matches[matchId] = xflagId;
          }
          break;
        default:
          throw new Error(`Unknown ov type ${ov}`);
        }

        this.checks.push({ game, ov, type, location, key, item, hint, scene });
      }
    }
  }

  async run() {
    /* Detect XML files */
    const inputDir = path.join(DATA_DIR, 'checks');
    const filepaths: string[] = [];
    for await (const entry of fs.glob('**/*.xml', { cwd: inputDir })) {
      filepaths.push(entry);
    }
    filepaths.sort();

    /* Process files */
    for (const f of filepaths) {
      const filepath = path.resolve(inputDir, f);
      await this.process(filepath);
    }

    /* Sort checks by key */
    this.checks.sort((a, b) => a.key - b.key);

    /* Detect duplicates */
    const set = new Set<number>();
    for (const check of this.checks) {
      if (set.has(check.key)) {
        const matching = this.checks.filter((e: any) => e.key === check.key).map((e: any) => e.location);
        console.error(`Duplicate check key ${check.key} for locations ${matching.join(', ')}`);
        process.exit(1);
      }
      set.add(check.key);
    }

    return { checks: this.checks, matches: this.matches };
  }
}

export async function parseChecks(state: BuildChecksState): Promise<any> {
  const builder = new ChecksBuilder(state);
  return builder.run();
}

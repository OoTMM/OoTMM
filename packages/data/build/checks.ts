import type { Game } from '../lib/game';

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

function makeOvKey(game: 'oot' | 'mm', ov: number, sceneId: number | undefined, value: number): number {
  if (sceneId === undefined) {
    throw new Error(`Scene ID is required for ov type ${ov}`);
  }
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

type CheckContext = {
  scene?: string;
  sceneId?: number;
  setupId?: number;
  roomId?: number;
  sliceId?: number;
  actorId?: number;
};

type XmlElement = {
  tag: string;
  attributes: Record<string, string>;
  children: XmlElement[];
};

function xmlNormalize(nodes: any[]): XmlElement[] {
  const elements: XmlElement[] = [];
  for (const node of nodes) {
    const tag = Object.keys(node).find(k => k !== ':@')!;
    if (tag === '#text') {
      continue;
    }
    elements.push({
      tag,
      attributes: node[':@'] ?? {},
      children: xmlNormalize(node[tag] ?? []),
    });
  }
  return elements;
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

  private enrich(game: Game, oldCtx: CheckContext, attrs: Record<string, string>): CheckContext {
    const newCtx: CheckContext = { ...oldCtx };
    const scene = attrs['scene'];
    const setup = attrs['setup'];
    const room = attrs['room'];
    const slice = attrs['slice'];
    const actor = attrs['actor'];

    if (scene !== undefined) {
      if (scene === 'NONE') {
        newCtx.scene = 'NONE';
        newCtx.sceneId = -1;
      } else {
        newCtx.scene = gameId(game, scene, '_');
        newCtx.sceneId = sceneLookup(newCtx.scene, this.state);
      }
    }

    if (setup !== undefined) {
      newCtx.setupId = parseInt(setup);
    }

    if (room !== undefined) {
      newCtx.roomId = parseInt(room);
    }

    if (slice !== undefined) {
      newCtx.sliceId = parseInt(slice);
    }

    if (actor !== undefined) {
      newCtx.actorId = parseInt(actor);
    }

    return newCtx;
  }

  private match(game: Game, ctx: CheckContext, id: number) {
    if (ctx.sceneId === undefined) {
      throw new Error(`Scene ID is required for match`);
    }

    const setupId = ctx.setupId ?? 0;
    const roomId = ctx.roomId ?? 0;
    const sliceId = ctx.sliceId ?? 0;
    const actorId = ctx.actorId ?? 0;

    let matchId = (actorId & 0xff) | ((sliceId & 0xf) << 8) | ((roomId & 0x3f) << 12) | ((setupId & 0x3) << 18) | ((ctx.sceneId & 0xff) << 20);
    if (game === 'mm') {
      matchId = (matchId | 0x80000000) >>> 0;
    }

    if (this.matches[matchId] !== undefined) {
      console.error(`Duplicate xflag match for scene ${ctx.sceneId} slice ${sliceId} room ${roomId} setup ${setupId} actor ${actorId}`);
      process.exit(1);
    }
    this.matches[matchId] = id;
  }

  private pushCheck(game: Game, ctx: CheckContext, ov: string, attrs: Record<string, string>, key: number) {
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

    this.checks.push({ game, ov, type, location, key, item, hint, scene: ctx.scene });
  }

  private processXmlActor(game: Game, ctx: CheckContext, xml: XmlElement) {
    const actorCtx = this.enrich(game, ctx, xml.attributes);
    const matches: Record<string, string>[] = [];
    const xflags: XmlElement[] = [];

    for (const child of xml.children) {
      switch (child.tag) {
      case 'match':
        matches.push(child.attributes);
        break;
      case 'xflag':
        xflags.push(child);
        break;
      default:
        throw new Error(`Unexpected <${child.tag}> inside <actor>`);
      }
    }

    /* An actor block without any match matches on its own attributes */
    if (matches.length === 0) {
      matches.push({});
    }

    for (const xflag of xflags) {
      const xflagCtx = this.enrich(game, actorCtx, xflag.attributes);
      const xflagId = this.nextXflagId++;
      const key = makeOvKey(game, OV_VALUES.xflag, 0, xflagId);

      /* Matches take precedence over the xflag itself */
      for (const m of matches) {
        this.match(game, this.enrich(game, xflagCtx, m), xflagId);
      }

      this.pushCheck(game, xflagCtx, 'xflag', xflag.attributes, key);
    }
  }

  private processXmlSceneEntry(game: Game, ctx: CheckContext, xml: XmlElement) {
    const ov = xml.tag;
    const attrs = xml.attributes;
    const checkCtx = this.enrich(game, ctx, attrs);
    let key: number;

    switch (ov) {
    case 'chest':
    case 'collectible':
    case 'sf':
      {
        const value = parseInt(attrs['flag']);
        key = makeOvKey(game, OV_VALUES[ov], checkCtx.sceneId, value);
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
        this.match(game, checkCtx, xflagId);
      }
      break;
    default:
      throw new Error(`Unknown ov type ${ov}`);
    }

    this.pushCheck(game, checkCtx, ov, attrs, key);
  }

  private processXmlScene(game: Game, xml: XmlElement) {
    const sceneCtx = this.enrich(game, {}, { scene: xml.attributes.id });
    for (const xmlCheck of xml.children) {
      if (xmlCheck.tag === 'actor') {
        this.processXmlActor(game, sceneCtx, xmlCheck);
      } else {
        this.processXmlSceneEntry(game, sceneCtx, xmlCheck);
      }
    }
  }

  private processXml(xml: XmlElement[]) {
    const xmlRoot = xml.find(e => e.tag === 'checks');
    if (!xmlRoot) {
      throw new Error(`Missing checks root element`);
    }
    const game = xmlRoot.attributes.game as Game;
    for (const xmlScene of xmlRoot.children) {
      this.processXmlScene(game, xmlScene);
    }
  }

  private async process(filepath: string) {
    /* Parse the XML file */
    const data = await fs.readFile(filepath, 'utf-8');
    const parser = new XMLParser({
      ignoreAttributes: false,
      preserveOrder: true,
      attributeNamePrefix: '',
    });
    const xml = xmlNormalize(parser.parse(data));
    this.processXml(xml);
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

import { XMLParser } from 'fast-xml-parser';
import { promises as fs } from 'fs';
import path from 'path';

import { Game } from '../combo/config';
import { CodeGen } from '../combo/util/codegen';

const DIR_OUT_BUILD = path.resolve(__dirname, '../../build');
const DIR_OUT_INCLUDES = path.resolve(path.join(DIR_OUT_BUILD, 'include/assets'));

function ensureArray<T>(value: T | T[]): T[] {
  return Array.isArray(value) ? value : [value];
}

type AddrMap = Map<string, number>;

const RES_TYPES = ['DList', 'Texture', 'Collision', 'Skeleton', 'Animation', 'PlayerAnimation', 'Array'];

async function extractAssetsMetadata(game: Game, srcType: string, file: string, addrMap: AddrMap) {
  const outDir = path.join(DIR_OUT_INCLUDES, game, srcType);
  await fs.mkdir(outDir, { recursive: true });
  const basename = path.basename(file, '.xml');
  const cgGuard = `COMBO_${srcType.toUpperCase()}_${game.toUpperCase()}_${basename.toUpperCase()}_H`;
  const cg = new CodeGen(path.join(outDir, `${basename}.h`), cgGuard);
  cg.include('combo/types.h');
  const data = await fs.readFile(file, 'utf8');
  const parser = new XMLParser({
    ignoreAttributes: false,
  });
  const parsed = parser.parse(data);
  const nodeRoot = parsed['Root'];
  if (!nodeRoot) {
    return;
  }
  const nodeFiles = ensureArray(nodeRoot['File'] || []);
  for (const nodeFile of nodeFiles) {
    const seg = Number(nodeFile['@_Segment']);
    for (const type of RES_TYPES) {
      const nodes = ensureArray(nodeFile[type] || []);
      for (const node of nodes) {
        const name = node['@_Name'];
        const offset = Number(node['@_Offset']);
        const addr = (seg << 24 | offset) >>> 0;
        let cType = 'u8';
        switch (type) {
        case 'DList':
          cType = 'Gfx';
          break;
        }
        cg.raw(`extern ${cType} ${name}[];`);
        if (addrMap.has(name)) {
          console.error(`Duplicate asset name: ${name}`);
          process.exit(1);
        }
        addrMap.set(name, addr);
      }
    }
  }
  await cg.emit();
}

async function buildAssetsMapLinkScript(game: Game, addrMap: AddrMap) {
  const outFile = path.join(DIR_OUT_BUILD, `link_assets_${game}.ld`);
  const cg = new CodeGen(outFile);
  const sortedKeys = Array.from(addrMap.keys()).sort();
  for (const key of sortedKeys) {
    const addr = addrMap.get(key)!;
    cg.raw(`${key} = 0x${addr.toString(16)};`);
  }
  await cg.emit();
}

async function buildAssetsMap(game: Game) {
  const addrMap: AddrMap = new Map();
  const srcDir = path.resolve(__dirname, `../../data/setup/${game}`);
  const dirs = await fs.readdir(srcDir);
  await Promise.all(dirs.map(async (dir) => {
    const ddir = path.join(srcDir, dir);
    const files = await fs.readdir(ddir);
    await Promise.all(files.map(async (file) => {
      if (file.endsWith('.xml')) {
        await extractAssetsMetadata(game, dir, path.join(ddir, file), addrMap);
      }
    }));
  }));
  await buildAssetsMapLinkScript(game, addrMap);
}

export async function setupAssetsMap() {
  await fs.mkdir(DIR_OUT_INCLUDES, { recursive: true });
  await Promise.all([
    buildAssetsMap('oot'),
    buildAssetsMap('mm'),
  ]);
}

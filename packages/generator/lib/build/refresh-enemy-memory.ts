import { promises as fs } from 'node:fs';
import path from 'node:path';

type Cost = { code: number; instance: number };
const align16 = (n: number) => Math.ceil(n / 16) * 16;

export async function refreshEnemyMemory(binDir: string, factsPath: string, checkOnly = false) {
  const original = await fs.readFile(factsPath, 'utf8');
  const bounds = /('(?<name>ovl\/[^']+\.zovlx)': \{ 'memory': )(?<memory>\d+), 'profileOffset': (?<profile>\d+), 'instance': (?<instance>\d+) \}/g;
  const foreign = new Map<string, Cost>();
  const standalone = new Map<string, Cost>();
  const helpers = new Map<string, Cost>();
  const replacements = new Map<string, string>();
  const record = (map: Map<string, Cost>, key: string, code: number, instance: number) => {
    const previous = map.get(key);
    map.set(key, { code: Math.max(code, previous?.code ?? 0), instance: Math.max(instance, previous?.instance ?? 0) });
  };

  for (const match of original.matchAll(bounds)) {
    const { name, memory, profile, instance } = match.groups!;
    const raw = await fs.readFile(path.join(binDir, name));
    if (raw.length < 64) throw new Error(`Invalid overlay header: ${name}`);
    const start = raw.readUInt32BE(0), end = raw.readUInt32BE(4);
    const kind = raw.readUInt32BE(8), actor = raw.readUInt32BE(12);
    const offset = raw.readUInt32BE(16) - start;
    const data = raw.subarray(64);
    if (end <= start || offset < 0 || offset % 4 || offset + 32 > data.length)
      throw new Error(`Invalid actor profile: ${name}`);
    const size = data.readUInt32BE(offset + 12);
    if (!size || size > 0xffff) throw new Error(`Invalid actor instance: ${name}`);
    const code = Math.max(Number(memory), align16(Math.max(end - start, data.length)));
    const instanceBound = Math.max(Number(instance), size);
    const next = `${match[1]}${code}, 'profileOffset': ${profile}, 'instance': ${instanceBound} }`;
    replacements.set(name, next);
    if (next !== match[0]) console.log(`Enemy memory: ${name}: ${memory}/${profile}/${instance} -> ${code}/${offset}/${instanceBound}`);

    const game = name.split('/')[1];
    const stem = path.basename(name, '.zovlx').split('_ovl_')[1];
    const port = /^Native_Enemy_(oot|mm)_/.exec(stem);
    if (port) {
      if (kind !== 4 || port[1] === game || actor >= 0x400) throw new Error(`Invalid native port: ${name}`);
      record(foreign, `${port[1]}/${actor}`, code, instanceBound);
    } else if (stem.startsWith('Standalone_')) {
      record(standalone, `${game}/${stem.slice('Standalone_'.length)}`, code + 16, align16(instanceBound) + 16);
    } else if (stem.startsWith('Enemy_') || stem === 'Custom_Enemy_Arena') {
      const helper = stem === 'Custom_Enemy_Arena' ? 'Arena' : stem.slice('Enemy_'.length).replace('Poe_Encounter', 'PoeEncounter');
      record(helpers, `${game}/${helper}`, code + 16, align16(instanceBound) + 16);
    }
  }
  if (!replacements.size) throw new Error('No enemy overlay bounds found');
  let updated = original.replace(bounds, (...args) => replacements.get(args.at(-1).name)!);

  for (const [section, costs] of [['foreign', foreign], ['standalone', standalone], ['helpers', helpers]] as const) {
    const pattern = new RegExp(`('${section}': \\{\\n)([\\s\\S]*?)(\\n  \\},)`);
    if (!pattern.test(updated)) throw new Error(`Missing enemy memory section: ${section}`);
    const seen = new Set<string>();
    updated = updated.replace(pattern, (_whole, prefix: string, body: string, suffix: string) => {
      const rows = /('([^']+)': \{ (?:'object': \d+, )?'code': )(\d+), 'instance': (\d+)/g;
      return prefix + body.replace(rows, (whole, start: string, key: string, code: string, instance: string) => {
        const cost = costs.get(key);
        if (!cost) return whole;
        seen.add(key);
        return `${start}${Math.max(Number(code), cost.code)}, 'instance': ${Math.max(Number(instance), cost.instance)}`;
      }) + suffix;
    });
    for (const key of costs.keys()) if (!seen.has(key)) throw new Error(`Missing ${section} memory cost: ${key}`);
  }

  if (updated !== original) {
    if (checkOnly) throw new Error('Enemy memory facts are stale. Run the generator build locally and commit packages/core/src/enemy-memory-data.ts');
    await fs.writeFile(factsPath, updated);
    console.log('Refreshed enemy-memory-data.ts; review and commit the changes.');
  }
  console.log(`Checked ${replacements.size} enemy overlay memory bounds.`);
}

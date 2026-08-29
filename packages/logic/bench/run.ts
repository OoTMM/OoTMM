import type { PartialDeep } from 'type-fest';
import type { Settings } from '@ootmm/core';

import { writeFileSync } from 'node:fs';
import { merge } from 'lodash-es';
import { Monitor, DEFAULT_SETTINGS, makeRandomSettings, makeCosmetics } from '@ootmm/core';

import { logic, solvedWorldState } from '../src/pipeline';
import { DEFAULT_CONFIG_NAMES, resolveConfigs } from './configs';

export type Sample = {
  config: string;
  seed: string;
  ms: number;
  ok: boolean;
  error?: string;
};

export type BenchResult = {
  node: string;
  full: boolean;
  samples: Sample[];
};

type Options = {
  configs: string[];
  seeds: number;
  reps: number;
  full: boolean;
  json?: string;
  quiet: boolean;
};

function parseArgs(argv: string[]): Options {
  const opts: Options = { configs: DEFAULT_CONFIG_NAMES, seeds: 3, reps: 3, full: false, quiet: false };
  for (let i = 0; i < argv.length; ++i) {
    const arg = argv[i];
    switch (arg) {
    case '--config': opts.configs = argv[++i].split(','); break;
    case '--seeds': opts.seeds = Number(argv[++i]); break;
    case '--reps': opts.reps = Number(argv[++i]); break;
    case '--json': opts.json = argv[++i]; break;
    case '--full': opts.full = true; break;
    case '--quiet': opts.quiet = true; break;
    default: throw new Error(`Unknown argument '${arg}'`);
    }
  }
  return opts;
}

async function generate(seed: string, settings: PartialDeep<Settings>, full: boolean) {
  const monitor = new Monitor({ onLog: () => {} });
  const opts = {
    cosmetics: makeCosmetics({}),
    seed,
    settings: merge({}, DEFAULT_SETTINGS, settings, { probabilisticFoolish: false }) as Settings,
    random: makeRandomSettings({}),
    mode: 'create' as const,
  };
  if (full) {
    await logic(monitor, opts);
  } else {
    await solvedWorldState(monitor, opts);
  }
}

const seedName = (config: string, i: number) => `BENCH-${config.toUpperCase()}-${i}`;

export function summarise(samples: Sample[]) {
  const byKey = new Map<string, number[]>();
  for (const s of samples) {
    if (!s.ok) continue;
    const key = `${s.config}/${s.seed}`;
    const list = byKey.get(key) || [];
    list.push(s.ms);
    byKey.set(key, list);
  }
  return byKey;
}

function median(xs: number[]) {
  const s = [...xs].sort((a, b) => a - b);
  return s.length % 2 ? s[(s.length - 1) / 2] : (s[s.length / 2 - 1] + s[s.length / 2]) / 2;
}

async function main() {
  const opts = parseArgs(process.argv.slice(2));
  const configs = resolveConfigs(opts.configs);
  const samples: Sample[] = [];

  /* Warm the JIT */
  for (const [name, settings] of configs) {
    await generate(`WARM-${name}`, settings, opts.full);
  }

  for (let rep = 0; rep < opts.reps; ++rep) {
    for (const [name, settings] of configs) {
      for (let i = 0; i < opts.seeds; ++i) {
        const seed = seedName(name, i);
        const t0 = performance.now();
        let ok = true;
        let error: string | undefined;
        try {
          await generate(seed, settings, opts.full);
        } catch (e) {
          ok = false;
          error = e instanceof Error ? e.message : String(e);
        }
        const ms = performance.now() - t0;
        samples.push({ config: name, seed, ms, ok, error });
        if (!opts.quiet) {
          process.stdout.write(`${ok ? 'ok  ' : 'FAIL'} ${name}/${i} rep${rep} ${ms.toFixed(0)}ms${error ? ` - ${error}` : ''}\n`);
        }
      }
    }
  }

  const failures = samples.filter(s => !s.ok);
  if (!opts.quiet) {
    process.stdout.write('\n');
    for (const [key, times] of summarise(samples)) {
      const min = Math.min(...times);
      const spread = (Math.max(...times) - min) / min * 100;
      process.stdout.write(`${key.padEnd(28)} min=${min.toFixed(0).padStart(6)}ms  median=${median(times).toFixed(0).padStart(6)}ms  spread=${spread.toFixed(1)}%\n`);
    }
    process.stdout.write(`\n${samples.length} generations, ${failures.length} failed\n`);
  }

  if (opts.json) {
    const result: BenchResult = { node: process.version, full: opts.full, samples };
    writeFileSync(opts.json, JSON.stringify(result, null, 2));
  }

  process.exit(failures.length ? 1 : 0);
}

main();

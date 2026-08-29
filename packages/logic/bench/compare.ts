import type { BenchResult, Sample } from './run';

import { execFileSync } from 'node:child_process';
import { cpSync, mkdtempSync, readFileSync, rmSync } from 'node:fs';
import { tmpdir } from 'node:os';
import { join } from 'node:path';

type Options = {
  refA: string;
  refB: string;
  rounds: number;
  configs?: string;
  seeds: number;
  full: boolean;
};

function git(...args: string[]): string {
  return execFileSync('git', args, { encoding: 'utf8' }).trim();
}

function parseArgs(argv: string[]): Options {
  const positional: string[] = [];
  const opts: Partial<Options> = { rounds: 4, seeds: 3, full: false };
  for (let i = 0; i < argv.length; ++i) {
    const arg = argv[i];
    switch (arg) {
    case '--rounds': opts.rounds = Number(argv[++i]); break;
    case '--config': opts.configs = argv[++i]; break;
    case '--seeds': opts.seeds = Number(argv[++i]); break;
    case '--full': opts.full = true; break;
    default:
      if (arg.startsWith('-')) throw new Error(`Unknown argument '${arg}'`);
      positional.push(arg);
    }
  }
  if (positional.length !== 2) {
    throw new Error('usage: bench:compare <refA> <refB> [--rounds N] [--seeds N] [--config a,b] [--full]');
  }
  return { ...opts, refA: positional[0], refB: positional[1] } as Options;
}

function currentRef(): string {
  try {
    return git('symbolic-ref', '--short', 'HEAD');
  } catch {
    return git('rev-parse', 'HEAD');
  }
}

function runSide(runnerDir: string, opts: Options, out: string) {
  const args = ['--import', 'tsx', join(runnerDir, 'run.ts'), '--json', out, '--quiet', '--seeds', String(opts.seeds), '--reps', '1'];
  if (opts.configs) args.push('--config', opts.configs);
  if (opts.full) args.push('--full');
  try {
    execFileSync(process.execPath, args, { cwd: process.cwd(), stdio: ['ignore', 'ignore', 'inherit'] });
  } catch {
  }
  return (JSON.parse(readFileSync(out, 'utf8')) as BenchResult).samples;
}

const key = (s: Sample) => `${s.config}/${s.seed}`;
const pct = (n: number) => `${n >= 0 ? '+' : ''}${n.toFixed(2)}%`;

function median(xs: number[]) {
  const s = [...xs].sort((a, b) => a - b);
  return s.length % 2 ? s[(s.length - 1) / 2] : (s[s.length / 2 - 1] + s[s.length / 2]) / 2;
}

async function main() {
  const opts = parseArgs(process.argv.slice(2));

  if (git('status', '--porcelain', '--untracked-files=no')) {
    throw new Error('working tree has uncommitted changes - commit or stash before comparing');
  }

  const original = currentRef();
  const shaA = git('rev-parse', '--short', opts.refA);
  const shaB = git('rev-parse', '--short', opts.refB);

  const tmp = mkdtempSync(join(tmpdir(), 'ootmm-bench-'));
  const runnerDir = join(process.cwd(), '.bench-runner');
  cpSync(new URL('.', import.meta.url).pathname, runnerDir, { recursive: true });

  const restore = () => {
    try { git('checkout', '-q', original); } catch { /* best effort */ }
    rmSync(runnerDir, { recursive: true, force: true });
    rmSync(tmp, { recursive: true, force: true });
  };
  process.on('SIGINT', () => { restore(); process.exit(130); });

  const samples: Record<'A' | 'B', Sample[]> = { A: [], B: [] };
  try {
    for (let round = 0; round < opts.rounds; ++round) {
      for (const side of ['A', 'B'] as const) {
        const ref = side === 'A' ? opts.refA : opts.refB;
        git('checkout', '-q', ref);
        process.stdout.write(`round ${round + 1}/${opts.rounds}  ${side} (${ref})\n`);
        samples[side].push(...runSide(runnerDir, opts, join(tmp, `${side}-${round}.json`)));
      }
    }
  } finally {
    restore();
  }

  const cells = new Set([...samples.A, ...samples.B].map(key));
  const deltas: number[] = [];
  const byConfig = new Map<string, number[]>();
  const noise: Record<'A' | 'B', number[]> = { A: [], B: [] };

  process.stdout.write(`\n${'cell'.padEnd(30)}${'A min'.padStart(9)}${'B min'.padStart(9)}${'delta'.padStart(9)}${'A spread'.padStart(10)}${'B spread'.padStart(10)}\n`);
  for (const cell of [...cells].sort()) {
    const pick = (side: 'A' | 'B') => samples[side].filter(s => s.ok && key(s) === cell).map(s => s.ms);
    const a = pick('A');
    const b = pick('B');
    if (!a.length || !b.length) continue;

    const [minA, minB] = [Math.min(...a), Math.min(...b)];
    const spreadA = (Math.max(...a) - minA) / minA * 100;
    const spreadB = (Math.max(...b) - minB) / minB * 100;
    const delta = (minB - minA) / minA * 100;

    deltas.push(delta);
    noise.A.push(spreadA);
    noise.B.push(spreadB);
    const config = cell.split('/')[0];
    byConfig.set(config, [...(byConfig.get(config) || []), delta]);

    process.stdout.write(`${cell.padEnd(30)}${minA.toFixed(0).padStart(9)}${minB.toFixed(0).padStart(9)}${pct(delta).padStart(9)}${spreadA.toFixed(1).padStart(9)}%${spreadB.toFixed(1).padStart(9)}%\n`);
  }

  process.stdout.write(`\nA = ${opts.refA} (${shaA})\nB = ${opts.refB} (${shaB})\n\n`);
  for (const [config, ds] of byConfig) {
    process.stdout.write(`${config.padEnd(16)} median ${pct(median(ds))}\n`);
  }

  const floor = median([...noise.A, ...noise.B]);
  const overall = median(deltas);
  process.stdout.write(`${'OVERALL'.padEnd(16)} median ${pct(overall)}\n`);
  process.stdout.write(`\nmedian spread ${floor.toFixed(2)}%\n`);
  const failed = [...samples.A, ...samples.B].filter(s => !s.ok);
  if (failed.length) {
    process.stdout.write(`\n${failed.length} generation(s) FAILED:\n`);
    for (const f of failed.slice(0, 10)) process.stdout.write(`  ${key(f)}: ${f.error}\n`);
  }
}

main();

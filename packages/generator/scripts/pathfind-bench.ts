import { Pathfinder } from '@ootmm/logic';
import { Monitor } from '@ootmm/core';
import { solvedWorldState } from '../lib/combo/logic';
import { options } from '../lib/combo/options';

const SEED = 'BENCH';
const ITERATIONS = parseInt(process.argv[2] ?? '1000', 10);

function stats(times: number[]) {
  const sorted = [...times].sort((a, b) => a - b);
  const sum = sorted.reduce((a, b) => a + b, 0);
  const pct = (p: number) => sorted[Math.floor(sorted.length * p)];
  return {
    min: sorted[0],
    mean: sum / sorted.length,
    median: pct(0.5),
    p95: pct(0.95),
    p99: pct(0.99),
    max: sorted[sorted.length - 1],
  };
}

async function main() {
  const monitor = new Monitor({ onLog: () => {} });

  console.log('Building solved world...');
  const state = await solvedWorldState(monitor, options({ seed: SEED }));
  const pathfinder = new Pathfinder(state.worlds, state.settings, state.startingItems);

  console.log(`Running pathfinder ${ITERATIONS} times...`);
  const times: number[] = [];

  for (let i = 0; i < ITERATIONS; i++) {
    const start = performance.now();
    pathfinder.run(null, { items: state.items, recursive: true });
    times.push(performance.now() - start);
  }

  const s = stats(times);
  const fmt = (n: number) => n.toFixed(3) + 'ms';

  console.log(`\nResults (${ITERATIONS} iterations):`);
  console.log(`  min:    ${fmt(s.min)}`);
  console.log(`  mean:   ${fmt(s.mean)}`);
  console.log(`  median: ${fmt(s.median)}`);
  console.log(`  p95:    ${fmt(s.p95)}`);
  console.log(`  p99:    ${fmt(s.p99)}`);
  console.log(`  max:    ${fmt(s.max)}`);
}

main().catch(console.error);

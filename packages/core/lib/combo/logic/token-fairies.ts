import { stat } from "fs";
import { Random, randomInt } from "../random";
import { Settings } from "../settings";
import { World } from "./world";

/*copied from shop sanity, ty to Nax for all of this*/

const MM_WOODFALL_GREAT_FAIRY = 15;
const MM_SNOWHEAD_GREAT_FAIRY = 15;
const MM_GREAT_BAY_GREAT_FAIRY = 15;
const MM_STONE_TOWER_GREAT_FAIRY = 15;
const MM_GREAT_FAIRIES = [
  MM_WOODFALL_GREAT_FAIRY,
  MM_SNOWHEAD_GREAT_FAIRY,
  MM_GREAT_BAY_GREAT_FAIRY,
  MM_STONE_TOWER_GREAT_FAIRY,
];
const MM_SWAMP_SPIDER_HOUSE = 30;
const MM_OCEAN_SPIDER_HOUSE = 30;
const MM_SPIDER_HOUSES = [
  MM_SWAMP_SPIDER_HOUSE,
  MM_OCEAN_SPIDER_HOUSE,
]
const SKULL_STRAYS = {
  MM_GREAT_FAIRIES,
  MM_SPIDER_HOUSES,
}as const;

const DEFAULT_COUNTS = Object.values(SKULL_STRAYS).flat();

const BOTH_COUNTS = Object.fromEntries(Object.entries(SKULL_STRAYS).map(([a, b]) => [a, b.length]));

export const COUNT_RANGES: {[k: string]: number} = {};

(function() {
  let offset = 0;
  for (const k of Object.keys(BOTH_COUNTS)) {
    COUNT_RANGES[k] = offset;
    offset += (BOTH_COUNTS as any)[k];
  }
})();

export type CountRandoType = 'vanilla' | 'random' | 'wrandom' | 'fixed';

export function defaultStraySkulls() {
  return [...DEFAULT_COUNTS];
}
/* Approximate beta func */
function betaApproxInt(random: Random, alpha: number, beta: number, max: number) {
  const draws = alpha + beta - 1;
  const values: number[] = [];
  for (let i = 0; i < draws; ++i) {
    values.push(randomInt(random, max));
  }
  return (values.sort((a, b) => a - b))[alpha - 1];
}

export class LogicPassCounts {
  constructor(
    private state: {
      settings: Settings,
      worlds: World[],
      random: Random,
    }
  ) {
  }

  private randomCount() {
    let max = 30;
    const r = randomInt(this.state.random, max);
    return r;
  }

  private randomCountBeta() {
    let max = 30; //want to update to prioritize middle numbers, also prob bad in general
    let beta = 4;
    const r = betaApproxInt(this.state.random, 2, beta, max + 1);
    return r;
  }
  private adjustCountStray(type: CountRandoType, value: number) {
    switch (type) {
    case 'vanilla': return value;
    case 'fixed': return this.state.settings.strayCount;
    case 'random': return Math.floor(this.randomCount()/2);
    case 'wrandom': return Math.floor(this.randomCountBeta()/2);
    }
  }
  private adjustCountSkull(type: CountRandoType, value: number) {
    switch (type) {
    case 'vanilla': return value;
    case 'fixed': return this.state.settings.skullCount;
    case 'random': return this.randomCount();
    case 'wrandom': return this.randomCountBeta();
    }
  }

  private shuffleCounts(world: World, type: CountRandoType) {
    let strayCountnew = this.adjustCountStray(type, 15); //makes all of the fairies/skulls be the same, setting to turn off?
    let skullCountnew = this.adjustCountSkull(type, 30);
    this.state.settings.strayCount = strayCountnew;
    this.state.settings.skullCount = skullCountnew;
    for (let i = 0; i < 4; ++i) {
      world.straySkulls[i] = strayCountnew;
    }
    for (let i = 4; i < 6; ++i) {
      world.straySkulls[i] = skullCountnew;
    }
  }

  run() {
    for (const world of this.state.worlds) {
      this.shuffleCounts(world, this.state.settings.strayCountSetter);
      this.shuffleCounts(world, this.state.settings.skullCountSetter);
    }

    return {};
  }
}

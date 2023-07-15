import { Random, randomInt } from "../random";
import { Settings } from "../settings";
import { World } from "./world";

const OOT_SHOP_KOKIRI = [ 10, 20, 60, 30, 15, 30, 10, 40 ];
const OOT_SHOP_BOMBCHU = [ 180, 180, 180, 180, 100, 100, 100, 100 ];
const OOT_SHOP_ZORA = [ 50, 90, 200, 15, 20, 60, 300, 10 ];
const OOT_SHOP_GORON = [ 10, 10, 40, 200, 25, 50, 80, 120 ];
const OOT_SHOP_BAZAAR = [ 20, 60, 90, 10, 35, 10, 15, 80 ];
const OOT_SHOP_POTION = [ 200, 50, 30, 15, 300, 50, 30, 30 ];

const OOT_SHOPS = [
  ...OOT_SHOP_KOKIRI,
  ...OOT_SHOP_BOMBCHU,
  ...OOT_SHOP_ZORA,
  ...OOT_SHOP_GORON,
  ...OOT_SHOP_BAZAAR,
  ...OOT_SHOP_POTION,
  ...OOT_SHOP_BAZAAR,
  ...OOT_SHOP_POTION,
];

const OOT_SCRUBS_OVERWORLD = [ 40, 15, 20, 40, 40, 40, 40, 10, 20, 40, 40, 20, 40, 40, 40, 20, 40, 40, 40, 40, 20, 40, 40, 40, 40, 40, 40 ];
const OOT_SCRUBS_DT = [ 0 ];
const OOT_SCRUBS_DT_MQ = [ 50 ];
const OOT_SCRUBS_DC = [ 40, 15, 20, 50 ];
const OOT_SCRUBS_DC_MQ = [ 40, 15, 50, 40 ];
const OOT_SCRUBS_JJ = [ 20 ];
const OOT_SCRUBS_JJ_MQ = [ 0 ];
const OOT_SCRUBS_GC = [40, 40, 70, 40, 0];
const OOT_SCRUBS_GC_MQ = [40, 40, 70, 40, 20];

const OOT_SCRUBS = [
  ...OOT_SCRUBS_OVERWORLD,
  ...OOT_SCRUBS_DT,
  ...OOT_SCRUBS_DC,
  ...OOT_SCRUBS_JJ,
  ...OOT_SCRUBS_GC,
];

const MM_SHOP_BOMB = [30, 40, 50, 90];
const MM_SHOP_CURIOSITY = [500];
const MM_SHOP_TRADING = [30, 80, 80, 50, 10, 30, 30, 30];
const MM_SHOP_POTION = [60, 10, 20];
const MM_SHOP_GORON = [40, 40, 80];
const MM_SHOP_ZORA = [90, 20, 60];

const MM_SHOPS = [
  ...MM_SHOP_BOMB,
  ...MM_SHOP_CURIOSITY,
  ...MM_SHOP_TRADING,
  ...MM_SHOP_POTION,
  ...MM_SHOP_GORON,
  ...MM_SHOP_ZORA,
];

const MM_SHOP_EX_CURIOSITY = [100];

const MM_SHOPS_EX = [
  ...MM_SHOP_EX_CURIOSITY,
];

const MM_TINGLE = [5, 40, 20, 40, 20, 40, 20, 40, 20, 40, 20, 40];

const PRICES = {
  OOT_SHOPS,
  OOT_SCRUBS,
  MM_SHOPS,
  MM_SHOPS_EX,
  MM_TINGLE,
  MAX: [],
} as const;

const DEFAULT_PRICES = Object.values(PRICES).flat();

const PRICE_COUNTS = Object.fromEntries(Object.entries(PRICES).map(([a, b]) => [a, b.length]));

export const PRICE_RANGES: {[k: string] :number} = {};

(function() {
  let offset = 0;
  for (const k of Object.keys(PRICE_COUNTS)) {
    PRICE_RANGES[k] = offset;
    offset += (PRICE_COUNTS as any)[k];
  }
})();

export type PriceRandoType = 'vanilla' | 'affordable' | 'random' | 'weighted';

export function defaultPrices(mq: Set<string>) {
  const p = [...DEFAULT_PRICES];

  /* Handle MQ */
  const ootScrubs: number[][] = [];
  ootScrubs.push(OOT_SCRUBS_OVERWORLD);
  if (mq.has('DT')) {
    ootScrubs.push(OOT_SCRUBS_DT_MQ);
  } else {
    ootScrubs.push(OOT_SCRUBS_DT);
  }

  if (mq.has('DC')) {
    ootScrubs.push(OOT_SCRUBS_DC_MQ);
  } else {
    ootScrubs.push(OOT_SCRUBS_DC);
  }

  if (mq.has('JJ')) {
    ootScrubs.push(OOT_SCRUBS_JJ_MQ);
  } else {
    ootScrubs.push(OOT_SCRUBS_JJ);
  }

  if (mq.has('Ganon')) {
    ootScrubs.push(OOT_SCRUBS_GC_MQ);
  } else {
    ootScrubs.push(OOT_SCRUBS_GC);
  }

  const ootScrubsFlat = ootScrubs.flat();
  for (let i = 0; i < ootScrubsFlat.length; ++i) {
    p[PRICE_RANGES.OOT_SCRUBS + i] = ootScrubsFlat[i];
  }

  return p;
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

export class LogicPassPrice {
  constructor(
    private state: {
      settings: Settings,
      worlds: World[],
      random: Random,
    }
  ) {
  }

  private randomPrice() {
    let max = 100;
    if (this.state.settings.colossalWallets) {
      max = 199;
      if (this.state.settings.bottomlessWallets) {
        max = 999;
      }
    }
    const r = randomInt(this.state.random, max + 1);
    return r * 5;
  }

  private randomPriceBeta() {
    let max = 100;
    let beta = 5;
    if (this.state.settings.colossalWallets) {
      max = 199;
      beta = 9;
      if (this.state.settings.bottomlessWallets) {
        max = 999;
        beta = 15;
      }
    }
    const r = betaApproxInt(this.state.random, 2, beta, max + 1);
    return r * 5;
  }

  private adjustPrice(type: PriceRandoType, value: number) {
    switch (type) {
    case 'vanilla': return value;
    case 'affordable': return 10;
    case 'random': return this.randomPrice();
    case 'weighted': return this.randomPriceBeta();
    }
  }

  private shufflePrices(world: World, range: string, type: PriceRandoType) {
    const base = PRICE_RANGES[range];
    const size = PRICE_COUNTS[range];
    for (let i = 0; i < size; ++i) {
      const value = world.prices[base + i];
      world.prices[base + i] = this.adjustPrice(type, value);
    }
  }

  run() {
    for (const world of this.state.worlds) {
      this.shufflePrices(world, 'OOT_SHOPS', this.state.settings.priceOotShops);
      this.shufflePrices(world, 'OOT_SCRUBS', this.state.settings.priceOotScrubs);
      this.shufflePrices(world, 'MM_SHOPS', this.state.settings.priceMmShops);
      this.shufflePrices(world, 'MM_SHOPS_EX', this.state.settings.priceMmShops);
      this.shufflePrices(world, 'MM_TINGLE', this.state.settings.priceMmTingle);
    }

    return {};
  }
}

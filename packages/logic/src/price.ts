import type { Settings } from '@ootmm/core';
import type { World, ResolvedWorldFlags } from './world';

import { Random, randomInt } from '@ootmm/core';

export const PRICES_RAW = {
  OOT_SHOP_KOKIRI: [ 10, 20, 60, 30, 15, 30, 10, 40 ],
  OOT_SHOP_BOMBCHU: [ 180, 180, 180, 180, 100, 100, 100, 100 ],
  OOT_SHOP_ZORA: [ 50, 90, 200, 15, 20, 60, 300, 10 ],
  OOT_SHOP_GORON: [ 10, 10, 40, 200, 25, 50, 80, 120 ],
  OOT_SHOP_BAZAAR: [ 20, 60, 90, 10, 35, 10, 15, 80 ],
  OOT_SHOP_POTION: [ 200, 50, 30, 15, 300, 50, 30, 30 ],

  OOT_SCRUBS_OVERWORLD: [ 40, 15, 20, 40, 40, 40, 40, 10, 20, 40, 40, 20, 40, 40, 40, 20, 40, 40, 40, 40, 20, 40, 40, 40, 40, 40, 40 ],
  OOT_SCRUBS_DT: [ 0 ],
  OOT_SCRUBS_DT_MQ: [ 50 ],
  OOT_SCRUBS_DC: [ 40, 15, 20, 50 ],
  OOT_SCRUBS_DC_MQ: [ 40, 15, 50, 40 ],
  OOT_SCRUBS_JJ: [ 20 ],
  OOT_SCRUBS_JJ_MQ: [ 0 ],
  OOT_SCRUBS_GC: [40, 40, 70, 40, 0],
  OOT_SCRUBS_GC_MQ: [40, 40, 70, 40, 20],

  OOT_MERCHANTS: [200, 200, 100, 30],

  MM_SHOP_BOMB: [30, 40, 50, 90],
  MM_SHOP_CURIOSITY: [500],
  MM_SHOP_TRADING: [30, 80, 80, 50, 10, 30, 30, 30],
  MM_SHOP_POTION: [60, 10, 20],
  MM_SHOP_GORON: [40, 40, 80],
  MM_SHOP_ZORA: [90, 20, 60],

  MM_SHOP_EX_CURIOSITY: [100],

  MM_TINGLE: [5, 40, 20, 40, 20, 40, 20, 40, 20, 40, 20, 40],
};

const OOT_SHOPS = [
  ...PRICES_RAW.OOT_SHOP_KOKIRI,
  ...PRICES_RAW.OOT_SHOP_BOMBCHU,
  ...PRICES_RAW.OOT_SHOP_ZORA,
  ...PRICES_RAW.OOT_SHOP_GORON,
  ...PRICES_RAW.OOT_SHOP_BAZAAR,
  ...PRICES_RAW.OOT_SHOP_POTION,
  ...PRICES_RAW.OOT_SHOP_BAZAAR,
  ...PRICES_RAW.OOT_SHOP_POTION,
];

const OOT_SCRUBS = [
  ...PRICES_RAW.OOT_SCRUBS_OVERWORLD,
  ...PRICES_RAW.OOT_SCRUBS_DT,
  ...PRICES_RAW.OOT_SCRUBS_DC,
  ...PRICES_RAW.OOT_SCRUBS_JJ,
  ...PRICES_RAW.OOT_SCRUBS_GC,
];

const MM_SHOPS = [
  ...PRICES_RAW.MM_SHOP_BOMB,
  ...PRICES_RAW.MM_SHOP_CURIOSITY,
  ...PRICES_RAW.MM_SHOP_TRADING,
  ...PRICES_RAW.MM_SHOP_POTION,
  ...PRICES_RAW.MM_SHOP_GORON,
  ...PRICES_RAW.MM_SHOP_ZORA,
];

const MM_SHOPS_EX = [
  ...PRICES_RAW.MM_SHOP_EX_CURIOSITY,
];

const PRICES = {
  OOT_SHOPS,
  OOT_SCRUBS,
  OOT_MERCHANTS: PRICES_RAW.OOT_MERCHANTS,
  MM_SHOPS,
  MM_SHOPS_EX,
  MM_TINGLE: PRICES_RAW.MM_TINGLE,
  MAX: [],
} as const;

export const DEFAULT_PRICES = Object.values(PRICES).flat();

export const PRICE_COUNTS = Object.fromEntries(Object.entries(PRICES).map(([a, b]) => [a, b.length]));

export const PRICE_RANGES: {[k: string]: number} = {};

(function() {
  let offset = 0;
  for (const k of Object.keys(PRICE_COUNTS)) {
    PRICE_RANGES[k] = offset;
    offset += (PRICE_COUNTS as any)[k];
  }
})();


export type PriceRandoType = 'vanilla' | 'affordable' | 'random' | 'weighted';

export function defaultPrices(flags: ResolvedWorldFlags) {
  const p = [...DEFAULT_PRICES];
  const mq = flags.mqDungeons;

  /* Handle MQ */
  const ootScrubs: number[][] = [];
  ootScrubs.push(PRICES_RAW.OOT_SCRUBS_OVERWORLD);
  if (mq.has('DT')) {
    ootScrubs.push(PRICES_RAW.OOT_SCRUBS_DT_MQ);
  } else {
    ootScrubs.push(PRICES_RAW.OOT_SCRUBS_DT);
  }

  if (mq.has('DC')) {
    ootScrubs.push(PRICES_RAW.OOT_SCRUBS_DC_MQ);
  } else {
    ootScrubs.push(PRICES_RAW.OOT_SCRUBS_DC);
  }

  if (mq.has('JJ')) {
    ootScrubs.push(PRICES_RAW.OOT_SCRUBS_JJ_MQ);
  } else {
    ootScrubs.push(PRICES_RAW.OOT_SCRUBS_JJ);
  }

  if (mq.has('Ganon')) {
    ootScrubs.push(PRICES_RAW.OOT_SCRUBS_GC_MQ);
  } else {
    ootScrubs.push(PRICES_RAW.OOT_SCRUBS_GC);
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

type LogicPassPriceState = {
  settings: Settings,
  worlds: World[],
  random: Random,
};

class LogicPassPrice {
  constructor(
    private readonly state: LogicPassPriceState,
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
      beta = 12;
      if (this.state.settings.bottomlessWallets) {
        max = 999;
        beta = 20;
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
      this.shufflePrices(world, 'OOT_MERCHANTS', this.state.settings.priceOotMerchants);
      this.shufflePrices(world, 'MM_SHOPS', this.state.settings.priceMmShops);
      this.shufflePrices(world, 'MM_SHOPS_EX', this.state.settings.priceMmShops);
      this.shufflePrices(world, 'MM_TINGLE', this.state.settings.priceMmTingle);
    }

    return {};
  }
}

export function logicPassPrice(state: LogicPassPriceState) {
  return new LogicPassPrice(state).run();
}

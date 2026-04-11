import type { Settings } from '@ootmm/core';
import type { ResolvedWorldFlags, World } from '@ootmm/logic';

import { Random, randomInt } from '@ootmm/core';
import { DEFAULT_PRICES, PRICES_RAW, PRICE_RANGES, PRICE_COUNTS } from '@ootmm/logic';

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

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
]

const PRICES = {
  OOT_SHOPS,
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
  return [...DEFAULT_PRICES];
}

/* Approximate beta func */
function betaApproxInt(random: Random, alpha: number, beta: number, max: number) {
  const draws = alpha + beta - 1;
  const values: number[] = [];
  for (let i = 0; i < draws; ++i) {
    values.push(randomInt(random, max));
  }
  return (values.sort())[alpha - 1];
}

export class LogicPassPrice {
  constructor(
    private state: {
      settings: Settings,
      world: World,
      random: Random,
    }
  ) {
  }

  private randomPrice() {
    let max = 100;
    if (this.state.settings.colossalWallets) {
      max = 199;
    }
    const r = randomInt(this.state.random, max);
    return r * 5;
  }

  private randomPriceBeta() {
    let max = 100;
    let beta = 5;
    if (this.state.settings.colossalWallets) {
      max = 199;
      beta = 9;
    }
    const r = betaApproxInt(this.state.random, 2, beta, max);
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

  private shufflePrices(range: string, type: PriceRandoType) {
    const base = PRICE_RANGES[range];
    const size = PRICE_COUNTS[range];
    for (let i = 0; i < size; ++i) {
      const value = this.state.world.prices[base + i];
      this.state.world.prices[base + i] = this.adjustPrice(type, value);
    }
  }

  run() {
    this.shufflePrices('OOT_SHOPS', this.state.settings.priceOotShops);

    return {};
  }
}

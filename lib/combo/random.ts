import crypto from 'crypto';

export class Random {
  private state: number[] = [0, 0, 0, 0, 0];
  private counter: number = 0;

  constructor() {
  }

  next() {
    let t = this.state[4];
    const s = this.state[0];
    this.state[4] = this.state[3];
    this.state[3] = this.state[2];
    this.state[2] = this.state[1];
    this.state[1] = s;

    t = ((t >> 2) ^ t) >>> 0;
    t = ((t << 1) ^ t) >>> 0;
    t = (((s << 4) ^ s) ^ t) >>> 0;
    this.state[0] = t;
    this.counter = (this.counter + 362437) >>> 0;
    return (t + this.counter) >>> 0;
  }

  seed(seed: string) {
    const hash = crypto.createHash('sha512');
    hash.update(seed);
    const digest = hash.digest('hex');
    for (let i = 0; i < 5; ++i) {
      this.state[i] = parseInt(digest.substr(i * 8, 8), 16);
    }
    this.counter = 0;
  }
}

export const randString = () => {
  return crypto.randomBytes(48).toString('hex');
}

const randomInt = (random: Random, max: number) => {
  /* Create a mask that is all 1s up to the max value */
  let mask = max - 1;
  mask |= mask >> 1;
  mask |= mask >> 2;
  mask |= mask >> 4;
  mask |= mask >> 8;
  mask |= mask >> 16;

  for (;;) {
    const value = (random.next() >>> 8) & mask;
    if (value < max) {
      return value;
    }
  }
};

export const sample = <T>(random: Random, arr: T[]): T => {
  if (arr.length === 0) {
    throw new Error('Empty Array');
  }

  const index = randomInt(random, arr.length);
  return ([...arr].sort())[index];
};

export const shuffle = <T>(random: Random, arr: T[]): T[] => {
  const copy = [...arr].sort();
  for (let i = 0; i < copy.length - 1; i++) {
    const j = i + randomInt(random, copy.length - i);
    [copy[i], copy[j]] = [copy[j], copy[i]];
  }
  return copy;
};

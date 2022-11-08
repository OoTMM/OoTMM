import crypto from 'crypto';

const getSeed = () => {
  const buf = crypto.randomBytes(4 * 6);
  const seed = [];
  for (let i = 0; i < 6; i++) {
    seed.push(buf.readUInt32LE(i * 4));
  }
  return seed;
};

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

  seed(seed?: number[]) {
    if (!seed) {
      seed = getSeed();
    }
    this.state[0] = seed[0];
    this.state[1] = seed[1];
    this.state[2] = seed[2];
    this.state[3] = seed[3];
    this.state[4] = seed[4];
    this.counter = seed[5];
  }
}

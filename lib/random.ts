import crypto from 'crypto';

const rotl = (x: number, k: number) => (x << k) | (x >>> (32 - k));

const getSeed = () => {
  const buf = crypto.randomBytes(16);
  const seed = [];
  for (let i = 0; i < 4; i++) {
    seed.push(buf.readUInt32LE(i * 4));
  }
  return seed;
};

export class Random {
  private state: number[] = [0, 0, 0, 0];

  constructor() {
  }

  next() {
    const s = this.state;
    const result = (rotl(s[0] + s[3], 7) + s[0]) >>> 0;
    const t = (s[1] << 9);
    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];
    s[2] ^= t;
    s[3] = rotl(s[3], 11);
    return result;
  }

  seed(seed?: number[]) {
    if (!seed) {
      seed = getSeed();
    }
    this.state = [...seed];
  }
}

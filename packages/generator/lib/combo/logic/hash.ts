import { Monitor } from '../monitor';
import { Random } from '../random';

const ALPHABET = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';

const seedHash = (random: Random): string => {
  const letters: string[] = [];
  for (let i = 0; i < 8; ++i) {
    letters.push(ALPHABET[random.next() % ALPHABET.length]);
  }
  return letters.join('');
};

export class LogicPassHash {
  constructor(
    private readonly state: {
      random: Random,
      monitor: Monitor,
    }
  ){
  }

  run() {
    this.state.monitor.log('Logic: Hash');

    const hash = seedHash(this.state.random);
    return { hash };
  }
};

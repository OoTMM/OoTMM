import { Random } from '@ootmm/core';

const ALPHABET = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';

type LogicPassHashState = {
  random: Random;
};
export function logicPassHash({ random }: LogicPassHashState) {
  const letters: string[] = [];
  for (let i = 0; i < 8; ++i) {
    letters.push(ALPHABET[random.next() % ALPHABET.length]);
  }
  const hash = letters.join('');
  return { hash };
}

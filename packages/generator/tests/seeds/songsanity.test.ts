import { test, expect } from 'vitest';

import { makeTestSeed } from '../helper';

test("Can make a seed - songsanity", async () => {
  const res = await makeTestSeed("SONGSANITY", { songs: "anywhere" });
  expect(res).toBeDefined();
});

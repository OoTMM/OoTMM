import { test, expect } from 'vitest';

import { makeTestSeed } from '../helpers';

test("Can make a seed - songsanity", async () => {
  const res = await makeTestSeed("SONGSANITY", { songs: "anywhere" });
  expect(res).toBeDefined();
});

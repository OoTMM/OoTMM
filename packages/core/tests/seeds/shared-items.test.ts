import { test, expect } from 'vitest';

import { makeTestSeed } from '../helper';

test("Can make a seed - Shared Items", async () => {
  const res = await makeTestSeed("SHARED-ITEMS", {
    sticksNutsUpgradesMm: true,
    sharedBows: true,
    sharedBombBags: true,
    sharedMagic: true,
    sharedMagicArrowFire: true,
    sharedMagicArrowIce: true,
    sharedMagicArrowLight: true,
    sharedSongEpona: true,
    sharedSongStorms: true,
    sharedSongTime: true,
    sharedSongSun: true,
    sharedNutsSticks: true,
    sharedHookshot: true,
    sharedLens: true,
    sharedOcarina: true,
    sharedMaskGoron: true,
    sharedMaskZora: true,
    sharedMaskBunny: true,
    sharedMaskTruth: true,
    sharedMaskKeaton: true,
    sharedWallets: true,
    sharedHealth: true,
  });
  expect(res).toBeDefined();
});

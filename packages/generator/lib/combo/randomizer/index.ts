import { Settings } from '@ootmm/core';

import { LogicResult } from '../logic';
import { Patchfile } from '../patch-build/patchfile';

import { RandomizerPatcherConfig } from './config';
import { RandomizerPatcherStartingItems } from './starting-items';
import { RandomizerPatcherHints } from './hints';
import { RandomizerPatcherEntrances } from './entrances';
import { RandomizerPatcherChecks } from './checks';

export function patchRandomizer(worldId: number, logic: LogicResult, settings: Settings, patchfile: Patchfile) {
  const bufConfig = RandomizerPatcherConfig.run({ worldId, logic, settings });
  const bufStartingItems = RandomizerPatcherStartingItems.run({ worldId, logic, settings });
  const bufChecksOot = RandomizerPatcherChecks.run({ worldId, logic, settings, game: 'oot' });
  const bufChecksMm = RandomizerPatcherChecks.run({ worldId, logic, settings, game: 'mm' });
  const bufHintsOot = RandomizerPatcherHints.run({ worldId, logic, settings, game: 'oot' });
  const bufHintsMm = RandomizerPatcherHints.run({ worldId, logic, settings, game: 'mm' });
  const bufEntrancesOot = RandomizerPatcherEntrances.run({ worldId, logic, game: 'oot' });
  const bufEntrancesMm = RandomizerPatcherEntrances.run({ worldId, logic, game: 'mm' });

  patchfile.addNewFile({ vrom: 0xf0200000, data: bufConfig, compressed: true });
  patchfile.addNewFile({ vrom: 0xf0300000, data: bufStartingItems, compressed: false });
  patchfile.addNewFile({ vrom: 0xf0400000, data: bufChecksOot, compressed: false });
  patchfile.addNewFile({ vrom: 0xf0500000, data: bufChecksMm, compressed: false });
  patchfile.addNewFile({ vrom: 0xf0600000, data: bufHintsOot, compressed: true });
  patchfile.addNewFile({ vrom: 0xf0700000, data: bufHintsMm, compressed: true });
  patchfile.addNewFile({ vrom: 0xf0800000, data: bufEntrancesOot, compressed: true });
  patchfile.addNewFile({ vrom: 0xf0900000, data: bufEntrancesMm, compressed: true });
}

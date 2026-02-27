import { SCENES, NPC } from '@ootmm/data';
import { Game, Settings, Random, sample } from '@ootmm/core';

import { LogicResult } from '../logic';
import { World, WorldCheck } from '../logic/world';
import { Patchfile } from '../patch-build/patchfile';
import { makeLocation, isLocationFullyShuffled } from '../logic/locations';
import { ItemGroups, ItemHelpers } from '../items';
import { bufReadU32BE, bufWriteU16BE, bufWriteU32BE } from '../util/buffer';
import { concatUint8Arrays } from 'uint8array-extras';
import { Options } from '../options';
import { RandomizerPatcherConfig } from './config';
import { gi, playerId } from './util';
import { RandomizerPatcherStartingItems } from './starting-items';
import { RandomizerPatcherHints } from './hints';
import { RandomizerPatcherEntrances } from './entrances';

const checkId = (check: WorldCheck) => {
  if (check.type === 'npc') {
    if (!NPC.hasOwnProperty(check.id)) {
      throw new Error(`Unknown NPC ${check.id}`);
    }
    return (NPC as any)[check.id];
  }
  return check.id;
}

function checkKey(check: WorldCheck): number {
  /* Extract the ID */
  const id = checkId(check);

  /* Extract the type */
  let typeId: number;
  switch (check.type) {
    case 'chest':
      typeId = 0x01;
      break;
    case 'collectible':
      typeId = 0x02;
      break;
    case 'npc':
      typeId = 0x03;
      break;
    case 'gs':
      typeId = 0x04;
      break;
    case 'sf':
      typeId = 0x05;
      break;
    case 'cow':
      typeId = 0x06;
      break;
    case 'shop':
      typeId = 0x07;
      break;
    case 'scrub':
      typeId = 0x08;
      break;
    case 'sr':
      typeId = 0x09;
      break;
    case 'fish':
      typeId = 0x0A;
      break;
    case 'pot':
    case 'crate':
    case 'barrel':
    case 'grass':
    case 'tree':
    case 'bush':
    case 'rock':
    case 'soil':
    case 'fairy':
    case 'snowball':
    case 'hive':
    case 'rupee':
    case 'heart':
    case 'fairy_spot':
    case 'wonder':
    case 'butterfly':
    case 'redboulder':
    case 'icicle':
    case 'redice':
      /* xflag */
      typeId = 0x10 + ((id >> 16) & 0xf);
      break;
  }

  /* Extract the scene ID */
  let sceneId = 0;
  switch (check.type) {
    case 'chest':
    case 'collectible':
    case 'sf':
    case 'pot':
    case 'crate':
    case 'barrel':
    case 'grass':
    case 'tree':
    case 'bush':
    case 'rock':
    case 'soil':
    case 'fairy':
    case 'snowball':
    case 'hive':
    case 'rupee':
    case 'heart':
    case 'fairy_spot':
    case 'wonder':
    case 'butterfly':
    case 'redboulder':
    case 'icicle':
    case 'redice':
      sceneId = (SCENES as any)[check.scene];
      if (sceneId === undefined) {
        throw new Error(`Unknown scene ${check.scene}`);
      }
      break;
    default:
      break;
  }

  /* Build the key */
  let key = 0;
  key = (key | ((typeId & 0xff) << 24)) >>> 0;
  key = (key | ((sceneId & 0xff) << 16)) >>> 0;
  key = (key | (id & 0xffff)) >>> 0;

  return key;
}

async function makeCloakGi(key: number, seed: string, settings: Settings, logic: LogicResult): Promise<number> {
  const random = new Random();
  await random.seed(key.toString(16) + '\x00' + seed);

  for (; ;) {
    const locs = [...logic.items.keys()];
    const loc = sample(random, locs);
    const item = logic.items.get(loc)!;

    if (!ItemHelpers.isItemMajor(item.item) && !ItemHelpers.isSilverRupee(item.item) && !ItemHelpers.isKey(item.item) && !ItemHelpers.isBossKey(item.item)) continue;
    if (ItemGroups.JUNK.has(item.item)) continue;
    if (!isLocationFullyShuffled(settings, logic.fixedLocations, logic.items, logic.plandoLocations, loc, { songs: true, noPlando: true })) continue;

    return gi(settings, 'oot', item.item, false);
  }
}

class PatchRandomizer {
  private world: World;

  constructor(
    private worldId: number,
    private logic: LogicResult,
    private options: Options,
    private settings: Settings,
    private patchfile: Patchfile,
  ) {
    this.world = logic.worlds[worldId];
  }

  async run() {
    const { worldId, logic, settings } = this;

    const bufConfig = RandomizerPatcherConfig.run({ worldId, logic, settings });
    const bufStartingItems = RandomizerPatcherStartingItems.run({ worldId, logic, settings });
    const bufHintsOot = RandomizerPatcherHints.run({ worldId, logic, settings, game: 'oot' });
    const bufHintsMm = RandomizerPatcherHints.run({ worldId, logic, settings, game: 'mm' });
    const bufEntrancesOot = RandomizerPatcherEntrances.run({ worldId, logic, settings, game: 'oot' });
    const bufEntrancesMm = RandomizerPatcherEntrances.run({ worldId, logic, settings, game: 'mm' });

    this.patchfile.addNewFile({ vrom: 0xf0200000, data: bufConfig, compressed: true });
    this.patchfile.addNewFile({ vrom: 0xf0300000, data: bufStartingItems, compressed: false });
    this.patchfile.addNewFile({ vrom: 0xf0400000, data: await this.gameChecks('oot'), compressed: false });
    this.patchfile.addNewFile({ vrom: 0xf0500000, data: await this.gameChecks('mm'), compressed: false });
    this.patchfile.addNewFile({ vrom: 0xf0600000, data: bufHintsOot, compressed: true });
    this.patchfile.addNewFile({ vrom: 0xf0700000, data: bufHintsMm, compressed: true });
    this.patchfile.addNewFile({ vrom: 0xf0800000, data: bufEntrancesOot, compressed: true });
    this.patchfile.addNewFile({ vrom: 0xf0900000, data: bufEntrancesMm, compressed: true });
  }

  private async gameChecks(game: Game): Promise<Uint8Array> {
    const buffers: Uint8Array[] = [];
    for (const locId in this.world.checks) {
      const loc = makeLocation(locId, this.worldId);
      const c = this.world.checks[locId];
      const item = this.logic.items.get(loc)!;

      if (c.game !== game) {
        continue;
      }

      /* Skip cows if not shuffled */
      if (c.game === 'oot' && c.type === 'cow' && !this.settings.cowShuffleOot) continue;
      if (c.game === 'mm' && c.type === 'cow' && !this.settings.cowShuffleMm) continue;

      const key = checkKey(c);
      const itemGi = gi(this.settings, game, item.item, true);
      const b = new Uint8Array(16);
      bufWriteU32BE(b, 0, key);
      bufWriteU16BE(b, 4, playerId(item.player));
      bufWriteU16BE(b, 6, itemGi);
      let cloakGi = 0;
      if (this.settings.cloakTraps && ItemGroups.TRAPS.has(item.item)) {
        cloakGi = await makeCloakGi(key, this.options.seed, this.settings, this.logic);
      }
      bufWriteU16BE(b, 8, cloakGi);
      buffers.push(b);
    }

    /* Sort by key ascending */
    buffers.sort((a, b) => bufReadU32BE(a, 0) < bufReadU32BE(b, 0) ? -1 : 1);
    const end = new Uint8Array(16);
    end.fill(0xff);
    buffers.push(end);
    return concatUint8Arrays(buffers);
  }
}

export async function patchRandomizer(worldId: number, logic: LogicResult, options: Options, settings: Settings, patchfile: Patchfile) {
  const pr = new PatchRandomizer(worldId, logic, options, settings, patchfile);
  await pr.run();
}

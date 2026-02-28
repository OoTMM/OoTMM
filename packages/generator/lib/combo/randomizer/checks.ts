import { concatUint8Arrays } from 'uint8array-extras';
import { Game, Settings } from '@ootmm/core';
import { NPC, SCENES } from '@ootmm/data';

import { World, WorldCheck } from '../logic/world';
import { LogicResult } from '../logic';
import { makeLocation } from '../logic/locations';
import { gi, playerId } from './util';
import { bufReadU32BE, bufWriteU16BE, bufWriteU32BE } from '../util/buffer';

const SHARED_ITEMS_OOT = new Map([
  ['SHARED_SWORD', 'OOT_SWORD_KOKIRI'],
  ['SHARED_BOW', 'OOT_BOW'],
  ['SHARED_BOMB_BAG', 'OOT_BOMB_BAG'],
  ['SHARED_ARROWS_5', 'OOT_ARROWS_5'],
  ['SHARED_ARROWS_10', 'OOT_ARROWS_10'],
  ['SHARED_ARROWS_30', 'OOT_ARROWS_30'],
  ['SHARED_ARROWS_40', 'MM_ARROWS_40'], /* OoT lacks 40 pack */
  ['SHARED_BOMB', 'OOT_BOMB'],
  ['SHARED_BOMBS_5', 'OOT_BOMBS_5'],
  ['SHARED_BOMBS_10', 'OOT_BOMBS_10'],
  ['SHARED_BOMBS_20', 'OOT_BOMBS_20'],
  ['SHARED_BOMBS_30', 'OOT_BOMBS_30'],
  ['SHARED_MAGIC_UPGRADE', 'OOT_MAGIC_UPGRADE'],
  ['SHARED_ARROW_FIRE', 'OOT_ARROW_FIRE'],
  ['SHARED_ARROW_ICE', 'OOT_ARROW_ICE'],
  ['SHARED_ARROW_LIGHT', 'OOT_ARROW_LIGHT'],
  ['SHARED_SONG_TIME', 'OOT_SONG_TIME'],
  ['SHARED_SONG_EPONA', 'OOT_SONG_EPONA'],
  ['SHARED_SONG_STORMS', 'OOT_SONG_STORMS'],
  ['SHARED_SONG_EMPTINESS', 'OOT_SONG_EMPTINESS'],
  ['SHARED_SONG_HEALING', 'OOT_SONG_HEALING'],
  ['SHARED_SONG_SOARING', 'OOT_SONG_SOARING'],
  ['SHARED_SONG_AWAKENING', 'OOT_SONG_AWAKENING'],
  ['SHARED_SONG_GORON_HALF', 'OOT_SONG_GORON_HALF'],
  ['SHARED_SONG_GORON', 'OOT_SONG_GORON'],
  ['SHARED_SONG_ZORA', 'OOT_SONG_ZORA'],
  ['SHARED_SONG_ORDER', 'OOT_SONG_ORDER'],
  ['SHARED_SONG_ZELDA', 'OOT_SONG_ZELDA'],
  ['SHARED_SONG_SARIA', 'OOT_SONG_SARIA'],
  ['SHARED_SONG_SUN', 'OOT_SONG_SUN'],
  ['SHARED_SONG_TP_FIRE', 'OOT_SONG_TP_FIRE'],
  ['SHARED_SONG_TP_FOREST', 'OOT_SONG_TP_FOREST'],
  ['SHARED_SONG_TP_LIGHT', 'OOT_SONG_TP_LIGHT'],
  ['SHARED_SONG_TP_SHADOW', 'OOT_SONG_TP_SHADOW'],
  ['SHARED_SONG_TP_SPIRIT', 'OOT_SONG_TP_SPIRIT'],
  ['SHARED_SONG_TP_WATER', 'OOT_SONG_TP_WATER'],
  ['SHARED_SONG_NOTE_TIME', 'OOT_SONG_NOTE_TIME'],
  ['SHARED_SONG_NOTE_EPONA', 'OOT_SONG_NOTE_EPONA'],
  ['SHARED_SONG_NOTE_STORMS', 'OOT_SONG_NOTE_STORMS'],
  ['SHARED_SONG_NOTE_EMPTINESS', 'OOT_SONG_NOTE_EMPTINESS'],
  ['SHARED_SONG_NOTE_ZELDA', 'OOT_SONG_NOTE_ZELDA'],
  ['SHARED_SONG_NOTE_SARIA', 'OOT_SONG_NOTE_SARIA'],
  ['SHARED_SONG_NOTE_SUN', 'OOT_SONG_NOTE_SUN'],
  ['SHARED_SONG_NOTE_TP_FIRE', 'OOT_SONG_NOTE_TP_FIRE'],
  ['SHARED_SONG_NOTE_TP_FOREST', 'OOT_SONG_NOTE_TP_FOREST'],
  ['SHARED_SONG_NOTE_TP_LIGHT', 'OOT_SONG_NOTE_TP_LIGHT'],
  ['SHARED_SONG_NOTE_TP_SHADOW', 'OOT_SONG_NOTE_TP_SHADOW'],
  ['SHARED_SONG_NOTE_TP_SPIRIT', 'OOT_SONG_NOTE_TP_SPIRIT'],
  ['SHARED_SONG_NOTE_TP_WATER', 'OOT_SONG_NOTE_TP_WATER'],
  ['SHARED_NUT', 'MM_NUT'] /* OoT lacks single nut */,
  ['SHARED_NUTS_5', 'OOT_NUTS_5'],
  ['SHARED_NUTS_10', 'OOT_NUTS_10'],
  ['SHARED_STICK', 'OOT_STICK'],
  ['SHARED_STICKS_5', 'OOT_STICKS_5'],
  ['SHARED_STICKS_10', 'OOT_STICKS_10'],
  ['SHARED_HOOKSHOT', 'OOT_HOOKSHOT'],
  ['SHARED_LENS', 'OOT_LENS'],
  ['SHARED_OCARINA', 'OOT_OCARINA'],
  ['SHARED_MASK_KEATON', 'OOT_MASK_KEATON'],
  ['SHARED_MASK_BUNNY', 'OOT_MASK_BUNNY'],
  ['SHARED_MASK_TRUTH', 'OOT_MASK_TRUTH'],
  ['SHARED_MASK_GORON', 'OOT_MASK_GORON'],
  ['SHARED_MASK_ZORA', 'OOT_MASK_ZORA'],
  ['SHARED_WALLET', 'OOT_WALLET'],
  ['SHARED_RUPEE_GREEN', 'OOT_RUPEE_GREEN'],
  ['SHARED_RUPEE_BLUE', 'OOT_RUPEE_BLUE'],
  ['SHARED_RUPEE_RED', 'OOT_RUPEE_RED'],
  ['SHARED_RUPEE_PURPLE', 'OOT_RUPEE_PURPLE'],
  ['SHARED_RUPEE_SILVER', 'MM_RUPEE_SILVER'], /* OoT lacks silver rupee */
  ['SHARED_RUPEE_GOLD', 'OOT_RUPEE_HUGE'], /* Equivalent */
  ['SHARED_RUPEE_RAINBOW', 'OOT_RUPEE_RAINBOW'],
  ['SHARED_HEART_PIECE', 'OOT_HEART_PIECE'],
  ['SHARED_HEART_CONTAINER', 'OOT_HEART_CONTAINER'],
  ['SHARED_RECOVERY_HEART', 'OOT_RECOVERY_HEART'],
  ['SHARED_DEFENSE_UPGRADE', 'OOT_DEFENSE_UPGRADE'],
  ['SHARED_SHIELD_DEKU', 'OOT_SHIELD_DEKU'],
  ['SHARED_SHIELD_HYLIAN', 'OOT_SHIELD_HYLIAN'],
  ['SHARED_SHIELD_MIRROR', 'OOT_SHIELD_MIRROR'],
  ['SHARED_SHIELD', 'OOT_SHIELD'],
  ['SHARED_MAGIC_JAR_SMALL', 'OOT_MAGIC_JAR_SMALL'],
  ['SHARED_MAGIC_JAR_LARGE', 'OOT_MAGIC_JAR_LARGE'],
  ['SHARED_BOMBCHU', 'MM_BOMBCHU'], /* OoT lacks single bombchu */
  ['SHARED_BOMBCHU_5', 'OOT_BOMBCHU_5'],
  ['SHARED_BOMBCHU_10', 'OOT_BOMBCHU_10'],
  ['SHARED_BOMBCHU_20', 'OOT_BOMBCHU_20'],
  ['SHARED_FAIRY_BIG', 'OOT_FAIRY_BIG'],
  ['SHARED_SPELL_FIRE', 'OOT_SPELL_FIRE'],
  ['SHARED_SPELL_WIND', 'OOT_SPELL_WIND'],
  ['SHARED_SPELL_LOVE', 'OOT_SPELL_LOVE'],
  ['SHARED_BOOTS_IRON', 'OOT_BOOTS_IRON'],
  ['SHARED_BOOTS_HOVER', 'OOT_BOOTS_HOVER'],
  ['SHARED_TUNIC_GORON', 'OOT_TUNIC_GORON'],
  ['SHARED_TUNIC_ZORA', 'OOT_TUNIC_ZORA'],
  ['SHARED_MASK_BLAST', 'OOT_MASK_BLAST'],
  ['SHARED_MASK_STONE', 'OOT_MASK_STONE'],
  ['SHARED_SCALE', 'OOT_SCALE'],
  ['SHARED_STRENGTH', 'OOT_STRENGTH'],
  ['SHARED_HAMMER', 'OOT_HAMMER'],
  ['SHARED_STICK_UPGRADE', 'OOT_STICK_UPGRADE'],
  ['SHARED_NUT_UPGRADE', 'OOT_NUT_UPGRADE'],
  ['SHARED_STONE_OF_AGONY', 'OOT_STONE_OF_AGONY'],
  ['SHARED_SPIN_UPGRADE', 'OOT_SPIN_UPGRADE'],
  ['SHARED_BOMBCHU_BAG', 'OOT_BOMBCHU_BAG'],
]);

const SHARED_ITEMS_MM = new Map([
  ['SHARED_SWORD', 'MM_SWORD_KOKIRI'],
  ['SHARED_BOW', 'MM_BOW'],
  ['SHARED_BOMB_BAG', 'MM_BOMB_BAG'],
  ['SHARED_ARROWS_5', 'OOT_ARROWS_5'], /* MM lacks 5 pack */
  ['SHARED_ARROWS_10', 'MM_ARROWS_10'],
  ['SHARED_ARROWS_30', 'MM_ARROWS_30'],
  ['SHARED_ARROWS_40', 'MM_ARROWS_40'],
  ['SHARED_BOMB', 'MM_BOMB'],
  ['SHARED_BOMBS_5', 'MM_BOMBS_5'],
  ['SHARED_BOMBS_10', 'MM_BOMBS_10'],
  ['SHARED_BOMBS_20', 'MM_BOMBS_20'],
  ['SHARED_BOMBS_30', 'MM_BOMBS_30'],
  ['SHARED_MAGIC_UPGRADE', 'MM_MAGIC_UPGRADE'],
  ['SHARED_ARROW_FIRE', 'MM_ARROW_FIRE'],
  ['SHARED_ARROW_ICE', 'MM_ARROW_ICE'],
  ['SHARED_ARROW_LIGHT', 'MM_ARROW_LIGHT'],
  ['SHARED_SONG_TIME', 'MM_SONG_TIME'],
  ['SHARED_SONG_EPONA', 'MM_SONG_EPONA'],
  ['SHARED_SONG_STORMS', 'MM_SONG_STORMS'],
  ['SHARED_SONG_EMPTINESS', 'MM_SONG_EMPTINESS'],
  ['SHARED_SONG_HEALING', 'MM_SONG_HEALING'],
  ['SHARED_SONG_SOARING', 'MM_SONG_SOARING'],
  ['SHARED_SONG_AWAKENING', 'MM_SONG_AWAKENING'],
  ['SHARED_SONG_GORON_HALF', 'MM_SONG_GORON_HALF'],
  ['SHARED_SONG_GORON', 'MM_SONG_GORON'],
  ['SHARED_SONG_ZORA', 'MM_SONG_ZORA'],
  ['SHARED_SONG_ORDER', 'MM_SONG_ORDER'],
  ['SHARED_SONG_ZELDA', 'MM_SONG_ZELDA'],
  ['SHARED_SONG_SARIA', 'MM_SONG_SARIA'],
  ['SHARED_SONG_SUN', 'MM_SONG_SUN'],
  ['SHARED_SONG_TP_FIRE', 'MM_SONG_TP_FIRE'],
  ['SHARED_SONG_TP_FOREST', 'MM_SONG_TP_FOREST'],
  ['SHARED_SONG_TP_LIGHT', 'MM_SONG_TP_LIGHT'],
  ['SHARED_SONG_TP_SHADOW', 'MM_SONG_TP_SHADOW'],
  ['SHARED_SONG_TP_SPIRIT', 'MM_SONG_TP_SPIRIT'],
  ['SHARED_SONG_TP_WATER', 'MM_SONG_TP_WATER'],
  ['SHARED_SONG_NOTE_TIME', 'MM_SONG_NOTE_TIME'],
  ['SHARED_SONG_NOTE_EPONA', 'MM_SONG_NOTE_EPONA'],
  ['SHARED_SONG_NOTE_STORMS', 'MM_SONG_NOTE_STORMS'],
  ['SHARED_SONG_NOTE_EMPTINESS', 'MM_SONG_NOTE_EMPTINESS'],
  ['SHARED_SONG_NOTE_ZELDA', 'MM_SONG_NOTE_ZELDA'],
  ['SHARED_SONG_NOTE_SARIA', 'MM_SONG_NOTE_SARIA'],
  ['SHARED_SONG_NOTE_SUN', 'MM_SONG_NOTE_SUN'],
  ['SHARED_SONG_NOTE_TP_FIRE', 'MM_SONG_NOTE_TP_FIRE'],
  ['SHARED_SONG_NOTE_TP_FOREST', 'MM_SONG_NOTE_TP_FOREST'],
  ['SHARED_SONG_NOTE_TP_LIGHT', 'MM_SONG_NOTE_TP_LIGHT'],
  ['SHARED_SONG_NOTE_TP_SHADOW', 'MM_SONG_NOTE_TP_SHADOW'],
  ['SHARED_SONG_NOTE_TP_SPIRIT', 'MM_SONG_NOTE_TP_SPIRIT'],
  ['SHARED_SONG_NOTE_TP_WATER', 'MM_SONG_NOTE_TP_WATER'],
  ['SHARED_NUT', 'MM_NUT'],
  ['SHARED_NUTS_5', 'MM_NUTS_5'],
  ['SHARED_NUTS_10', 'MM_NUTS_10'],
  ['SHARED_STICK', 'MM_STICK'],
  ['SHARED_STICKS_5', 'OOT_STICKS_5'],  /* MM lacks 5 pack */
  ['SHARED_STICKS_10', 'OOT_STICKS_10'], /* MM lacks 10 pack */
  ['SHARED_HOOKSHOT', 'MM_HOOKSHOT'],
  ['SHARED_LENS', 'MM_LENS'],
  ['SHARED_OCARINA', 'OOT_OCARINA'], /* Progressive */
  ['SHARED_MASK_KEATON', 'MM_MASK_KEATON'],
  ['SHARED_MASK_BUNNY', 'MM_MASK_BUNNY'],
  ['SHARED_MASK_TRUTH', 'MM_MASK_TRUTH'],
  ['SHARED_MASK_GORON', 'MM_MASK_GORON'],
  ['SHARED_MASK_ZORA', 'MM_MASK_ZORA'],
  ['SHARED_WALLET', 'MM_WALLET'],
  ['SHARED_RUPEE_GREEN', 'MM_RUPEE_GREEN'],
  ['SHARED_RUPEE_BLUE', 'MM_RUPEE_BLUE'],
  ['SHARED_RUPEE_RED', 'MM_RUPEE_RED'],
  ['SHARED_RUPEE_PURPLE', 'MM_RUPEE_PURPLE'],
  ['SHARED_RUPEE_SILVER', 'MM_RUPEE_SILVER'],
  ['SHARED_RUPEE_GOLD', 'MM_RUPEE_GOLD'],
  ['SHARED_RUPEE_RAINBOW', 'OOT_RUPEE_RAINBOW'], /* No MM equivalent */
  ['SHARED_HEART_PIECE', 'MM_HEART_PIECE'],
  ['SHARED_HEART_CONTAINER', 'MM_HEART_CONTAINER'],
  ['SHARED_RECOVERY_HEART', 'MM_RECOVERY_HEART'],
  ['SHARED_DEFENSE_UPGRADE', 'MM_DEFENSE_UPGRADE'],
  ['SHARED_SHIELD_DEKU', 'MM_SHIELD_DEKU'],
  ['SHARED_SHIELD_HYLIAN', 'MM_SHIELD_HERO'],
  ['SHARED_SHIELD_MIRROR', 'MM_SHIELD_MIRROR'],
  ['SHARED_SHIELD', 'MM_SHIELD'], /* Progressive */
  ['SHARED_MAGIC_JAR_SMALL', 'MM_MAGIC_JAR_SMALL'],
  ['SHARED_MAGIC_JAR_LARGE', 'MM_MAGIC_JAR_LARGE'],
  ['SHARED_BOMBCHU', 'MM_BOMBCHU'],
  ['SHARED_BOMBCHU_5', 'MM_BOMBCHU_5'],
  ['SHARED_BOMBCHU_10', 'MM_BOMBCHU_10'],
  ['SHARED_BOMBCHU_20', 'MM_BOMBCHU_20'],
  ['SHARED_FAIRY_BIG', 'MM_FAIRY_BIG'],
  ['SHARED_SPELL_FIRE', 'MM_SPELL_FIRE'],
  ['SHARED_SPELL_WIND', 'MM_SPELL_WIND'],
  ['SHARED_SPELL_LOVE', 'MM_SPELL_LOVE'],
  ['SHARED_BOOTS_IRON', 'MM_BOOTS_IRON'],
  ['SHARED_BOOTS_HOVER', 'MM_BOOTS_HOVER'],
  ['SHARED_TUNIC_GORON', 'MM_TUNIC_GORON'],
  ['SHARED_TUNIC_ZORA', 'MM_TUNIC_ZORA'],
  ['SHARED_MASK_BLAST', 'MM_MASK_BLAST'],
  ['SHARED_MASK_STONE', 'MM_MASK_STONE'],
  ['SHARED_SCALE', 'MM_SCALE'],
  ['SHARED_STRENGTH', 'MM_STRENGTH'],
  ['SHARED_HAMMER', 'MM_HAMMER'],
  ['SHARED_STICK_UPGRADE', 'MM_STICK_UPGRADE'],
  ['SHARED_NUT_UPGRADE', 'MM_NUT_UPGRADE'],
  ['SHARED_STONE_OF_AGONY', 'MM_STONE_OF_AGONY'],
  ['SHARED_SPIN_UPGRADE', 'MM_SPIN_UPGRADE'],
  ['SHARED_BOMBCHU_BAG', 'MM_BOMBCHU_BAG'],
]);

export const SHARED_ITEMS = {
  oot: SHARED_ITEMS_OOT,
  mm: SHARED_ITEMS_MM,
};

export const ITEMS_SUBSTITUTIONS: { [k: string]: string } = {
  OOT_SWORD: 'OOT_SWORD_KOKIRI',
  OOT_SWORD_GORON: 'OOT_SWORD_KNIFE',
  OOT_OCARINA: 'OOT_OCARINA_FAIRY',
  OOT_STRENGTH: 'OOT_GORON_BRACELET',
  OOT_SCALE: 'OOT_SCALE_BRONZE',
  OOT_SHIELD: 'OOT_PROGRESSIVE_SHIELD_DEKU',
  MM_SWORD: 'MM_SWORD_KOKIRI',
  MM_SHIELD: 'MM_PROGRESSIVE_SHIELD_DEKU',
  MM_OCARINA: 'MM_OCARINA_OF_TIME',
  MM_SCALE: 'MM_SCALE_BRONZE',
  MM_STRENGTH: 'MM_GORON_BRACELET',
  MM_CLOCK: 'MM_CLOCK1',
  SHARED_TRIFORCE: 'OOT_TRIFORCE',
  SHARED_TRIFORCE_POWER: 'OOT_TRIFORCE_POWER',
  SHARED_TRIFORCE_COURAGE: 'OOT_TRIFORCE_COURAGE',
  SHARED_TRIFORCE_WISDOM: 'OOT_TRIFORCE_WISDOM',
};

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

type RandomizerPatcherChecksContext = {
  game: Game;
  worldId: number;
  logic: LogicResult;
  settings: Settings;
};

export class RandomizerPatcherChecks {
  private ctx: RandomizerPatcherChecksContext;
  private world: World;

  constructor(ctx: RandomizerPatcherChecksContext) {
    this.ctx = ctx;
    this.world = ctx.logic.worlds[ctx.worldId];
  }

  public static run(ctx: RandomizerPatcherChecksContext) {
    return (new RandomizerPatcherChecks(ctx)).execute();
  }

  private execute(): Uint8Array {
    const buffers: Uint8Array[] = [];
    for (const locId in this.world.checks) {
      const loc = makeLocation(locId, this.ctx.worldId);
      const c = this.world.checks[locId];
      const item = this.ctx.logic.items.get(loc)!;

      if (c.game !== this.ctx.game) {
        continue;
      }

      /* Skip cows if not shuffled */
      if (c.game === 'oot' && c.type === 'cow' && !this.ctx.settings.cowShuffleOot) continue;
      if (c.game === 'mm' && c.type === 'cow' && !this.ctx.settings.cowShuffleMm) continue;

      const key = checkKey(c);
      const itemGi = gi(this.ctx.settings, this.ctx.game, item.item, true);
      const b = new Uint8Array(16);
      const cloakItem = this.ctx.logic.itemCloaks.get(loc);
      let cloakGi = 0;
      if (cloakItem) {
        cloakGi = gi(this.ctx.settings, this.ctx.game, cloakItem, true);
      }
      bufWriteU32BE(b, 0, key);
      bufWriteU16BE(b, 4, playerId(item.player));
      bufWriteU16BE(b, 6, itemGi);
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

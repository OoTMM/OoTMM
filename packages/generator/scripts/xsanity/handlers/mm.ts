import type { Check, RoomActor } from '../types';
import type { Handler } from './types';

import { ITEM00_DROPS_MM, MM_ITEM00_DROP_COLLECTIBLE_TABLE, MM_POTS_RANDOM_DROPS, MM_POTS_SET_DROPS, RUPEES } from '../data';
import { handleWood02 } from './helpers';

let altGrassAcc = 0;

function mmCollectibleDrop(index: number) {
  let index00: number;
  let item: string;

  if (index < 0 || index >= MM_ITEM00_DROP_COLLECTIBLE_TABLE.length) {
    index00 = -1;
  } else {
    index00 = MM_ITEM00_DROP_COLLECTIBLE_TABLE[index];
  }

  if (index00 === -1 || index00 >= ITEM00_DROPS_MM.length) {
    item = 'NOTHING';
  } else {
    item = ITEM00_DROPS_MM[index00];
  }

  return item;
}

export const ObjSwprize: Handler = ({ checks, ra }) => {
  const type = ((ra.actor.params) >> 8) & 3;
  const COUNTS = [1, 1, 3, 1];
  const ITEMS = ['RUPEE_RED', 'RUPEE_PURPLE', 'RUPEE_BLUE', 'ARROWS_30'];
  const count = COUNTS[type];
  const item = ITEMS[type];

  if (count > 1) {
    for (let i = 0; i < count; ++i)
      checks.push({ roomActor: ra, item, name: `Soil`, type: 'soil', sliceId: i, name2: `Item ${i + 1}` });
  } else {
    checks.push({ roomActor: ra, item, name: `Soil`, type: 'soil' });
  }
}

export const ObjBombiwa: Handler = ({ checks, ra }) => {
  const name = (ra.actor.params & 0x100) ? 'Large Boulder' : 'Boulder';
  checks.push({ roomActor: ra, item: 'NOTHING', name, type: 'boulder' });
}

export const EnWood02: Handler = ({ checks, ra }) => {
  handleWood02(checks, ra, 'mm');
}

export const ObjYasi: Handler = ({ checks, ra }) => {
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Palm Tree', type: 'tree' });
}

export const EnSnowwd: Handler = ({ checks, ra }) => {
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Snow Tree', type: 'tree' });
}

export const ObjTree: Handler = ({ checks, ra }) => {
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Forked Tree', type: 'tree' });
}

export const EnKusa: Handler = ({ checks, ra }) => {
  const grassType = (ra.actor.params) & 3;
  let item: string;
  if (grassType == 0 || grassType == 2) {
    item = 'RANDOM';
  } else if (grassType === 3) {
    const item00value = (ra.actor.params & 0xfc) >> 2;
    if (item00value >= ITEM00_DROPS_MM.length) {
      item = 'NOTHING';
    } else {
      item = ITEM00_DROPS_MM[item00value];
    }
  } else {
    item = (altGrassAcc & 1) ? 'RECOVERY_HEART' : 'ARROWS_5';
    altGrassAcc++;
  }
  checks.push({ roomActor: ra, item, name: 'Grass', type: 'grass', subtype: 'normal' });
}

export const EnKusa2: Handler = ({ checks, ra }) => {
  for (let i = 0; i < 9; ++i) {
    const item = (i == 8) ? 'RUPEE_RED' : 'RUPEE_GREEN';
    checks.push({ roomActor: ra, item, name: 'Keaton Grass', type: 'grass', subtype: 'keaton', sliceId: i, name2: `Grass ${i + 1}` });
  }
}

export const ObjGrassUnit: Handler = ({ checks, ra }) => {
  const count = (ra.actor.params & 1) ? 12 : 9;
  const item = 'RANDOM';
  for (let i = 0; i < count; ++i) {
    checks.push({ roomActor: ra, item, name: 'Grass Unit Pack', type: 'grass', subtype: 'unit', sliceId: i, name2: `Grass ${i + 1}` });
  }
}

export const EnButte: Handler = ({ checks, ra }) => {
  const item = (ra.actor.params === 0xffff || !(ra.actor.params & 1)) ? 'NOTHING' : 'FAIRY';
  checks.push({ roomActor: ra, item, name: 'Butterfly', type: 'butterfly' });
}

export const ObjComb: Handler = ({ checks, ra }) => {
  const flag = !!(ra.actor.params & 0x10);
  let type = 0;
  if (ra.actor.params & 0x80) type += 1;
  if (ra.actor.params & 0x8000) type += 2;
  if (type === 1 && flag) return; /* Pirate */

  let item: string;
  switch (type) {
  case 0: item = mmCollectibleDrop(ra.actor.params & 0x3f); break;
  case 1: item = 'NOTHING'; break;
  default: return; /* Skulltulas */
  }

  if (item === 'STRAY_FAIRY' || item === 'HEART_PIECE') return;

  checks.push({ roomActor: ra, item, name: 'Hive', type: 'hive' });
}

export const ObjFlowerpot: Handler = ({ checks, ra }) => {
  const item = mmCollectibleDrop(ra.actor.params & 0x3f);
  checks.push({ roomActor: ra, sliceId: 0, item: 'NOTHING', name: 'Potted Plant', name2: 'Pot', type: 'pot' });
  checks.push({ roomActor: ra, sliceId: 1, item: item, name: 'Potted Plant', name2: 'Grass', type: 'grass' });
}

export const ObjSnowball: Handler = ({ checks, ra }) => {
  if (ra.actor.ry == 1) return; /* Goron Elder */

  const item = mmCollectibleDrop(ra.actor.params & 0x3f);
  checks.push({ roomActor: ra, item, name: 'Big Snowball', type: 'snowball' })
}

export const ObjSnowball2: Handler = ({ checks, ra }) => {
  const item = mmCollectibleDrop(ra.actor.params & 0x3f);
  checks.push({ roomActor: ra, item, name: 'Small Snowball', type: 'snowball' })
}

export const ObjTaru: Handler = ({ checks, ra }) => {
  if (ra.actor.params & 0x80) return; /* Weird fake-barrel */
  const item = mmCollectibleDrop(ra.actor.params & 0x3f);
  if (item === 'STRAY_FAIRY' || item === 'HEART_PIECE') return;
  checks.push({ roomActor: ra, item, name: 'Barrel', type: 'barrel' });
}

export const BgIcicle: Handler = ({ checks, ra }) => {
  let item = 'NOTHING';
  switch(ra.actor.params & 0x3) {
    case 0:
    if(((ra.actor.params >> 8) & 0xFF) != 0xff) item = 'RANDOM'; break;
    case 3: item = mmCollectibleDrop((ra.actor.params >> 2) & 0x3F); break;
    default: return;
  }

  checks.push({ roomActor: ra, item, name: 'Icicle', type: 'icicle' });
}

export const EnIshi: Handler = ({ checks, ra }) => {
  const type = (ra.actor.params & 1);
  const isWall = (ra.actor.params & 4) !== 0;
  if (type !== 0) return;
  const item = 'RANDOM';
  checks.push({ roomActor: ra, item, name: isWall ? 'Rock Wall' : 'Rock', type: 'rock' });
}

export const EnItem00: Handler = ({ checks, ra }) => {
  const item00arg = ra.actor.params & 0xff;
  if (item00arg >= ITEM00_DROPS_MM.length) {
    return;
  }
  const item = ITEM00_DROPS_MM[item00arg];
  if (RUPEES.has(item)) {
    checks.push({ roomActor: ra, item, name: 'Rupee', type: 'rupee' });
  } else if (item === 'RECOVERY_HEART') {
    checks.push({ roomActor: ra, item, name: 'Heart', type: 'heart' });
  }
}

export const ObjTsubo: Handler = ({ checks, ra }) => {
  let item: string;
  const potType = (ra.actor.params >> 7) & 3;

  switch (potType) {
  case 0:
  case 2:
    /* Set item */
    item = MM_POTS_SET_DROPS[ra.actor.params & 0x1f];
    break;
  case 1:
    /* Magic Pot */
    item = 'MAGIC_JAR_LARGE';
    break;
  case 3:
    /* Random item */
    if (ra.actor.params & 0x10) {
      item = 'NOTHING';
    } else {
      item = MM_POTS_RANDOM_DROPS[ra.actor.params & 0x1f];
    }
    break;
  default:
    item = 'DUMMY';
    break;
  }
  if (item === 'STRAY_FAIRY') {
    return;
  }
  checks.push({ roomActor: ra, item, name: 'Pot', type: 'pot' });
}

export const EnTuboTrap: Handler = ({ checks, ra }) => {
  const item = '???'; /* TODO: Document this */
  checks.push({ roomActor: ra, item, name: 'Flying Pot', type: 'pot' });
}

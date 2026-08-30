import { ITEM00_DROPS_MM, MM_ITEM00_DROP_COLLECTIBLE_TABLE, RUPEES } from '../data';
import type { Check, RoomActor } from '../types';
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

export function ObjSwprize(checks: Check[], ra: RoomActor) {
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

export function ObjBombiwa(checks: Check[], ra: RoomActor) {
  const name = (ra.actor.params & 0x100) ? 'Large Boulder' : 'Boulder';
  checks.push({ roomActor: ra, item: 'NOTHING', name, type: 'boulder' });
}

export function EnWood02(checks: Check[], ra: RoomActor) {
  handleWood02(checks, ra, 'mm');
}

export function ObjYasi(checks: Check[], ra: RoomActor) {
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Palm Tree', type: 'tree' });
}

export function EnSnowwd(checks: Check[], ra: RoomActor) {
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Snow Tree', type: 'tree' });
}

export function ObjTree(checks: Check[], ra: RoomActor) {
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Forked Tree', type: 'tree' });
}

export function EnKusa(checks: Check[], ra: RoomActor) {
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

export function EnKusa2(checks: Check[], ra: RoomActor) {
  for (let i = 0; i < 9; ++i) {
    const item = (i == 8) ? 'RUPEE_RED' : 'RUPEE_GREEN';
    checks.push({ roomActor: ra, item, name: 'Keaton Grass', type: 'grass', subtype: 'keaton', sliceId: i, name2: `Grass ${i + 1}` });
  }
}

export function ObjGrassUnit(checks: Check[], ra: RoomActor) {
  const count = (ra.actor.params & 1) ? 12 : 9;
  const item = 'RANDOM';
  for (let i = 0; i < count; ++i) {
    checks.push({ roomActor: ra, item, name: 'Grass Unit Pack', type: 'grass', subtype: 'unit', sliceId: i, name2: `Grass ${i + 1}` });
  }
}

export function EnButte(checks: Check[], ra: RoomActor) {
  const item = (ra.actor.params === 0xffff || !(ra.actor.params & 1)) ? 'NOTHING' : 'FAIRY';
  checks.push({ roomActor: ra, item, name: 'Butterfly', type: 'butterfly' });
}

export function ObjMure(checks: Check[], ra: RoomActor) {
  const subtype = ra.actor.params & 0x1f;
  let count = (ra.actor.params >> 12);
  if (count === 0) {
    const lut = [12, 9, 8];
    const id = (ra.actor.params >> 8) & 3;
    count = id < lut.length ? lut[id] : 0;
  }
  for (let i = 0; i < count; ++i) {
    if (subtype === 0x04) {
      const item = (i === 0) ? 'FAIRY' : 'NOTHING';
      checks.push({ roomActor: ra, item, name: 'Butterfly Pack', type: 'butterfly', sliceId: i, name2: `Butterfly ${i + 1}` });
    }
  }
}

export function ObjComb(checks: Check[], ra: RoomActor) {
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

export function ObjFlowerpot(checks: Check[], ra: RoomActor) {
  const item = mmCollectibleDrop(ra.actor.params & 0x3f);
  checks.push({ roomActor: ra, sliceId: 0, item: 'NOTHING', name: 'Potted Plant', name2: 'Pot', type: 'pot' });
  checks.push({ roomActor: ra, sliceId: 1, item: item, name: 'Potted Plant', name2: 'Grass', type: 'grass' });
}

export function ObjSnowball(checks: Check[], ra: RoomActor) {
  if (ra.actor.ry == 1) return; /* Goron Elder */

  const item = mmCollectibleDrop(ra.actor.params & 0x3f);
  checks.push({ roomActor: ra, item, name: 'Big Snowball', type: 'snowball' })
}

export function ObjSnowball2(checks: Check[], ra: RoomActor) {
  const item = mmCollectibleDrop(ra.actor.params & 0x3f);
  checks.push({ roomActor: ra, item, name: 'Small Snowball', type: 'snowball' })
}

export function ObjTaru(checks: Check[], ra: RoomActor) {
  if (ra.actor.params & 0x80) return; /* Weird fake-barrel */
  const item = mmCollectibleDrop(ra.actor.params & 0x3f);
  if (item === 'STRAY_FAIRY' || item === 'HEART_PIECE') return;
  checks.push({ roomActor: ra, item, name: 'Barrel', type: 'barrel' });
}

export function BgIcicle(checks: Check[], ra: RoomActor) {
  let item = 'NOTHING';
  switch(ra.actor.params & 0x3) {
    case 0:
    if(((ra.actor.params >> 8) & 0xFF) != 0xff) item = 'RANDOM'; break;
    case 3: item = mmCollectibleDrop((ra.actor.params >> 2) & 0x3F); break;
    default: return;
  }

  checks.push({ roomActor: ra, item, name: 'Icicle', type: 'icicle' });
}

export function EnIshi(checks: Check[], ra: RoomActor) {
  const type = (ra.actor.params & 1);
  const isWall = (ra.actor.params & 4) !== 0;
  if (type !== 0) return;
  const item = 'RANDOM';
  checks.push({ roomActor: ra, item, name: isWall ? 'Rock Wall' : 'Rock', type: 'rock' });
}

export function EnItem00(checks: Check[], ra: RoomActor) {
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

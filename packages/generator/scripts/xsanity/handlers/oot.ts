import type { Check, RoomActor } from '../types';
import { ITEM00_DROPS_OOT, OOT_WONDER_ITEM_DROPS, RUPEES } from '../data';
import { handleWood02 } from './helpers';

let altGrassAcc = 0;

export function EnItem00(checks: Check[], ra: RoomActor) {
  const item00arg = ra.actor.params & 0xff;
  if (item00arg >= ITEM00_DROPS_OOT.length) {
    return;
  }
  const item = ITEM00_DROPS_OOT[item00arg];
  if (RUPEES.has(item)) {
    checks.push({ roomActor: ra, item, name: 'Rupee', type: 'rupee' });
  } else if (item === 'RECOVERY_HEART') {
    checks.push({ roomActor: ra, item, name: 'Heart', type: 'heart' });
  }
}

export function EnKusa(checks: Check[], ra: RoomActor) {
  const { actor } = ra;
  const grassType = (actor.params) & 3;
  let item: string;
  if (grassType == 0 || grassType == 2) {
    item = 'RANDOM';
  } else {
    item = (altGrassAcc & 1) ? 'RECOVERY_HEART' : 'DEKU_SEEDS_5/ARROWS_5';
    altGrassAcc++;
  }
  checks.push({ roomActor: ra, item, name: 'Grass', type: 'grass' });
}

export function EnWonderItem(checks: Check[], ra: RoomActor) {
  const type = (ra.actor.params >>> 11) & 0x1f;
  if (type !== 2 && type !== 3 && type !== 0 && type !== 5) return;
  const itemId = ((ra.actor.params & 0x07c0) >>> 6) & 0x1f;
  const item = OOT_WONDER_ITEM_DROPS[itemId];
  checks.push({ roomActor: ra, item, name: 'Wonder Item', type: 'wonder' });
}


export function ObjComb(checks: Check[], ra: RoomActor) {
  const item = ITEM00_DROPS_OOT[ra.actor.params & 0x1f];
  checks.push({ roomActor: ra, item, name: 'Hive', type: 'hive' });
}

export function ObjKibako(checks: Check[], ra: RoomActor) {
  let item: string;
  if (ra.actor.params === 0xffff) {
    item = 'NOTHING';
  } else {
    const itemId = ra.actor.params & 0xff;
    if (itemId >= ITEM00_DROPS_OOT.length) {
      item = 'NOTHING';
    } else {
      item = ITEM00_DROPS_OOT[itemId];
    }
  }
  checks.push({ roomActor: ra, item, name: 'Small Crate', type: 'crate' });
}

export function ObjKibako2(checks: Check[], ra: RoomActor) {
  if (ra.actor.params !== 0xffff) return; /* Skulltulas */
  const itemId = ra.actor.rx & 0xff;
  let item: string;
  if (itemId >= ITEM00_DROPS_OOT.length) {
    item = 'NOTHING';
  } else {
    item = ITEM00_DROPS_OOT[itemId];
  }
  checks.push({ roomActor: ra, item, name: 'Large Crate', type: 'crate' });
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

export function BgIcicle(checks: Check[], ra: RoomActor) {
  if(ra.actor.params != 0) return;
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Icicle', type: 'icicle' });
}


export function BgIceShelter(checks: Check[], ra: RoomActor) {
  switch(ra.actor.params >> 8) {
    case 0: // Large
    case 1: // Small
    case 2: // Weird structure on Map Chest in MQ
      checks.push({ roomActor: ra, item: 'NOTHING', name: 'Red Ice', type: 'redice' });
    default:
      return;
  }
}

export function EnIshi(checks: Check[], ra: RoomActor) {
  let type: string;
  let name: string;
  let item: string;

  if (ra.actor.params & 1) {
    type = 'boulder-silver';
    name = 'Silver Boulder';
    item = 'NOTHING';
  } else {
    type = 'rock';
    name = 'Rock';
    item = 'RANDOM';
  }

  checks.push({ roomActor: ra, item, name, type });
}

export function ObjHana(checks: Check[], ra: RoomActor) {
  const type = ra.actor.params & 3;
  if (type === 2) {
    checks.push({ roomActor: ra, item: 'NOTHING', name: 'Grass Weird', type: 'grass' });
  }
  if (type === 1) {
    checks.push({ roomActor: ra, item: 'NOTHING', name: 'Rock Weird', type: 'rock' });
  }
}

export function EnWood02(checks: Check[], ra: RoomActor) {
  handleWood02(checks, ra, 'oot');
}

export function ObjBean(checks: Check[], ra: RoomActor) {
  for (let i = 0; i < 3; ++i) {
    checks.push({ roomActor: ra, item: 'FAIRY', name: `Soil ${i + 1}`, type: 'soil', sliceId: i });
  }
}

export function ObjBombiwa(checks: Check[], ra: RoomActor) {
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Boulder', type: 'boulder' });
}

export function EnGs(checks: Check[], ra: RoomActor) {
  checks.push({ roomActor: ra, item: 'FAIRY', name: 'Gossip Fairy', type: 'gossip', sliceId: 0 });
  checks.push({ roomActor: ra, item: 'FAIRY_BIG', name: 'Gossip Big Fairy', type: 'gossip-big', sliceId: 1 });
}

export function EnElf(checks: Check[], ra: RoomActor) {
  const item = 'FAIRY';
  checks.push({ roomActor: ra, item, name: '???', type: 'fairy' });
}

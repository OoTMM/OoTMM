import type { Handler } from './types';

import { ITEM00_DROPS_OOT, OOT_WONDER_ITEM_DROPS, RUPEES, FLYING_POT_DROPS } from '../data';
import { handleWood02 } from './helpers';

let altGrassAcc = 0;

export const EnItem00: Handler = ({ checks, ra }) => {
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

export const EnKusa: Handler = ({ checks, ra }) => {
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

export const EnWonderItem: Handler = ({ checks, ra }) => {
  const type = (ra.actor.params >>> 11) & 0x1f;
  if (type !== 2 && type !== 3 && type !== 0 && type !== 5) return;
  const itemId = ((ra.actor.params & 0x07c0) >>> 6) & 0x1f;
  const item = OOT_WONDER_ITEM_DROPS[itemId];
  checks.push({ roomActor: ra, item, name: 'Wonder Item', type: 'wonder' });
}

export const ObjComb: Handler = ({ checks, ra }) => {
  const item = ITEM00_DROPS_OOT[ra.actor.params & 0x1f];
  checks.push({ roomActor: ra, item, name: 'Hive', type: 'hive' });
}

export const ObjKibako: Handler = ({ checks, ra }) => {
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

export const ObjKibako2: Handler = ({ checks, ra }) => {
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

export const EnButte: Handler = ({ checks, ra }) => {
  const item = (ra.actor.params === 0xffff || !(ra.actor.params & 1)) ? 'NOTHING' : 'FAIRY';
  checks.push({ roomActor: ra, item, name: 'Butterfly', type: 'butterfly' });
}

export const BgIcicle: Handler = ({ checks, ra }) => {
  if(ra.actor.params != 0) return;
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Icicle', type: 'icicle' });
}

export const BgIceShelter: Handler = ({ checks, ra }) => {
  switch(ra.actor.params >> 8) {
    case 0: // Large
    case 1: // Small
    case 2: // Weird structure on Map Chest in MQ
      checks.push({ roomActor: ra, item: 'NOTHING', name: 'Red Ice', type: 'redice' });
    default:
      return;
  }
}

export const EnIshi: Handler = ({ checks, ra }) => {
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

export const ObjHana: Handler = ({ checks, ra }) => {
  const type = ra.actor.params & 3;
  if (type === 2) {
    checks.push({ roomActor: ra, item: 'NOTHING', name: 'Grass Weird', type: 'grass' });
  }
  if (type === 1) {
    checks.push({ roomActor: ra, item: 'NOTHING', name: 'Rock Weird', type: 'rock' });
  }
}

export const EnWood02: Handler = ({ checks, ra }) => {
  handleWood02(checks, ra, 'oot');
}

export const ObjBean: Handler = ({ checks, ra }) => {
  for (let i = 0; i < 3; ++i) {
    checks.push({ roomActor: ra, item: 'FAIRY', name: `Soil ${i + 1}`, type: 'soil', sliceId: i });
  }
}

export const ObjBombiwa: Handler = ({ checks, ra }) => {
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Boulder', type: 'boulder' });
}

export const EnElf: Handler = ({ checks, ra }) => {
  for (let i = 0; i < 8; ++i) {
    checks.push({ roomActor: ra, item: 'FAIRY', name: `Fairy ${i + 1}`, type: 'fairy', sliceId: i });
  }
}

export const BgSpot11Oasis: Handler = ({ checks, ra }) => {
  for (let i = 0; i < 8; ++i) {
    checks.push({ roomActor: ra, item: 'FAIRY', name: `Oasis Fairy ${i + 1}`, type: 'fairy', sliceId: i });
  }
}

export const ObjTsubo: Handler = ({ checks, ra }) => {
  const item00 = (ra.actor.params >> 0) & 0xff;
  let item: string;
  if (item00 >= 0x1a) {
    item = 'NOTHING';
  } else {
    item = ITEM00_DROPS_OOT[item00];
  }
  checks.push({ roomActor: ra, item, name: 'Pot', type: 'pot' });
}

export const EnTuboTrap: Handler = ({ checks, ra }) => {
  const itemId = (ra.actor.params >> 8) & 0xff;
  let item: string;
  if (itemId >= 0x07) {
    item = 'NOTHING';
  } else {
    item = FLYING_POT_DROPS[itemId];
  }
  checks.push({ roomActor: ra, item, name: 'Flying Pot', type: 'pot' });
}

import type { Check, RoomActor } from '../types';

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

export function ObjMure2(checks: Check[], ra: RoomActor) {
  const type = (ra.actor.params) & 3;
  let checkType: string;
  let checkName: string;
  let checkName2: string;
  let count: number;
  if (type >= 2) {
    count = 8;
    checkType = 'rock';
    checkName = 'Rock Circle';
    checkName2 = 'Rock';
  } else {
    count = (type === 0) ? 9 : 12;
    checkType = 'grass';
    checkName = 'Grass Pack';
    checkName2 = 'Grass';
  }

  const item = 'RANDOM';
  for (let i = 0; i < count; ++i) {
    checks.push({ roomActor: ra, item, name: checkName, type: checkType, sliceId: i, name2: `${checkName2} ${i + 1}` });
  }
}

export function ObjMure3(checks: Check[], ra: RoomActor) {
  let items: string[] = [];
  switch (ra.actor.params & 0xe000) {
  case 0x0000:
    items = Array(5).fill('RUPEE_BLUE');
    break;
  case 0x2000:
    items = Array(5).fill('RUPEE_GREEN');
    break;
  case 0x4000:
    items = Array(6).fill('RUPEE_GREEN');
    items.push('RUPEE_RED');
    break;
  default:
    return;
  }

  for (let i = 0; i < items.length; ++i) {
    const item = items[i];
    checks.push({ roomActor: ra, item, name: 'Rupee Circle', type: 'rupee', sliceId: i, name2: `Rupee ${i + 1}` });
  }
}

export function ObjHamishi(checks: Check[], ra: RoomActor) {
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Red Boulder', type: 'boulder-red' });
}

export function EnGs(checks: Check[], ra: RoomActor) {
  checks.push({ roomActor: ra, item: 'FAIRY', name: 'Gossip Fairy', type: 'gossip', sliceId: 0 });
  checks.push({ roomActor: ra, item: 'FAIRY_BIG', name: 'Gossip Big Fairy', type: 'gossip-big', sliceId: 1 });
}

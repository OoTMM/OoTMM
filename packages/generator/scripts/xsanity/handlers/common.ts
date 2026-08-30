import type { Check, RoomActor } from '../types';

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

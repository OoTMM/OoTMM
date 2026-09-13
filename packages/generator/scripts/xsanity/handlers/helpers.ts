import type { Game } from '@ootmm/data';
import type { Check, RoomActor } from '../types';

export function handleWood02(checks: Check[], ra: RoomActor, game: Game) {
  const { actor } = ra;
  let checkType: 'bush' | 'tree';
  const type = (actor.params) & 0xff;
  if (type > 0x16) return;
  if (type <= 0x0a)
    checkType = 'tree';
  else
    checkType = 'bush';

  if (game === 'oot' && (ra.actor.rz & 0xff)) return;
  let count = 1;
  if (type === 3 || type === 6 || type === 8 || type === 0x0d || type === 0x0f || type === 0x13 || type === 0x15)
    count = 6;
  let item: string;

  if (checkType === 'tree') {
    switch ((ra.actor.params >>> 8) & 0xff) {
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x06:
    case 0x07:
    case 0x0d:
    case 0x0e:
      item = 'RANDOM';
      break;
    case 0x08:
      item = 'DEKU_SEEDS_5/ARROWS_5';
      break;
    case 0x09:
      item = 'MAGIC_JAR_SMALL';
      break;
    case 0x0a:
      item = 'BOMBS_5';
      break;
    case 0x0b:
      item = 'RUPEE_GREEN';
      break;
    case 0x0c:
      item = 'RUPEE_BLUE';
      break;
    default:
      item = 'NOTHING';
      break;
    }
  } else {
    item = 'NOTHING';
  }

  let name = checkType === 'tree' ? 'Tree' : 'Bush';
  let nameCluster = checkType === 'tree' ? 'Tree Cluster' : 'Bush Cluster';

  if (count > 1) {
    for (let i = 0; i < count; ++i)
      checks.push({ roomActor: ra, item, name: nameCluster, type: checkType, sliceId: i, name2: `${name} ${i + 1}` });
  } else {
    checks.push({ roomActor: ra, item, name, type: checkType });
  }
}

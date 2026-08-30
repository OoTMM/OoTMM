import type { Game } from '@ootmm/data';
import type { Check, RoomActor } from '../types';

import { ACTORS_MM, ACTORS_OOT, ITEM00_DROPS_MM, ITEM00_DROPS_OOT, MM_ITEM00_DROP_COLLECTIBLE_TABLE, OOT_WONDER_ITEM_DROPS, RUPEES } from '../data';

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

let altGrassAcc = 0;

function actorHandlerOotEnWonderItem(checks: Check[], ra: RoomActor) {
  const type = (ra.actor.params >>> 11) & 0x1f;
  if (type !== 2 && type !== 3 && type !== 0 && type !== 5) return;
  const itemId = ((ra.actor.params & 0x07c0) >>> 6) & 0x1f;
  const item = OOT_WONDER_ITEM_DROPS[itemId];
  checks.push({ roomActor: ra, item, name: 'Wonder Item', type: 'wonder' });
}

function actorHandlerOotObjBean(checks: Check[], ra: RoomActor) {
  for (let i = 0; i < 3; ++i) {
    checks.push({ roomActor: ra, item: 'FAIRY', name: `Soil ${i + 1}`, type: 'soil', sliceId: i });
  }
}

function actorHandlerMmObjSwprize(checks: Check[], ra: RoomActor) {
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

function actorHandlerOotEnKusa(checks: Check[], ra: RoomActor) {
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

function actorHandlerOotObjBombiwa(checks: Check[], ra: RoomActor) {
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Boulder', type: 'boulder' });
}

function actorHandlerMmObjBombiwa(checks: Check[], ra: RoomActor) {
  const name = (ra.actor.params & 0x100) ? 'Large Boulder' : 'Boulder';
  checks.push({ roomActor: ra, item: 'NOTHING', name, type: 'boulder' });
}

function handleWood02(checks: Check[], ra: RoomActor, game: Game) {
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

function actorHandlerOotEnWood02(checks: Check[], ra: RoomActor) {
  handleWood02(checks, ra, 'oot');
}

function actorHandlerMmEnWood02(checks: Check[], ra: RoomActor) {
  handleWood02(checks, ra, 'mm');
}

function actorHandlerMmObjYasi(checks: Check[], ra: RoomActor) {
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Palm Tree', type: 'tree' });
}

function actorHandlerMmEnSnowwd(checks: Check[], ra: RoomActor) {
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Snow Tree', type: 'tree' });
}

function actorHandlerMmObjTree(checks: Check[], ra: RoomActor) {
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Forked Tree', type: 'tree' });
}

function actorHandlerMmEnKusa(checks: Check[], ra: RoomActor) {
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

function actorHandlerMmEnKusa2(checks: Check[], ra: RoomActor) {
  for (let i = 0; i < 9; ++i) {
    const item = (i == 8) ? 'RUPEE_RED' : 'RUPEE_GREEN';
    checks.push({ roomActor: ra, item, name: 'Keaton Grass', type: 'grass', subtype: 'keaton', sliceId: i, name2: `Grass ${i + 1}` });
  }
}

function actorHandlerOotObjHana(checks: Check[], ra: RoomActor) {
  const type = ra.actor.params & 3;
  if (type === 2) {
    checks.push({ roomActor: ra, item: 'NOTHING', name: 'Grass Weird', type: 'grass' });
  }
  if (type === 1) {
    checks.push({ roomActor: ra, item: 'NOTHING', name: 'Rock Weird', type: 'rock' });
  }
}

function actorHandlerObjMure2(checks: Check[], ra: RoomActor) {
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

function actorHandlerMmObjGrassUnit(checks: Check[], ra: RoomActor) {
  const count = (ra.actor.params & 1) ? 12 : 9;
  const item = 'RANDOM';
  for (let i = 0; i < count; ++i) {
    checks.push({ roomActor: ra, item, name: 'Grass Unit Pack', type: 'grass', subtype: 'unit', sliceId: i, name2: `Grass ${i + 1}` });
  }
}

function actorHandlerOotObjComb(checks: Check[], ra: RoomActor) {
  const item = ITEM00_DROPS_OOT[ra.actor.params & 0x1f];
  checks.push({ roomActor: ra, item, name: 'Hive', type: 'hive' });
}

function actorHandlerOotObjKibako(checks: Check[], ra: RoomActor) {
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

function actorHandlerOotObjKibako2(checks: Check[], ra: RoomActor) {
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

function actorHandlerOotEnButte(checks: Check[], ra: RoomActor) {
  const item = (ra.actor.params === 0xffff || !(ra.actor.params & 1)) ? 'NOTHING' : 'FAIRY';
  checks.push({ roomActor: ra, item, name: 'Butterfly', type: 'butterfly' });
}

function actorHandlerMmEnButte(checks: Check[], ra: RoomActor) {
  const item = (ra.actor.params === 0xffff || !(ra.actor.params & 1)) ? 'NOTHING' : 'FAIRY';
  checks.push({ roomActor: ra, item, name: 'Butterfly', type: 'butterfly' });
}

function actorHandlerOotObjMure(checks: Check[], ra: RoomActor) {
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

function actorHandlerMmObjMure(checks: Check[], ra: RoomActor) {
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

function actorHandlerMmObjComb(checks: Check[], ra: RoomActor) {
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

function actorHandlerMmObjFlowerpot(checks: Check[], ra: RoomActor) {
  const item = mmCollectibleDrop(ra.actor.params & 0x3f);
  checks.push({ roomActor: ra, sliceId: 0, item: 'NOTHING', name: 'Potted Plant', name2: 'Pot', type: 'pot' });
  checks.push({ roomActor: ra, sliceId: 1, item: item, name: 'Potted Plant', name2: 'Grass', type: 'grass' });
}

function actorHandlerMmObjSnowball(checks: Check[], ra: RoomActor) {
  if (ra.actor.ry == 1) return; /* Goron Elder */

  const item = mmCollectibleDrop(ra.actor.params & 0x3f);
  checks.push({ roomActor: ra, item, name: 'Big Snowball', type: 'snowball' })
}

function actorHandlerMmObjSnowball2(checks: Check[], ra: RoomActor) {
  const item = mmCollectibleDrop(ra.actor.params & 0x3f);
  checks.push({ roomActor: ra, item, name: 'Small Snowball', type: 'snowball' })
}

function actorHandlerMmObjTaru(checks: Check[], ra: RoomActor) {
  if (ra.actor.params & 0x80) return; /* Weird fake-barrel */
  const item = mmCollectibleDrop(ra.actor.params & 0x3f);
  if (item === 'STRAY_FAIRY' || item === 'HEART_PIECE') return;
  checks.push({ roomActor: ra, item, name: 'Barrel', type: 'barrel' });
}

function actorHandlerObjHamishi(checks: Check[], ra: RoomActor) {
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Red Boulder', type: 'boulder-red' });
}

function actorHandlerOotBgIcicle(checks: Check[], ra: RoomActor) {
  if(ra.actor.params != 0) return;
  checks.push({ roomActor: ra, item: 'NOTHING', name: 'Icicle', type: 'icicle' });
}

function actorHandlerMmBgIcicle(checks: Check[], ra: RoomActor) {
  let item = 'NOTHING';
  switch(ra.actor.params & 0x3) {
    case 0:
    if(((ra.actor.params >> 8) & 0xFF) != 0xff) item = 'RANDOM'; break;
    case 3: item = mmCollectibleDrop((ra.actor.params >> 2) & 0x3F); break;
    default: return;
  }

  checks.push({ roomActor: ra, item, name: 'Icicle', type: 'icicle' });
}

function actorHandlerOotBgIceShelter(checks: Check[], ra: RoomActor) {
  switch(ra.actor.params >> 8) {
    case 0: // Large
    case 1: // Small
    case 2: // Weird structure on Map Chest in MQ
      checks.push({ roomActor: ra, item: 'NOTHING', name: 'Red Ice', type: 'redice' });
    default:
      return;
  }
}

function actorHandlerOotEnItem00(checks: Check[], ra: RoomActor) {
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

function actorHandlerOotEnIshi(checks: Check[], ra: RoomActor) {
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

function actorHandlerMmEnIshi(checks: Check[], ra: RoomActor) {
  const type = (ra.actor.params & 1);
  const isWall = (ra.actor.params & 4) !== 0;
  if (type !== 0) return;
  const item = 'RANDOM';
  checks.push({ roomActor: ra, item, name: isWall ? 'Rock Wall' : 'Rock', type: 'rock' });
}

function actorHandlerMmEnItem00(checks: Check[], ra: RoomActor) {
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

function actorHandlerCommonObjMure3(checks: Check[], ra: RoomActor) {
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

const ACTORS_HANDLERS_OOT = {
  [ACTORS_OOT.EN_ITEM00]: actorHandlerOotEnItem00,
  [ACTORS_OOT.OBJ_MURE3]: actorHandlerCommonObjMure3,
  [ACTORS_OOT.EN_KUSA]: actorHandlerOotEnKusa,
  [ACTORS_OOT.OBJ_COMB]: actorHandlerOotObjComb,
  [ACTORS_OOT.OBJ_KIBAKO]: actorHandlerOotObjKibako,
  [ACTORS_OOT.OBJ_KIBAKO2]: actorHandlerOotObjKibako2,
  [ACTORS_OOT.EN_BUTTE]: actorHandlerOotEnButte,
  [ACTORS_OOT.OBJ_MURE]: actorHandlerOotObjMure,
  [ACTORS_OOT.OBJ_HANA]: actorHandlerOotObjHana,
  [ACTORS_OOT.OBJ_MURE2]: actorHandlerObjMure2,
  [ACTORS_OOT.OBJ_HAMISHI]: actorHandlerObjHamishi,
  [ACTORS_OOT.BG_ICICLE]: actorHandlerOotBgIcicle,
  [ACTORS_OOT.BG_ICE_SHELTER]: actorHandlerOotBgIceShelter,
  [ACTORS_OOT.EN_ISHI]: actorHandlerOotEnIshi,
  [ACTORS_OOT.EN_WOOD02]: actorHandlerOotEnWood02,
  [ACTORS_OOT.OBJ_BEAN]: actorHandlerOotObjBean,
  [ACTORS_OOT.OBJ_BOMBIWA]: actorHandlerOotObjBombiwa,
  [ACTORS_OOT.EN_WONDER_ITEM]: actorHandlerOotEnWonderItem,
};

const ACTORS_HANDLERS_MM = {
  [ACTORS_MM.EN_ITEM00]: actorHandlerMmEnItem00,
  [ACTORS_MM.OBJ_MURE3]: actorHandlerCommonObjMure3,
  [ACTORS_MM.EN_KUSA]: actorHandlerMmEnKusa,
  [ACTORS_MM.EN_KUSA2]: actorHandlerMmEnKusa2,
  [ACTORS_MM.OBJ_COMB]: actorHandlerMmObjComb,
  [ACTORS_MM.OBJ_FLOWERPOT]: actorHandlerMmObjFlowerpot,
  [ACTORS_MM.OBJ_TARU]: actorHandlerMmObjTaru,
  [ACTORS_MM.OBJ_SNOWBALL]: actorHandlerMmObjSnowball,
  [ACTORS_MM.OBJ_SNOWBALL2]: actorHandlerMmObjSnowball2,
  [ACTORS_MM.EN_BUTTE]: actorHandlerMmEnButte,
  [ACTORS_MM.OBJ_MURE]: actorHandlerMmObjMure,
  [ACTORS_MM.OBJ_MURE2]: actorHandlerObjMure2,
  [ACTORS_MM.OBJ_GRASS_UNIT]: actorHandlerMmObjGrassUnit,
  [ACTORS_MM.OBJ_HAMISHI]: actorHandlerObjHamishi,
  [ACTORS_MM.BG_ICICLE]: actorHandlerMmBgIcicle,
  [ACTORS_MM.EN_ISHI]: actorHandlerMmEnIshi,
  [ACTORS_MM.EN_WOOD02]: actorHandlerMmEnWood02,
  [ACTORS_MM.OBJ_YASI]: actorHandlerMmObjYasi,
  [ACTORS_MM.EN_SNOWWD]: actorHandlerMmEnSnowwd,
  [ACTORS_MM.OBJ_TREE]: actorHandlerMmObjTree,
  [ACTORS_MM.OBJ_SWPRIZE]: actorHandlerMmObjSwprize,
  [ACTORS_MM.OBJ_BOMBIWA]: actorHandlerMmObjBombiwa,
};

export const ACTORS_HANDLERS = {
  oot: ACTORS_HANDLERS_OOT,
  mm: ACTORS_HANDLERS_MM,
};

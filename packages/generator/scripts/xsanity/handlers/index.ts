import type { HandlerMap } from './types';

import { ACTORS_MM, ACTORS_OOT } from '../data';
import * as CommonHandlers from './common';
import * as OotHandlers from './oot';
import * as MmHandlers from './mm';

const ACTORS_HANDLERS_OOT: HandlerMap = {
  [ACTORS_OOT.OBJ_MURE]: CommonHandlers.ObjMure,
  [ACTORS_OOT.OBJ_MURE2]: CommonHandlers.ObjMure2,
  [ACTORS_OOT.OBJ_MURE3]: CommonHandlers.ObjMure3,
  [ACTORS_OOT.OBJ_HAMISHI]: CommonHandlers.ObjHamishi,
  [ACTORS_OOT.EN_GS]: CommonHandlers.EnGs,
  [ACTORS_OOT.EN_ELF]: CommonHandlers.EnElf,
  [ACTORS_OOT.SHOT_SUN]: OotHandlers.ShotSun,
  [ACTORS_OOT.OBJ_TSUBO]: OotHandlers.ObjTsubo,
  [ACTORS_OOT.EN_TUBO_TRAP]: OotHandlers.EnTuboTrap,
  [ACTORS_OOT.EN_ITEM00]: OotHandlers.EnItem00,
  [ACTORS_OOT.EN_KUSA]: OotHandlers.EnKusa,
  [ACTORS_OOT.OBJ_COMB]: OotHandlers.ObjComb,
  [ACTORS_OOT.OBJ_KIBAKO]: OotHandlers.ObjKibako,
  [ACTORS_OOT.OBJ_KIBAKO2]: OotHandlers.ObjKibako2,
  [ACTORS_OOT.EN_BUTTE]: OotHandlers.EnButte,
  [ACTORS_OOT.OBJ_HANA]: OotHandlers.ObjHana,
  [ACTORS_OOT.BG_ICICLE]: OotHandlers.BgIcicle,
  [ACTORS_OOT.BG_ICE_SHELTER]: OotHandlers.BgIceShelter,
  [ACTORS_OOT.EN_ISHI]: OotHandlers.EnIshi,
  [ACTORS_OOT.EN_WOOD02]: OotHandlers.EnWood02,
  [ACTORS_OOT.OBJ_BEAN]: OotHandlers.ObjBean,
  [ACTORS_OOT.OBJ_BOMBIWA]: OotHandlers.ObjBombiwa,
  [ACTORS_OOT.EN_WONDER_ITEM]: OotHandlers.EnWonderItem,
  [ACTORS_OOT.BG_SPOT11_OASIS]: OotHandlers.BgSpot11Oasis,
};

const ACTORS_HANDLERS_MM: HandlerMap = {
  [ACTORS_MM.OBJ_MURE]: CommonHandlers.ObjMure,
  [ACTORS_MM.OBJ_MURE2]: CommonHandlers.ObjMure2,
  [ACTORS_MM.OBJ_MURE3]: CommonHandlers.ObjMure3,
  [ACTORS_MM.OBJ_HAMISHI]: CommonHandlers.ObjHamishi,
  [ACTORS_MM.EN_GS]: CommonHandlers.EnGs,
  [ACTORS_MM.EN_ELF]: CommonHandlers.EnElf,
  [ACTORS_MM.OBJ_TSUBO]: MmHandlers.ObjTsubo,
  [ACTORS_MM.EN_TUBO_TRAP]: MmHandlers.EnTuboTrap,
  [ACTORS_MM.EN_ITEM00]: MmHandlers.EnItem00,
  [ACTORS_MM.EN_KUSA]: MmHandlers.EnKusa,
  [ACTORS_MM.EN_KUSA2]: MmHandlers.EnKusa2,
  [ACTORS_MM.OBJ_COMB]: MmHandlers.ObjComb,
  [ACTORS_MM.OBJ_FLOWERPOT]: MmHandlers.ObjFlowerpot,
  [ACTORS_MM.OBJ_TARU]: MmHandlers.ObjTaru,
  [ACTORS_MM.OBJ_SNOWBALL]: MmHandlers.ObjSnowball,
  [ACTORS_MM.OBJ_SNOWBALL2]: MmHandlers.ObjSnowball2,
  [ACTORS_MM.EN_BUTTE]: MmHandlers.EnButte,
  [ACTORS_MM.OBJ_GRASS_UNIT]: MmHandlers.ObjGrassUnit,
  [ACTORS_MM.BG_ICICLE]: MmHandlers.BgIcicle,
  [ACTORS_MM.EN_ISHI]: MmHandlers.EnIshi,
  [ACTORS_MM.EN_WOOD02]: MmHandlers.EnWood02,
  [ACTORS_MM.OBJ_YASI]: MmHandlers.ObjYasi,
  [ACTORS_MM.EN_SNOWWD]: MmHandlers.EnSnowwd,
  [ACTORS_MM.OBJ_TREE]: MmHandlers.ObjTree,
  [ACTORS_MM.OBJ_SWPRIZE]: MmHandlers.ObjSwprize,
  [ACTORS_MM.OBJ_BOMBIWA]: MmHandlers.ObjBombiwa,
};

export const ACTORS_HANDLERS = {
  oot: ACTORS_HANDLERS_OOT,
  mm: ACTORS_HANDLERS_MM,
};

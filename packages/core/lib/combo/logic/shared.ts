import { Item, Items } from '../items/defs';

export type ItemSharedDef = {
  shared: Item,
  oot: Item,
  mm: Item,
};

const SOULS: ItemSharedDef[] = [
  { shared: Items.SHARED_SOUL_OCTOROK,             oot: Items.OOT_SOUL_OCTOROK,            mm: Items.MM_SOUL_OCTOROK },
  { shared: Items.SHARED_SOUL_WALLMASTER,          oot: Items.OOT_SOUL_WALLMASTER,         mm: Items.MM_SOUL_WALLMASTER },
  { shared: Items.SHARED_SOUL_DODONGO,             oot: Items.OOT_SOUL_DODONGO,            mm: Items.MM_SOUL_DODONGO },
  { shared: Items.SHARED_SOUL_KEESE,               oot: Items.OOT_SOUL_KEESE,              mm: Items.MM_SOUL_KEESE },
  { shared: Items.SHARED_SOUL_TEKTITE,             oot: Items.OOT_SOUL_TEKTITE,            mm: Items.MM_SOUL_TEKTITE },
  { shared: Items.SHARED_SOUL_PEAHAT,              oot: Items.OOT_SOUL_PEAHAT,             mm: Items.MM_SOUL_PEAHAT },
  { shared: Items.SHARED_SOUL_LIZALFOS_DINALFOS,   oot: Items.OOT_SOUL_LIZALFOS_DINALFOS,  mm: Items.MM_SOUL_LIZALFOS_DINALFOS },
  { shared: Items.SHARED_SOUL_SKULLTULA,           oot: Items.OOT_SOUL_SKULLTULA,          mm: Items.MM_SOUL_SKULLTULA },
  { shared: Items.SHARED_SOUL_ARMOS,               oot: Items.OOT_SOUL_ARMOS,              mm: Items.MM_SOUL_ARMOS },
  { shared: Items.SHARED_SOUL_DEKU_BABA,           oot: Items.OOT_SOUL_DEKU_BABA,          mm: Items.MM_SOUL_DEKU_BABA },
  { shared: Items.SHARED_SOUL_DEKU_SCRUB,          oot: Items.OOT_SOUL_DEKU_SCRUB,         mm: Items.MM_SOUL_DEKU_SCRUB },
  { shared: Items.SHARED_SOUL_BUBBLE,              oot: Items.OOT_SOUL_BUBBLE,             mm: Items.MM_SOUL_BUBBLE },
  { shared: Items.SHARED_SOUL_BEAMOS,              oot: Items.OOT_SOUL_BEAMOS,             mm: Items.MM_SOUL_BEAMOS },
  { shared: Items.SHARED_SOUL_REDEAD_GIBDO,        oot: Items.OOT_SOUL_REDEAD_GIBDO,       mm: Items.MM_SOUL_REDEAD_GIBDO },
  { shared: Items.SHARED_SOUL_SKULLWALLTULA,       oot: Items.OOT_SOUL_SKULLWALLTULA,      mm: Items.MM_SOUL_SKULLWALLTULA },
  { shared: Items.SHARED_SOUL_SHELL_BLADE,         oot: Items.OOT_SOUL_SHELL_BLADE,        mm: Items.MM_SOUL_SHELL_BLADE },
  { shared: Items.SHARED_SOUL_LIKE_LIKE,           oot: Items.OOT_SOUL_LIKE_LIKE,          mm: Items.MM_SOUL_LIKE_LIKE },
  { shared: Items.SHARED_SOUL_IRON_KNUCKLE,        oot: Items.OOT_SOUL_IRON_KNUCKLE,       mm: Items.MM_SOUL_IRON_KNUCKLE },
  { shared: Items.SHARED_SOUL_FREEZARD,            oot: Items.OOT_SOUL_FREEZARD,           mm: Items.MM_SOUL_FREEZARD },
  { shared: Items.SHARED_SOUL_WOLFOS,              oot: Items.OOT_SOUL_WOLFOS,             mm: Items.MM_SOUL_WOLFOS },
  { shared: Items.SHARED_SOUL_GUAY,                oot: Items.OOT_SOUL_GUAY,               mm: Items.MM_SOUL_GUAY },
  { shared: Items.SHARED_SOUL_FLYING_POT,          oot: Items.OOT_SOUL_FLYING_POT,         mm: Items.MM_SOUL_FLYING_POT },
  { shared: Items.SHARED_SOUL_FLOORMASTER,         oot: Items.OOT_SOUL_FLOORMASTER,        mm: Items.MM_SOUL_FLOORMASTER },
  { shared: Items.SHARED_SOUL_LEEVER,              oot: Items.OOT_SOUL_LEEVER,             mm: Items.MM_SOUL_LEEVER },
  { shared: Items.SHARED_SOUL_STALCHILD,           oot: Items.OOT_SOUL_STALCHILD,          mm: Items.MM_SOUL_STALCHILD },
];

const SKELETON_KEY: ItemSharedDef[] = [
  { shared: Items.SHARED_SKELETON_KEY, oot: Items.OOT_SKELETON_KEY, mm: Items.MM_SKELETON_KEY },
];

const OCARINA_BUTTONS: ItemSharedDef[] = [
  { shared: Items.SHARED_BUTTON_A, oot: Items.OOT_BUTTON_A, mm: Items.MM_BUTTON_A },
  { shared: Items.SHARED_BUTTON_C_RIGHT, oot: Items.OOT_BUTTON_C_RIGHT, mm: Items.MM_BUTTON_C_RIGHT },
  { shared: Items.SHARED_BUTTON_C_LEFT, oot: Items.OOT_BUTTON_C_LEFT, mm: Items.MM_BUTTON_C_LEFT },
  { shared: Items.SHARED_BUTTON_C_UP, oot: Items.OOT_BUTTON_C_UP, mm: Items.MM_BUTTON_C_UP },
  { shared: Items.SHARED_BUTTON_C_DOWN, oot: Items.OOT_BUTTON_C_DOWN, mm: Items.MM_BUTTON_C_DOWN },
];


export const SharedItemGroups = { SOULS, SKELETON_KEY, OCARINA_BUTTONS };

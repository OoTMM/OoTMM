import { Item, Items } from '../items/defs';

export type ItemSharedDef = {
  shared: Item,
  oot: Item,
  mm: Item,
};

const SOULS_ENEMY: ItemSharedDef[] = [
  { shared: Items.SHARED_SOUL_ENEMY_OCTOROK,             oot: Items.OOT_SOUL_ENEMY_OCTOROK,            mm: Items.MM_SOUL_ENEMY_OCTOROK },
  { shared: Items.SHARED_SOUL_ENEMY_WALLMASTER,          oot: Items.OOT_SOUL_ENEMY_WALLMASTER,         mm: Items.MM_SOUL_ENEMY_WALLMASTER },
  { shared: Items.SHARED_SOUL_ENEMY_DODONGO,             oot: Items.OOT_SOUL_ENEMY_DODONGO,            mm: Items.MM_SOUL_ENEMY_DODONGO },
  { shared: Items.SHARED_SOUL_ENEMY_KEESE,               oot: Items.OOT_SOUL_ENEMY_KEESE,              mm: Items.MM_SOUL_ENEMY_KEESE },
  { shared: Items.SHARED_SOUL_ENEMY_TEKTITE,             oot: Items.OOT_SOUL_ENEMY_TEKTITE,            mm: Items.MM_SOUL_ENEMY_TEKTITE },
  { shared: Items.SHARED_SOUL_ENEMY_PEAHAT,              oot: Items.OOT_SOUL_ENEMY_PEAHAT,             mm: Items.MM_SOUL_ENEMY_PEAHAT },
  { shared: Items.SHARED_SOUL_ENEMY_LIZALFOS_DINALFOS,   oot: Items.OOT_SOUL_ENEMY_LIZALFOS_DINALFOS,  mm: Items.MM_SOUL_ENEMY_LIZALFOS_DINALFOS },
  { shared: Items.SHARED_SOUL_ENEMY_SKULLTULA,           oot: Items.OOT_SOUL_ENEMY_SKULLTULA,          mm: Items.MM_SOUL_ENEMY_SKULLTULA },
  { shared: Items.SHARED_SOUL_ENEMY_ARMOS,               oot: Items.OOT_SOUL_ENEMY_ARMOS,              mm: Items.MM_SOUL_ENEMY_ARMOS },
  { shared: Items.SHARED_SOUL_ENEMY_DEKU_BABA,           oot: Items.OOT_SOUL_ENEMY_DEKU_BABA,          mm: Items.MM_SOUL_ENEMY_DEKU_BABA },
  { shared: Items.SHARED_SOUL_ENEMY_DEKU_SCRUB,          oot: Items.OOT_SOUL_ENEMY_DEKU_SCRUB,         mm: Items.MM_SOUL_ENEMY_DEKU_SCRUB },
  { shared: Items.SHARED_SOUL_ENEMY_BUBBLE,              oot: Items.OOT_SOUL_ENEMY_BUBBLE,             mm: Items.MM_SOUL_ENEMY_BUBBLE },
  { shared: Items.SHARED_SOUL_ENEMY_BEAMOS,              oot: Items.OOT_SOUL_ENEMY_BEAMOS,             mm: Items.MM_SOUL_ENEMY_BEAMOS },
  { shared: Items.SHARED_SOUL_ENEMY_REDEAD_GIBDO,        oot: Items.OOT_SOUL_ENEMY_REDEAD_GIBDO,       mm: Items.MM_SOUL_ENEMY_REDEAD_GIBDO },
  { shared: Items.SHARED_SOUL_ENEMY_SKULLWALLTULA,       oot: Items.OOT_SOUL_ENEMY_SKULLWALLTULA,      mm: Items.MM_SOUL_ENEMY_SKULLWALLTULA },
  { shared: Items.SHARED_SOUL_ENEMY_SHELL_BLADE,         oot: Items.OOT_SOUL_ENEMY_SHELL_BLADE,        mm: Items.MM_SOUL_ENEMY_SHELL_BLADE },
  { shared: Items.SHARED_SOUL_ENEMY_LIKE_LIKE,           oot: Items.OOT_SOUL_ENEMY_LIKE_LIKE,          mm: Items.MM_SOUL_ENEMY_LIKE_LIKE },
  { shared: Items.SHARED_SOUL_ENEMY_IRON_KNUCKLE,        oot: Items.OOT_SOUL_ENEMY_IRON_KNUCKLE,       mm: Items.MM_SOUL_ENEMY_IRON_KNUCKLE },
  { shared: Items.SHARED_SOUL_ENEMY_FREEZARD,            oot: Items.OOT_SOUL_ENEMY_FREEZARD,           mm: Items.MM_SOUL_ENEMY_FREEZARD },
  { shared: Items.SHARED_SOUL_ENEMY_WOLFOS,              oot: Items.OOT_SOUL_ENEMY_WOLFOS,             mm: Items.MM_SOUL_ENEMY_WOLFOS },
  { shared: Items.SHARED_SOUL_ENEMY_GUAY,                oot: Items.OOT_SOUL_ENEMY_GUAY,               mm: Items.MM_SOUL_ENEMY_GUAY },
  { shared: Items.SHARED_SOUL_ENEMY_FLYING_POT,          oot: Items.OOT_SOUL_ENEMY_FLYING_POT,         mm: Items.MM_SOUL_ENEMY_FLYING_POT },
  { shared: Items.SHARED_SOUL_ENEMY_FLOORMASTER,         oot: Items.OOT_SOUL_ENEMY_FLOORMASTER,        mm: Items.MM_SOUL_ENEMY_FLOORMASTER },
  { shared: Items.SHARED_SOUL_ENEMY_LEEVER,              oot: Items.OOT_SOUL_ENEMY_LEEVER,             mm: Items.MM_SOUL_ENEMY_LEEVER },
  { shared: Items.SHARED_SOUL_ENEMY_STALCHILD,           oot: Items.OOT_SOUL_ENEMY_STALCHILD,          mm: Items.MM_SOUL_ENEMY_STALCHILD },
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


export const SharedItemGroups = { SOULS_ENEMY, SKELETON_KEY, OCARINA_BUTTONS };

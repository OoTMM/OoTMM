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

const SOULS_NPC: ItemSharedDef[] = [
  { shared: Items.SHARED_SOUL_NPC_SHOOTING_GALLERY_OWNER, oot: Items.OOT_SOUL_NPC_SHOOTING_GALLERY_OWNER, mm: Items.MM_SOUL_NPC_SHOOTING_GALLERY_OWNER },
  { shared: Items.SHARED_SOUL_NPC_BAZAAR_SHOPKEEPER, oot: Items.OOT_SOUL_NPC_BAZAAR_SHOPKEEPER, mm: Items.MM_SOUL_NPC_BAZAAR_SHOPKEEPER },
  { shared: Items.SHARED_SOUL_NPC_GORON, oot: Items.OOT_SOUL_NPC_GORON, mm: Items.MM_SOUL_NPC_GORON },
  { shared: Items.SHARED_SOUL_NPC_GORON_CHILD, oot: Items.OOT_SOUL_NPC_GORON_CHILD, mm: Items.MM_SOUL_NPC_GORON_CHILD },
  { shared: Items.SHARED_SOUL_NPC_BOMBCHU_SHOPKEEPER, oot: Items.OOT_SOUL_NPC_BOMBCHU_SHOPKEEPER, mm: Items.MM_SOUL_NPC_BOMBCHU_SHOPKEEPER },
  { shared: Items.SHARED_SOUL_NPC_BOMBERS, oot: Items.OOT_SOUL_NPC_BOMBERS, mm: Items.MM_SOUL_NPC_BOMBERS },
  { shared: Items.SHARED_SOUL_NPC_CITIZEN, oot: Items.OOT_SOUL_NPC_CITIZEN, mm: Items.MM_SOUL_NPC_CITIZEN },
  { shared: Items.SHARED_SOUL_NPC_COMPOSER_BROS, oot: Items.OOT_SOUL_NPC_COMPOSER_BROS, mm: Items.MM_SOUL_NPC_COMPOSER_BROS },
  { shared: Items.SHARED_SOUL_NPC_DAMPE, oot: Items.OOT_SOUL_NPC_DAMPE, mm: Items.MM_SOUL_NPC_DAMPE },
  { shared: Items.SHARED_SOUL_NPC_CHEST_GAME_OWNER, oot: Items.OOT_SOUL_NPC_CHEST_GAME_OWNER, mm: Items.MM_SOUL_NPC_CHEST_GAME_OWNER },
  { shared: Items.SHARED_SOUL_NPC_GORON_SHOPKEEPER, oot: Items.OOT_SOUL_NPC_GORON_SHOPKEEPER, mm: Items.MM_SOUL_NPC_GORON_SHOPKEEPER },
  { shared: Items.SHARED_SOUL_NPC_HONEY_DARLING, oot: Items.OOT_SOUL_NPC_HONEY_DARLING, mm: Items.MM_SOUL_NPC_HONEY_DARLING },
  { shared: Items.SHARED_SOUL_NPC_RUTO, oot: Items.OOT_SOUL_NPC_RUTO, mm: Items.MM_SOUL_NPC_RUTO },
  { shared: Items.SHARED_SOUL_NPC_MEDIGORON, oot: Items.OOT_SOUL_NPC_MEDIGORON, mm: Items.MM_SOUL_NPC_MEDIGORON },
  { shared: Items.SHARED_SOUL_NPC_BIGGORON, oot: Items.OOT_SOUL_NPC_BIGGORON, mm: Items.MM_SOUL_NPC_BIGGORON },
  { shared: Items.SHARED_SOUL_NPC_TALON, oot: Items.OOT_SOUL_NPC_TALON, mm: Items.MM_SOUL_NPC_TALON },
  { shared: Items.SHARED_SOUL_NPC_ASTRONOMER, oot: Items.OOT_SOUL_NPC_ASTRONOMER, mm: Items.MM_SOUL_NPC_ASTRONOMER },
  { shared: Items.SHARED_SOUL_NPC_POE_COLLECTOR, oot: Items.OOT_SOUL_NPC_POE_COLLECTOR, mm: Items.MM_SOUL_NPC_POE_COLLECTOR },
  { shared: Items.SHARED_SOUL_NPC_BOMBCHU_BOWLING_LADY, oot: Items.OOT_SOUL_NPC_BOMBCHU_BOWLING_LADY, mm: Items.MM_SOUL_NPC_BOMBCHU_BOWLING_LADY },
  { shared: Items.SHARED_SOUL_NPC_FISHING_POND_OWNER, oot: Items.OOT_SOUL_NPC_FISHING_POND_OWNER, mm: Items.MM_SOUL_NPC_FISHING_POND_OWNER },
  { shared: Items.SHARED_SOUL_NPC_ROOFTOP_MAN, oot: Items.OOT_SOUL_NPC_ROOFTOP_MAN, mm: Items.MM_SOUL_NPC_ROOFTOP_MAN },
  { shared: Items.SHARED_SOUL_NPC_ZORA, oot: Items.OOT_SOUL_NPC_ZORA, mm: Items.MM_SOUL_NPC_ZORA },
  { shared: Items.SHARED_SOUL_NPC_ZORA_SHOPKEEPER, oot: Items.OOT_SOUL_NPC_ZORA_SHOPKEEPER, mm: Items.MM_SOUL_NPC_ZORA_SHOPKEEPER },
  { shared: Items.SHARED_SOUL_NPC_MALON, oot: Items.OOT_SOUL_NPC_MALON, mm: Items.MM_SOUL_NPC_MALON },
  { shared: Items.SHARED_SOUL_NPC_BEAN_SALESMAN, oot: Items.OOT_SOUL_NPC_BEAN_SALESMAN, mm: Items.MM_SOUL_NPC_BEAN_SALESMAN },
  { shared: Items.SHARED_SOUL_NPC_CARPENTERS, oot: Items.OOT_SOUL_NPC_CARPENTERS, mm: Items.MM_SOUL_NPC_CARPENTERS },
  { shared: Items.SHARED_SOUL_NPC_ANJU, oot: Items.OOT_SOUL_NPC_ANJU, mm: Items.MM_SOUL_NPC_ANJU },
  { shared: Items.SHARED_SOUL_NPC_GURU_GURU, oot: Items.OOT_SOUL_NPC_GURU_GURU, mm: Items.MM_SOUL_NPC_GURU_GURU },
  { shared: Items.SHARED_SOUL_NPC_SCIENTIST, oot: Items.OOT_SOUL_NPC_SCIENTIST, mm: Items.MM_SOUL_NPC_SCIENTIST },
  { shared: Items.SHARED_SOUL_NPC_GORMAN, oot: Items.OOT_SOUL_NPC_GORMAN, mm: Items.MM_SOUL_NPC_GORMAN },
  { shared: Items.SHARED_SOUL_NPC_GROG, oot: Items.OOT_SOUL_NPC_GROG, mm: Items.MM_SOUL_NPC_GROG },
  { shared: Items.SHARED_SOUL_NPC_DOG_LADY, oot: Items.OOT_SOUL_NPC_DOG_LADY, mm: Items.MM_SOUL_NPC_DOG_LADY },
  { shared: Items.SHARED_SOUL_NPC_CARPET_MAN, oot: Items.OOT_SOUL_NPC_CARPET_MAN, mm: Items.MM_SOUL_NPC_CARPET_MAN },
  { shared: Items.SHARED_SOUL_NPC_OLD_HAG, oot: Items.OOT_SOUL_NPC_OLD_HAG, mm: Items.MM_SOUL_NPC_OLD_HAG },
  { shared: Items.SHARED_SOUL_NPC_BANKER, oot: Items.OOT_SOUL_NPC_BANKER, mm: Items.MM_SOUL_NPC_BANKER },
];

const SOULS_MISC: ItemSharedDef[] = [
  { shared: Items.SHARED_SOUL_MISC_GS,              oot: Items.OOT_SOUL_MISC_GS,              mm: Items.MM_SOUL_MISC_GS },
  { shared: Items.SHARED_SOUL_MISC_BUSINESS_SCRUB,  oot: Items.OOT_SOUL_MISC_BUSINESS_SCRUB,  mm: Items.MM_SOUL_MISC_BUSINESS_SCRUB },
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

/* Junk shields handled separately */
const SHIELDS: ItemSharedDef[] = [
  { shared: Items.SHARED_SHIELD, oot: Items.OOT_SHIELD, mm: Items.MM_SHIELD },
  { shared: Items.SHARED_SHIELD_MIRROR, oot: Items.OOT_SHIELD_MIRROR, mm: Items.MM_SHIELD_MIRROR },
];


export const SharedItemGroups = { SOULS_ENEMY, SOULS_NPC, SOULS_MISC, SKELETON_KEY, OCARINA_BUTTONS, SHIELDS };

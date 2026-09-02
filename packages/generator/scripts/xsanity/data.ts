import path from 'node:path';

export const ROOT_DIR = path.resolve(import.meta.dirname, '../../../..');
export const ROMS_DIR = path.join(ROOT_DIR, 'roms');

export const OOT_GENERIC_GROTTOS = [
  0x00, /* Hyrule Field Market */
  0x02, /* Hyrule Field Southwest */
  0x03, /* Hyrule Field Open */
  0x08, /* Kakariko */
  0x09, /* Zora River */
  0x0c, /* Kokiri Forest */
  0x14, /* Lost Woods */
  0x17, /* Death Mountain Trail */
  0x1a, /* Death Mountain Crater */
];

export const OOT_SCRUBS_X2_GROTTOS = [
  0x01, /* Sacred Forest Meadow */
  0x04, /* River */
  0x05, /* Valley */
  0x06, /* Colossus */
];

export const OOT_SCRUBS_X3_GROTTOS = [
  0x07, /* RANCH */
  0x0a, /* GORON_CITY */
  0x0b, /* DMC */
  0x0d, /* LAKE */
];

export const OOT_FAIRY_FOUNTAINS = [
  0x00,
  0x01,
  0x02,
  0x03,
  0x04,
];

export const MM_GENERIC_GROTTOS = [
  0x13, /* Path to Snowhead Grotto,  */
  0x14, /* Ikana Valley Grotto */
  0x15, /* Zora Cape Grotto */
  0x16, /* Road to Ikana Grotto */
  0x17, /* Great Bay Coast Fisherman Grotto, */
  0x18, /* Ikana Graveyard Grotto */
  0x19, /* Twin Islands Ramp Grotto */
  0x1a, /* Termina Field Pillar Grotto */
  0x1b, /* Mountain Village Tunnel Grotto */
  0x1c, /* Woods of Mystery Grotto */
  0x1d, /* Southern Swamp Grotto */
  0x1e, /* Road to Southern Swamp Grotto */
  0x1f, /* Termina Field Tall Grass Grotto */
];


export const MM_SCENES_WITH_EXTRA_SETUPS: {[k: number]: number} = {
  0x2036000: 3, /* Ikana Canyon */
  0x2249000: 1, /* Road to Mountain Village */
  0x263b000: 1, /* Goron Shrine */
  0x265e000: 1, /* Zora Hall */
  0x26bf000: 1, /* Great bay coast */
  0x26fc000: 1, /* Zora cape */
  0x2778000: 1, /* Deku Throne room */
  0x2879000: 2, /* Woodfall */
  0x2bfe000: 1, /* Road to Snowhead */
  0x2c09000: 1, /* Snowhead */
  0x2da7000: 1, /* Goron Racetrack */
  0x2e1d000: 1, /* North Clock Town */
};

export const ACTORS_OOT = {
  EN_ITEM00: 0x15,
  EN_ELF: 0x18,
  OBJ_TSUBO: 0x111,
  EN_TUBO_TRAP: 0x11d,
  EN_KUSA: 0x125,
  OBJ_HANA: 0x14f,
  BG_SPOT11_OASIS: 0x1C2,
  OBJ_MURE: 0x094,
  OBJ_MURE2: 0x151,
  OBJ_MURE3: 0x1ab,
  EN_BUTTE: 0x01e,
  SHOT_SUN: 0x183,
  EN_WONDER_ITEM: 0x112,
  OBJ_KIBAKO: 0x110,
  OBJ_KIBAKO2: 0x1a0,
  OBJ_COMB: 0x19e,
  OBJ_HAMISHI: 0x1d2,
  BG_ICICLE: 0x1c7,
  BG_ICE_SHELTER: 0x0ef,
  EN_ISHI: 0x14e,
  EN_WOOD02: 0x077,
  OBJ_BEAN: 0x126,
  OBJ_BOMBIWA: 0x127,
  EN_GS: 0x1b9,
};

export const ACTORS_MM = {
  EN_GS: 0xef,
  EN_ITEM00: 0x0e,
  POT: 0x82,
  FLYING_POT: 0x8d,
  EN_KUSA: 0x90,
  EN_BUTTE: 0x15,
  OBJ_MURE: 0x4f,
  OBJ_MURE2: 0xb3,
  OBJ_MURE3: 0xe8,
  OBJ_GRASS: 0x10b,
  OBJ_GRASS_UNIT: 0x10d,
  EN_KUSA2: 0x171,
  EN_ELF: 0x10,
  EN_HIT_TAG: 0x265,
  EN_INVISIBLE_RUPPE: 0x2af,
  OBJ_KIBAKO: 0x81,
  OBJ_KIBAKO2: 0xe5,
  OBJ_COMB: 0x0e4,
  OBJ_FLOWERPOT: 0x13e,
  OBJ_TARU: 0x22d,
  OBJ_SNOWBALL: 0x1dc,
  OBJ_SNOWBALL2: 0x1f9,
  OBJ_HAMISHI: 0x0fc,
  BG_ICICLE: 0x11f,
  EN_ISHI: 0x0b0,
  EN_WOOD02: 0x041,
  OBJ_YASI: 0x23c,
  EN_SNOWWD: 0x1d4,
  OBJ_TREE: 0x229,
  OBJ_SWPRIZE: 0x2ae,
  OBJ_BOMBIWA: 0x092,
};

export const CONFIGS = {
  oot: {
    SCENE_TABLE_ADDR: 0xb71440,
    SCENE_TABLE_SIZE: 101,
  },
  mq: {
    SCENE_TABLE_ADDR: 0xba0bb0,
    SCENE_TABLE_SIZE: 101,
  },
  mm: {
    SCENE_TABLE_ADDR: 0x00C5A1E0,
    SCENE_TABLE_SIZE: 113,
  }
};

export const RUPEES = new Set([
  'RUPEE_GREEN',
  'RUPEE_BLUE',
  'RUPEE_RED',
  'RUPEE_PURPLE',
  'RUPEE_HUGE',
  'RUPEE_GOLD',
]);

export const OOT_WONDER_ITEM_DROPS = [
  'NUTS_5',
  '???',
  'MAGIC_JAR_LARGE',
  'MAGIC_JAR_SMALL',
  'RECOVERY_HEART',
  'ARROWS_5',
  'ARROWS_10',
  'ARROWS_30',
  'RUPEE_GREEN',
  'RUPEE_BLUE',
  'RUPEE_RED',
  'FLEXIBLE',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
  'RANDOM',
];

export const ITEM00_DROPS_OOT = [
  'RUPEE_GREEN',
  'RUPEE_BLUE',
  'RUPEE_RED',
  'RECOVERY_HEART',
  'BOMB',
  '???',
  '???',
  '???',
  'ARROWS_5',
  'ARROWS_10',
  'ARROWS_30',
  'BOMBS_5',
  'NUTS_5',
  'STICK',
  'MAGIC_JAR_LARGE',
  'MAGIC_JAR_SMALL',
  'DEKU_SEEDS_5',
  'SMALL_KEY',
  'FLEXIBLE',
  'RUPEE_HUGE',
  'RUPEE_PURPLE',
  'SHIELD_DEKU',
  'SHIELD_HYLIAN',
  'TUNIC_ZORA',
  'TUNIC_GORON',
  'BOMBS_5',
];

export const ITEM00_DROPS_MM = [
  'RUPEE_GREEN',
  'RUPEE_BLUE',
  'RUPEE_RED',
  'RECOVERY_HEART',
  'BOMBS_5',
  'ARROWS_10',
  'HEART_PIECE',
  'HEART_CONTAINER',
  'ARROWS_20',
  'ARROWS_30',
  'ARROWS_30',
  'BOMBS_5',
  'NUT',
  'STICK',
  'MAGIC_JAR_LARGE',
  'MAGIC_JAR_SMALL',
  '???',
  'SMALL_KEY',
  'FLEXIBLE',
  'RUPEE_HUGE',
  'RUPEE_PURPLE',
  'RECOVERY_HEART',
  '???',
  'NUTS_10',
  '???',
  '???',
  'STRAY_FAIRY',
  '???',
  '???',
  '???',
  '???',
  '???',
];

export const MM_ITEM00_DROP_COLLECTIBLE_TABLE = [
  -1, 0, 1, -1,
  0x02, 0x14, -1, 0x13,
  0x1c, 0x1d, 0x3, 0x15,
  0x6, 0x7, 0xf, 0xe,
  0x12, 0x1a, -1, 0x17,
  -1, 0x4, -1, -1,
  -1, 0xd, -1, -1,
  -1, -1, 5, 8,
];

/* Unused? */
const MM_POTS_SET_DROPS = [
  'NOTHING',
  'RUPEE_GREEN',
  'RUPEE_BLUE',
  'NOTHING',
  'RUPEE_RED',
  'RUPEE_PURPLE',
  'NOTHING',
  'RUPEE_ORANGE',
  '???',
  '???',
  'RECOVERY_HEART',
  'RECOVERY_HEART',
  '???',
  '???',
  'MAGIC_JAR_SMALL',
  'MAGIC_JAR_LARGE',
  'FAIRY',
  'STRAY_FAIRY',
  'NOTHING',
  'NUTS_10',
  'NOTHING',
  'BOMBS_5',
  'NOTHING',
  'NOTHING',
  'NOTHING',
  'STICK',
  'NOTHING',
  'NOTHING',
  'NOTHING',
  'NOTHING',
  'ARROWS_10',
  'ARROWS_20',
];

const MM_POTS_RANDOM_DROPS = [
  'RUPEE_GREEN',
  'MAGIC_JAR_SMALL',
  'RECOVERY_HEART',
  'RUPEE_RED',
  'NOTHING',
  'NOTHING',
  'RECOVERY_HEART',
  'RECOVERY_HEART',
  'RUPEE_GREEN',
  'RUPEE_BLUE',
  'ARROWS_10',
  'BOMBS_5',
  'MAGIC_JAR_SMALL',
  'MAGIC_JAR_LARGE',
  'NUT',
  'STICK',
];

export const FLYING_POT_DROPS = [
  'RECOVERY_HEART',
  'BOMBS_5',
  'DEKU_SEEDS_5',
  'NUTS_5',
  'DEKU_SEEDS_5',
  'RUPEE_PURPLE',
  'TUNIC_GORON',
];

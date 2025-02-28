const ITEM_IDS = [
  'OOT_TRAP_ICE',
  'OOT_BOTTLE_BLUE_FIRE',
  'OOT_DEFENSE_UPGRADE',
  'MM_DEFENSE_UPGRADE',
  'SHARED_DEFENSE_UPGRADE',
  'OOT_STICK_UPGRADE',
  'OOT_NUT_UPGRADE',
  'MM_STICK_UPGRADE',
  'MM_NUT_UPGRADE',
  'SHARED_STICK_UPGRADE',
  'SHARED_NUT_UPGRADE',
  'MM_BOMBER_NOTEBOOK',
  'OOT_SPELL_WIND',
  'OOT_SPELL_LOVE',
  'OOT_HEART_PIECE',
  'OOT_HEART_CONTAINER',
  'MM_HEART_PIECE',
  'MM_HEART_CONTAINER',
  'SHARED_HEART_PIECE',
  'SHARED_HEART_CONTAINER',
  'MM_ARROW_FIRE',
  'MM_ARROW_ICE',
  'MM_ARROW_LIGHT',
  'MM_ARROWS_10',
  'MM_ARROWS_30',
  'MM_ARROWS_40',
  'MM_BOMB_BAG',
  'MM_BOMB',
  'MM_BOMBCHU',
  'MM_BOMBCHU_10',
  'MM_BOMBCHU_20',
  'MM_BOMBCHU_5',
  'MM_BOMBCHU_BAG',
  'MM_BOMBS_5',
  'MM_BOMBS_10',
  'MM_BOMBS_20',
  'MM_BOMBS_30',
  'MM_BOSS_KEY_GB',
  'MM_BOSS_KEY_SH',
  'MM_BOSS_KEY_ST',
  'MM_BOSS_KEY_WF',
  'MM_BOTTLE_CHATEAU',
  'MM_BOTTLE_EMPTY',
  'MM_BOTTLE_MILK',
  'MM_BOTTLE_POTION_RED',
  'MM_BOTTLE_POTION_GREEN',
  'MM_BOTTLE_POTION_BLUE',
  'MM_BOTTLED_GOLD_DUST',
  'MM_BOW',
  'MM_BUGS',
  'MM_CHATEAU',
  'MM_COMPASS_GB',
  'MM_COMPASS_SH',
  'MM_COMPASS_ST',
  'MM_COMPASS_WF',
  'MM_DEED_LAND',
  'MM_DEED_MOUNTAIN',
  'MM_DEED_OCEAN',
  'MM_DEED_SWAMP',
  'MM_FAIRY',
  'MM_FISH',
  'MM_GREAT_FAIRY_SWORD',
  'MM_GS_TOKEN_OCEAN',
  'MM_GS_TOKEN_SWAMP',
  'MM_HOOKSHOT',
  'MM_LENS',
  'MM_LETTER_TO_KAFEI',
  'MM_LETTER_TO_MAMA',
  'MM_MAGIC_BEAN',
  'MM_MAGIC_UPGRADE',
  'MM_MAP_GB',
  'MM_MAP_SH',
  'MM_MAP_ST',
  'MM_MAP_WF',
  'MM_MASK_ALL_NIGHT',
  'MM_MASK_BLAST',
  'MM_MASK_BREMEN',
  'MM_MASK_BUNNY',
  'MM_MASK_CAPTAIN',
  'MM_MASK_COUPLE',
  'MM_MASK_DEKU',
  'MM_MASK_DON_GERO',
  'MM_MASK_FIERCE_DEITY',
  'MM_MASK_GARO',
  'MM_MASK_GIANT',
  'MM_MASK_GIBDO',
  'MM_MASK_GORON',
  'MM_MASK_GREAT_FAIRY',
  'MM_MASK_KAFEI',
  'MM_MASK_KAMARO',
  'MM_MASK_KEATON',
  'MM_MASK_POSTMAN',
  'MM_MASK_ROMANI',
  'MM_MASK_SCENTS',
  'MM_MASK_STONE',
  'MM_MASK_TROUPE_LEADER',
  'MM_MASK_TRUTH',
  'MM_MASK_ZORA',
  'MM_MILK',
  'MM_MOON_TEAR',
  'MM_NUT',
  'MM_NUTS_5',
  'MM_NUTS_10',
  'MM_OCARINA',
  'MM_OWL_CLOCK_TOWN',
  'MM_OWL_GREAT_BAY',
  'MM_OWL_IKANA_CANYON',
  'MM_OWL_MILK_ROAD',
  'MM_OWL_MOUNTAIN_VILLAGE',
  'MM_OWL_SNOWHEAD',
  'MM_OWL_SOUTHERN_SWAMP',
  'MM_OWL_STONE_TOWER',
  'MM_OWL_WOODFALL',
  'MM_OWL_ZORA_CAPE',
  'MM_PENDANT_OF_MEMORIES',
  'MM_PICTOGRAPH_BOX',
  'MM_POTION_BLUE',
  'MM_POTION_GREEN',
  'MM_POTION_RED',
  'MM_POWDER_KEG',
  'MM_RECOVERY_HEART',
  'MM_REMAINS_GOHT',
  'MM_REMAINS_GYORG',
  'MM_REMAINS_ODOLWA',
  'MM_REMAINS_TWINMOLD',
  'MM_ROOM_KEY',
  'MM_RUPEE_BLUE',
  'MM_RUPEE_GOLD',
  'MM_RUPEE_GREEN',
  'MM_RUPEE_PURPLE',
  'MM_RUPEE_RED',
  'MM_RUPEE_SILVER',
  'MM_SHIELD_DEKU',
  'MM_SHIELD_HERO',
  'MM_SHIELD_MIRROR',
  'MM_SHIELD',
  'MM_SMALL_KEY_GB',
  'MM_SMALL_KEY_SH',
  'MM_SMALL_KEY_ST',
  'MM_SMALL_KEY_WF',
  'MM_SONG_AWAKENING',
  'MM_SONG_AWAKENING_PROGRESSIVE_NOTE',
  'MM_SONG_AWAKENING_C_UP_1',
  'MM_SONG_AWAKENING_C_LEFT_1',
  'MM_SONG_AWAKENING_C_UP_2',
  'MM_SONG_AWAKENING_C_LEFT_2',
  'MM_SONG_AWAKENING_A_1',
  'MM_SONG_AWAKENING_C_RIGHT',
  'MM_SONG_AWAKENING_A_2',
  'MM_SONG_EMPTINESS',
  'MM_SONG_EMPTINESS_PROGRESSIVE_NOTE',
  'MM_SONG_EMPTINESS_C_RIGHT_1',
  'MM_SONG_EMPTINESS_C_LEFT_1',
  'MM_SONG_EMPTINESS_C_RIGHT_2',
  'MM_SONG_EMPTINESS_C_DOWN',
  'MM_SONG_EMPTINESS_C_RIGHT_3',
  'MM_SONG_EMPTINESS_C_UP',
  'MM_SONG_EMPTINESS_C_LEFT_2',
  'MM_SONG_EPONA',
  'MM_SONG_EPONA_PROGRESSIVE_NOTE',
  'MM_SONG_EPONA_C_UP_1',
  'MM_SONG_EPONA_C_LEFT_1',
  'MM_SONG_EPONA_C_RIGHT_1',
  'MM_SONG_EPONA_C_UP_2',
  'MM_SONG_EPONA_C_LEFT_2',
  'MM_SONG_EPONA_C_RIGHT_2',
  'MM_SONG_GORON_HALF',
  'MM_SONG_GORON_HALF_PROGRESSIVE_NOTE',
  'MM_SONG_GORON_HALF_A_1',
  'MM_SONG_GORON_HALF_C_RIGHT_1',
  'MM_SONG_GORON_HALF_C_LEFT_1',
  'MM_SONG_GORON_HALF_A_2',
  'MM_SONG_GORON_HALF_C_RIGHT_2',
  'MM_SONG_GORON_HALF_C_LEFT_2',
  'MM_SONG_GORON',
  'MM_SONG_GORON_PROGRESSIVE_NOTE',
  'MM_SONG_GORON_A_1',
  'MM_SONG_GORON_C_RIGHT_1',
  'MM_SONG_GORON_C_LEFT_1',
  'MM_SONG_GORON_A_2',
  'MM_SONG_GORON_C_RIGHT_2',
  'MM_SONG_GORON_C_LEFT_2',
  'MM_SONG_GORON_C_RIGHT_3',
  'MM_SONG_GORON_A_3',
  'MM_SONG_HEALING',
  'MM_SONG_HEALING_PROGRESSIVE_NOTE',
  'MM_SONG_HEALING_C_LEFT_1',
  'MM_SONG_HEALING_C_RIGHT_1',
  'MM_SONG_HEALING_C_DOWN_1',
  'MM_SONG_HEALING_C_LEFT_2',
  'MM_SONG_HEALING_C_RIGHT_2',
  'MM_SONG_HEALING_C_DOWN_2',
  'MM_SONG_ORDER',
  'MM_SONG_ORDER_PROGRESSIVE_NOTE',
  'MM_SONG_ORDER_C_RIGHT_1',
  'MM_SONG_ORDER_C_DOWN_1',
  'MM_SONG_ORDER_A',
  'MM_SONG_ORDER_C_DOWN_2',
  'MM_SONG_ORDER_C_RIGHT_2',
  'MM_SONG_ORDER_C_UP',
  'MM_SONG_SOARING',
  'MM_SONG_SOARING_PROGRESSIVE_NOTE',
  'MM_SONG_SOARING_C_DOWN_1',
  'MM_SONG_SOARING_C_LEFT_1',
  'MM_SONG_SOARING_C_UP_1',
  'MM_SONG_SOARING_C_DOWN_2',
  'MM_SONG_SOARING_C_LEFT_2',
  'MM_SONG_SOARING_C_UP_2',
  'MM_SONG_STORMS',
  'MM_SONG_STORMS_PROGRESSIVE_NOTE',
  'MM_SONG_STORMS_A_1',
  'MM_SONG_STORMS_C_DOWN_1',
  'MM_SONG_STORMS_C_UP_1',
  'MM_SONG_STORMS_A_2',
  'MM_SONG_STORMS_C_DOWN_2',
  'MM_SONG_STORMS_C_UP_2',
  'MM_SONG_SUN',
  'MM_SONG_SUN_PROGRESSIVE_NOTE',
  'MM_SONG_SUN_C_RIGHT_1',
  'MM_SONG_SUN_C_DOWN_1',
  'MM_SONG_SUN_C_UP_1',
  'MM_SONG_SUN_C_RIGHT_2',
  'MM_SONG_SUN_C_DOWN_2',
  'MM_SONG_SUN_C_UP_2',
  'MM_SONG_TIME',
  'MM_SONG_TIME_PROGRESSIVE_NOTE',
  'MM_SONG_TIME_C_RIGHT_1',
  'MM_SONG_TIME_A_1',
  'MM_SONG_TIME_C_DOWN_1',
  'MM_SONG_TIME_C_RIGHT_2',
  'MM_SONG_TIME_A_2',
  'MM_SONG_TIME_C_DOWN_3',
  'MM_SONG_ZORA',
  'MM_SONG_ZORA_PROGRESSIVE_NOTE',
  'MM_SONG_ZORA_C_LEFT_1',
  'MM_SONG_ZORA_C_UP',
  'MM_SONG_ZORA_C_LEFT_2',
  'MM_SONG_ZORA_C_RIGHT_1',
  'MM_SONG_ZORA_C_DOWN',
  'MM_SONG_ZORA_C_LEFT_3',
  'MM_SONG_ZORA_C_RIGHT_2',
  'MM_SPIN_UPGRADE',
  'OOT_SPIN_UPGRADE',
  'SHARED_SPIN_UPGRADE',
  'MM_STICK',
  'MM_STRAY_FAIRY_GB',
  'MM_STRAY_FAIRY_SH',
  'MM_STRAY_FAIRY_ST',
  'MM_STRAY_FAIRY_TOWN',
  'MM_STRAY_FAIRY_WF',
  'MM_SWORD',
  'MM_WALLET',
  'MM_WORLD_MAP_CLOCK_TOWN',
  'MM_WORLD_MAP_GREAT_BAY',
  'MM_WORLD_MAP_ROMANI_RANCH',
  'MM_WORLD_MAP_SNOWHEAD',
  'MM_WORLD_MAP_STONE_TOWER',
  'MM_WORLD_MAP_WOODFALL',
  'OOT_ARROW_FIRE',
  'OOT_ARROW_ICE',
  'OOT_ARROW_LIGHT',
  'OOT_ARROWS_10',
  'OOT_ARROWS_30',
  'OOT_ARROWS_5',
  'OOT_BLUE_FIRE',
  'OOT_BOMB_BAG',
  'OOT_BOMB',
  'OOT_BOMBCHU_10',
  'OOT_BOMBCHU_20',
  'OOT_BOMBCHU_5',
  'OOT_BOMBCHU_BAG',
  'OOT_BOMBS_10',
  'OOT_BOMBS_20',
  'OOT_BOMBS_30',
  'OOT_BOMBS_5',
  'OOT_BOOMERANG',
  'OOT_BOOTS_HOVER',
  'OOT_BOOTS_IRON',
  'OOT_BOSS_KEY_FIRE',
  'OOT_BOSS_KEY_FOREST',
  'OOT_BOSS_KEY_GANON',
  'OOT_BOSS_KEY_SHADOW',
  'OOT_BOSS_KEY_SPIRIT',
  'OOT_BOSS_KEY_WATER',
  'OOT_BOTTLE_EMPTY',
  'OOT_BOTTLE_MILK',
  'OOT_BOW',
  'OOT_BROKEN_GORON_SWORD',
  'OOT_BUG',
  'OOT_BUG',
  'OOT_CHICKEN',
  'OOT_CLAIM_CHECK',
  'OOT_COJIRO',
  'OOT_COMPASS_BOTW',
  'OOT_COMPASS_DC',
  'OOT_COMPASS_DT',
  'OOT_COMPASS_FIRE',
  'OOT_COMPASS_FOREST',
  'OOT_COMPASS_IC',
  'OOT_COMPASS_JJ',
  'OOT_COMPASS_SHADOW',
  'OOT_COMPASS_SPIRIT',
  'OOT_COMPASS_WATER',
  'OOT_DEKU_SEEDS_30',
  'OOT_DEKU_SEEDS_5',
  'OOT_EYE_DROPS',
  'OOT_EYEBALL_FROG',
  'OOT_FAIRY',
  'OOT_FISH',
  'OOT_GERUDO_CARD',
  'OOT_GS_TOKEN',
  'OOT_GS_TOKEN',
  'OOT_HAMMER',
  'OOT_HOOKSHOT',
  'OOT_LENS',
  'OOT_MAGIC_BEAN',
  'OOT_MAGIC_UPGRADE',
  'OOT_MAP_BOTW',
  'OOT_MAP_DC',
  'OOT_MAP_DT',
  'OOT_MAP_FIRE',
  'OOT_MAP_FOREST',
  'OOT_MAP_IC',
  'OOT_MAP_JJ',
  'OOT_MAP_SHADOW',
  'OOT_MAP_SPIRIT',
  'OOT_MAP_WATER',
  'OOT_MASK_BUNNY',
  'OOT_MASK_GERUDO',
  'OOT_MASK_GORON',
  'OOT_MASK_KEATON',
  'OOT_MASK_SKULL',
  'OOT_MASK_SPOOKY',
  'OOT_MASK_TRUTH',
  'OOT_MASK_ZORA',
  'OOT_MEDALLION_FIRE',
  'OOT_MEDALLION_FOREST',
  'OOT_MEDALLION_LIGHT',
  'OOT_MEDALLION_SHADOW',
  'OOT_MEDALLION_SPIRIT',
  'OOT_MEDALLION_WATER',
  'OOT_MILK',
  'OOT_NUTS_5',
  'OOT_NUTS_5_ALT',
  'OOT_NUTS_10',
  'OOT_OCARINA',
  'OOT_ODD_MUSHROOM',
  'OOT_ODD_POTION',
  'OOT_POACHER_SAW',
  'OOT_POCKET_CUCCO',
  'OOT_POCKET_EGG',
  'OOT_POE',
  'OOT_BIG_POE',
  'OOT_POTION_BLUE',
  'OOT_POTION_GREEN',
  'OOT_POTION_RED',
  'OOT_PRESCRIPTION',
  'OOT_RECOVERY_HEART',
  'OOT_RUPEE_GREEN',
  'OOT_RUPEE_BLUE',
  'OOT_RUPEE_RED',
  'OOT_RUPEE_PURPLE',
  'OOT_RUPEE_HUGE',
  'OOT_RUPEE_RAINBOW',
  'OOT_RUTO_LETTER',
  'OOT_SCALE',
  'OOT_SHIELD_DEKU',
  'OOT_SHIELD_HYLIAN',
  'OOT_SHIELD_MIRROR',
  'OOT_SHIELD',
  'OOT_SLINGSHOT',
  'OOT_SMALL_KEY_BOTW',
  'OOT_SMALL_KEY_FIRE',
  'OOT_SMALL_KEY_FOREST',
  'OOT_SMALL_KEY_GANON',
  'OOT_SMALL_KEY_GF',
  'OOT_SMALL_KEY_GTG',
  'OOT_SMALL_KEY_SHADOW',
  'OOT_SMALL_KEY_SPIRIT',
  'OOT_SMALL_KEY_WATER',
  'OOT_SONG_EPONA',
  'OOT_SONG_EPONA_PROGRESSIVE_NOTE',
  'OOT_SONG_EPONA_C_UP_1',
  'OOT_SONG_EPONA_C_LEFT_1',
  'OOT_SONG_EPONA_C_RIGHT_1',
  'OOT_SONG_EPONA_C_UP_2',
  'OOT_SONG_EPONA_C_LEFT_2',
  'OOT_SONG_EPONA_C_RIGHT_2',
  'OOT_SONG_SARIA',
  'OOT_SONG_SARIA_PROGRESSIVE_NOTE',
  'OOT_SONG_SARIA_C_DOWN_1',
  'OOT_SONG_SARIA_C_RIGHT_1',
  'OOT_SONG_SARIA_C_LEFT_1',
  'OOT_SONG_SARIA_C_DOWN_2',
  'OOT_SONG_SARIA_C_RIGHT_2',
  'OOT_SONG_SARIA_C_LEFT_2',
  'OOT_SONG_STORMS',
  'OOT_SONG_STORMS_PROGRESSIVE_NOTE',
  'OOT_SONG_STORMS_A_1',
  'OOT_SONG_STORMS_C_DOWN_1',
  'OOT_SONG_STORMS_C_UP_1',
  'OOT_SONG_STORMS_A_2',
  'OOT_SONG_STORMS_C_DOWN_2',
  'OOT_SONG_STORMS_C_UP_2',
  'OOT_SONG_SUN',
  'OOT_SONG_SUN_PROGRESSIVE_NOTE',
  'OOT_SONG_SUN_C_RIGHT_1',
  'OOT_SONG_SUN_C_DOWN_1',
  'OOT_SONG_SUN_C_UP_1',
  'OOT_SONG_SUN_C_RIGHT_2',
  'OOT_SONG_SUN_C_DOWN_2',
  'OOT_SONG_SUN_C_UP_2',
  'OOT_SONG_TIME',
  'OOT_SONG_TIME_PROGRESSIVE_NOTE',
  'OOT_SONG_TIME_C_RIGHT_1',
  'OOT_SONG_TIME_A_1',
  'OOT_SONG_TIME_C_DOWN_1',
  'OOT_SONG_TIME_C_RIGHT_2',
  'OOT_SONG_TIME_A_2',
  'OOT_SONG_TIME_C_DOWN_3',
  'OOT_SONG_TP_FIRE',
  'OOT_SONG_TP_FIRE_PROGRESSIVE_NOTE',
  'OOT_SONG_TP_FIRE_C_DOWN_1',
  'OOT_SONG_TP_FIRE_A_1',
  'OOT_SONG_TP_FIRE_C_DOWN_2',
  'OOT_SONG_TP_FIRE_A_2',
  'OOT_SONG_TP_FIRE_C_RIGHT_1',
  'OOT_SONG_TP_FIRE_C_DOWN_3',
  'OOT_SONG_TP_FIRE_C_RIGHT_2',
  'OOT_SONG_TP_FIRE_C_DOWN_4',
  'OOT_SONG_TP_FOREST',
  'OOT_SONG_TP_FOREST_PROGRESSIVE_NOTE',
  'OOT_SONG_TP_FOREST_A',
  'OOT_SONG_TP_FOREST_C_UP',
  'OOT_SONG_TP_FOREST_C_LEFT_1',
  'OOT_SONG_TP_FOREST_C_RIGHT_1',
  'OOT_SONG_TP_FOREST_C_LEFT_2',
  'OOT_SONG_TP_FOREST_C_RIGHT_2',
  'OOT_SONG_TP_LIGHT',
  'OOT_SONG_TP_LIGHT_PROGRESSIVE_NOTE',
  'OOT_SONG_TP_LIGHT_C_UP_1',
  'OOT_SONG_TP_LIGHT_C_RIGHT_1',
  'OOT_SONG_TP_LIGHT_C_UP_2',
  'OOT_SONG_TP_LIGHT_C_RIGHT_2',
  'OOT_SONG_TP_LIGHT_C_LEFT',
  'OOT_SONG_TP_LIGHT_C_UP_3',
  'OOT_SONG_TP_SHADOW',
  'OOT_SONG_TP_SHADOW_PROGRESSIVE_NOTE',
  'OOT_SONG_TP_SHADOW_C_LEFT_1',
  'OOT_SONG_TP_SHADOW_C_RIGHT_1',
  'OOT_SONG_TP_SHADOW_C_RIGHT_2',
  'OOT_SONG_TP_SHADOW_A',
  'OOT_SONG_TP_SHADOW_C_LEFT_2',
  'OOT_SONG_TP_SHADOW_C_RIGHT_3',
  'OOT_SONG_TP_SHADOW_C_DOWN',
  'OOT_SONG_TP_SPIRIT',
  'OOT_SONG_TP_SPIRIT_PROGRESSIVE_NOTE',
  'OOT_SONG_TP_SPIRIT_A_1',
  'OOT_SONG_TP_SPIRIT_C_DOWN_1',
  'OOT_SONG_TP_SPIRIT_A_2',
  'OOT_SONG_TP_SPIRIT_C_RIGHT',
  'OOT_SONG_TP_SPIRIT_C_DOWN_2',
  'OOT_SONG_TP_SPIRIT_A_3',
  'OOT_SONG_TP_WATER',
  'OOT_SONG_TP_WATER_PROGRESSIVE_NOTE',
  'OOT_SONG_TP_WATER_A',
  'OOT_SONG_TP_WATER_C_DOWN',
  'OOT_SONG_TP_WATER_C_RIGHT_1',
  'OOT_SONG_TP_WATER_C_RIGHT_2',
  'OOT_SONG_TP_WATER_C_LEFT',
  'OOT_SONG_ZELDA',
  'OOT_SONG_ZELDA_PROGRESSIVE_NOTE',
  'OOT_SONG_ZELDA_C_LEFT_1',
  'OOT_SONG_ZELDA_C_UP_1',
  'OOT_SONG_ZELDA_C_RIGHT_1',
  'OOT_SONG_ZELDA_C_LEFT_2',
  'OOT_SONG_ZELDA_C_UP_2',
  'OOT_SONG_ZELDA_C_RIGHT_2',
  'OOT_SPELL_FIRE',
  'OOT_STICK',
  'OOT_STICKS_10',
  'OOT_STICKS_5',
  'OOT_STONE_EMERALD',
  'OOT_STONE_OF_AGONY',
  'OOT_STONE_RUBY',
  'OOT_STONE_SAPPHIRE',
  'OOT_STRENGTH',
  'OOT_SWORD_BIGGORON',
  'OOT_SWORD_GORON',
  'OOT_SWORD_KNIFE',
  'OOT_SWORD_KOKIRI',
  'OOT_SWORD_MASTER',
  'OOT_SWORD',
  'OOT_TUNIC_GORON',
  'OOT_TUNIC_ZORA',
  'OOT_WALLET',
  'OOT_WEIRD_EGG',
  'OOT_ZELDA_LETTER',
  'SHARED_SWORD',
  'SHARED_ARROW_FIRE',
  'SHARED_ARROW_ICE',
  'SHARED_ARROW_LIGHT',
  'SHARED_ARROWS_10',
  'SHARED_ARROWS_30',
  'SHARED_ARROWS_40',
  'SHARED_ARROWS_5',
  'SHARED_BOMB_BAG',
  'SHARED_BOMB',
  'SHARED_BOMBCHU',
  'SHARED_BOMBCHU_5',
  'SHARED_BOMBCHU_10',
  'SHARED_BOMBCHU_20',
  'SHARED_BOMBCHU_BAG',
  'SHARED_BOMBS_10',
  'SHARED_BOMBS_20',
  'SHARED_BOMBS_30',
  'SHARED_BOMBS_5',
  'SHARED_BOW',
  'SHARED_HOOKSHOT',
  'SHARED_LENS',
  'SHARED_MAGIC_UPGRADE',
  'SHARED_MASK_BUNNY',
  'SHARED_MASK_GORON',
  'SHARED_MASK_KEATON',
  'SHARED_MASK_TRUTH',
  'SHARED_MASK_ZORA',
  'SHARED_NUT',
  'SHARED_NUTS_10',
  'SHARED_NUTS_5',
  'SHARED_OCARINA',
  'SHARED_RECOVERY_HEART',
  'SHARED_RUPEE_GREEN',
  'SHARED_RUPEE_BLUE',
  'SHARED_RUPEE_RED',
  'SHARED_RUPEE_PURPLE',
  'SHARED_RUPEE_SILVER',
  'SHARED_RUPEE_GOLD',
  'SHARED_RUPEE_RAINBOW',
  'SHARED_SONG_EPONA',
  'SHARED_SONG_EPONA_PROGRESSIVE_NOTE',
  'SHARED_SONG_EPONA_C_UP_1',
  'SHARED_SONG_EPONA_C_LEFT_1',
  'SHARED_SONG_EPONA_C_RIGHT_1',
  'SHARED_SONG_EPONA_C_UP_2',
  'SHARED_SONG_EPONA_C_LEFT_2',
  'SHARED_SONG_EPONA_C_RIGHT_2',
  'SHARED_SONG_STORMS',
  'SHARED_SONG_STORMS_PROGRESSIVE_NOTE',
  'SHARED_SONG_STORMS_A_1',
  'SHARED_SONG_STORMS_C_DOWN_1',
  'SHARED_SONG_STORMS_C_UP_1',
  'SHARED_SONG_STORMS_A_2',
  'SHARED_SONG_STORMS_C_DOWN_2',
  'SHARED_SONG_STORMS_C_UP_2',
  'SHARED_SONG_SUN',
  'SHARED_SONG_SUN_PROGRESSIVE_NOTE',
  'SHARED_SONG_SUN_C_RIGHT_1',
  'SHARED_SONG_SUN_C_DOWN_1',
  'SHARED_SONG_SUN_C_UP_1',
  'SHARED_SONG_SUN_C_RIGHT_2',
  'SHARED_SONG_SUN_C_DOWN_2',
  'SHARED_SONG_SUN_C_UP_2',
  'SHARED_SONG_TIME',
  'SHARED_SONG_TIME_PROGRESSIVE_NOTE',
  'SHARED_SONG_TIME_C_RIGHT_1',
  'SHARED_SONG_TIME_A_1',
  'SHARED_SONG_TIME_C_DOWN_1',
  'SHARED_SONG_TIME_C_RIGHT_2',
  'SHARED_SONG_TIME_A_2',
  'SHARED_SONG_TIME_C_DOWN_3',
  'SHARED_STICK',
  'SHARED_STICKS_10',
  'SHARED_STICKS_5',
  'SHARED_WALLET',
  'SHARED_TRIFORCE',
  'SHARED_TRIFORCE_POWER',
  'SHARED_TRIFORCE_COURAGE',
  'SHARED_TRIFORCE_WISDOM',
  'SHARED_SHIELD',
  'SHARED_SHIELD_DEKU',
  'SHARED_SHIELD_HYLIAN',
  'SHARED_SHIELD_MIRROR',
  'SHARED_SPELL_FIRE',
  'SHARED_SPELL_LOVE',
  'SHARED_SPELL_WIND',
  'SHARED_BOOTS_IRON',
  'SHARED_BOOTS_HOVER',
  'SHARED_TUNIC_GORON',
  'SHARED_TUNIC_ZORA',
  'SHARED_SCALE',
  'SHARED_STRENGTH',
  'OOT_RUPEE_SILVER_DC',
  'OOT_RUPEE_SILVER_BOTW',
  'OOT_RUPEE_SILVER_SPIRIT_CHILD',
  'OOT_RUPEE_SILVER_SPIRIT_SUN',
  'OOT_RUPEE_SILVER_SPIRIT_BOULDERS',
  'OOT_RUPEE_SILVER_SPIRIT_LOBBY',
  'OOT_RUPEE_SILVER_SPIRIT_ADULT',
  'OOT_RUPEE_SILVER_SHADOW_SCYTHE',
  'OOT_RUPEE_SILVER_SHADOW_PIT',
  'OOT_RUPEE_SILVER_SHADOW_SPIKES',
  'OOT_RUPEE_SILVER_SHADOW_BLADES',
  'OOT_RUPEE_SILVER_IC_SCYTHE',
  'OOT_RUPEE_SILVER_IC_BLOCK',
  'OOT_RUPEE_SILVER_GTG_SLOPES',
  'OOT_RUPEE_SILVER_GTG_LAVA',
  'OOT_RUPEE_SILVER_GTG_WATER',
  'OOT_RUPEE_SILVER_GANON_SPIRIT',
  'OOT_RUPEE_SILVER_GANON_LIGHT',
  'OOT_RUPEE_SILVER_GANON_FIRE',
  'OOT_RUPEE_SILVER_GANON_FOREST',
  'OOT_RUPEE_SILVER_GANON_SHADOW',
  'OOT_RUPEE_SILVER_GANON_WATER',
  'OOT_POUCH_SILVER_DC',
  'OOT_POUCH_SILVER_BOTW',
  'OOT_POUCH_SILVER_SPIRIT_CHILD',
  'OOT_POUCH_SILVER_SPIRIT_SUN',
  'OOT_POUCH_SILVER_SPIRIT_BOULDERS',
  'OOT_POUCH_SILVER_SPIRIT_LOBBY',
  'OOT_POUCH_SILVER_SPIRIT_ADULT',
  'OOT_POUCH_SILVER_SHADOW_SCYTHE',
  'OOT_POUCH_SILVER_SHADOW_PIT',
  'OOT_POUCH_SILVER_SHADOW_SPIKES',
  'OOT_POUCH_SILVER_SHADOW_BLADES',
  'OOT_POUCH_SILVER_IC_SCYTHE',
  'OOT_POUCH_SILVER_IC_BLOCK',
  'OOT_POUCH_SILVER_GTG_SLOPES',
  'OOT_POUCH_SILVER_GTG_LAVA',
  'OOT_POUCH_SILVER_GTG_WATER',
  'OOT_POUCH_SILVER_GANON_SPIRIT',
  'OOT_POUCH_SILVER_GANON_LIGHT',
  'OOT_POUCH_SILVER_GANON_FIRE',
  'OOT_POUCH_SILVER_GANON_FOREST',
  'OOT_POUCH_SILVER_GANON_SHADOW',
  'OOT_POUCH_SILVER_GANON_WATER',
  'OOT_SOUL_ENEMY_STALFOS',
  'OOT_SOUL_ENEMY_OCTOROK',
  'OOT_SOUL_ENEMY_WALLMASTER',
  'OOT_SOUL_ENEMY_DODONGO',
  'OOT_SOUL_ENEMY_KEESE',
  'OOT_SOUL_ENEMY_TEKTITE',
  'OOT_SOUL_ENEMY_LEEVER',
  'OOT_SOUL_ENEMY_PEAHAT',
  'OOT_SOUL_ENEMY_LIZALFOS_DINALFOS',
  'OOT_SOUL_ENEMY_GOHMA_LARVA',
  'OOT_SOUL_ENEMY_SHABOM',
  'OOT_SOUL_ENEMY_BABY_DODONGO',
  'OOT_SOUL_ENEMY_DARK_LINK',
  'OOT_SOUL_ENEMY_BIRI_BARI',
  'OOT_SOUL_ENEMY_TAILPASARN',
  'OOT_SOUL_ENEMY_PARASITE',
  'OOT_SOUL_ENEMY_SKULLTULA',
  'OOT_SOUL_ENEMY_TORCH_SLUG',
  'OOT_SOUL_ENEMY_MOBLIN',
  'OOT_SOUL_ENEMY_ARMOS',
  'OOT_SOUL_ENEMY_DEKU_BABA',
  'OOT_SOUL_ENEMY_DEKU_SCRUB',
  'OOT_SOUL_ENEMY_BUBBLE',
  'OOT_SOUL_ENEMY_BEAMOS',
  'OOT_SOUL_ENEMY_FLOORMASTER',
  'OOT_SOUL_ENEMY_REDEAD_GIBDO',
  'OOT_SOUL_ENEMY_SKULLWALLTULA',
  'OOT_SOUL_ENEMY_FLARE_DANCER',
  'OOT_SOUL_ENEMY_DEAD_HAND',
  'OOT_SOUL_ENEMY_SHELL_BLADE',
  'OOT_SOUL_ENEMY_LIKE_LIKE',
  'OOT_SOUL_ENEMY_SPIKE',
  'OOT_SOUL_ENEMY_ANUBIS',
  'OOT_SOUL_ENEMY_IRON_KNUCKLE',
  'OOT_SOUL_ENEMY_SKULL_KID',
  'OOT_SOUL_ENEMY_FLYING_POT',
  'OOT_SOUL_ENEMY_FREEZARD',
  'OOT_SOUL_ENEMY_STINGER',
  'OOT_SOUL_ENEMY_WOLFOS',
  'OOT_SOUL_ENEMY_STALCHILD',
  'OOT_SOUL_ENEMY_GUAY',
  'OOT_SOUL_BOSS_QUEEN_GOHMA',
  'OOT_SOUL_BOSS_KING_DODONGO',
  'OOT_SOUL_BOSS_BARINADE',
  'OOT_SOUL_BOSS_PHANTOM_GANON',
  'OOT_SOUL_BOSS_VOLVAGIA',
  'OOT_SOUL_BOSS_MORPHA',
  'OOT_SOUL_BOSS_BONGO_BONGO',
  'OOT_SOUL_BOSS_TWINROVA',
  'MM_SOUL_ENEMY_OCTOROK',
  'MM_SOUL_ENEMY_WALLMASTER',
  'MM_SOUL_ENEMY_DODONGO',
  'MM_SOUL_ENEMY_KEESE',
  'MM_SOUL_ENEMY_TEKTITE',
  'MM_SOUL_ENEMY_PEAHAT',
  'MM_SOUL_ENEMY_LIZALFOS_DINALFOS',
  'MM_SOUL_ENEMY_SKULLTULA',
  'MM_SOUL_ENEMY_ARMOS',
  'MM_SOUL_ENEMY_DEKU_BABA',
  'MM_SOUL_ENEMY_DEKU_SCRUB',
  'MM_SOUL_ENEMY_BUBBLE',
  'MM_SOUL_ENEMY_BEAMOS',
  'MM_SOUL_ENEMY_REDEAD_GIBDO',
  'MM_SOUL_ENEMY_SKULLWALLTULA',
  'MM_SOUL_ENEMY_SHELL_BLADE',
  'MM_SOUL_ENEMY_LIKE_LIKE',
  'MM_SOUL_ENEMY_IRON_KNUCKLE',
  'MM_SOUL_ENEMY_FREEZARD',
  'MM_SOUL_ENEMY_WOLFOS',
  'MM_SOUL_ENEMY_GUAY',
  'MM_SOUL_ENEMY_FLYING_POT',
  'MM_SOUL_ENEMY_FLOORMASTER',
  'MM_SOUL_ENEMY_CHUCHU',
  'MM_SOUL_ENEMY_DEEP_PYTHON',
  'MM_SOUL_ENEMY_STALCHILD',
  'MM_SOUL_ENEMY_LEEVER',
  'MM_SOUL_ENEMY_SKULLFISH',
  'MM_SOUL_ENEMY_DEXIHAND',
  'MM_SOUL_ENEMY_DRAGONFLY',
  'MM_SOUL_ENEMY_EENO',
  'MM_SOUL_ENEMY_EYEGORE',
  'MM_SOUL_ENEMY_HIPLOOP',
  'MM_SOUL_ENEMY_REAL_BOMBCHU',
  'MM_SOUL_ENEMY_TAKKURI',
  'MM_SOUL_ENEMY_BOE',
  'MM_SOUL_ENEMY_NEJIRON',
  'MM_SOUL_ENEMY_BIO_BABA',
  'MM_SOUL_ENEMY_GARO',
  'MM_SOUL_ENEMY_WIZZROBE',
  'MM_SOUL_ENEMY_GOMESS',
  'MM_SOUL_ENEMY_GEKKO',
  'MM_SOUL_ENEMY_BAD_BAT',
  'MM_SOUL_ENEMY_SNAPPER',
  'MM_SOUL_ENEMY_WART',
  'MM_SOUL_ENEMY_CAPTAIN_KEETA',
  'MM_SOUL_BOSS_ODOLWA',
  'MM_SOUL_BOSS_GOHT',
  'MM_SOUL_BOSS_GYORG',
  'MM_SOUL_BOSS_TWINMOLD',
  'MM_SOUL_BOSS_IGOS',
  'SHARED_SOUL_ENEMY_OCTOROK',
  'SHARED_SOUL_ENEMY_WALLMASTER',
  'SHARED_SOUL_ENEMY_DODONGO',
  'SHARED_SOUL_ENEMY_KEESE',
  'SHARED_SOUL_ENEMY_TEKTITE',
  'SHARED_SOUL_ENEMY_PEAHAT',
  'SHARED_SOUL_ENEMY_LIZALFOS_DINALFOS',
  'SHARED_SOUL_ENEMY_SKULLTULA',
  'SHARED_SOUL_ENEMY_ARMOS',
  'SHARED_SOUL_ENEMY_DEKU_BABA',
  'SHARED_SOUL_ENEMY_DEKU_SCRUB',
  'SHARED_SOUL_ENEMY_BUBBLE',
  'SHARED_SOUL_ENEMY_BEAMOS',
  'SHARED_SOUL_ENEMY_REDEAD_GIBDO',
  'SHARED_SOUL_ENEMY_SKULLWALLTULA',
  'SHARED_SOUL_ENEMY_SHELL_BLADE',
  'SHARED_SOUL_ENEMY_LIKE_LIKE',
  'SHARED_SOUL_ENEMY_IRON_KNUCKLE',
  'SHARED_SOUL_ENEMY_FREEZARD',
  'SHARED_SOUL_ENEMY_WOLFOS',
  'SHARED_SOUL_ENEMY_GUAY',
  'SHARED_SOUL_ENEMY_FLYING_POT',
  'SHARED_SOUL_ENEMY_FLOORMASTER',
  'SHARED_SOUL_ENEMY_LEEVER',
  'SHARED_SOUL_ENEMY_STALCHILD',
  'MM_SOUL_NPC_AROMA',
  'MM_SOUL_NPC_SHOOTING_GALLERY_OWNER',
  'MM_SOUL_NPC_BAZAAR_SHOPKEEPER',
  'MM_SOUL_NPC_GORON',
  'MM_SOUL_NPC_GORON_CHILD',
  'MM_SOUL_NPC_BOMBCHU_SHOPKEEPER',
  'MM_SOUL_NPC_BOMBERS',
  'MM_SOUL_NPC_BUTLER_DEKU',
  'MM_SOUL_NPC_CITIZEN',
  'MM_SOUL_NPC_COMPOSER_BROS',
  'MM_SOUL_NPC_DAMPE',
  'MM_SOUL_NPC_DEKU_KING',
  'MM_SOUL_NPC_DEKU_PRINCESS',
  'MM_SOUL_NPC_PLAYGROUND_SCRUBS',
  'MM_SOUL_NPC_GORON_ELDER',
  'MM_SOUL_NPC_CHEST_GAME_OWNER',
  'MM_SOUL_NPC_GORON_SHOPKEEPER',
  'MM_SOUL_NPC_HONEY_DARLING',
  'MM_SOUL_NPC_TOTO',
  'MM_SOUL_NPC_RUTO',
  'MM_SOUL_NPC_ZORA_MUSICIANS',
  'MM_SOUL_NPC_KAFEI',
  'MM_SOUL_NPC_KEATON',
  'MM_SOUL_NPC_KOUME_KOTAKE',
  'MM_SOUL_NPC_MAYOR_DOTOUR',
  'MM_SOUL_NPC_MEDIGORON',
  'MM_SOUL_NPC_BIGGORON',
  'MM_SOUL_NPC_TALON',
  'MM_SOUL_NPC_ASTRONOMER',
  'MM_SOUL_NPC_POE_COLLECTOR',
  'MM_SOUL_NPC_BLACKSMITHS',
  'MM_SOUL_NPC_BOMBCHU_BOWLING_LADY',
  'MM_SOUL_NPC_TINGLE',
  'MM_SOUL_NPC_TOILET_HAND',
  'MM_SOUL_NPC_FISHING_POND_OWNER',
  'MM_SOUL_NPC_ROOFTOP_MAN',
  'MM_SOUL_NPC_ZORA',
  'MM_SOUL_NPC_ZORA_SHOPKEEPER',
  'MM_SOUL_NPC_MALON',
  'MM_SOUL_NPC_BEAN_SALESMAN',
  'MM_SOUL_NPC_CARPENTERS',
  'MM_SOUL_NPC_ANJU',
  'MM_SOUL_NPC_GURU_GURU',
  'MM_SOUL_NPC_SCIENTIST',
  'MM_SOUL_NPC_GORMAN',
  'MM_SOUL_NPC_GROG',
  'MM_SOUL_NPC_DOG_LADY',
  'MM_SOUL_NPC_CARPET_MAN',
  'MM_SOUL_NPC_OLD_HAG',
  'MM_SOUL_NPC_TOURIST_CENTER',
  'MM_SOUL_NPC_BANKER',
  'MM_SOUL_NPC_MOON_CHILDREN',
  'OOT_KEY_RING_FOREST',
  'OOT_KEY_RING_FIRE',
  'OOT_KEY_RING_WATER',
  'OOT_KEY_RING_SPIRIT',
  'OOT_KEY_RING_SHADOW',
  'OOT_KEY_RING_BOTW',
  'OOT_KEY_RING_GTG',
  'OOT_KEY_RING_GANON',
  'OOT_KEY_RING_GF',
  'MM_KEY_RING_WF',
  'MM_KEY_RING_SH',
  'MM_KEY_RING_GB',
  'MM_KEY_RING_ST',
  'OOT_SKELETON_KEY',
  'MM_SKELETON_KEY',
  'SHARED_SKELETON_KEY',
  'OOT_COIN_YELLOW',
  'OOT_COIN_RED',
  'OOT_COIN_GREEN',
  'OOT_COIN_BLUE',
  'OOT_RUPEE_MAGICAL',
  'OOT_BUTTON_A',
  'OOT_BUTTON_C_RIGHT',
  'OOT_BUTTON_C_LEFT',
  'OOT_BUTTON_C_UP',
  'OOT_BUTTON_C_DOWN',
  'MM_BUTTON_A',
  'MM_BUTTON_C_RIGHT',
  'MM_BUTTON_C_LEFT',
  'MM_BUTTON_C_UP',
  'MM_BUTTON_C_DOWN',
  'SHARED_BUTTON_A',
  'SHARED_BUTTON_C_RIGHT',
  'SHARED_BUTTON_C_LEFT',
  'SHARED_BUTTON_C_UP',
  'SHARED_BUTTON_C_DOWN',
  'OOT_SMALL_KEY_TCG',
  'OOT_KEY_RING_TCG',
  'OOT_SOUL_NPC_SARIA',
  'OOT_SOUL_NPC_DARUNIA',
  'OOT_SOUL_NPC_RUTO',
  'OOT_SOUL_NPC_KING_ZORA',
  'OOT_SOUL_NPC_ZELDA',
  'OOT_SOUL_NPC_SHEIK',
  'OOT_SOUL_NPC_ANJU',
  'OOT_SOUL_NPC_CARPENTERS',
  'OOT_SOUL_NPC_GURU_GURU',
  'OOT_SOUL_NPC_MIDO',
  'OOT_SOUL_NPC_KOKIRI',
  'OOT_SOUL_NPC_KOKIRI_SHOPKEEPER',
  'OOT_SOUL_NPC_HYLIAN_GUARD',
  'OOT_SOUL_NPC_CITIZEN',
  'OOT_SOUL_NPC_MALON',
  'OOT_SOUL_NPC_TALON',
  'OOT_SOUL_NPC_FISHING_POND_OWNER',
  'OOT_SOUL_NPC_GORON',
  'OOT_SOUL_NPC_MEDIGORON',
  'OOT_SOUL_NPC_BIGGORON',
  'OOT_SOUL_NPC_GORON_CHILD',
  'OOT_SOUL_NPC_GORON_SHOPKEEPER',
  'OOT_SOUL_NPC_ZORA',
  'OOT_SOUL_NPC_ZORA_SHOPKEEPER',
  'OOT_SOUL_NPC_BAZAAR_SHOPKEEPER',
  'OOT_SOUL_NPC_BOMBCHU_SHOPKEEPER',
  'OOT_SOUL_NPC_POTION_SHOPKEEPER',
  'OOT_SOUL_NPC_BOMBCHU_BOWLING_LADY',
  'OOT_SOUL_NPC_CHEST_GAME_OWNER',
  'OOT_SOUL_NPC_SHOOTING_GALLERY_OWNER',
  'OOT_SOUL_NPC_DAMPE',
  'OOT_SOUL_NPC_BOMBERS',
  'OOT_SOUL_NPC_POE_COLLECTOR',
  'OOT_SOUL_NPC_COMPOSER_BROS',
  'OOT_SOUL_NPC_HONEY_DARLING',
  'OOT_SOUL_NPC_ASTRONOMER',
  'OOT_SOUL_NPC_ROOFTOP_MAN',
  'OOT_SOUL_NPC_BEAN_SALESMAN',
  'OOT_SOUL_NPC_SCIENTIST',
  'OOT_SOUL_NPC_GORMAN',
  'OOT_SOUL_NPC_GROG',
  'OOT_SOUL_NPC_DOG_LADY',
  'OOT_SOUL_NPC_CARPET_MAN',
  'OOT_SOUL_NPC_OLD_HAG',
  'OOT_SOUL_NPC_BANKER',
  'SHARED_SOUL_NPC_SHOOTING_GALLERY_OWNER',
  'SHARED_SOUL_NPC_BAZAAR_SHOPKEEPER',
  'SHARED_SOUL_NPC_GORON',
  'SHARED_SOUL_NPC_GORON_CHILD',
  'SHARED_SOUL_NPC_BOMBCHU_SHOPKEEPER',
  'SHARED_SOUL_NPC_BOMBERS',
  'SHARED_SOUL_NPC_CITIZEN',
  'SHARED_SOUL_NPC_COMPOSER_BROS',
  'SHARED_SOUL_NPC_DAMPE',
  'SHARED_SOUL_NPC_CHEST_GAME_OWNER',
  'SHARED_SOUL_NPC_GORON_SHOPKEEPER',
  'SHARED_SOUL_NPC_HONEY_DARLING',
  'SHARED_SOUL_NPC_RUTO',
  'SHARED_SOUL_NPC_MEDIGORON',
  'SHARED_SOUL_NPC_BIGGORON',
  'SHARED_SOUL_NPC_TALON',
  'SHARED_SOUL_NPC_ASTRONOMER',
  'SHARED_SOUL_NPC_POE_COLLECTOR',
  'SHARED_SOUL_NPC_BOMBCHU_BOWLING_LADY',
  'SHARED_SOUL_NPC_FISHING_POND_OWNER',
  'SHARED_SOUL_NPC_ROOFTOP_MAN',
  'SHARED_SOUL_NPC_ZORA',
  'SHARED_SOUL_NPC_ZORA_SHOPKEEPER',
  'SHARED_SOUL_NPC_MALON',
  'SHARED_SOUL_NPC_BEAN_SALESMAN',
  'SHARED_SOUL_NPC_CARPENTERS',
  'SHARED_SOUL_NPC_ANJU',
  'SHARED_SOUL_NPC_GURU_GURU',
  'SHARED_SOUL_NPC_SCIENTIST',
  'SHARED_SOUL_NPC_GORMAN',
  'SHARED_SOUL_NPC_GROG',
  'SHARED_SOUL_NPC_DOG_LADY',
  'SHARED_SOUL_NPC_CARPET_MAN',
  'SHARED_SOUL_NPC_OLD_HAG',
  'SHARED_SOUL_NPC_BANKER',
  'OOT_BOTTLE_POTION_RED',
  'OOT_BOTTLE_POTION_GREEN',
  'OOT_BOTTLE_POTION_BLUE',
  'OOT_FISHING_POND_CHILD_FISH',
  'OOT_FISHING_POND_CHILD_FISH_2LBS',
  'OOT_FISHING_POND_CHILD_FISH_3LBS',
  'OOT_FISHING_POND_CHILD_FISH_4LBS',
  'OOT_FISHING_POND_CHILD_FISH_5LBS',
  'OOT_FISHING_POND_CHILD_FISH_6LBS',
  'OOT_FISHING_POND_CHILD_FISH_7LBS',
  'OOT_FISHING_POND_CHILD_FISH_8LBS',
  'OOT_FISHING_POND_CHILD_FISH_9LBS',
  'OOT_FISHING_POND_CHILD_FISH_10LBS',
  'OOT_FISHING_POND_CHILD_FISH_11LBS',
  'OOT_FISHING_POND_CHILD_FISH_12LBS',
  'OOT_FISHING_POND_CHILD_FISH_13LBS',
  'OOT_FISHING_POND_CHILD_FISH_14LBS',
  'OOT_FISHING_POND_ADULT_FISH',
  'OOT_FISHING_POND_ADULT_FISH_4LBS',
  'OOT_FISHING_POND_ADULT_FISH_5LBS',
  'OOT_FISHING_POND_ADULT_FISH_6LBS',
  'OOT_FISHING_POND_ADULT_FISH_7LBS',
  'OOT_FISHING_POND_ADULT_FISH_8LBS',
  'OOT_FISHING_POND_ADULT_FISH_9LBS',
  'OOT_FISHING_POND_ADULT_FISH_10LBS',
  'OOT_FISHING_POND_ADULT_FISH_11LBS',
  'OOT_FISHING_POND_ADULT_FISH_12LBS',
  'OOT_FISHING_POND_ADULT_FISH_13LBS',
  'OOT_FISHING_POND_ADULT_FISH_14LBS',
  'OOT_FISHING_POND_ADULT_FISH_15LBS',
  'OOT_FISHING_POND_ADULT_FISH_16LBS',
  'OOT_FISHING_POND_ADULT_FISH_17LBS',
  'OOT_FISHING_POND_ADULT_FISH_18LBS',
  'OOT_FISHING_POND_ADULT_FISH_19LBS',
  'OOT_FISHING_POND_ADULT_FISH_20LBS',
  'OOT_FISHING_POND_ADULT_FISH_21LBS',
  'OOT_FISHING_POND_ADULT_FISH_22LBS',
  'OOT_FISHING_POND_ADULT_FISH_23LBS',
  'OOT_FISHING_POND_ADULT_FISH_24LBS',
  'OOT_FISHING_POND_ADULT_FISH_25LBS',
  'OOT_FISHING_POND_CHILD_LOACH',
  'OOT_FISHING_POND_CHILD_LOACH_14LBS',
  'OOT_FISHING_POND_CHILD_LOACH_15LBS',
  'OOT_FISHING_POND_CHILD_LOACH_16LBS',
  'OOT_FISHING_POND_CHILD_LOACH_17LBS',
  'OOT_FISHING_POND_CHILD_LOACH_18LBS',
  'OOT_FISHING_POND_CHILD_LOACH_19LBS',
  'OOT_FISHING_POND_ADULT_LOACH',
  'OOT_FISHING_POND_ADULT_LOACH_29LBS',
  'OOT_FISHING_POND_ADULT_LOACH_30LBS',
  'OOT_FISHING_POND_ADULT_LOACH_31LBS',
  'OOT_FISHING_POND_ADULT_LOACH_32LBS',
  'OOT_FISHING_POND_ADULT_LOACH_33LBS',
  'OOT_FISHING_POND_ADULT_LOACH_34LBS',
  'OOT_FISHING_POND_ADULT_LOACH_35LBS',
  'OOT_FISHING_POND_ADULT_LOACH_36LBS',
  'OOT_BOTTLE_FAIRY',
  'MM_BOTTLE_FAIRY',
  'MM_POE',
  'MM_BIG_POE',
  'OOT_BOTTLE_POE',
  'MM_BOTTLE_POE',
  'OOT_BOTTLE_BIG_POE',
  'MM_BOTTLE_BIG_POE',
  'OOT_MAGIC_JAR_SMALL',
  'OOT_MAGIC_JAR_LARGE',
  'MM_MAGIC_JAR_SMALL',
  'MM_MAGIC_JAR_LARGE',
  'SHARED_MAGIC_JAR_SMALL',
  'SHARED_MAGIC_JAR_LARGE',
  'OOT_TRAP_RUPOOR',
  'MM_TRAP_RUPOOR',
  'SHARED_TRAP_RUPOOR',
  'NOTHING',
  'OOT_FAIRY_BIG',
  'MM_FAIRY_BIG',
  'SHARED_FAIRY_BIG',
  'MM_SPELL_FIRE',
  'MM_SPELL_WIND',
  'MM_SPELL_LOVE',
  'MM_BOOTS_IRON',
  'MM_BOOTS_HOVER',
  'MM_TUNIC_GORON',
  'MM_TUNIC_ZORA',
  'MM_SCALE',
  'MM_STRENGTH',
  'OOT_MASK_BLAST',
  'SHARED_MASK_BLAST',
  'OOT_MASK_STONE',
  'SHARED_MASK_STONE',
  'OOT_SONG_EMPTINESS',
  'OOT_SONG_EMPTINESS_PROGRESSIVE_NOTE',
  'OOT_SONG_EMPTINESS_C_RIGHT_1',
  'OOT_SONG_EMPTINESS_C_LEFT_1',
  'OOT_SONG_EMPTINESS_C_RIGHT_2',
  'OOT_SONG_EMPTINESS_C_DOWN',
  'OOT_SONG_EMPTINESS_C_RIGHT_3',
  'OOT_SONG_EMPTINESS_C_UP',
  'OOT_SONG_EMPTINESS_C_LEFT_2',
  'SHARED_SONG_EMPTINESS',
  'SHARED_SONG_EMPTINESS_PROGRESSIVE_NOTE',
  'SHARED_SONG_EMPTINESS_C_RIGHT_1',
  'SHARED_SONG_EMPTINESS_C_LEFT_1',
  'SHARED_SONG_EMPTINESS_C_RIGHT_2',
  'SHARED_SONG_EMPTINESS_C_DOWN',
  'SHARED_SONG_EMPTINESS_C_RIGHT_3',
  'SHARED_SONG_EMPTINESS_C_UP',
  'SHARED_SONG_EMPTINESS_C_LEFT_2',
  'MM_CLOCK',
  'MM_CLOCK1',
  'MM_CLOCK2',
  'MM_CLOCK3',
  'MM_CLOCK4',
  'MM_CLOCK5',
  'MM_CLOCK6',
  'OOT_SOUL_MISC_GS',
  'OOT_SOUL_MISC_BUSINESS_SCRUB',
  'MM_SOUL_MISC_GS',
  'MM_SOUL_MISC_BUSINESS_SCRUB',
  'SHARED_SOUL_MISC_GS',
  'SHARED_SOUL_MISC_BUSINESS_SCRUB',
  'OOT_SOUL_NPC_THIEVES',
  'MM_SOUL_NPC_THIEVES',
  'OOT_SOUL_ENEMY_THIEVES',
  'MM_SOUL_ENEMY_THIEVES',
  'SHARED_SOUL_NPC_THIEVES',
  'SHARED_SOUL_ENEMY_THIEVES',
  'MM_HAMMER',
  'SHARED_HAMMER',
  'MM_STONE_OF_AGONY',
  'SHARED_STONE_OF_AGONY',
] as const;

export type ItemID = typeof ITEM_IDS[number];
type ItemDefs = {[k in ItemID]: Item};

export type Item = {
  id: string;
  __brand: 'Item';
};

export const Items: ItemDefs = Object.fromEntries(ITEM_IDS.map(id => [id, { id } as Item])) as ItemDefs;

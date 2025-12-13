import { CacheLoad, SongType } from "./enums";
import { MusicGroup } from "./music-groups";

export type MusicEntry = {
  name: string;
  songType: SongType;
  seqCacheType?: CacheLoad;
  bankCacheType?: CacheLoad;
  musicGroups: MusicGroup[];
  canSongtest?: boolean;
  seqOot?: number[];
  seqMm?: number[];
}

/*
 * Instead of MUSIC, this is renamed to SEQS
 * because it almost basically mirrors MMR's SEQS file
 */
export const SEQS: {[k: string]: MusicEntry} = {
  // #region Ocarina of Time BGM
  OOT_FILE_SELECT: {
    name: 'OoT - File Select',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.CalmThemes,
    ],
    canSongtest: true,
    seqOot: [0x57],
  },

  OOT_HYRULE_FIELD: {
    name: 'OoT - Hyrule Field',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fields,
      MusicGroup.Outdoors,
    ],
    seqOot: [0x02],
  },

  OOT_DODONGO_CAVERN: {
    name: 'OoT - Dodongo Cavern',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
    ],
    seqOot: [0x18],
  },

  OOT_KAKARIKO_ADULT: {
    name: 'OoT - Kakariko Adult',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Outdoors,
      MusicGroup.Towns,
    ],
    seqOot: [0x19],
  },

  OOT_BATTLE: {
    name: 'OoT - Battle',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fights,
    ],
    seqOot: [0x1A],
  },

  OOT_BATTLE_BOSS: {
    name: 'OoT - Boss Battle',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fights,
    ],
    seqOot: [0x1B],
  },

  OOT_DEKU_TREE: {
    name: 'OoT - Deku Tree',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.CalmThemes,
    ],
    seqOot: [0x1C],
  },

  OOT_MARKET: {
    name: 'OoT - Market',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Towns,
      MusicGroup.Outdoors,
    ],
    seqOot: [0x1D],
  },

  OOT_TITLE: {
    name: 'OoT - Title Theme',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.CalmThemes,
    ],
    seqOot: [0x1E],
  },

  OOT_HOUSES: {
    name: 'OoT - Houses',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
    ],
    seqOot: [0x1F],
  },

  OOT_JABU_JABU: {
    name: 'OoT - Jabu Jabu',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
    ],
    seqOot: [0x26],
  },

  OOT_KAKARIKO_CHILD: {
    name: 'OoT - Kakariko Child',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Towns,
      MusicGroup.Outdoors,
    ],
    seqOot: [0x27],
  },

  OOT_FAIRY_FOUNTAIN: {
    name: 'OoT - Fairy Fountain',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
      MusicGroup.CalmThemes,
    ],
    seqOot: [0x28],
  },

  OOT_ZELDA_THEME: {
    name: 'OoT - Zelda Theme',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.CalmThemes,
    ],
    seqOot: [0x29],
  },

  OOT_TEMPLE_FIRE: {
    name: 'OoT - Fire Temple',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
    ],
    seqOot: [0x2A],
  },

  OOT_TEMPLE_FOREST: {
    name: 'OoT - Forest Temple',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
    ],
    seqOot: [0x2C],
  },

  OOT_CASTLE_COURTYARD: {
    name: 'OoT - Castle Courtyard',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Minigames,
    ],
    seqOot: [0x2D],
  },

  OOT_GANONDORF_THEME: {
    name: 'OoT - Ganondorf Theme',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.CalmThemes,
    ],
    seqOot: [0x2E],
  },

  OOT_LON_LON_RANCH: {
    name: 'OoT - Lon Lon Ranch',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Towns,
      MusicGroup.Outdoors,
    ],
    seqOot: [0x2F],
  },

  OOT_GORON_CITY: {
    name: 'OoT - Goron City',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Towns,
      MusicGroup.Indoors,
    ],
    seqOot: [0x30],
  },

  OOT_BATTLE_MINIBOSS: {
    name: 'OoT - Miniboss Battle',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fights,
    ],
    seqOot: [0x38],
  },

  OOT_TEMPLE_OF_TIME: {
    name: 'OoT - Temple of Time',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
      MusicGroup.CalmThemes,
    ],
    seqOot: [0x3A],
  },

  OOT_KOKIRI_FOREST: {
    name: 'OoT - Kokiri Forest',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Towns,
      MusicGroup.Outdoors,
    ],
    seqOot: [0x3C],
  },

  OOT_LOST_WOODS: {
    name: 'OoT - Lost Woods',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fields,
      MusicGroup.Towns,
      MusicGroup.Outdoors,
    ],
    seqOot: [0x3E],
  },

  OOT_TEMPLE_SPIRIT: {
    name: 'OoT - Spirit Temple',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
    ],
    seqOot: [0x3F],
  },

  OOT_HORSE_RACE: {
    name: 'OoT - Horse Race',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Minigames,
    ],
    seqOot: [0x40],
  },

  OOT_INGO_THEME: {
    name: 'OoT - Ingo Theme',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.CalmThemes,
    ],
    seqOot: [0x42],
  },

  OOT_FAIRY_FLYING: {
    name: 'OoT - Fairy Flying',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.ActionThemes,
      MusicGroup.CalmThemes,
    ],
    seqOot: [0x4A],
  },

  OOT_THEME_DEKU_TREE: {
    name: 'OoT - Deku Tree Theme',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.CalmThemes,
    ],
    seqOot: [0x4B],
  },

  OOT_WINDMILL_HUT: {
    name: 'OoT - Windmill Hut',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
      MusicGroup.CalmThemes,
    ],
    seqOot: [0x4C],
  },

  OOT_SHOOTING_GALLERY: {
    name: 'OoT - Shooting Gallery',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
      MusicGroup.Minigames,
    ],
    seqOot: [0x4E],
  },

  OOT_SHEIK_THEME: {
    name: 'OoT - Sheik Theme',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.CalmThemes,
    ],
    seqOot: [0x4F],
  },

  OOT_ZORAS_DOMAIN: {
    name: 'OoT - Zoras Domain',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Towns,
      MusicGroup.Indoors,
    ],
    seqOot: [0x50],
  },

  OOT_SHOP: {
    name: 'OoT - Shop',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
    ],
    seqOot: [0x55],
  },

  OOT_SAGES: {
    name: 'OoT - Chamber of the Sages',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
      MusicGroup.CalmThemes,
    ],
    seqOot: [0x56]
  },

  OOT_ICE_CAVERN: {
    name: 'OoT - Ice Cavern',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
      MusicGroup.CalmThemes,
    ],
    seqOot: [0x58],
  },

  OOT_KAEPORA_GAEBORA: {
    name: 'OoT - Kaepora Gaebora',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.PERSISTENT,
    musicGroups: [
      MusicGroup.CalmThemes,
    ],
    seqOot: [0x5A],
  },

  OOT_TEMPLE_SHADOW: {
    name: 'OoT - Shadow Temple',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
    ],
    seqOot: [0x5B],
  },

  OOT_TEMPLE_WATER: {
    name: 'OoT - Water Temple',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
    ],
    seqOot: [0x5C],
  },

  OOT_GERUDO_VALLEY: {
    name: 'OoT - Gerudo Valley',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fields,
      MusicGroup.Outdoors,
    ],
    seqOot: [0x5F],
  },

  OOT_POTION_SHOP: {
    name: 'OoT - Potion Shop (OoT)',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
    ],
    seqOot: [0x60],
  },

  OOT_KOTAKE_KOUME: {
    name: 'OoT - Kotake and Koume',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
      MusicGroup.CalmThemes,
    ],
    seqOot: [0x61],
  },

  OOT_ESCAPE_CASTLE: {
    name: 'OoT - Castle Escape',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.ActionThemes,
      MusicGroup.Fights,
    ],
    seqOot: [0x62],
  },

  OOT_UNDERGROUND_CASTLE: {
    name: 'OoT - Castle Underground',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
    ],
    seqOot: [0x63],
  },

  OOT_BATTLE_GANONDORF: {
    name: 'OoT - Ganondorf Battle',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fights,
    ],
    seqOot: [0x64],
  },

  OOT_BATTLE_GANON: {
    name: 'OoT - Ganon Battle',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fights,
    ],
    seqOot: [0x65],
  },

  OOT_BATTLE_BOSS_FIRE: {
    name: 'OoT - Fire Temple Boss',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fights,
    ],
    seqOot: [0x6B],
  },

  OOT_MINIGAME: {
    name: 'OoT - Minigame',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Minigames,
    ],
    seqOot: [0x6C],
  },

  OOT_GROTTOS: {
    name: 'OoT -  Grottos',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
      MusicGroup.Indoors,
    ],
    seqOot: [0x6E],
  },

  OOT_GRAVES: {
    name: 'OoT - Graves',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
      MusicGroup.Indoors,
    ],
    seqOot: [0x6F],
  },

  OOT_GERUDO_TRAINING_GROUNDS: {
    name: 'OoT - Gerudo Training Grounds',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
      MusicGroup.Indoors,
    ],
    seqOot: [0x70],
  },
  // #endregion

  // #region Majora's Mask BGM
  MM_TERMINA_FIELD: {
    name: 'MM - Termina Field',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fields,
      MusicGroup.Outdoors,
    ],
    seqMm: [0x02],
  },

  MM_TEMPLE_STONE_TOWER: {
    name: 'MM - Stone Tower Temple',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
    ],
    seqMm: [0x06],
  },

  MM_TEMPLE_STONE_TOWER_INVERTED: {
    name: 'MM - Stone Tower Temple Inverted',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
    ],
    seqMm: [0x07],
  },

  MM_SOUTHERN_SWAMP: {
    name: 'MM - Southern Swamp',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fields,
      MusicGroup.Outdoors,
    ],
    seqMm: [0x0C],
  },

  MM_ALIENS: {
    name: 'MM - Aliens',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.ActionThemes,
    ],
    seqMm: [0x0D],
  },

  MM_MINIGAME: {
    name: 'MM - Minigame',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Minigames,
    ],
    seqMm: [0x0E],
  },

  MM_SHARP_CURSE: {
    name: 'MM - Sharp Curse',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.ActionThemes,
    ],
    seqMm: [0x0F],
  },

  MM_GREAT_BAY_COAST: {
    name: 'MM - Great Bay Coast',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fields,
      MusicGroup.Outdoors,
    ],
    seqMm: [0x10],
  },

  MM_IKANA_VALLEY: {
    name: 'MM - Ikana Valley',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fields,
      MusicGroup.Outdoors,
    ],
    seqMm: [0x11],
  },

  MM_COURT_DEKU_KING: {
    name: 'MM - Court of the Deku King',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Towns,
    ],
    seqMm: [0x12],
  },

  MM_MOUNTAIN_VILLAGE: {
    name: 'MM - Mountain Village',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fields,
      MusicGroup.Outdoors,
    ],
    seqMm: [0x13],
  },

  MM_PIRATES_FORTRESS: {
    name: 'MM - Pirates Fortress',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
    ],
    seqMm: [0x14],
  },

  MM_CLOCK_TOWN_DAY_1: {
    name: 'MM - Clock Town Day 1',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Towns,
    ],
    seqMm: [0x15, 0x1D],
  },

  MM_CLOCK_TOWN_DAY_2: {
    name: 'MM - Clock Town Day 2',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Towns,
    ],
    seqMm: [0x16, 0x23],
  },

  MM_CLOCK_TOWN_DAY_3: {
    name: 'MM - Clock Town Day 3',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Towns,
    ],
    seqMm: [0x17],
  },

  MM_BATTLE_BOSS: {
    name: 'MM - Boss Battle',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fights,
    ],
    seqMm: [0x1B],
  },

  MM_WOODFALL_TEMPLE: {
    name: 'MM - Woodfall Temple',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
    ],
    seqMm: [0x1C],
  },

  MM_STOCK_POT_INN: {
    name: 'MM - Stock Pot Inn',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
    ],
    seqMm: [0x1F],
  },

  MM_MINIGAME2: {
    name: 'MM - Minigame 2',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Minigames,
    ],
    seqMm: [0x25],
  },

  MM_GORON_RACE: {
    name: 'MM - Goron Race',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Minigames,
    ],
    seqMm: [0x26],
  },

  MM_MUSIC_BOX_HOUSE:  {
    name: 'MM - Music Box House',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
      MusicGroup.CalmThemes,
    ],
    seqMm: [0x27],
  },

  MM_FAIRYS_FOUNTAIN:  {
    name: 'MM - Fairy\'s Fountain',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
      MusicGroup.CalmThemes,
    ],
    seqMm: [0x28], /* 0x18 = File Select */
  },

  MM_MARINE_RESEARCH_LABORATORY: {
    name: 'MM - Marine Research Laboratory',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
    ],
    seqMm: [0x2C],
  },

  MM_ROMANI_RANCH: {
    name: 'MM - Romani Ranch',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Towns,
      MusicGroup.Outdoors,
    ],
    seqMm: [0x2F],
  },

  MM_GORON_VILLAGE: {
    name: 'MM - Goron Village',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Towns,
      MusicGroup.Indoors,
    ],
    seqMm: [0x30],
  },

  MM_MAYOR_DOTOUR: {
    name: 'MM - Mayor Dotour',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
      MusicGroup.ActionThemes,
    ],
    seqMm: [0x31],
  },

  MM_ZORA_HALL: {
    name: 'MM - Zora Hall',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Towns,
      MusicGroup.Indoors,
    ],
    seqMm: [0x36],
  },

  MM_MINIBOSS: {
    name: 'MM - Mini Boss',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fights,
    ],
    seqMm: [0x38],
  },

  MM_ASTRAL_OBSERVATORY: {
    name: 'MM - Astral Observatory',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
      MusicGroup.CalmThemes,
    ],
    seqMm: [0x3A],
  },

  MM_BOMBERS_HIDEOUT: {
    name: 'MM - Bombers Hideout',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
      MusicGroup.Indoors,
    ],
    seqMm: [0x3B],
  },

  MM_MILK_BAR: {
    name: 'MM - Milk Bar',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
      MusicGroup.CalmThemes,
    ],
    seqMm: [0x3C, 0x56],
  },

  MM_WOODS_OF_MYSTERY: {
    name: 'MM - Woods of Mystery',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fields,
      MusicGroup.Outdoors,
    ],
    seqMm: [0x3E],
  },

  MM_GORMAN_RACE: {
    name: 'MM - Gorman Race',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Minigames,
    ],
    seqMm: [0x40],
  },

  MM_GORMAN_BROS: {
    name: 'MM - Gorman Bros.',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.CalmThemes,
    ],
    seqMm: [0x42],
  },

  MM_KOTAKE_POTION_SHOP: {
    name: 'MM - Kotake\'s Potion Shop',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
      MusicGroup.CalmThemes,
    ],
    seqMm: [0x43],
  },

  MM_STORE: {
    name: 'MM - Store',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
    ],
    seqMm: [0x44],
  },

  MM_TARGET_PRACTICE: {
    name: 'MM - Target Practice',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Minigames,
    ],
    seqMm: [0x46],
  },

  MM_SWORD_TRAINING: {
    name: 'MM - Sword Training',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Indoors,
      MusicGroup.ActionThemes,
    ],
    seqMm: [0x50],
  },

  MM_FINAL_HOURS: {
    name: 'MM - Final Hours',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.CalmThemes,
    ],
    seqMm: [0x57, 0x60], /*0x60 = Majora's Boss Room (Pointer: 0x57) */
  },

  MM_TEMPLE_SNOWHEAD: {
    name: 'MM - Snowhead Temple',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
    ],
    seqMm: [0x65],
  },

  MM_TEMPLE_GREAT_BAY: {
    name: 'MM - Great Bay Temple',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
    ],
    seqMm: [0x66],
  },

  MM_BATTLE_MAJORA3: {
    name: 'MM - Majora\'s Wrath',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fights,
    ],
    seqMm: [0x69],
  },

  MM_BATTLE_MAJORA2: {
    name: 'MM - Majora\'s Incarnation',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fights,
    ],
    seqMm: [0x6A],
  },

  MM_BATTLE_MAJORA1: {
    name: 'MM - Majora\'s Mask',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Fights,
    ],
    seqMm: [0x6B],
  },

  MM_IKANA_CASTLE: {
    name: 'MM - Ikana Castle',
    songType: SongType.Bgm,
    bankCacheType: CacheLoad.TEMPORARY,
    musicGroups: [
      MusicGroup.Dungeons,
    ],
    seqMm: [0x6F],
  },
  // #endregion

  // #region Shared Fanfares
  FANFARE_SHARED_ITEM_MAJOR: {
    name: 'Shared - Fanfare Item Major',
    songType: SongType.Fanfare,
    bankCacheType: CacheLoad.PERSISTENT,
    musicGroups: [
      MusicGroup.ItemFanfares,
    ],
    seqOot: [0x22],
    seqMm: [0x22],
  },

  FANFARE_SHARED_ITEM_HEART_PIECE: {
    name: 'Shared - Fanfare Item Heart Piece',
    songType: SongType.Fanfare,
    bankCacheType: CacheLoad.PERSISTENT,
    musicGroups: [
      MusicGroup.ItemFanfares,
    ],
    seqOot: [0x39],
    seqMm: [0x39],
  },

  FANFARE_SHARED_ITEM_HEART_CONTAINER: {
    name: 'Shared - Fanfare Item Heart Container',
    songType: SongType.Fanfare,
    bankCacheType: CacheLoad.PERSISTENT,
    musicGroups: [
      MusicGroup.ItemFanfares,
    ],
    seqOot: [0x24],
    seqMm: [0x24],
  },

  FANFARE_SHARED_ITEM_MASK: {
    name: 'Shared - Fanfare Item Mask',
    songType: SongType.Fanfare,
    bankCacheType: CacheLoad.PERSISTENT,
    musicGroups: [
      MusicGroup.ItemFanfares,
    ],
    seqOot: [],
    seqMm: [0x37],
  },

  FANFARE_SHARED_ITEM_STONE: {
    name: 'Shared - Fanfare Item Stone',
    songType: SongType.Fanfare,
    bankCacheType: CacheLoad.PERSISTENT,
    musicGroups: [
      MusicGroup.ItemFanfares,
    ],
    seqOot: [0x32],
    seqMm: [],
  },

  FANFARE_SHARED_ITEM_MEDALLION: {
    name: 'Shared - Fanfare Item Medallion',
    songType: SongType.Fanfare,
    bankCacheType: CacheLoad.PERSISTENT,
    musicGroups: [
      MusicGroup.ItemFanfares,
    ],
    seqOot: [0x43],
    seqMm: [],
  },

  FANFARE_SHARED_ITEM_OCARINA: {
    name: 'Shared - Fanfare Item Ocarina',
    songType: SongType.Fanfare,
    bankCacheType: CacheLoad.PERSISTENT,
    musicGroups: [
      MusicGroup.ItemFanfares,
    ],
    seqOot: [0x3D],
    seqMm: [0x52],
  },
  // #endregion

  // #region Ocarina of Time Fanfares
  FANFARE_OOT_GAME_OVER: {
    name: 'OoT - Fanfare Game Over',
    songType: SongType.Fanfare,
    bankCacheType: CacheLoad.PERSISTENT,
    musicGroups: [
      MusicGroup.EventFanfares,
    ],
    seqOot: [0x20],
  },

  FANFARE_OOT_BOSS_CLEAR: {
    name: 'OoT - Fanfare Boss Clear',
    songType: SongType.Fanfare,
    bankCacheType: CacheLoad.PERSISTENT,
    musicGroups: [
      MusicGroup.ClearFanfares,
    ],
    seqOot: [0x21],
  },
  // #endregion

  // #region Majora's Mask Fanfares
  FANFARE_MM_CLEAR_SHORT: { // Originally MM_CLEAR_WOODFALL
    name: 'MM - Temple Clear Short',
    songType: SongType.Fanfare, // Was originally BGM, why? It's fanfare in MMR
    bankCacheType: CacheLoad.PERSISTENT,
    musicGroups: [
      MusicGroup.ClearFanfares,
    ],
    seqMm: [0x78],
  },

  FANFARE_MM_CLEAR_LONG: { // Originally MM_CLEAR_SNOWHEAD
    name: 'MM - Temple Clear Long',
    songType: SongType.Fanfare, // Was originally BGM, why? It's fanfare in MMR
    bankCacheType: CacheLoad.PERSISTENT,
    musicGroups: [
      MusicGroup.ClearFanfares,
    ],
    seqMm: [0x79],
  },
  // #endregion
};
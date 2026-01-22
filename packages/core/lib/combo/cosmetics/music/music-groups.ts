/**
 * Music Groups
 *
 * This file contains the smart enum that handles
 * music groups for categorizing music.
 *
 * Currently, only has group categorization, but it
 * can be expanded for individual slot categorization.
 * However, indivdual slot categorization is a lot harder
 * to implement and maintain across two games.
 */

import { SongType } from './enums';

type MusicGroupOpts = {
  type: SongType;
  aliases: string[];
}

/**
 * Attempts to normalize MMR category values from hex string into decimal strings.
 * If the input is not a valid MMR category (or is an OOTR music group), the function
 * returns the input string in lowercase.
 *
 * MMR categories are expected to be hex-formatted strings, optionally prefixed with "0x".
 *
 * @param v The raw string or number representing a music group.
 * @returns A decimal string representation of the input if it is a valid hex number
 *          for MMR categories, otherwise the input string in lowercase for OOTR music groups.
 */
function normalizeLegacy(v: string | number): string {
  let n: number;

  if (typeof v === 'number') {
    n = v;
  } else {
    const str = v.toLowerCase().trim();
    n = str.startsWith('0x') ? parseInt(str, 16) : parseInt(str, 16);
  }

  if (!isNaN(n)) return n.toString();

  return v.toString().toLowerCase().trim();
}

/**
 * A "smart-enum" representingmusic groups used for categorizing music and
 * sequence slots in OOTMM.
 *
 * @constant all - An array of all available MusicGroup instances.
 * @constant Fields - A MusicGroup instance representing "field" BGM music.
 * @constant Towns - A MusicGroup instance representing "town" BGM music.
 * @constant Dungeons - A MusicGroup instance representing "dungeon" BGM music.
 * @constant Outdoors - A MusicGroup instance representing "outdoor" BGM music.
 * @constant Indoors - A MusicGroup instance representing "indoor" BGM music.
 * @constant Minigames - A MusicGroup instance representing "minigame" BGM music.
 * @constant ActionThemes - A MusicGroup instance representing "action" BGM music.
 * @constant CalmThemes - A MusicGroup instance representing "calm" BGM music.
 * @constant ItemFanfares - A MusicGroup instance representing "item" fanfare music.
 * @constant EventFanfares - A MusicGroup instance representing "event" fanfare music.
 * @constant ClearFanfares - A MusicGroup instance representing "clear" fanfare music.
 * @constant DefaultBgmGroups - An array of default BGM MusicGroup instances.
 * @constant DefaultFanfareGroups - An array of defaul fanfare MusicGroup instances.
 * @constant AllGroups - An array of all MusicGroup instances.
 * @constant AllBgmGroups - An array of all BGM MusicGroup instances.
 * @constant AllFanfareGroups - An array of all fanfare MusicGroup instances.
 */
export class MusicGroup {
  readonly type: SongType;
  readonly aliases: readonly string[];
  private readonly _aliases: Set<string>;

  private constructor({ type, aliases = [] }: MusicGroupOpts) {
    this.type = type;
    this.aliases = aliases.map(a => a.toLowerCase());
    this._aliases = new Set(this.aliases);

    MusicGroup._register(this);
  }

  /**
   * Determines if the input value matches an alias in one of the available
   * MusicGroup instances.
   * @param v The value to search for.
   * @returns `true` if the value matches an alias in a MusicGroup instance, otherwise `false`.
   */
  matches(v: string | number): boolean {
    return this._aliases.has(normalizeLegacy(v));
  }

  private static readonly _all: MusicGroup[] = [];

  private static _register(group: MusicGroup) {
    MusicGroup._all.push(group);
  }

  static get all(): readonly MusicGroup[] {
    return this._all;
  }

  /**
   * Builds a MusicGroup array for matches with the given raw music group string array.
   * This function matches raw group names to MusicGroup instances.
   * @param rawGroups A string array containing raw group names.
   * @param filename The name of the file.
   * @param monitor A monitor object used for warnings or logs.
   * @returns An object literal with `type` (song type) and `groups` (matched MusicGroup instances),
   *          or `null` if no matches are found or there are conflicting song types.
   */
  static matchGroups(rawGroups: string[], filename: string, monitor: any) : { type: SongType, groups: MusicGroup[] } | null {
    const matchedGroups: MusicGroup[] = [];

    for (const group of rawGroups) {
      const match = MusicGroup.all.find(g => g.matches(group));
      if (match) matchedGroups.push(match);
    }

    if (matchedGroups.length === 0) {
      monitor.warn(`Skipped music file ${filename}: categories contained no valid groups`);
      return null;
    }

    const distinctTypes = new Set(matchedGroups.map(g => g.type));
    if (distinctTypes.size > 1) {
      // OOTRS files are not actually skipped... maybe add file type aware conditional?
      monitor.warn(`Skipped music file ${filename}: conflicting bgm and fanfare music groups`);
      return null;
    }

    return {
      type: [...distinctTypes][0]!,
      groups: matchedGroups
    };
  }

  /**
   * OOTR's specificity system is a PITA to support.
   * Why not just do groups and singles like MMR?
   * Also, who decided on inconsistent plurality?
   * Either make none of them plural, or all plural...
   * Seriously... It's literally a system made to never be used...
   *
   * — Sincerely, a very annoyed girl.
   */
  // #region Group BGM
  static readonly Fields = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // MMR
      '0',   // Fields
      '258', // Termina Field
      '268', // Southern Swamp
      '272', // Great Bay
      '273', // Ikana Canyon
      '275', // Snowhead Mountains
      '318', // Woods of Mystery
      // OOTR
      'Fields',
      'Overworld',
      'HyruleField',
      'LostWoods',
      'GerudoValley',
    ]
  });

  static readonly Towns = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // MMR
      '1',   // Towns
      '274', // Deku Palace
      '277', // Clock Town 1
      '278', // Clock Town 2
      '279', // Clock Town 3
      '303', // Romani Ranch
      '304', // Goron Shrine
      '310', // Zora Hall
      // OOTR
      'Town',
      'Market',
      'KakarikoChild',
      'KakarikoAdult',
      'KokiriForest',
      'LonLonRanch',
      'GoronCity',
      'ZorasDomain',
    ]
  })

  static readonly Dungeons = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // MMR
      '2',   // Dungeons
      '262', // Stone Tower
      '263', // Inverted Stone Tower
      '276', // Pirates' Fortress
      '284', // Woodfall Temple
      '315', // Secret Grotto
      '357', // Snowhead Temple
      '358', // Great Bay Temple
      '367', // Ancient Castle of Ikana
      // OOTR
      'Dungeon',
      'ChildDungeon',
      'AdultDungeon',
      'AncientDungeon',
      'MysticalDungeon',
      'SpookyDungeon',
      'InsideDekuTree',
      'DodongosCavern',
      'JabuJabu',
      'CastleUnderground',
      'IceCavern',
      'ForestTemple',
      'FireTemple',
      'WaterTemple',
      'ShadowTemple',
      'SpiritTemple',
    ]
  })

  static readonly Outdoors = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // OOTR
      'Outdoors' // Not in MMR
    ]
  })

  static readonly Indoors = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // MMR
      '3',   // Indoors
      '261', // Clock Tower Interior
      '287', // House
      '300', // Curiosity Shop & Marine Research Lab
      '305', // Mayor Dotour's Office
      '314', // Astral Observatory
      '316', // Milk Bar
      '324', // Item Shop
      '326', // Minigame Shop
      '336', // Swordsman's School
      '364', // Japas' Room
      '365', // Tijo's Room
      '366', // Evan's Room
      // OOTR
      'Indoors',
      'House',
      'ItemShop',
      'SalesArea',
      'Shop',
      'ShootingGallery',
      'WindmillHut',
      'PotionShop',
      'TempleOfTime',
      'ChamberofTheSages',
    ]
  })

  static readonly Minigames = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // MMR
      '4',   // Minigames
      '270', // Old Koume's Boat Cruise
      '293', // Minigame Theme
      '294', // Goron Race
      '320', // Horse Race
      // OOTR
      'HorseRace',
      'Fun',
      'CastleCourtyard',
      'Mini-game',
    ]
  })

  static readonly ActionThemes = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // MMR
      '5',   // Action Themes
      '259', // Pursuit Theme
      '269', // Aliens' Theme
      '271', // Sharp's Curse
      '379', // The Moon Enraged
      // OOTR
      'HeroTheme',
      'VillainTheme',
      'CastleEscape',
    ]
  })

  static readonly CalmThemes = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // MMR
      '6',   // Calm Themes & Character Themes
      '22',  // Cutscenes
      '260', // Majora's Theme
      '267', // Song of Healing Theme
      '280', // File Select
      '295', // Music-Box House
      '296', // Great Fairy's Fountain (Pointer: 0x18)
      '297', // Zelda's Theme
      '298', // Rosa Sisters' Theme
      '301', // Giants' Theme
      '302', // Guru-Guru's Theme
      '322', // Gorman Bros.' Theme
      '323', // Koume & Kotake's Theme
      '325', // Kaepora Gaebora's Theme
      '343', // Final Hours
      '369', // Kamaro's Theme
      '370', // Cremia's Theme
      '371', // Keaton's Theme
      '381', // Reunion Theme
      // OOTR
      'CharacterTheme',
      'MagicalPlace',
      'DekuTree',
      'GanondorfTheme',
      'FairyFountain',
      'ZeldaTheme',
      'SheikTheme',
      'IngoTheme',
      'KotakeAndKoume',
      'FairyFlying',
      'KaeporaGaebora',
    ]
  })

  static readonly Fights = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // MMR
      '7',   // Fights
      '282', // Small Enemy
      '283', // Boss Enemy
      '312', // Big Enemy
      '361', // Majora's Wrath
      '362', // Majora's Incarnation
      '363', // Majora's Mask
      // OOTR
      'Fight',
      'SmallFight',
      'BigFight',
      'Battle',
      'MinibossBattle',
      'BossFight',
      'FireBoss',
      'GanondorfBattle',
      'GanonBattle',
      'FinalFight',
    ]
  })
  // #endregion

  // #region Group Fanfares
  static readonly ItemFanfares = new MusicGroup({
    type: SongType.Fanfare,
    aliases: [
      // MMR
      '8',   // Item Fanfares
      '290', // Item Get
      '292', // Heart Container Get
      '299', // Large Treasure Chest
      '311', // Mask Get
      '313', // Heart Piece Get
      '338', // Song Get
      // OOTR
      'ItemGet',
      'HeartContainerGet',
      'HeartPieceGet',
      'SongGet',
      'SongFanfare',
      'ItemFanfare',
      'UtilitySong',
      'BigFanfare',
      'TreasureChest',
    ]
  })

  static readonly EventFanfares = new MusicGroup({
    type: SongType.Fanfare,
    aliases: [
      // MMR
      '9',   // Event Fanfares
      '264', // Event Fail 1
      '265', // Event Fail 2
      '281', // Event Success
      '288', // Game over
      '317', // Truth Revealed
      '341', // Song of Soaring Theme
      '375', // Temple Appears
      '380', // Giants Leave
      // OOTR
      'GameOver',
      'EventFanfare',
      'GanondorfAppears',
      'PreludeOfLight',
      'MinuetOfForest',
      'BoleroOfFire',
      'SerenadeOfWater',
      'RequiemOfSpirit',
      'NocturneOfShadow',
      'ZeldasLullaby',
      'EponasSong',
      'SariasSong',
      'SunsSong',
      'SongOfTime',
      'SongOfStorms',
      'EscapeFromRanch',
      'ZeldaTurnsAround',
      'WarpSong',
      'MasterSword',
      'DoorOfTime',
    ]
  })

  static readonly ClearFanfares = new MusicGroup({
    type: SongType.Fanfare,
    aliases: [
      // MMR
      '16',  // Clear Fanfares
      '289', // Boss Defeated
      '319', // Goron Race Win
      '321', // Horse Race Win
      '376', // Temple Clear (Short)
      '377', // Temple Clear (Long)
      '382', // Moon Destroyed
      // OOTR
      'BossDefeated',
      'SuccessFanfare',
      'EponaGoal',
      'SpiritStoneGet',
      'MedallionGet',
    ]
  })
  // #endregion

  /**
   * Default music group lists in the case that
   * a music file requires defaults to prevent it
   * from just being skipped.
   */
  // #region Default Lists
  static readonly DefaultBgmGroups: readonly MusicGroup[] = [
    MusicGroup.Fields,
    MusicGroup.Towns,
    MusicGroup.Dungeons,
    MusicGroup.Outdoors,
    MusicGroup.Indoors,
    MusicGroup.Minigames,
    MusicGroup.ActionThemes,
    MusicGroup.CalmThemes,
    MusicGroup.Fights,
  ] as const;

  static readonly DefaultFanfareGroups: readonly MusicGroup[] = [
    MusicGroup.ItemFanfares,
    MusicGroup.EventFanfares,
    MusicGroup.ClearFanfares,
  ] as const;
  // #endregion

  // #region All Music Group Lists
  static readonly AllGroups: readonly MusicGroup[] = [
    MusicGroup.Fields,
    MusicGroup.Towns,
    MusicGroup.Dungeons,
    MusicGroup.Outdoors,
    MusicGroup.Indoors,
    MusicGroup.Minigames,
    MusicGroup.ActionThemes,
    MusicGroup.CalmThemes,
    MusicGroup.Fights,
    MusicGroup.ItemFanfares,
    MusicGroup.EventFanfares,
    MusicGroup.ClearFanfares,
  ] as const;

  static readonly AllBgmGroups: readonly MusicGroup[] = [
    MusicGroup.Fields,
    MusicGroup.Towns,
    MusicGroup.Dungeons,
    MusicGroup.Outdoors,
    MusicGroup.Indoors,
    MusicGroup.Minigames,
    MusicGroup.ActionThemes,
    MusicGroup.CalmThemes,
    MusicGroup.Fights,
  ] as const;

  static readonly AllFanfareGroups: readonly MusicGroup[] = [
    MusicGroup.ItemFanfares,
    MusicGroup.EventFanfares,
    MusicGroup.ClearFanfares,
  ] as const;
  // #endregion
}
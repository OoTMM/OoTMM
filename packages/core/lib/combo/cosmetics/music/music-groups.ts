/**
 * Music Groups
 *
 * This file contains the smart enum that handles
 * music groups for categorizing music.
 *
 * This is based off of my smart enum for MMR's "new system":
 * https://github.com/crinuleiroz/mm-rando/blob/a0d2618448ff7aee1aeb9d022959cb233d767317/MMR.Randomizer/Audio/Enums/MusicGroup.cs
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
 * All MMR categories are assumed to be hex, even without the prefix
 * Returns a decimal int string!
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
 * IMPORTANT:
 * Remove the numbered entries in aliases if MMR updates
 * to the new system the number IDs will be made
 * inaccurate and deprecated for string-based music groups!
 *
 * NOTE:
 * Only canon music groups are supported, for MMR I decided to
 * add support for "proper" naming because end-users who
 * want to edit it may use proper naming, etc. Do not add
 * support for non-canon music groups!
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
   * Just typing monitor as any to avoid required import
   * could add the import, but bleh...
   * It could be part of music-injector, but it is tightly
   * coupled with music groups
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
      // MMR LEGACY
      '0',
      '258',
      '268',
      '272',
      '273',
      '275',
      '318',
      // SHARED
      'Fields',
      // OOTR
      'Overworld',
      'HyruleField',
      'LostWoods',
      'GerudoValley',
      // MMR
      'TerminaField',
      'SouthernSwamp',
      'GreatBay',
      'IkanaCanyon',
      'Snowhead',
      'WoodsOfMystery',
      'MysteryWoods',
    ]
  });

  static readonly Towns = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // MMR LEGACY
      '1',
      '274',
      '277',
      '278',
      '279',
      '303',
      '304',
      '310',
      // OOTR
      'Town',
      'Market',
      'KakarikoChild',
      'KakarikoAdult',
      'KokiriForest',
      'LonLonRanch',
      'GoronCity',
      'ZorasDomain',
      // MMR
      'Towns',
      'ClockTown1',
      'ClockTown2',
      'ClockTown3',
      'DekuPalace',
      'RomaniRanch',
      'GoronShrine',
      'ZoraHall',
    ]
  })

  static readonly Dungeons = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // MMR LEGACY
      '2',
      '262',
      '263',
      '276',
      '284',
      '315',
      '357',
      '358',
      '367',
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
      // MMR
      'Dungeons',
      'SecretGrotto',
      'WoodfallTemple',
      'SnowheadTemple',
      'PiratesFortress',
      'GreatBayTemple',
      'IkanaCastle',
      'StoneTower',
      'InvertedStoneTower',
    ]
  })

  static readonly Outdoors = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // SHARED
      'Outdoors' // Not in MMR 1.16 and below
    ]
  })

  static readonly Indoors = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // MMR LEGACY
      '3',
      '261',
      '287',
      '300',
      '305',
      '314',
      '316',
      '324',
      '326',
      '336',
      '364',
      '365',
      '366',
      // SHARED
      'Indoors',
      'House',
      // OOTR
      'ItemShop',
      'SalesArea',
      'Shop',
      'ShootingGallery',
      'WindmillHut',
      'PotionShop',
      'TempleOfTime',
      'ChamberofTheSages',
      // MMR
      'ClockTower',
      'CuriosityShop',
      'MarineResearchLab',
      'MayorsOffice',
      'AstralObservatory',
      'MilkBar',
      'MinigameShop',
      'SwordSchool',
      'JapasRoom',
      'TijosRoom',
      'EvansRoom',
    ]
  })

  static readonly Minigames = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // MMR LEGACY
      '4',
      '270',
      '293',
      '294',
      '320',
      // SHARED
      'HorseRace',
      // OOTR
      'Fun',
      'CastleCourtyard',
      'Mini-game',
      // MMR
      'Minigames',
      'Mini-games',
      'BoatCruise',
      'MinigameTheme',
      'GoronRace',
    ]
  })

  static readonly ActionThemes = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // MMR LEGACY
      '5',
      '259',
      '269',
      '271',
      '379',
      // OOTR
      'HeroTheme',
      'VillainTheme',
      'CastleEscape',
      // MMR
      'ActionThemes',
      'PursuitTheme',
      'AliensTheme',
      'SharpsCurse',
      'MoonEnraged',
    ]
  })

  static readonly CalmThemes = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // MMR LEGACY
      '6',
      '260',
      '267',
      '280',
      '295',
      '296',
      '297',
      '298',
      '301',
      '302',
      '322',
      '323',
      '325',
      '343',
      '369',
      '370',
      '371',
      '381',
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
      // MMR
      'CalmThemes',
      'FileSelect',
      'MajorasTheme',
      'HealingTheme',
      'MusicBoxHouse',
      'GreatFairysFountain',
      'FairysFountain',
      'ZeldasTheme',
      'RosaSistersTheme',
      'GiantsTheme',
      'GuruGurusTheme',
      'GormanBrosTheme',
      'WitchesTheme',
      'KoumeAndKotakesTheme',
      'OwlsTheme',
      'KaeporaGaeborasTheme',
      'FinalHours',
      'KamarosTheme',
      'CremiasTheme',
      'KeatonsTheme',
      'ReunionTheme',
    ]
  })

  static readonly Fights = new MusicGroup({
    type: SongType.Bgm,
    aliases: [
      // MMR LEGACY
      '7',
      '282',
      '283',
      '312',
      '361',
      '362',
      '363',
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
      // MMR
      'Fights',
      'SmallEnemy',
      'BigEnemy',
      'BossEnemy',
      'BossBattle',
      'MajorasWrath',
      'MajorasIncarnation',
      'MajorasMask',
    ]
  })
  // #endregion

  // #region Group Fanfares
  static readonly ItemFanfares = new MusicGroup({
    type: SongType.Fanfare,
    aliases: [
      // MMR LEGACY
      '8',
      '290',
      '292',
      '299',
      '311',
      '313',
      '338',
      // SHARED
      'ItemGet',
      'HeartContainerGet',
      'HeartPieceGet',
      'SongGet',
      // OOTR
      'SongFanfare',
      'ItemFanfare',
      'UtilitySong',
      'BigFanfare',
      'TreasureChest',
      // MMR
      'ItemFanfares',
      'OpenChest',
      'MaskGet',
    ]
  })

  static readonly EventFanfares = new MusicGroup({
    type: SongType.Fanfare,
    aliases: [
      // MMR LEGACY
      '9',
      '264',
      '265',
      '281',
      '288',
      '317',
      '341',
      '375',
      '380',
      // SHARED
      'GameOver',
      // OOTR
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
      // MMR
      'EventFanfares',
      'EventFail1',
      'EventFail2',
      'EventSuccess',
      'TruthRevealed',
      'SoaringTheme',
      'TempleAppears',
      'GiantsLeave',
    ]
  })

  static readonly ClearFanfares = new MusicGroup({
    type: SongType.Fanfare,
    aliases: [
      // MMR LEGACY
      '16',
      '289',
      '319',
      '321',
      '376',
      '377',
      '382',
      // SHARED
      'BossDefeated',
      // OOTR
      'SuccessFanfare',
      'EponaGoal',
      'SpiritStoneGet',
      'MedallionGet',
      // MMR
      'ClearFanfares',
      'GoronRaceWin',
      'HorseRaceWin',
      'TempleClearShort',
      'TempleClearLong',
      'MoonDestroyed',
    ]
  })
  // #endregion

  // #region Group Cutscenes
  public static readonly Cutscenes = new MusicGroup({
    type: SongType.Cutscene,
    aliases: [
      'Cutscenes',
      'GiantsAppear',
      'TitleDemo',
      'TitleTheme',
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
    MusicGroup.Cutscenes,
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
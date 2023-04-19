export const SETTINGS = [{
  key: 'goal',
  name: 'Goal',
  category: 'main',
  type: 'enum',
  values: [
    { value: 'any', name: 'Any Final Boss' },
    { value: 'ganon', name: 'Ganon' },
    { value: 'majora', name: 'Majora' },
    { value: 'both', name: 'Ganon & Majora' },
  ],
  default: 'both'
}, {
  key: 'logic',
  name: 'Logic',
  category: 'main',
  type: 'enum',
  values: [
    { value: 'allLocations', name: 'All Locations' },
    { value: 'beatable', name: 'Beatable Only' },
    { value: 'none', name: 'No Logic' },
  ],
  default: 'allLocations'
}, {
  key: 'generateSpoilerLog',
  name: 'Generate Spoiler Log',
  category: 'main',
  type: 'boolean',
  default: true
}, {
  key: 'songs',
  name: 'Song Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'songLocations', name: 'Song Locations' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'songLocations'
}, {
  key: 'goldSkulltulaTokens',
  name: 'Gold Skulltula Tokens Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'none', name: 'No Shuffle' },
    { value: 'dungeons', name: 'Dungeons Only' },
    { value: 'overworld', name: 'Overworld Only' },
    { value: 'all', name: 'All Tokens' },
  ],
  default: 'none'
}, {
  key: 'housesSkulltulaTokens',
  name: 'Houses Skulltula Tokens Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'none', name: 'No Shuffle' },
    { value: 'cross', name: 'Gold Skulltulas Only' },
    { value: 'all', name: 'All Tokens' },
  ],
  default: 'none'
}, {
  key: 'tingleShuffle',
  name: 'Tingle Maps Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'vanilla', name: 'Vanilla' },
    { value: 'anywhere', name: 'Anywhere' },
    { value: 'starting', name: 'Starting Items' },
    { value: 'removed', name: 'Removed' },
  ],
  default: 'vanilla'
}, {
  key: 'mapCompassShuffle',
  name: 'Map / Compass Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon' },
    { value: 'anywhere', name: 'Anywhere' },
    { value: 'starting', name: 'Starting Items' },
    { value: 'removed', name: 'Removed' },
  ],
  default: 'ownDungeon'
}, {
  key: 'smallKeyShuffle',
  name: 'Small Key Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'ownDungeon'
}, {
  key: 'smallKeyShuffleHideout',
  name: 'Hideout Small Key Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'ownDungeon'
}, {
  key: 'bossKeyShuffle',
  name: 'Boss Key Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'ownDungeon'
}, {
  key: 'townFairyShuffle',
  name: 'Town Stray Fairy Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'vanilla', name: 'Vanilla' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'vanilla'
}, {
  key: 'strayFairyShuffle',
  name: 'Dungeon Stray Fairy Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'vanilla', name: 'Vanilla' },
    { value: 'mixed', name: 'Chests Own Dungeon' },
    { value: 'ownDungeon', name: 'Own Dungeon' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'mixed'
}, {
  key: 'ganonBossKey',
  name: 'Ganon Boss Key Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'removed', name: 'Removed' },
    { value: 'vanilla', name: 'Vanilla' },
    { value: 'ganon', name: 'Ganon\'s Castle' },
    { value: 'anywhere', name: 'Anywhere' },
    { value: 'custom', name: 'Custom' },
  ],
  default: 'removed'
}, {
  key: 'dungeonRewardShuffle',
  name: 'Dungeon Reward Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'dungeonBlueWarps', name: 'Dungeon Blue Warps' },
    { value: 'dungeonsLimited', name: 'Dungeons (Max one per dungeon)' },
    { value: 'dungeons', name: 'Dungeons (Unrestricted)' },
    { value: 'anywhere', name: 'Anywhere' },
  ],
  default: 'dungeonBlueWarps'
}, {
  key: 'scrubShuffleOot',
  name: 'Scrub Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'boolean',
  default: false
}, {
  key: 'cowShuffleOot',
  name: 'Cow Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'boolean',
  default: false
}, {
  key: 'cowShuffleMm',
  name: 'Cow Shuffle (MM)',
  category: 'main.shuffle',
  type: 'boolean',
  default: false
}, {
  key: 'shopShuffleOot',
  name: 'Shop Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'none', name: 'None' },
    { value: 'full', name: 'Full' },
  ],
  default: 'none'
}, {
  key: 'shopShuffleMm',
  name: 'Shop Shuffle (MM)',
  category: 'main.shuffle',
  type: 'enum',
  values: [
    { value: 'none', name: 'None' },
    { value: 'full', name: 'Full' },
  ],
  default: 'none'
}, {
  key: 'shuffleMasterSword',
  name: 'Master Sword Shuffle',
  category: 'main.shuffle',
  type: 'boolean',
  default: true
}, {
  key: 'shuffleGerudoCard',
  name: 'Gerudo Card Shuffle',
  category: 'main.shuffle',
  type: 'boolean',
  default: true
}, {
  key: 'eggShuffle',
  name: 'Weird / Pocket Egg Content Shuffle',
  category: 'main.shuffle',
  type: 'boolean',
  default: false
}, {
  key: 'doorOfTime',
  name: 'Door of Time',
  category: 'main.events',
  type: 'enum',
  values: [
    { value: 'closed', name: 'Closed' },
    { value: 'open', name: 'Open' },
  ],
  default: 'closed'
}, {
  key: 'kakarikoGate',
  name: 'Kakariko Gate',
  category: 'main.events',
  type: 'enum',
  values: [
    { value: 'closed', name: 'Closed' },
    { value: 'open', name: 'Open' },
  ],
  default: 'closed'
}, {
  key: 'zoraKing',
  name: 'Zora King',
  category: 'main.events',
  type: 'enum',
  values: [
    { value: 'vanilla', name: 'Vanilla' },
    { value: 'adult', name: 'Open (Adult Only)' },
    { value: 'open', name: 'Open' },
  ],
  default: 'vanilla'
}, {
  key: 'skipZelda',
  name: 'Skip Child Zelda',
  category: 'main.events',
  type: 'boolean',
  default: false,
}, {
  key: 'lacs',
  name: 'Light Arrow Cutscene',
  category: 'main.events',
  type: 'enum',
  values: [
    { value: 'vanilla', name: 'Vanilla' },
    { value: 'custom', name: 'Custom' },
  ],
  default: 'vanilla'
}, {
  key: 'majoraChild',
  name: 'Majora Child Requirements',
  category: 'main.events',
  type: 'enum',
  values: [
    { value: 'none', name: 'None' },
    { value: 'custom', name: 'Custom' },
  ],
  default: 'none'
}, {
  key: 'crossWarpOot',
  name: 'Cross-Games OoT Warp Songs',
  category: 'main.cross',
  type: 'boolean',
  default: false
}, {
  key: 'crossWarpMm',
  name: 'Cross-Games MM Song of Soaring',
  category: 'main.cross',
  type: 'enum',
  values: [
    { value: 'none', name: 'None' },
    { value: 'childOnly', name: 'Child Only' },
    { value: 'full', name: 'Child & Adult' },
  ],
  default: 'none'
}, {
  key: 'sunSongMm',
  name: 'Sun\'s Song in MM',
  category: 'main.cross',
  type: 'boolean',
  default: false
}, {
  key: 'csmc',
  name: 'Chest Size Matches Content',
  category: 'main.misc',
  type: 'boolean',
  default: false
}, {
  key: 'blastMaskCooldown',
  name: 'Blast Mask Cooldown',
  category: 'main.misc',
  type: 'enum',
  values: [
    { value: 'instant', name: 'Instant (0s)' },
    { value: 'veryshort', name: 'Very Short (~2s)' },
    { value: 'short', name: 'Short (~6s)' },
    { value: 'default', name: 'Default (~15s)' },
    { value: 'long', name: 'Long (~25s)' },
    { value: 'verylong', name: 'Very Long (~51s)' },
  ],
  default: 'default'
}, {
  key: 'fillWallets',
  name: 'Fill Wallets',
  category: 'items',
  type: 'boolean',
  default: false
}, {
  key: 'progressiveShieldsOot',
  name: 'OoT Shields',
  category: 'items.progressive',
  type: 'enum',
  values: [
    { value: 'separate', name: 'Separate' },
    { value: 'progressive', name: 'Progressive' },
  ],
  default: 'separate'
}, {
  key: 'progressiveSwordsOot',
  name: 'OoT Swords',
  category: 'items.progressive',
  type: 'enum',
  values: [
    { value: 'separate', name: 'Separate' },
    { value: 'goron', name: 'Progressive Knife and Biggoron' },
    { value: 'progressive', name: 'Progressive' },
  ],
  default: 'goron'
}, {
  key: 'progressiveShieldsMm',
  name: 'MM Shields',
  category: 'items.progressive',
  type: 'enum',
  values: [
    { value: 'separate', name: 'Separate' },
    { value: 'progressive', name: 'Progressive' },
  ],
  default: 'start'
}, {
  key: 'progressiveGoronLullaby',
  name: 'MM Goron Lullaby',
  category: 'items.progressive',
  type: 'enum',
  values: [
    { value: 'single', name: 'Full Lullaby Only' },
    { value: 'progressive', name: 'Progressive' },
  ],
  default: 'progressive'
}, {
  key: 'fairyOcarinaMm',
  name: 'Fairy Ocarina in MM',
  category: 'items.extensions',
  type: 'boolean',
  default: false
}, {
  key: 'shortHookshotMm',
  name: 'Short Hookshot in MM',
  category: 'items.extensions',
  type: 'boolean',
  default: false
}, {
  key: 'childWallets',
  name: 'Child Wallets',
  category: 'items.extensions',
  type: 'boolean',
  default: false
}, {
  key: 'colossalWallets',
  name: 'Colossal Wallets',
  category: 'items.extensions',
  type: 'boolean',
  default: false
}, {
  key: 'sharedNutsSticks',
  name: 'Shared Nuts & Sticks',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedBows',
  name: 'Shared Bows',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedBombBags',
  name: 'Shared Bomb Bags',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMagic',
  name: 'Shared Magic',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMagicArrowFire',
  name: 'Shared Fire Arrow',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMagicArrowIce',
  name: 'Shared Ice Arrow',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMagicArrowLight',
  name: 'Shared Light Arrow',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedSongEpona',
  name: 'Shared Epona\'s Song',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedSongStorms',
  name: 'Shared Song of Storms',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedSongTime',
  name: 'Shared Song of Time',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedSongSun',
  name: 'Shared Sun\'s Song',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedHookshot',
  name: 'Shared Hookshots',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedLens',
  name: 'Shared Lens of Truth',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedOcarina',
  name: 'Shared Ocarina of Time',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMasks',
  name: 'Shared Masks',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedWallets',
  name: 'Shared Wallets',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedHealth',
  name: 'Shared Health',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'probabilisticFoolish',
  name: 'Probabilistic Foolish Hints (If you don\'t know what this is, leave it ON)',
  category: 'hints',
  type: 'boolean',
  default: true
}, {
  key: 'erBoss',
  name: 'Boss Entrance Shuffle',
  category: 'entrances',
  type: 'enum',
  values: [
    { value: 'none', name: 'None' },
    { value: 'ownGame', name: 'Own Game' },
    { value: 'full', name: 'Full' },
  ],
  default: 'none'
}, {
  key: 'erDungeons',
  name: 'Dungeon Entrance Shuffle',
  category: 'entrances',
  type: 'enum',
  values: [
    { value: 'none', name: 'None' },
    { value: 'ownGame', name: 'Own Game' },
    { value: 'full', name: 'Full' },
  ],
  default: 'none'
}, {
  key: 'erMinorDungeons',
  name: 'Shuffle OoT Minor Dungeons with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false
}, {
  key: 'erSpiderHouses',
  name: 'Shuffle Spider Houses with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false
}, {
  key: 'erPirateFortress',
  name: 'Shuffle Pirate Fortress with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false
}, {
  key: 'erBeneathWell',
  name: 'Shuffle Beneath the Well with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false
}, {
  key: 'erIkanaCastle',
  name: 'Shuffle Ikana Castle Interior with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false
}, {
  key: 'erSecretShrine',
  name: 'Shuffle Secret Shrine with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false
}, {
  key: 'erOverworld',
  name: 'Shuffle Major Regions',
  category: 'entrances',
  type: 'boolean',
  default: false
}] as const;

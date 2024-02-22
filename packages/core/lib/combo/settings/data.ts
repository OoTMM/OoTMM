const SETTING_PRICE = {
  type: 'enum',
  values: [
    { value: 'affordable', name: 'Affordable', description: 'All prices are set to 10 rupees.' },
    { value: 'vanilla', name: 'Vanilla', description: 'All prices are set to their vanilla values.' },
    { value: 'weighted', name: 'Weighted Random', description: 'All prices are randomized, but lower prices are favored.' },
    { value: 'random', name: 'Random', description: 'All prices are randomized.' },
  ],
  category: 'main.prices',
  default: 'vanilla',
} as const;

export const SETTINGS = [{
  key: 'mode',
  name: 'Mode',
  category: 'main',
  type: 'enum',
  description: 'The game mode.',
  values: [
    { value: 'single', name: 'Singleplayer', description: 'A regular, one player seed' },
    { value: 'coop', name: 'Co-op', description: 'Everyone plays the same seed, all items are shared' },
    { value: 'multi', name: 'Multiworld', description: 'A multiplayer setting where players can find each other\'s items' },
  ],
  default: 'single'
}, {
  key: 'players',
  name: 'Players',
  category: 'main',
  type: 'number',
  description: 'For multiworld, how many players are in the game.',
  min: (x: any) => x.mode === 'multi' ? 2 : 1,
  max: (x: any) => x.mode === 'multi' ? 127 : 1,
  cond: (x: any) => x.mode === 'multi',
  default: 1
}, {
  key: 'distinctWorlds',
  name: 'Distinct Worlds',
  category: 'main',
  type: 'boolean',
  description: 'For multiworld, with settings that involve randomness, should the worlds have distinct properties (ER, MQ, price rando...).',
  cond: (x: any) => x.mode === 'multi',
  default: false
}, {
  key: 'goal',
  name: 'Goal',
  category: 'main',
  type: 'enum',
  description: 'The objective of the seed. The game will end when the specified goal is reached.',
  values: [
    { value: 'any', name: 'Any Final Boss', description: 'You can beat either Ganon or Majora.' },
    { value: 'ganon', name: 'Ganon', description: 'You must beat Ganon.' },
    { value: 'majora', name: 'Majora', description: 'You must beat Majora.' },
    { value: 'both', name: 'Ganon & Majora', description: 'You must beat Ganon AND Majora. You can do so in any order.' },
    { value: 'triforce', name: 'Triforce Hunt', description: 'You must collect Triforce Pieces to win.' },
    { value: 'triforce3', name: 'Triforce Quest', description: 'You must collect the three parts of the Triforce (Power, Courage and Wisdom) to win. Specific hints will guide you.' },
  ],
  default: 'both'
}, {
  key: 'triforceGoal',
  name: 'Triforce Goal',
  category: 'main',
  type: 'number',
  description: 'The amount of Triforce Pieces that are required to win.',
  default: 20,
  cond: (s: any) => s.goal === 'triforce',
  min: 1,
  max: (s: any) => s.triforcePieces,
}, {
  key: 'triforcePieces',
  name: 'Triforce Pieces',
  category: 'main',
  type: 'number',
  description: 'The total amount of Triforce Pieces in the item pool.',
  default: 30,
  cond: (s: any) => s.goal === 'triforce',
  min: 1,
  max: 999,
}, {
  key: 'logic',
  name: 'Logic',
  category: 'main',
  type: 'enum',
  description: 'The guarantees you have regarding reachability of locations.',
  values: [
    { value: 'allLocations', name: 'All Locations', description: 'Every location in the game can be reached. The seed can always be completed.' },
    { value: 'beatable', name: 'Beatable Only', description: 'Some items can be placed in locations that are never reacheable. The seed can always be completed.' },
    { value: 'none', name: 'No Logic', description: 'Items are placed completely at random. The seed might be unbeatable.' },
  ],
  default: 'allLocations'
}, {
  key: 'itemPool',
  name: 'Item Pool',
  category: 'main',
  type: 'enum',
  description: 'Change the item pool.',
  values: [
    { value: 'plentiful', name: 'Plentiful', description: 'One extra copy of every major item. Heart containers only.' },
    { value: 'normal', name: 'Normal', description: 'The regular item count for each game.' },
    { value: 'scarce', name: 'Scarce', description: 'One less of every major item. No Heart Pieces.' },
    { value: 'minimal', name: 'Minimal', description: 'Only one of each major item. No Heart Pieces or Containers.' },
    { value: 'barren', name: 'Barren', description: 'Minimal item pool, plus every shuffled item that is not strictly required to reach the goal (beatable only) or any location (all locations) gets removed.' },
  ],
  default: 'normal'
}, {
  key: 'generateSpoilerLog',
  name: 'Generate Spoiler Log',
  category: 'main',
  type: 'boolean',
  description: 'Generate an extra downloadable text file with the location of every item in the seed as well as other information. It is recommended to enable this option.',
  default: true
}, {
  key: 'probabilisticFoolish',
  name: 'Probabilistic Foolish Hints',
  category: 'main',
  type: 'boolean',
  description: 'If you don\'t know what this is, leave it ON',
  default: true
}, {
  key: 'noPlandoHints',
  name: 'No Plando Hints',
  category: 'main',
  type: 'boolean',
  description: 'Prevents items that are part of a plando from being hinted',
  default: true
}, {
  key: 'hintImportance',
  name: 'Hint Importance',
  category: 'main',
  type: 'boolean',
  description: 'Hints will tell if an item is foolish, sometimes required, or always required',
  default: false
}, {
  key: 'songs',
  name: 'Song Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where songs can be obtained, in both games.',
  values: [
    { value: 'songLocations', name: 'Song Locations', description: 'Only locations that contain songs in the vanilla games will have songs.' },
    { value: 'anywhere', name: 'Anywhere', description: 'Songs can be placed anywhere.' },
  ],
  default: 'songLocations'
}, {
  key: 'goldSkulltulaTokens',
  name: 'Gold Skulltula Tokens Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls how Gold Skulltulas will be shuffled',
  values: [
    { value: 'none', name: 'No Shuffle', description: 'All Gold Skulltula Tokens will be vanilla' },
    { value: 'dungeons', name: 'Dungeons Only', description: 'Only the Gold Skulltulas within dungeons will be shuffled' },
    { value: 'overworld', name: 'Overworld Only', description: 'Only the Gold Skulltulas outside of dungeons will be shuffled' },
    { value: 'all', name: 'All Tokens', description: 'Every single Gold Skulltula will be shuffled' },
  ],
  default: 'none'
}, {
  key: 'housesSkulltulaTokens',
  name: 'House Skulltula Tokens Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls how Swamp and Ocean Skulltulas will be shuffled',
  values: [
    { value: 'none', name: 'No Shuffle', description: 'Swamp and Ocean Tokens will be vanilla' },
    { value: 'cross', name: 'Gold Skulltulas Only', description: 'Any unshuffled Token can be found on any other unshuffled Skulltula' },
    { value: 'all', name: 'All Tokens', description: 'Swamp and Ocean Tokens can be found anywhere' },
  ],
  default: 'none'
}, {
  key: 'tingleShuffle',
  name: 'Tingle Maps Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where Tingle Maps are',
  values: [
    { value: 'vanilla', name: 'Vanilla', description: 'Tingle Maps can be bought at Tingle, in their original locations' },
    { value: 'anywhere', name: 'Anywhere', description: 'Tingle Maps will be anywhere' },
    { value: 'starting', name: 'Starting Items', description: 'Tingle Maps are in Link\'s pocket' },
    { value: 'removed', name: 'Removed', description: 'Tingle Maps are fully removed and cannot be obtained' },
  ],
  default: 'vanilla'
}, {
  key: 'mapCompassShuffle',
  name: 'Map / Compass Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where Maps and Compasses can be',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon', description: ' Maps and Compasses will be in their own dungeons' },
    { value: 'anywhere', name: 'Anywhere', description: ' Maps and Compasses can be on any location' },
    { value: 'starting', name: 'Starting Items', description: ' Maps and Compasses will be in Link\'s Pocket' },
    { value: 'removed', name: 'Removed', description: 'Fully removed and cannot be obtained' },
  ],
  default: 'ownDungeon'
}, {
  key: 'smallKeyShuffleOot',
  name: 'Small Key Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where Small Keys (for Dungeons) can be in OoT',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon', description: 'Dungeon Small Keys can only be found in their own dungeons' },
    { value: 'anywhere', name: 'Anywhere', description: 'Dungeon Small Keys can be found anywhere' },
    { value: 'removed', name: 'Removed', description: 'Small keys are removed and small key doors are unlocked' },
  ],
  default: 'ownDungeon'
}, {
  key: 'smallKeyShuffleMm',
  name: 'Small Key Shuffle (MM)',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where Small Keys (for Dungeons) can be in MM',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon', description: 'Dungeon Small Keys can only be found in their own dungeons' },
    { value: 'anywhere', name: 'Anywhere', description: 'Dungeon Small Keys can be found anywhere' },
    { value: 'removed', name: 'Removed', description: 'Small keys are removed and small key doors are unlocked' },
  ],
  default: 'ownDungeon'
}, {
  key: 'smallKeyShuffleHideout',
  name: 'Hideout Small Key Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where Hideout (Gerudo Fortress) Small Keys can be',
  values: [
    { value: 'vanilla', name: 'Vanilla', description: 'Hideout Small Keys are always on the guards' },
    { value: 'ownDungeon', name: 'Own Dungeon', description: 'Hideout Small Keys can only be found within Gerudo Fortress INTERIOR' },
    { value: 'anywhere', name: 'Anywhere', description: 'Hideout Small Keys can be found anywhere' },
  ],
  default: 'ownDungeon'
}, {
  key: 'smallKeyShuffleChestGame',
  name: 'Chest Game Small Key Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where Chest Game Small Keys can be',
  values: [
    { value: 'vanilla', name: 'Vanilla', description: 'Chest Minigame will behave as in vanilla' },
    { value: 'ownDungeon', name: 'Own Minigame', description: 'Chest Minigame Keys can be found inside the minigame' },
    { value: 'anywhere', name: 'Anywhere', description: 'Chest Minigame Keys can be found anywhere' },
  ],
  default: 'vanilla'
}, {
  key: 'bossKeyShuffleOot',
  name: 'Boss Key Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where Boss Keys can be in OoT',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon', description: 'Boss Keys can only be in their own dungeons' },
    { value: 'anywhere', name: 'Anywhere', description: 'Boss Keys can be found anywhere' },
    { value: 'removed', name: 'Removed', description: 'Boss Keys are removed and boss doors are unlocked' },
  ],
  default: 'ownDungeon'
}, {
  key: 'bossKeyShuffleMm',
  name: 'Boss Key Shuffle (MM)',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where Boss Keys can be in MM',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon', description: 'Boss Keys can only be in their own dungeons' },
    { value: 'anywhere', name: 'Anywhere', description: 'Boss Keys can be found anywhere' },
    { value: 'removed', name: 'Removed', description: 'Boss Keys are removed and boss doors are unlocked' },
  ],
  default: 'ownDungeon'
}, {
  key: 'smallKeyRingOot',
  name: 'Small Key Ring (OoT)',
  category: 'main.shuffle',
  type: 'set',
  description: 'Controls the grouping of keys into key rings, for OoT',
  values: [
    { value: 'Forest', name: 'Forest Temple' },
    { value: 'Fire',   name: 'Fire Temple' },
    { value: 'Water',  name: 'Water Temple' },
    { value: 'Shadow', name: 'Shadow Temple' },
    { value: 'Spirit', name: 'Spirit Temple' },
    { value: 'BotW',   name: 'Bottom of the Well' },
    { value: 'GTG',    name: 'Gerudo Training Grounds' },
    { value: 'Ganon',  name: 'Ganon\'s Castle' },
    { value: 'GF',     name: 'Hideout' },
    { value: 'TCG',    name: 'Chest Game' },
  ],
  default: 'none'
}, {
  key: 'smallKeyRingMm',
  name: 'Small Key Ring (MM)',
  category: 'main.shuffle',
  type: 'set',
  description: 'Controls the grouping of keys into key rings, for MM',
  values: [
    { value: 'WF', name: 'Woodfall Temple' },
    { value: 'SH', name: 'Snowhead Temple' },
    { value: 'GB', name: 'Great Bay Temple' },
    { value: 'ST', name: 'Stone Tower Temple' },
  ],
  default: 'none'
}, {
  key: 'silverRupeeShuffle',
  name: 'Silver Rupee Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Make silver rupees items that can be shuffled.',
  values: [
    { value: 'vanilla', name: 'Vanilla', description: 'Silver Rupees are vanilla' },
    { value: 'ownDungeon', name: 'Own Dungeon', description: 'Silver Rupees are found within their own dungeon' },
    { value: 'anywhere', name: 'Anywhere', description: 'Silver Rupees are shuffled in the item pool' },
  ],
  default: 'vanilla'
}, {
  key: 'silverRupeePouches',
  name: 'Silver Rupee Pouches',
  category: 'main.shuffle',
  type: 'set',
  description: 'Controls grouping of silver rupees into a single item.',
  values: [
    { value: 'DC', name: 'Dodongo\'s Cavern' },
    { value: 'BotW', name: 'Bottom of the Well' },
    { value: 'Spirit_Child', name: 'Spirit Temple (Child)' },
    { value: 'Spirit_Sun', name: 'Spirit Temple (Sun)' },
    { value: 'Spirit_Boulders', name: 'Spirit Temple (Boulders)' },
    { value: 'Spirit_Lobby', name: 'Spirit Temple (Lobby)' },
    { value: 'Spirit_Adult', name: 'Spirit Temple (Adult)' },
    { value: 'Shadow_Scythe', name: 'Shadow Temple (Scythe)' },
    { value: 'Shadow_Pit', name: 'Shadow Temple (Pit)' },
    { value: 'Shadow_Spikes', name: 'Shadow Temple (Spikes)' },
    { value: 'Shadow_Blades', name: 'Shadow Temple (Blades)' },
    { value: 'IC_Scythe', name: 'Ice Cavern (Scythe)' },
    { value: 'IC_Block', name: 'Ice Cavern (Block)' },
    { value: 'GTG_Slopes', name: 'GTG (Slopes)' },
    { value: 'GTG_Lava', name: 'GTG (Lava)' },
    { value: 'GTG_Water', name: 'GTG (Water)' },
    { value: 'Ganon_Light', name: 'Ganon (Light)' },
    { value: 'Ganon_Forest', name: 'Ganon (Forest)' },
    { value: 'Ganon_Fire', name: 'Ganon (Fire)' },
    { value: 'Ganon_Water', name: 'Ganon (Water)' },
    { value: 'Ganon_Shadow', name: 'Ganon (Shadow)' },
    { value: 'Ganon_Spirit', name: 'Ganon (Spirit)' },
  ],
  default: 'none',
  cond: (s: any) => s.silverRupeeShuffle !== 'vanilla',
}, {
  key: 'townFairyShuffle',
  name: 'Town Stray Fairy Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where the Clock Town Stray Fairy can be',
  values: [
    { value: 'vanilla', name: 'Vanilla', description: 'The Clock Town Stray Fairy will be at its original location'},
    { value: 'anywhere', name: 'Anywhere', description: 'The Clock Town Stray Fairy can be found anywhere' },
  ],
  default: 'vanilla'
}, {
  key: 'strayFairyChestShuffle',
  name: 'Dungeon Chest Fairy Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where the Dungeon Chest Stray Fairies can be',
  values: [
    { value: 'starting', name: 'Starting', description: 'Start with the fairies' },
    { value: 'vanilla', name: 'Vanilla', description: 'The Dungeon Chest Stray Fairies will be at their original locations' },
    { value: 'ownDungeon', name: 'Own Dungeon', description: 'All Dungeon Chest Stray Fairies are shuffled within their own dungeon' },
    { value: 'anywhere', name: 'Anywhere', description: 'All Dungeon Chest Stray Fairies are shuffled anywhere' },
  ],
  default: 'ownDungeon'
}, {
  key: 'strayFairyOtherShuffle',
  name: 'Dungeon Freestanding Fairy Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where the Dungeon Freestanding Stray Fairies can be',
  values: [
    { value: 'removed', name: 'Removed', description: 'Start with the fairies, and the bubbles are removed.' },
    { value: 'starting', name: 'Starting', description: 'Start with the fairies' },
    { value: 'vanilla', name: 'Vanilla', description: 'The Dungeon Freestanding Stray Fairies will be at their original locations' },
    { value: 'ownDungeon', name: 'Own Dungeon', description: 'All Dungeon Freestanding Stray Fairies are shuffled within their own dungeon' },
    { value: 'anywhere', name: 'Anywhere', description: 'All Dungeon Freestanding Stray Fairies are shuffled anywhere' },
  ],
  default: 'vanilla'
}, {
  key: 'ganonBossKey',
  name: 'Ganon Boss Key Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where Ganon Boss Key should be',
  values: [
    { value: 'removed', name: 'Removed', description: 'Ganon Boss Key is removed, and so is the lock on the door leading to Ganondorf' },
    { value: 'vanilla', name: 'Vanilla', description: 'Ganon Boss Key will be in its original chest, within Ganon\'s Castle' },
    { value: 'ganon', name: 'Ganon\'s Castle', description: 'Ganon Boss Key will be anywhere within Ganon\'s Castle' },
    { value: 'anywhere', name: 'Anywhere', description: 'Ganon Boss Key can be found anywhere' },
    { value: 'custom', name: 'Custom', description: '' },
  ],
  default: 'removed',
  cond: (s: any) => s.goal !== 'triforce' && s.goal !== 'triforce3',
}, {
  key: 'dungeonRewardShuffle',
  name: 'Dungeon Reward Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where the dungeons rewards should be',
  values: [
    { value: 'dungeonBlueWarps', name: 'Dungeon Blue Warps', description: 'Only the blue warps will grant the rewards' },
    { value: 'dungeonsLimited', name: 'Dungeons (Max one per dungeon)', description: 'Anywhere in dungeons (one max)' },
    { value: 'dungeons', name: 'Dungeons (Unrestricted)', description: 'Anywhere in dungeons' },
    { value: 'anywhere', name: 'Anywhere', description: 'Can be anywhere' },
  ],
  default: 'dungeonBlueWarps'
}, {
  key: 'scrubShuffleOot',
  name: 'Scrub Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not Business Scrubs are shuffled (OoT). If not, the one in Hyrule Field by Lake Hylia\'s fences, the one by the Bridge in Lost Woods, and the front one in the grotto near Sacred Forest Meadow will still be shuffled',
  default: false
}, {
  key: 'scrubShuffleMm',
  name: 'Scrub Shuffle (MM)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not Business Scrubs are shuffled (MM). If not, the one in Termina Field near the Observatory and the one in Goron Village will still be shuffled',
  default: false
}, {
  key: 'cowShuffleOot',
  name: 'Cow Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not playing Epona\'s Song near cows will give an item (OOT)',
  default: false
}, {
  key: 'cowShuffleMm',
  name: 'Cow Shuffle (MM)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not playing Epona\'s Song near cows will give an item (MM)',
  default: false
}, {
  key: 'shopShuffleOot',
  name: 'Shop Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls whether or not shops in OOT should have their items shuffled',
  values: [
    { value: 'none', name: 'None', description: 'All the items are vanilla' },
    { value: 'full', name: 'Full', description: 'All 8 items are shuffled' },
  ],
  default: 'none'
}, {
  key: 'shopShuffleMm',
  name: 'Shop Shuffle (MM)',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls whether or not shops in MM should have their items shuffled. If not, the Bomb Bag purchases will still be shuffled',
  values: [
    { value: 'none', name: 'None', description: 'All the items are vanilla' },
    { value: 'full', name: 'Full', description: 'All 8 items are shuffled' },
  ],
  default: 'none'
}, {
  key: 'owlShuffle',
  name: 'Owl Statue Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Make owl statue items that can be shuffled.',
  values: [
    { value: 'none', name: 'None', description: 'Owl statues are vanilla' },
    { value: 'anywhere', name: 'Anywhere', description: 'Owl statues are shuffled in the item pool' },
  ],
  default: 'none'
}, {
  key: 'shufflePotsOot',
  name: 'Pots Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not the pots are shuffled (OoT).',
  default: false
}, {
  key: 'shufflePotsMm',
  name: 'Pots Shuffle (MM)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not the pots are shuffled (MM).',
  default: false
}, {
  key: 'shuffleGrassOot',
  name: 'Grass Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not the grass is shuffled (OoT)',
  default: false
}, {
  key: 'shuffleGrassMm',
  name: 'Grass Shuffle (MM)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not the grass is shuffled (MM)',
  default: false
}, {
  key: 'shuffleFreeRupeesOot',
  name: 'Freestanding Rupees Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not the freestanding rupees are shuffled (OoT)',
  default: false
}, {
  key: 'shuffleFreeRupeesMm',
  name: 'Freestanding Rupees Shuffle (MM)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not the freestanding rupees are shuffled (MM)',
  default: false
}, {
  key: 'shuffleFreeHeartsOot',
  name: 'Freestanding Hearts Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not the freestanding hearts are shuffled (OoT)',
  default: false
}, {
  key: 'shuffleFreeHeartsMm',
  name: 'Freestanding Hearts Shuffle (MM)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not the freestanding hearts are shuffled (MM)',
  default: false
}, {
  key: 'shuffleOcarinasOot',
  name: 'Ocarina Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not the two Ocarinas in OoT are shuffled',
  default: true
}, {
  key: 'shuffleMasterSword',
  name: 'Master Sword Shuffle',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not the Master Sword is shuffled',
  default: true
}, {
  key: 'shuffleGerudoCard',
  name: 'Gerudo Card Shuffle',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not the Gerudo Membership Card is shuffled',
  default: true
}, {
  key: 'shuffleMerchantsMm',
  name: 'Merchants Shuffle (MM)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not the Milk Bar and Gorman milk purchases in MM are shuffled',
  default: false
}, {
  key: 'pondFishShuffle',
  name: 'Fishing Pond Fish Shuffle',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not the Fish (and the Loaches) in the Fishing Pond are shuffled amongst all the items.',
  default: false
}, {
  key: 'divingGameRupeeShuffle',
  name: 'Diving Game Rupee Shuffle',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not the Zora\'s Domain Diving Game has 5 random items instead of green, blue, red, purple and 500 rupees.',
  default: false
}, {
  key: 'fairyFountainFairyShuffleOot',
  name: 'Fairy Fountain Fairy Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not fairies in fairy fountains are shuffled (OoT).',
  default: false
}, {
  key: 'fairyFountainFairyShuffleMm',
  name: 'Fairy Fountain Fairy Shuffle (MM)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not fairies in fairy fountains are shuffled (MM).',
  default: false
}, {
  key: 'fairySpotShuffleOot',
  name: 'Fairy Spot Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not big fairies in fairy spots are shuffled (OoT). Play either Song of Storms or Sun\'s Song',
  default: false
}, {
  key: 'eggShuffle',
  name: 'Weird / Pocket Egg Content Shuffle',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Fun setting: should using the Weird/Pocket Eggs give an item? If not, they\'re entirely removed from the game',
  default: false
},
{ ...SETTING_PRICE, key: 'priceOotShops', name: 'OoT Shops Prices', description: 'Sets the price of items inside OoT shops' },
{ ...SETTING_PRICE, key: 'priceOotScrubs', name: 'OoT Scrubs Prices', description: 'Sets the price of items sold by OoT scrubs' },
{ ...SETTING_PRICE, key: 'priceMmShops', name: 'MM Shops Prices', description: 'Sets the price of items sold inside MM shops' },
{ ...SETTING_PRICE, key: 'priceMmTingle', name: 'MM Tingle Prices', description: 'Sets the price of items sold by Tingle' },
{
  key: 'ganonTrials',
  name: 'Ganon Trials',
  category: 'main.events',
  type: 'set',
  description: 'Controls which trials in Ganon\'s Castle are enabled',
  values: [
    { value: 'Light',   name: 'Light Trial' },
    { value: 'Forest',  name: 'Forest Trial' },
    { value: 'Fire',    name: 'Fire Trial' },
    { value: 'Water',   name: 'Water Trial' },
    { value: 'Shadow',  name: 'Shadow Trial' },
    { value: 'Spirit',  name: 'Spirit Trial' },
  ],
  default: 'none'
}, {
  key: 'moonCrash',
  name: 'Moon Crash Behavior',
  category: 'main.events',
  type: 'enum',
  description: 'Change the behavior of moon crashing',
  values: [
    { value: 'reset', name:  'Reset',  description: 'Moon Crash will restore the last save. No progress will be kept.' },
    { value: 'cycle', name:  'New Cycle',  description: 'Moon Crash will initiate a new cycle, keeping progress.' },
  ],
  default: 'reset'
}, {
  key: 'startingAge',
  name: 'Starting Age',
  category: 'main.events',
  type: 'enum',
  description: 'Choose the starting age',
  values: [
    { value: 'child', name:  'Child',  description: 'Link will start off as child' },
    { value: 'adult', name:  'Adult',  description: 'Link will start off as adult' },
    { value: 'random', name: 'Random', description: 'Link will start off as either adult or child, with a 50/50 probability' },
  ],
  default: 'child'
}, {
  key: 'doorOfTime',
  name: 'Door of Time',
  category: 'main.events',
  type: 'enum',
  description: 'Alters the Door of Time state',
  values: [
    { value: 'closed', name: 'Closed', description: 'The Door will be closed, and you will need to play Song of Time in front of the Temple of Time altar to open it. (The Spiritual Stones and Ocarina of Time are NOT needed)' },
    { value: 'open', name: 'Open', description: 'The Door is already open' },
  ],
  default: 'closed'
}, {
  key: 'ageChange',
  name: 'Age Change upon Song of Time',
  category: 'main.events',
  type: 'enum',
  values: [
    { value: 'none', name: 'None', description: 'Cannot change age by playing Song of Time.' },
    { value: 'oot', name: 'Ocarina of Time', description: 'Can change age by playing Song of Time with the Ocarina of Time specifically.' },
    { value: 'always', name: 'Always', description: 'Can always change age by playing Song of Time.' },
  ],
  description: 'Allows you to switch ages by playing Song of Time, if you\'ve been to Temple of Time as both ages',
  default: 'none',
}, {
  key: 'dekuTree',
  name: 'Deku Tree',
  category: 'main.events',
  type: 'enum',
  description: 'Controls the behavior of Mido blocking the Deku Tree as child',
  values: [
    { value: 'closed', name: 'Closed', description: 'Mido will block the way to the Deku Tree until you have a Deku Shield and the Kokiri Sword.' },
    { value: 'open', name: 'Open', description: 'The Deku Tree will be open from the start' },
  ],
  default: 'open'
}, {
  key: 'dekuTreeAdult',
  name: 'Deku Tree as Adult',
  category: 'main.events',
  type: 'boolean',
  description: 'Allows access to the Deku Tree as adult.',
  default: false
}, {
  key: 'wellAdult',
  name: 'Well as Adult',
  category: 'main.events',
  type: 'boolean',
  description: 'Allows access to the Well as adult.',
  default: false
}, {
  key: 'fireChild',
  name: 'Fire Temple as Child',
  category: 'main.events',
  type: 'boolean',
  description: 'Allows access to the Fire Temple as child.',
  default: false
}, {
  key: 'openDungeonsMm',
  name: 'Open Dungeons (MM)',
  category: 'main.events',
  type: 'set',
  description: 'Controls whether or not MM dungeons will need their respective song. Takes priority over Clear State',
  values: [
    { value: 'WF', name: 'Woodfall Temple' },
    { value: 'SH', name: 'Snowhead Temple' },
    { value: 'GB', name: 'Great Bay Temple' },
    { value: 'ST', name: 'Stone Tower Temples' },
  ],
  default: 'none'
}, {
  key: 'clearStateDungeonsMm',
  name: 'Clear State Dungeons (MM)',
  category: 'main.events',
  type: 'enum',
  description: 'Controls whether or not MM dungeons will need their respectives songs in the cleared state',
  values: [
    { value: 'none', name: 'None' },
    { value: 'WF', name: 'Woodfall Temple' },
    { value: 'GB', name: 'Great Bay Temple' },
    { value: 'both', name: 'Both' },
  ],
  default: 'none'
}, {
  key: 'kakarikoGate',
  name: 'Kakariko Gate',
  category: 'main.events',
  type: 'enum',
  description: 'Controls the behavior of the gate in Kakariko blocking Death Mountain as child',
  values: [
    { value: 'closed', name: 'Closed', description: 'The gate will be closed until you show Zelda\'s Letter to the guard' },
    { value: 'open', name: 'Open', description: 'The gate will be open from the start' },
  ],
  default: 'closed'
}, {
  key: 'openZdShortcut',
  name: 'Open Zora\'s Domain Shortcut',
  category: 'main.events',
  type: 'boolean',
  description: 'Removes the ice blocking Zora\'s Domain in Lake Hylia as adult',
  default: false
}, {
  key: 'zoraKing',
  name: 'King Zora',
  category: 'main.events',
  type: 'enum',
  description: 'Controls the behavior of King Zora in Zora\'s Domain',
  values: [
    { value: 'vanilla', name: 'Vanilla', description: 'You will need to present him Ruto\'s Letter in order to enter Zora\'s Fountain as both Child and Adult' },
    { value: 'adult', name: 'Open (Adult Only)', description: 'Already on the side as Adult, granting free Zora\'s Fountain access. Child still needs Ruto\'s Letter' },
    { value: 'open', name: 'Open', description: 'He will already be on the side for both Child and Adult, and Ruto\'s Letter is replaced by an empty bottle' },
  ],
  default: 'vanilla'
}, {
  key: 'gerudoFortress',
  name: 'Gerudo Fortress',
  category: 'main.events',
  type: 'enum',
  description: 'Controls the behavior of the Carpenters in Gerudo Fortress',
  values: [
    { value: 'vanilla', name: 'Vanilla', description: 'You will need to rescue all carpenters.' },
    { value: 'single', name: 'One Carpenter', description: 'You will need to rescue only one carpenter.' },
    { value: 'open', name: 'Open', description: 'Carpenters are rescued from the start and the bridge in Gerudo Valley as adult is repaired.' },
  ],
  default: 'vanilla'
}, {
  key: 'skipZelda',
  name: 'Skip Child Zelda',
  category: 'main.events',
  type: 'boolean',
  description: 'This changes the beginning of the child trade quest. True means you\'ll start having already met Zelda and got her item along with the one from Impa. The Chicken is also removed from the game, but Malon will still be at Hyrule Castle',
  default: false,
}, {
  key: 'openMoon',
  name: 'Skip Oath to Order',
  category: 'main.events',
  type: 'boolean',
  description: 'Skip playing Oath to Order to reach the Moon.',
  default: false,
}, {
  key: 'lacs',
  name: 'Light Arrow Cutscene',
  category: 'main.events',
  type: 'enum',
  description: 'Controls how the Light Arrow Cutscene should be triggered',
  values: [
    { value: 'vanilla', name: 'Vanilla', description: 'Triggers at Temple of Time with Shadow and Spirit Medallions' },
    { value: 'custom', name: 'Custom', description: 'Triggers at Temple of Time with a special condition'},
  ],
  default: 'vanilla'
}, {
  key: 'rainbowBridge',
  name: 'Rainbow Bridge',
  category: 'main.events',
  type: 'enum',
  description: 'Controls how the Rainbow Bridge should be triggered',
  values: [
    { value: 'open', name: 'Open', description: 'The Rainbow Bridge is always open' },
    { value: 'vanilla', name: 'Vanilla', description: 'Opens when you have the Light Arrows, Shadow Medallion, and Spirit Medallion' },
    { value: 'medallions', name: 'Medallions', description: 'Opens when you have all Medallions' },
    { value: 'custom', name: 'Custom', description: 'You will need to meet a special condition to open the bridge' },
  ],
  default: 'medallions',
}, {
  key: 'majoraChild',
  name: 'Majora Child Requirements',
  category: 'main.events',
  type: 'enum',
  description: 'Controls the requirements to enter Majora\'s arena',
  values: [
    { value: 'none', name: 'None', description: 'As soon as you have access to the Moon you can enter Majora\'s arena' },
    { value: 'custom', name: 'Custom', description: 'You will need to meet a special condition to enter Majora\'s arena' },
  ],
  default: 'none',
  cond: (s: any) => s.goal !== 'triforce' && s.goal !== 'triforce3',
}, {
  key: 'bossWarpPads',
  name: 'Boss Warp Pads',
  category: 'main.events',
  type: 'enum',
  description: 'Controls the behavior of the MM Boss Warp Pads.',
  values: [
    { value: 'bossBeaten', name: 'Boss Beaten', description: 'Enabled when the boss is beaten' },
    { value: 'remains', name: 'Remains', description: 'Enabled when the matching remain is obtained' },
  ],
  default: 'bossBeaten'
}, {
  key: 'freeScarecrowOot',
  name: 'Free Scarecrow (OoT)',
  category: 'main.events',
  type: 'boolean',
  description: 'Allows to spawn Pierre the Scarecrow just by pulling the Ocarina out',
  default: false,
}, {
  key: 'preCompletedDungeons',
  name: 'Pre-Completed Dungeons',
  category: 'main.events',
  type: 'boolean',
  description: 'Allow dungeons to be pre-completed depending on rules.',
  default: false,
  cond: (s: any) => (s.mode !== 'multi' || s.distinctWorlds),
}, {
  key: 'preCompletedDungeonsMajor',
  name: 'Pre-Completed Dungeons (Major)',
  category: 'main.events',
  type: 'number',
  min: 0,
  max: 12,
  description: 'How many major dungeons sould be pre-completed. Can be combined with other pre-completed dungeon rules.',
  default: 0,
  cond: (s: any) => s.preCompletedDungeons,
}, {
  key: 'preCompletedDungeonsStones',
  name: 'Pre-Completed Dungeons (Stones)',
  category: 'main.events',
  type: 'number',
  min: 0,
  max: 3,
  description: 'Pre-completes dungeons containing at least one stone, until it reaches that many stones. Can be combined with other pre-completed dungeon rules.',
  default: 0,
  cond: (s: any) => s.preCompletedDungeons,
}, {
  key: 'preCompletedDungeonsMedallions',
  name: 'Pre-Completed Dungeons (Medallions)',
  category: 'main.events',
  type: 'number',
  min: 0,
  max: 6,
  description: 'Pre-completes dungeons containing at least one medallion, until it reaches that many medallions. Can be combined with other pre-completed dungeon rules.',
  default: 0,
  cond: (s: any) => s.preCompletedDungeons,
}, {
  key: 'preCompletedDungeonsRemains',
  name: 'Pre-Completed Dungeons (Remains)',
  category: 'main.events',
  type: 'number',
  min: 0,
  max: 4,
  description: 'Pre-completes dungeons containing at least one remain, until in reaches that many remains. Can be combined with other pre-completed dungeon rules.',
  default: 0,
  cond: (s: any) => s.preCompletedDungeons,
}, {
  key: 'openMaskShop',
  name: 'Open Mask Shop at night',
  category: 'main.events',
  type: 'boolean',
  description: 'Makes the Mask Shop in Market open during the night',
  default: false
}, {
  key: 'crossWarpOot',
  name: 'Cross-Games OoT Warp Songs',
  category: 'main.cross',
  type: 'boolean',
  description: 'Allows you to play OOT Warp Songs from MM to warp to their respective locations. Logic could even expect you to do so',
  default: false
}, {
  key: 'crossWarpMm',
  name: 'Cross-Games MM Song of Soaring',
  category: 'main.cross',
  type: 'enum',
  description: 'Controls whether you can play Song of Soaring from OOT to warp to MM Owl Statues and how logic should be affected',
  values: [
    { value: 'none', name: 'None', description: 'Song of Soaring is fully disabled in OOT' },
    { value: 'childOnly', name: 'Child Only', description: 'Song of Soaring in OOT is enabled and logical only for Child' },
    { value: 'full', name: 'Child & Adult', description: 'Song of Soaring in OOT is enabled and logical for both Child and Adult' },
  ],
  default: 'none'
}, {
  key: 'csmc',
  name: 'Container Appearance Matches Content',
  category: 'main.misc',
  type: 'enum',
  values: [
    { value: 'never',  name: 'Never', description: 'Containers will be vanilla' },
    { value: 'agony',  name: 'Stone of Agony', description: 'Containers will match content when you have the Stone of Agony in OoT' },
    { value: 'always', name: 'Always', description: 'Containers will always match content' },
  ],
  description: 'Modifies the chest, grass, and pot appearance so that they match their content. Works for unique items, keys, fairies, and souls. Grass and pots will otherwise always be gold if the item has not been collected',
  default: 'always'
}, {
  key: 'csmcHearts',
  name: 'CAMC for Heart Pieces/Containers',
  category: 'main.misc',
  type: 'boolean',
  default: true,
  description: 'Use a specific texture for heart pieces/containers',
  cond: (x: any) => x.csmc !== 'never',
}, {
  key: 'csmcExtra',
  name: 'Skulltula CAMC',
  category: 'main.misc',
  type: 'boolean',
  default: false,
  description: 'Enables CAMC for shuffled Gold, Swamp, and Ocean Skulltulas',
  cond: (x: any) => x.csmc !== 'never',
}, {
  key: 'blastMaskCooldown',
  name: 'Blast Mask Cooldown',
  category: 'main.misc',
  type: 'enum',
  description: 'Changes the cooldown between each explostion of the Blast Mask',
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
  key: 'clockSpeed',
  name: 'Clock Speed',
  category: 'main.misc',
  type: 'enum',
  description: 'Alters the speed of the clock. Some options may render seeds unbeatable due to being unable to do time-related checks',
  values: [
    { value: 'veryslow', name: 'Very Slow', description: 'The time will flow 66% slower than usual' },
    { value: 'slow', name: 'Slow', description: 'The time will flow 33% slower than usual' },
    { value: 'default', name: 'Default', description: 'The normal clock speed' },
    { value: 'fast', name: 'Fast', description: 'The clock speed will twice as fast than usual' },
    { value: 'veryfast', name: 'Very Fast', description: 'USE AT OWN RISK!! - The clock speed will be 3x faster than usual, even on inverted' },
    { value: 'superfast', name: 'Super Fast', description: 'USE AT OWN RISK!! - The clock speed will be 6x faster than usual, even on inverted' },
  ],
  default: 'default'
}, {
  key: 'fierceDeityAnywhere',
  name: 'Fierce Deity Anywhere in MM',
  category: 'main.misc',
  type: 'boolean',
  description: 'Controls the ability to use Fierce Deity outside of boss lairs. No logical applications',
  default: false
}, {
  key: 'hookshotAnywhereOot',
  name: 'Hookshot Anywhere (OoT)',
  category: 'main.misc',
  type: 'boolean',
  description: 'Modifies all surfaces to be hooked onto',
  default: false
}, {
  key: 'hookshotAnywhereMm',
  name: 'Hookshot Anywhere (MM)',
  category: 'main.misc',
  type: 'boolean',
  description: 'Modifies all surfaces to be hooked onto',
  default: false
}, {
  key: 'climbMostSurfacesOot',
  name: 'Climb Most Surfaces (OoT)',
  category: 'main.misc',
  type: 'boolean',
  description: 'Modifies most surface to be climbable',
  default: false
}, {
  key: 'climbMostSurfacesMm',
  name: 'Climb Most Surfaces (MM)',
  category: 'main.misc',
  type: 'boolean',
  description: 'Modifies most surface to be climbable',
  default: false
}, {
  key: 'fastBunnyHood',
  name: 'Fast Bunny Hood',
  category: 'main.misc',
  type: 'boolean',
  description: 'Modifies the Bunny Hood in OoT to give a speed increase',
  default: true
}, {
  key: 'defaultHoldTarget',
  name: 'Default Hold Target',
  category: 'main.misc',
  type: 'boolean',
  description: 'Forces the default target system to HOLD instead of SWITCH',
  default: true
}, {
  key: 'critWiggleDisable',
  name: 'Disable Crit Wiggle',
  category: 'main.misc',
  type: 'boolean',
  description: 'Disables the camera zoom-in and weird movements when low on health',
  default: true
}, {
  key: 'restoreBrokenActors',
  name: 'Restore Broken Actors',
  category: 'main.misc',
  type: 'boolean',
  description: 'In vanilla OoT, some actors fails to load properly in some rooms due to errors in the room files. When this is on, these actors will load.',
  default: false
}, {
  key: 'alterLostWoodsExits',
  name: 'Alter Lost Woods Exits',
  category: 'main.misc',
  type: 'boolean',
  description: 'There are unused exits in the Lost Woods that return you to the lost woods. When this is on, all the "got lost" exits in the Lost Woods that would normally take you to Kokiri Forest instead take you back to the Lost Woods, keeping your compass direction intact.',
  default: false
}, {
  key: 'voidWarpMm',
  name: 'Void Warp in MM',
  category: 'main.misc',
  type: 'boolean',
  description: 'In vanilla OoT, various code only checks for transitionTrigger, but in MM it also checks for transitionMode. When this is on, MM will no longer check transitionMode in those circumstances.',
  default: false
}, {
  key: 'autoInvert',
  name: 'Auto-Invert Time (MM)',
  category: 'main.misc',
  type: 'enum',
  description: 'Auto-inverts time at the start of a cycle',
  values: [
    { value: 'never', name: 'Never' },
    { value: 'firstCycle', name: 'First Cycle' },
    { value: 'always', name: 'Always' },
  ],
  default: 'never'
}, {
  key: 'fillWallets',
  name: 'Fill Wallets',
  category: 'items',
  type: 'boolean',
  description: 'Fills the wallet upon finding a new one',
  default: false
}, {
  key: 'progressiveShieldsOot',
  name: 'OoT Shields',
  category: 'items.progressive',
  type: 'enum',
  description: 'Alters OOT Shields behavior',
  values: [
    { value: 'separate', name: 'Separate', description: 'They can be found independently from each other' },
    { value: 'progressive', name: 'Progressive', description: 'Each Progressive Shield will grant you the next one: Deku Shield -> Hylian Shield -> Mirror Shield. Other Deku and Hylian Shields do not count towards this chain, only the Progressive Shield item.' },
  ],
  default: 'separate'
}, {
  key: 'progressiveSwordsOot',
  name: 'OoT Swords',
  category: 'items.progressive',
  type: 'enum',
  description: 'Alters OOT Swords behavior',
  values: [
    { value: 'separate', name: 'Separate', description: 'They can be found independently from each other' },
    { value: 'goron', name: 'Progressive Knife and Biggoron', description: 'Kokiri Sword and Master Sword are independent. However Giant\'s Knife and Biggoron Sword are progressive.' },
    { value: 'progressive', name: 'Progressive', description: 'Each Progressive Sword will grant you the next one: Kokiri Sword -> Master Sword -> Giant\'s Knife -> Biggoron Sword' },
  ],
  default: 'goron'
}, {
  key: 'progressiveShieldsMm',
  name: 'MM Shields',
  category: 'items.progressive',
  type: 'enum',
  description: 'Alters MM Shields behavior',
  values: [
    { value: 'separate', name: 'Separate', description: 'They can be found independently from each other' },
    { value: 'progressive', name: 'Progressive', description: 'Each Progressive Shield will grant you the next one: Hero Shield -> Mirror Shield. Other Hero Shields do not count towards this chain, only the Progressive Shield item. If shields are shared, Hero Shield will be obtained alongside Hylian Shield' },
  ],
  default: 'separate'
}, {
  key: 'progressiveGFS',
  name: 'MM Great Fairy Sword',
  category: 'items.progressive',
  type: 'enum',
  description: 'Controls whether Great Fairy Sword is included in sword progression',
  values: [
    { value: 'separate', name: 'Separate' },
    { value: 'progressive', name: 'Progressive' },
  ],
  default: 'separate'
}, {
  key: 'progressiveGoronLullaby',
  name: 'MM Goron Lullaby',
  category: 'items.progressive',
  type: 'enum',
  description: 'Alters the behavior of Goron Lullaby',
  values: [
    { value: 'single', name: 'Full Lullaby Only', description: 'Only the Goron Lullaby can be found, and when playing with Songs on Song Locations, Baby Goron in MM is no longer a Song Location' },
    { value: 'progressive', name: 'Progressive', description: 'Lullaby Intro will be received first before getting the full song' },
  ],
  default: 'progressive'
}, {
  key: 'progressiveClocks',
  name: 'Clocks',
  category: 'items.progressive',
  type: 'enum',
  description: 'Alters the behavior of Clocks',
  values: [
    { value: 'separate', name: 'Separate', description: 'Clocks will be independant of each other. If you don\t select a starting clock, one will be given to you at random.' },
    { value: 'ascending', name: 'Ascending', description: 'Clocks will be received in ascending order.' },
    { value: 'descending', name: 'Descending ', description: 'Clocks will be received in descending order.' },
  ],
  default: 'ascending',
  cond: (s: any) => s.clocks,
}, {
  key: 'bottleContentShuffle',
  name: 'Random Bottle Contents',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Randomize the content of the bottles',
  default: false
}, {
  key: 'sunSongMm',
  name: 'Sun\'s Song in MM',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Enables Sun\'s Song as an item in MM. If Songs are on Songs, you must share or start with at least one song',
  default: false
}, {
  key: 'fairyOcarinaMm',
  name: 'Fairy Ocarina in MM',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Functionally identical as the Ocarina of Time, but now there\'s 2 Ocarinas for Majora\'s Mask!',
  default: false
}, {
  key: 'blueFireArrows',
  name: 'Blue Fire Arrows',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Gives the OOT Ice Arrows the properties of Blue Fire',
  default: false,
}, {
  key: 'sunlightArrows',
  name: 'Sunlight Arrows',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Gives the OOT Light Arrows the ability to activate most sun switches',
  default: false,
}, {
  key: 'shortHookshotMm',
  name: 'Short Hookshot in MM',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Adds a short hookshot in MM (logic accounts for this). A trick is also there for some of the harder spots',
  default: false
}, {
  key: 'childWallets',
  name: 'Child Wallets',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Shuffles the starting Wallets... making it so you have to find it to hold anything!',
  default: false
}, {
  key: 'colossalWallets',
  name: 'Colossal Wallets',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Adds a Wallet that can hold up to 999 rupees in each game',
  default: false
}, {
  key: 'bottomlessWallets',
  name: 'Bottomless Wallets',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Adds a Wallet that can hold up to 9999 rupees in each game',
  default: false,
  cond: (s: any) => s.colossalWallets,
}, {
  key: 'skeletonKeyOot',
  name: 'Skeleton Key (OoT)',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Adds a Skeleton Key that can open every small-key-locked door.',
  default: false,
}, {
  key: 'skeletonKeyMm',
  name: 'Skeleton Key (MM)',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Adds a Skeleton Key that can open every small-key-locked door.',
  default: false,
}, {
  key: 'magicalRupee',
  name: 'Magical Rupee',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Adds a Magical Rupee that can trigger every silver-rupee event.',
  default: false,
  cond: (s: any) => s.silverRupeeShuffle !== 'vanilla',
}, {
  key: 'bombchuBagOot',
  name: 'Bombchu Bag (OoT)',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Turns the first out-of-shop bombchu pack you find into the bombchu bag. Has logical implications.',
  default: false,
}, {
  key: 'bombchuBagMm',
  name: 'Bombchu Bag (MM)',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Turns the first out-of-shop bombchu pack you find into the bombchu bag. Has logical implications.',
  default: false,
}, {
  key: 'spellFireMm',
  name: "Din's Fire (MM)",
  category: 'items.extensions',
  type: 'boolean',
  description: "Adds Din's Fire in Majora's Mask.",
  default: false
}, {
  key: 'spellWindMm',
  name: "Farore's Wind (MM)",
  category: 'items.extensions',
  type: 'boolean',
  description: "Adds Farore's Wind in Majora's Mask.",
  default: false
}, {
  key: 'spellLoveMm',
  name: "Nayru's Love (MM)",
  category: 'items.extensions',
  type: 'boolean',
  description: "Adds Nayru's Love in Majora's Mask.",
  default: false
}, {
  key: 'bootsIronMm',
  name: "Iron Boots (MM)",
  category: 'items.extensions',
  type: 'boolean',
  description: "Adds Iron Boots in Majora's Mask.",
  default: false
}, {
  key: 'bootsHoverMm',
  name: "Hover Boots (MM)",
  category: 'items.extensions',
  type: 'boolean',
  description: "Adds Hover Boots in Majora's Mask.",
  default: false
}, {
  key: 'tunicGoronMm',
  name: "Goron Tunic (MM)",
  category: 'items.extensions',
  type: 'boolean',
  description: "Adds Goron Tunic in Majora's Mask.",
  default: false
}, {
  key: 'tunicZoraMm',
  name: "Zora Tunic (MM)",
  category: 'items.extensions',
  type: 'boolean',
  description: "Adds Zora Tunic in Majora's Mask.",
  default: false
}, {
  key: 'scalesMm',
  name: "Scales (MM)",
  category: 'items.extensions',
  type: 'boolean',
  description: "Adds Silver Scale and Golden Scale in Majora's Mask.",
  default: false
}, {
  key: 'strengthMm',
  name: "Strength (MM)",
  category: 'items.extensions',
  type: 'boolean',
  description: "Adds Goron's Bracelet, Silver Gauntlets, and Golden Gauntlets in Majora's Mask.",
  default: false
}, {
  key: 'blastMaskOot',
  name: "Blast Mask (OoT)",
  category: 'items.extensions',
  type: 'boolean',
  description: "Add the Blast Mask in Ocarina of Time.",
  default: false
}, {
  key: 'stoneMaskOot',
  name: "Stone Mask (OoT)",
  category: 'items.extensions',
  type: 'boolean',
  description: "Add the Stone Mask in Ocarina of Time.",
  default: false
}, {
  key: 'elegyOot',
  name: "Elegy of Emptiness (OoT)",
  category: 'items.extensions',
  type: 'boolean',
  description: "Add the Elegy of Emptiness in Ocarina of Time.",
  default: false
}, {
  key: 'ocarinaButtonsShuffleOot',
  name: 'Ocarina Buttons Shuffle (OoT)',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Adds the Ocarina Buttons as items that are shuffled.',
  default: false,
}, {
  key: 'ocarinaButtonsShuffleMm',
  name: 'Ocarina Buttons Shuffle (MM)',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Adds the Ocarina Buttons as items that are shuffled.',
  default: false,
}, {
  key: 'soulsEnemyOot',
  name: 'Enemy Souls (OoT)',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Add enemy souls into the item pool. Enemies won\'t spawn unless their soul is obtained.',
  default: false
}, {
  key: 'soulsEnemyMm',
  name: 'Enemy Souls (MM)',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Add enemy souls into the item pool. Enemies won\'t spawn unless their soul is obtained.',
  default: false
}, {
  key: 'soulsBossOot',
  name: 'Boss Souls (OoT)',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Add boss souls into the item pool. Enemies won\'t spawn unless their soul is obtained.',
  default: false
}, {
  key: 'soulsBossMm',
  name: 'Boss Souls (MM)',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Add boss souls into the item pool. Enemies won\'t spawn unless their soul is obtained.',
  default: false
}, {
  key: 'soulsNpcOot',
  name: 'NPC Souls (OoT) (EXPERIMENTAL)',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Add NPC souls into the item pool. NPCs won\'t spawn unless their soul is obtained.',
  default: false
}, {
  key: 'clocks',
  name: 'Clocks as Items',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Add items representing every Majora\'s Mask half day into the pool. The moon will crash early unless you collect these items.',
  default: false
}, {
  key: 'lenientSpikes',
  name: 'Lenient Goron Spikes',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Goron spikes can charge midair and keep their charge. Minimum speed for goron spikes is removed.',
  default: true
}, {
  key: 'menuNotebook',
  name: 'Bombers\' Tracker',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Locks the in-game tracker behind the Bombers\' Notebook',
  default: false
}, {
  key: 'coins',
  name: 'Coins',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Enable the leftover coin items. These can be used for special conds',
  default: false
}, {
  key: 'coinsRed',
  name: 'Red Coins',
  category: 'items.extensions',
  type: 'number',
  description: 'How many Red Coins to add to the item pool',
  cond: (s: any) => s.coins,
  default: 0,
  min: 0,
  max: 999,
}, {
  key: 'coinsGreen',
  name: 'Green Coins',
  category: 'items.extensions',
  type: 'number',
  description: 'How many Green Coins to add to the item pool',
  cond: (s: any) => s.coins,
  default: 0,
  min: 0,
  max: 999,
}, {
  key: 'coinsBlue',
  name: 'Blue Coins',
  category: 'items.extensions',
  type: 'number',
  description: 'How many Blue Coins to add to the item pool',
  cond: (s: any) => s.coins,
  default: 0,
  min: 0,
  max: 999,
}, {
  key: 'coinsYellow',
  name: 'Yellow Coins',
  category: 'items.extensions',
  type: 'number',
  description: 'How many Yellow Coins to add to the item pool',
  cond: (s: any) => s.coins,
  default: 0,
  min: 0,
  max: 999,
}, {
  key: 'trapRupoor',
  name: 'Rupoors',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Add rupoors to the item pool. They remove 10 rupees when collected',
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
  default: false,
  cond: (s: any) => s.sunSongMm,
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
  key: 'sharedMaskGoron',
  name: 'Shared Goron Mask',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMaskZora',
  name: 'Shared Zora Mask',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMaskBunny',
  name: 'Shared Bunny Hood',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMaskKeaton',
  name: 'Shared Keaton Mask',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMaskTruth',
  name: 'Shared Mask of Truth',
  category: 'items.shared',
  type: 'boolean',
  default: false
}, {
  key: 'sharedMaskBlast',
  name: 'Shared Blast Mask',
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.blastMaskOot,
}, {
  key: 'sharedMaskStone',
  name: 'Shared Stone Mask',
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.stoneMaskOot,
}, {
  key: 'sharedSongElegy',
  name: 'Shared Elegy of Emptiness',
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.elegyOot,
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
  key: 'sharedShields',
  name: 'Shared Shields',
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.progressiveShieldsOot === s.progressiveShieldsMm,
}, {
  key: 'sharedSoulsEnemy',
  name: 'Shared Enemy Souls',
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.soulsEnemyOot && s.soulsEnemyMm,
}, {
  key: 'sharedOcarinaButtons',
  name: 'Shared Ocarina Buttons',
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.ocarinaButtonsShuffleOot && s.ocarinaButtonsShuffleMm,
}, {
  key: 'sharedSkeletonKey',
  name: 'Shared Skeleton Key',
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.skeletonKeyOot && s.skeletonKeyMm,
}, {
  key: 'sharedBombchuBags',
  name: 'Shared Bombchu Bags',
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.bombchuBagOot && s.bombchuBagMm,
}, {
  key: 'sharedSpellFire',
  name: "Shared Din's Fire",
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.spellFireMm,
}, {
  key: 'sharedSpellWind',
  name: "Shared Farore's Wind",
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.spellWindMm,
}, {
  key: 'sharedSpellLove',
  name: "Shared Nayru's Love",
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.spellLoveMm,
}, {
  key: 'sharedBootsIron',
  name: "Shared Iron Boots",
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.bootsIronMm,
}, {
  key: 'sharedBootsHover',
  name: "Shared Hover Boots",
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.bootsHoverMm,
}, {
  key: 'sharedTunicGoron',
  name: "Shared Goron Tunic",
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.tunicGoronMm,
}, {
  key: 'sharedTunicZora',
  name: "Shared Zora Tunic",
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.tunicZoraMm,
}, {
  key: 'sharedScales',
  name: "Shared Scales",
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.scalesMm,
}, {
  key: 'sharedStrength',
  name: "Shared Strength",
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.strengthMm,
}, {
  key: 'agelessSwords',
  name: 'Ageless Swords',
  category: 'items.ageless',
  type: 'boolean',
  description: 'Allows Link to use swords independently of his age',
  default: false
}, {
  key: 'agelessShields',
  name: 'Ageless Shields',
  category: 'items.ageless',
  type: 'boolean',
  description: 'Allows Link to use shields independently of his age',
  default: false
}, {
  key: 'agelessTunics',
  name: 'Ageless Tunics',
  category: 'items.ageless',
  type: 'boolean',
  description: 'Allows Link to use tunics independently of his age',
  default: false
}, {
  key: 'agelessBoots',
  name: 'Ageless Boots',
  category: 'items.ageless',
  type: 'boolean',
  description: 'Allows Link to use boots independently of his age',
  default: false
}, {
  key: 'agelessSticks',
  name: 'Ageless Sticks',
  category: 'items.ageless',
  type: 'boolean',
  description: 'Allows Link to use deku sticks independently of his age',
  default: false
}, {
  key: 'agelessBoomerang',
  name: 'Ageless Boomerang',
  category: 'items.ageless',
  type: 'boolean',
  description: 'Allows Link to use the boomerang independently of his age',
  default: false
}, {
  key: 'agelessHammer',
  name: 'Ageless Hammer',
  category: 'items.ageless',
  type: 'boolean',
  description: 'Allows Link to use the hammer independently of his age',
  default: false
}, {
  key: 'agelessHookshot',
  name: 'Ageless Hookshot',
  category: 'items.ageless',
  type: 'boolean',
  description: 'Allows Link to use the hookshot independently of his age',
  default: false
}, {
  key: 'agelessChildTrade',
  name: 'Ageless Child Trade',
  category: 'items.ageless',
  type: 'boolean',
  description: 'Allows Link to use the child trade items independently of his age',
  default: false
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
  description: 'Shuffle bosses either within their own game or across both',
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
  description: 'Enable the ability to shuffle dungeons within their own game or across both.',
  default: 'none'
}, {
  key: 'erWallmasters',
  name: 'Wallmaster Shuffle',
  category: 'entrances',
  type: 'enum',
  values: [
    { value: 'none', name: 'None' },
    { value: 'ownGame', name: 'Own Game' },
    { value: 'full', name: 'Full' },
  ],
  description: 'Enables the ability for Wallmasters to take you to random locations within their own game or across both games, based on other entrance settings',
  default: 'none'
}, {
  key: 'erMajorDungeons',
  name: 'Shuffle Major Dungeons with Dungeons',
  category: 'entrances',
  type: 'boolean',
  description: 'If turned on, it means the boss-containing dungeons and uninverted Stone Tower Temple will be shuffled.',
  default: false,
  cond: (x: any) => x.erDungeons !== 'none',
}, {
  key: 'erMinorDungeons',
  name: 'Shuffle OoT Minor Dungeons with Dungeons',
  category: 'entrances',
  type: 'boolean',
  description: 'If turned on, it means Bottom of the Well, Ice Cavern and Gerudo Training Grounds are also shuffled',
  default: false,
  cond: (x: any) => x.erDungeons !== 'none'
}, {
  key: 'erGanonCastle',
  name: 'Shuffle Ganon\'s Castle with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false,
  cond: (x: any) => x.erDungeons !== 'none'
}, {
  key: 'erGanonTower',
  name: 'Shuffle Ganon\'s Tower with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false,
  cond: (x: any) => x.erDungeons !== 'none'
}, {
  key: 'erMoon',
  name: 'Shuffle Clock Tower with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false,
  cond: (x: any) => x.erDungeons !== 'none'
}, {
  key: 'erSpiderHouses',
  name: 'Shuffle Spider Houses with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false,
  cond: (x: any) => x.erDungeons !== 'none'
}, {
  key: 'erPirateFortress',
  name: 'Shuffle Pirate Fortress with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false,
  cond: (x: any) => x.erDungeons !== 'none'
}, {
  key: 'erBeneathWell',
  name: 'Shuffle Beneath The Well with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false,
  cond: (x: any) => x.erDungeons !== 'none'
}, {
  key: 'erIkanaCastle',
  name: 'Shuffle Ikana Castle Interior with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false,
  cond: (x: any) => x.erDungeons !== 'none'
}, {
  key: 'erSecretShrine',
  name: 'Shuffle Secret Shrine with Dungeons',
  category: 'entrances',
  type: 'boolean',
  default: false,
  cond: (x: any) => x.erDungeons !== 'none'
}, {
  key: 'erRegions',
  name: 'Shuffle Major Regions',
  category: 'entrances',
  type: 'enum',
  values: [
    { value: 'none', name: 'None' },
    { value: 'ownGame', name: 'Own Game' },
    { value: 'full', name: 'Full' },
  ],
  default: 'none',
  description: '- Every entrance to Hyrule Field except Market<br>- The entrance to Gerudo Fortress from Gerudo Valley<br>- The entrance to Death Mountain from Kakariko<br>- The entrances to the four main regions in MM<br>- The entrance to Romani Ranch',
}, {
  key: 'erRegionsExtra',
  name: 'Shuffle Market Entrance',
  category: 'entrances',
  type: 'boolean',
  description: 'Shuffle Hyrule Field\'s Market entrance among the regions.',
  default: false,
  cond: (x: any) => x.erRegions !== 'none'
}, {
  key: 'erRegionsShortcuts',
  name: 'Shuffle Regional Shortcuts',
  category: 'entrances',
  type: 'boolean',
  description: 'Shuffles the various shortcuts between regions.<br>- Lost Woods/Goron City<br>- Lost Woods/Zora\'s River<br>- Zora\'s Domain/Lake Hylia',
  default: false,
  cond: (x: any) => x.erRegions !== 'none'
}, {
  key: 'erIndoors',
  name: 'Shuffle Interiors',
  category: 'entrances',
  type: 'enum',
  values: [
    { value: 'none', name: 'None' },
    { value: 'ownGame', name: 'Own Game' },
    { value: 'full', name: 'Full' },
  ],
  default: 'none',
  description: 'Enable the ability to shuffle interiors in their own game or across both games.',
}, {
  key: 'erIndoorsMajor',
  name: 'Shuffle Most Interiors',
  category: 'entrances',
  type: 'boolean',
  description: 'Shuffle most interiors with each other.',
  default: false,
  cond: (x: any) => x.erIndoors !== 'none'
}, {
  key: 'erIndoorsExtra',
  name: 'Shuffle Extra Interiors',
  category: 'entrances',
  type: 'boolean',
  description: 'Shuffle additional, more complex interiors. These include:<br>- OOT: Link\'s House, Temple of Time, Windmill, Kak Potion Shop<br>- MM: Stock Pot Inn, Astral Observatory/Bombers\' Hideout, Swamp Tourist Hut, Ikana Spring Cave',
  default: false,
  cond: (x: any) => x.erIndoors !== 'none'
}, {
  key: 'erWarps',
  name: 'Shuffle Warp Songs and Soaring',
  category: 'entrances',
  type: 'enum',
  values: [
    { value: 'none', name: 'None', description: 'Warp songs and soaring are not shuffled.' },
    { value: 'ootOnly', name: 'OoT Only', description: 'Shuffles only OoT\'s warp songs among each other.' },
    { value: 'mmOnly', name: 'MM Only', description: 'Shuffles only MM\'s soaring spots among each other.' },
    { value: 'ownGame', name: 'Own Game', description: 'Shuffles both warp songs and soaring spots within their own game.' },
    { value: 'full', name: 'Full', description: 'Shuffles both warp songs and soaring spots together.' },
  ],
  description: 'Allows separate shuffling of the warp songs and soaring spots. This setting is disabled if both are selected in "Shuffle One-Way Entrances".',
  default: 'none',
  cond: (s: any) => !s.erOneWaysSongs || !s.erOneWaysStatues
}, {
  key: 'erOneWays',
  name: 'Shuffle One-Way Entrances',
  category: 'entrances',
  type: 'enum',
  values: [
    { value: 'none', name: 'None', description: 'One-Way entrances are not shuffled.' },
    { value: 'ownGame', name: 'Own Game', description: 'One-Way entrances are only shuffled among their own game.' },
    { value: 'full', name: 'Full', description: 'One-Way entrances can be shuffled among both games.' },
  ],
  description: 'Enables the option of shuffling various one-way entrances.',
  default: 'none',
}, {
  key: 'erOneWaysMajor',
  name: 'Shuffle Standard One-Ways',
  category: 'entrances',
  type: 'boolean',
  description: 'Shuffles the Gerudo Valley to Lake Hylia and Ikana Canyon to Southern Swamp entrances.',
  default: false,
  cond: (x: any) => x.erOneWays !== 'none'
}, {
  key: 'erOneWaysIkana',
  name: 'Shuffle Ikana Castle One-Ways',
  category: 'entrances',
  type: 'boolean',
  description: 'Shuffles the one-way entrances in Ikana Castle with the others. This is the keg-blocked entrance and the block moved by the switch.',
  default: false,
  cond: (x: any) => x.erOneWays !== 'none'
}, {
  key: 'erOneWaysSongs',
  name: 'Shuffle One-Ways with Warp Songs',
  category: 'entrances',
  type: 'boolean',
  description: 'Shuffles the warp songs from OOT among one-way entrances. This setting is disabled if the warp songs are shuffled with "Shuffle Warp Songs and Soaring".',
  default: false,
  cond: (x: any) => x.erOneWays !== 'none' && x.erWarps !== 'ootOnly' && x.erWarps !== 'full' && x.erWarps !== 'ownGame'
}, {
  key: 'erOneWaysStatues',
  name: 'Shuffle One-Ways with Owl Statues',
  category: 'entrances',
  type: 'boolean',
  description: 'Shuffles the owl statues from MM among one-way entrances. This setting is disabled if the soaring spots are shuffled with "Shuffle Warp Songs and Soaring".',
  default: false,
  cond: (x: any) => x.erOneWays !== 'none' && x.erWarps !== 'mmOnly' && x.erWarps !== 'full' && x.erWarps !== 'ownGame'
}, {
  key: 'erOneWaysOwls',
  name: 'Shuffle Child Owl Flights',
  category: 'entrances',
  type: 'boolean',
  description: 'Shuffle the two owl flights among one-way entrances.',
  default: false,
  cond: (x: any) => x.erOneWays !== 'none'
}, {
  key: 'erOneWaysWoods',
  name: 'Shuffle Altered Lost Woods Exits',
  category: 'entrances',
  type: 'boolean',
  description: 'Shuffle the Lost Woods entrances added by "Alter Lost Woods Exits" among one-way entrances.',
  default: false,
  cond: (x: any) => x.erOneWays !== 'none' && x.alterLostWoodsExits
}] as const;

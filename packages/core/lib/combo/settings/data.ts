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
    { value: 'multi', name: 'Multiworld', description: 'A multiplayer settings where players can find each other\'s items' },
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
    { value: 'triforce', name: 'Triforce', description: 'You must collect triforce pieces to win.' },
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
  max: (s: any) => s.triforcePieces
}, {
  key: 'triforcePieces',
  name: 'Triforce Pieces',
  category: 'main',
  type: 'number',
  description: 'The total amount of Triforce Pieces in the item pool.',
  default: 30,
  cond: (s: any) => s.goal === 'triforce',
  min: 1,
  max: 999
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
  ],
  default: 'normal'
}, {
  key: 'generateSpoilerLog',
  name: 'Generate Spoiler Log',
  category: 'main',
  type: 'boolean',
  description: 'Generate an extra downloadable text file with the location of every item in the seed as well as other informations. It is recommended to enable this option.',
  default: true
}, {
  key: 'probabilisticFoolish',
  name: 'Probabilistic Foolish Hints',
  category: 'main',
  type: 'boolean',
  description: 'If you don\'t know what this is, leave it ON',
  default: true
}, {
  key: 'songs',
  name: 'Song Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where songs can be obtained, in both games.',
  values: [
    { value: 'songLocations', name: 'Song Locations', description: 'Only locations that contains songs in the vanilla games will have songs.' },
    { value: 'anywhere', name: 'Anywhere', description: 'Songs can be placed anywhere.' },
  ],
  default: 'songLocations'
}, {
  key: 'goldSkulltulaTokens',
  name: 'Gold Skulltula Tokens Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls which Gold Skulltulas should conceal items different than Gold Tokens',
  values: [
    { value: 'none', name: 'No Shuffle', description: 'All Gold Skulltula Tokens will be vanilla' },
    { value: 'dungeons', name: 'Dungeons Only', description: 'Only the Gold Skulltulas within dungeons will be shuffled' },
    { value: 'overworld', name: 'Overworld Only', description: 'Only the Gold Skulltulas outside of dungeons will be shuffled' },
    { value: 'all', name: 'All Tokens', description: 'Every single Gold Skulltulas will be shuffled' },
  ],
  default: 'none'
}, {
  key: 'housesSkulltulaTokens',
  name: 'Houses Skulltula Tokens Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls whether Swamp and Ocean should be shuffled or not',
  values: [
    { value: 'none', name: 'No Shuffle', description: 'Swamp and Ocean Tokens will be vanilla' },
    { value: 'cross', name: 'Gold Skulltulas Only', description: '' },
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
    { value: 'starting', name: 'Starting Items', description: 'Tingle Maps are in your pocket' },
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
  key: 'bossKeyShuffleOot',
  name: 'Boss Key Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where Boss Keys can be in OoT',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon', description: 'Boss Keys can only be in their own dungeons' },
    { value: 'anywhere', name: 'Anywhere', description: 'Boss Keys can be found anywhere' },
    { value: 'removed', name: 'Removed', description: 'Boss keys are removed and boss doors are unlocked' },
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
    { value: 'removed', name: 'Removed', description: 'Boss keys are removed and boss doors are unlocked' },
  ],
  default: 'ownDungeon'
}, {
  key: 'smallKeyRingOot',
  name: 'Small Key Ring (OoT)',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls the grouping of keys into keyrings, for OoT',
  values: [
    { value: 'disabled', name: 'Disabled', description: 'Small keys are found individually, like in the base game.' },
    { value: 'keyRings', name: 'Key Rings', description: 'Small keys are grouped into key rings.' },
  ],
  default: 'disabled'
}, {
  key: 'smallKeyRingMm',
  name: 'Small Key Ring (MM)',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls the grouping of keys into keyrings, for MM',
  values: [
    { value: 'disabled', name: 'Disabled', description: 'Small keys are found individually, like in the base game.' },
    { value: 'keyRings', name: 'Key Rings', description: 'Small keys are grouped into key rings.' },
  ],
  default: 'disabled'
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
  key: 'townFairyShuffle',
  name: 'Town Stray Fairy Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where the Clock Town Stray Fairy can be',
  values: [
    { value: 'vanilla', name: 'Vanilla', description: 'The Clock Town Stray Fairy will be at its original locations'},
    { value: 'anywhere', name: 'Anywhere', description: 'The Clock Town Stray Fairy can be found anywhere' },
  ],
  default: 'vanilla'
}, {
  key: 'strayFairyChestShuffle',
  name: 'Dungeon Chest Fairy Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where the Chest Dungeon Stray Fairies can be',
  values: [
    { value: 'starting', name: 'Starting', description: 'Start with the fairies' },
    { value: 'vanilla', name: 'Vanilla', description: 'The Dungeon Stray Fairies will be at their original locations' },
    { value: 'ownDungeon', name: 'Own Dungeon', description: 'All Dungeon Stray Fairies are shuffled within their own dungeon' },
    { value: 'anywhere', name: 'Anywhere', description: 'All Dungeon Stray Fairies are shuffled anywhere' },
  ],
  default: 'ownDungeon'
}, {
  key: 'strayFairyOtherShuffle',
  name: 'Dungeon Freestanding Fairy Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where the Freestanding Dungeon Stray Fairies can be',
  values: [
    { value: 'removed', name: 'Removed', description: 'Start with the fairies, and the bubbles are removed.' },
    { value: 'starting', name: 'Starting', description: 'Start with the fairies' },
    { value: 'vanilla', name: 'Vanilla', description: 'The Dungeon Stray Fairies will be at their original locations' },
    { value: 'ownDungeon', name: 'Own Dungeon', description: 'All Dungeon Stray Fairies are shuffled within their own dungeon' },
    { value: 'anywhere', name: 'Anywhere', description: 'All Dungeon Stray Fairies are shuffled anywhere' },
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
  cond: (s: any) => s.goal !== 'triforce'
}, {
  key: 'dungeonRewardShuffle',
  name: 'Dungeon Reward Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where the dungeons rewards should be',
  values: [
    { value: 'dungeonBlueWarps', name: 'Dungeon Blue Warps', description: 'Only the blue warps will grand the rewards' },
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
  description: 'Should Business Scrubs in OOT shuffled? If not, the one in Hyrule Field by Lake Hylia fences, the one by the Bridge in Lost Woods and the front one in the grotto near Sacred Forest Meadow will still be shuffled',
  default: false
}, {
  key: 'scrubShuffleMm',
  name: 'Scrub Shuffle (MM)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Shuffle the three business scrubs that sells regular items in MM',
  default: false
}, {
  key: 'cowShuffleOot',
  name: 'Cow Shuffle (OoT)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Should playing Epona\'s Song near cows in OOT give an item?',
  default: false
}, {
  key: 'cowShuffleMm',
  name: 'Cow Shuffle (MM)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Should playing Epona\'s Song near cows in MM give an item?',
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
  description: 'Controls whether or not shops in MM should have their items shuffled',
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
  description: 'Controls whether or not the pots are shuffled (OoT). !!!!! MQ NOT INCLUDED YET !!!!!',
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
  description: 'Controls whether or not the Master Sword is shuffled amongst all the items',
  default: true
}, {
  key: 'shuffleGerudoCard',
  name: 'Gerudo Card Shuffle',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Controls whether or not the Gerudo Membership Card is shuffled amongst all the items',
  default: true
}, {
  key: 'shuffleMerchantsMm',
  name: 'Merchants Shuffle (MM)',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Shuffle the Milk Bar and Gorman Racetrack purchases.',
  default: false,
  cond: (x: any) => x.shopShuffleMM === 'full'
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
  key: 'doorOfTime',
  name: 'Door of Time',
  category: 'main.events',
  type: 'enum',
  description: 'Alters the Door of Time state',
  values: [
    { value: 'closed', name: 'Closed', description: 'The Door will be closed, and you will need to play Song of Time in front of the Temple of Time altar to open it. (The Spiritual Stones are NOT needed)' },
    { value: 'open', name: 'Open', description: 'The Door is already open' },
  ],
  default: 'closed'
}, {
  key: 'dekuTree',
  name: 'Deku Tree',
  category: 'main.events',
  type: 'enum',
  description: 'Alters the Deku Tree state',
  values: [
    { value: 'closed', name: 'Closed', description: 'Mido will block the way to the deku tree until you have a deku shield and the kokiri sword.' },
    { value: 'open', name: 'Open', description: 'The deku tree will be open from the start' },
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
  key: 'kakarikoGate',
  name: 'Kakariko Gate',
  category: 'main.events',
  type: 'enum',
  description: 'Alters the Kakariko Gate (leading to Death Mountain Trail) state',
  values: [
    { value: 'closed', name: 'Closed' },
    { value: 'open', name: 'Open' },
  ],
  default: 'closed'
}, {
  key: 'zoraKing',
  name: 'King Zora',
  category: 'main.events',
  type: 'enum',
  description: 'Alters the behavior of King Zora in Zora\'s Domain',
  values: [
    { value: 'vanilla', name: 'Vanilla', description: 'You will need to present him Ruto\'s Letter in order to enter Zora\'s Fountain as both Child and Adult' },
    { value: 'adult', name: 'Open (Adult Only)', description: 'Already on the side as Adult, leaving Zora\'s Fountain access free. Child still needs Ruto\'s Letter' },
    { value: 'open', name: 'Open', description: 'He will already be on the side for both Child and Adult, and Ruto\'s Letter is removed from the game, in favor of an empty bottle' },
  ],
  default: 'vanilla'
}, {
  key: 'gerudoFortress',
  name: 'Gerudo Fortress',
  category: 'main.events',
  type: 'enum',
  description: 'Alters the behavior of the Carpenters in Gerudo Fortress',
  values: [
    { value: 'vanilla', name: 'Vanilla', description: 'You will need to rescue all carpenters.' },
    { value: 'single', name: 'One Carpenter', description: 'You will need to rescue only one carpenter.' },
    { value: 'open', name: 'Open', description: 'Carpenters are rescued from the start.' },
  ],
  default: 'vanilla'
}, {
  key: 'skipZelda',
  name: 'Skip Child Zelda',
  category: 'main.events',
  type: 'boolean',
  description: 'This changes the beginning of the child trade quest. True means you\'ll start having already met Zelda and got her item along the one from Impa. And the Chicken is also removed from the game',
  default: false
}, {
  key: 'lacs',
  name: 'Light Arrow Cutscene',
  category: 'main.events',
  type: 'enum',
  description: 'Alters how the Light Arrow Cutscene should be triggered',
  values: [
    { value: 'vanilla', name: 'Vanilla', description: 'Triggers at Temple of Time with Shadow and Spirit Medallions' },
    { value: 'custom', name: 'Custom', description: 'Triggers at Temple of Time with a special condition'},
  ],
  default: 'vanilla'
}, {
  key: 'majoraChild',
  name: 'Majora Child Requirements',
  category: 'main.events',
  type: 'enum',
  description: 'Alters the requirements to fight Majora',
  values: [
    { value: 'none', name: 'None', description: 'As soon as you have access to the Moon you can go fight Majora' },
    { value: 'custom', name: 'Custom', description: 'You will need to meet a special condition to fight Majora' },
  ],
  default: 'none',
  cond: (s: any) => s.goal !== 'triforce'
}, {
  key: 'bossWarpPads',
  name: 'Boss Warp Pads',
  category: 'main.events',
  type: 'enum',
  description: 'Alters the behavior of the MM Boss Warp Pads.',
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
    { value: 'childOnly', name: 'Child Only', description: 'Song of Soaring in OOT is enabled, and logic only accounts for Child using it' },
    { value: 'full', name: 'Child & Adult', description: 'Song of Soaring in OOT is enabled, and logic accounts for both Child and Adult using it' },
  ],
  default: 'none'
}, {
  key: 'csmc',
  name: 'Chest Size Matches Content',
  category: 'main.misc',
  type: 'enum',
  values: [
    { value: 'never',  name: 'Never', description: 'Chests will be vanilla' },
    { value: 'agony',  name: 'Stone of Agony', description: 'Chests will match content when you have the stone of agony in OoT' },
    { value: 'always', name: 'Always', description: 'Chests will always match content' },
  ],
  description: 'Modifies the chests appearance so they match their content. Works for unique items, keys and fairies.',
  default: 'always'
}, {
  key: 'csmcHearts',
  name: 'CSMC for Heart Pieces/Containers',
  category: 'main.misc',
  type: 'boolean',
  default: true,
  description: 'Use a specific texture for heart pieces/containers',
  cond: (x: any) => x.csmc !== 'never'
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
  description: 'Controls the ability to use Fierce Deity outside of boss lairs',
  default: false
}, {
  key: 'hookshotAnywhereOot',
  name: 'Hookshot Anywhere (OOT)',
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
  name: 'Climb Most Surfaces (OOT)',
  category: 'main.misc',
  type: 'boolean',
  description: 'Modifies most surface to be climbable in OOT',
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
  values: [
    { value: 'single', name: 'Full Lullaby Only', description: 'Only the Goron Lullaby can be found, and when playing with Songs on Song Locations, Baby Goron in MM is no longer a Song Location' },
    { value: 'progressive', name: 'Progressive' },
  ],
  default: 'progressive'
}, {
  key: 'sunSongMm',
  name: 'Sun\'s Song in MM',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Enables Sun\'s Song as an item in MM.',
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
  description: 'Changes the OoT Ice Arrows to have Blue Fire properties',
  default: false,
}, {
  key: 'shortHookshotMm',
  name: 'Short Hookshot in MM',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Adds a short hookshot in MM (logic accounts for that). A trick is also there for some of the harder spots',
  default: false
}, {
  key: 'childWallets',
  name: 'Child Wallets',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Shuffles the starting Wallet... making it so you have to find it to spend anything!',
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
  cond: (s: any) => s.colossalWallets
}, {
  key: 'skeletonKeyOot',
  name: 'Skeleton Key (OoT)',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Adds a Skeleton Key that can open every small-key-locked door.',
  default: false
}, {
  key: 'skeletonKeyMm',
  name: 'Skeleton Key (MM)',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Adds a Skeleton Key that can open every small-key-locked door.',
  default: false
}, {
  key: 'enemySoulsOot',
  name: 'Enemy Souls (OoT)',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Add enemy souls into the item pool. Enemies won\'t spawn unless their soul is obtained.',
  default: false
}, {
  key: 'enemySoulsMm',
  name: 'Enemy Souls (MM)',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Add enemy souls into the item pool. Enemies won\'t spawn unless their soul is obtained.',
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
  cond: (s: any) => s.sunSongMm
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
  key: 'sharedSouls',
  name: 'Shared Souls',
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.enemySoulsOot && s.enemySoulsMm
}, {
  key: 'sharedSkeletonKey',
  name: 'Shared Skeleton Key',
  category: 'items.shared',
  type: 'boolean',
  default: false,
  cond: (s: any) => s.skeletonKeyOot && s.skeletonKeyMm
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
  key: 'erOverworld',
  name: 'Shuffle Overworld Entrances',
  category: 'entrances',
  type: 'enum',
  values: [
    { value: 'none', name: 'None', description: 'No entrances are shuffled' },
    { value: 'regionsOwnGame', name: 'Regional - Own Game', description: 'Shuffles the major regions in their own game' },
    { value: 'regionsFull', name: 'Regional - Full', description: 'Shuffles the major regions across both games' },
    { value: 'ownGame', name: 'Own Game', description: 'Shuffles all entrances in their own game' },
    { value: 'full', name: 'Full', description: 'Shuffles all entrances across both games' },
  ],
  default: 'none',
  description: 'Shuffles all the overworld entrances among each other. Currently only OOT is supported by Own Game and Full. Major regions include:<br>- Every entrance to Hyrule Field except Gerudo Valley and Market<br>- The entrance to Gerudo Fortress from Gerudo Valley<br>- The entrance to Death Mountain from Kakariko<br>- The entrances to the four main regions in MM<br>- The entrance to Romani Ranch'
}, {
  key: 'erRegionsExtra',
  name: 'Shuffle Market Entrance',
  category: 'entrances',
  type: 'boolean',
  description: 'Shuffle Hyrule Field\'s Market entrance among the regions.',
  default: false,
  cond: (x: any) => x.erOverworld === 'regionsOwnGame' || x.erOverworld === 'regionsFull'
}, {
  key: 'erRegionsShortcuts',
  name: 'Shuffle Regional Shortcuts',
  category: 'entrances',
  type: 'boolean',
  description: 'Shuffles the various shortcuts between regions.<br>- Lost Woods -> Goron City<br>- Lost Woods -> Zora\'s River<br>- Zora\'s Domain -> Lake Hylia<br>- Gerudo Valley -> Lake Hylia<br>- Ikana Canyon -> Southern Swamp',
  default: false,
  cond: (x: any) => x.erOverworld === 'regionsOwnGame' || x.erOverworld === 'regionsFull'
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
  description: 'Shuffle most interiors with each other'
}, {
  key: 'erIndoorsExtra',
  name: 'Shuffle Extra Interiors',
  category: 'entrances',
  type: 'boolean',
  description: 'Shuffle additional, more complex interiors. These include:<br>- OOT: Link\'s House, Temple of Time, Windmill, Kak Potion Shop<br>- MM: Stock Pot Inn, Astral Observatory/Bombers\' Hideout, Swamp Tourist Hut, Ikana Spring Cave',
  default: false,
  cond: (x: any) => x.erIndoors !== 'none'
}] as const;

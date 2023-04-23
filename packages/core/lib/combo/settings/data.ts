export const SETTINGS = [{
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
  ],
  default: 'both'
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
  key: 'generateSpoilerLog',
  name: 'Generate Spoiler Log',
  category: 'main',
  type: 'boolean',
  description: 'Generate an extra downloadable text file with the location of every item in the seed as well as other informations. It is recommended to enable this option.',
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
  key: 'smallKeyShuffle',
  name: 'Small Key Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where Small Keys (for Dungeons) can be',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon', description: 'Dungeon Small Keys can only be found in their own dungeons' },
    { value: 'anywhere', name: 'Anywhere', description: 'Dungeon Small Keys can be found anywhere' },
  ],
  default: 'ownDungeon'
}, {
  key: 'smallKeyShuffleHideout',
  name: 'Hideout Small Key Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where Hideout (Gerudo Fortress) Small Keys can be',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon', description: 'Hideout Small Keys can only be found within Gerudo Fortress INTERIOR' },
    { value: 'anywhere', name: 'Anywhere', description: 'Hideout Small Keys can be found anywhere' },
  ],
  default: 'ownDungeon'
}, {
  key: 'bossKeyShuffle',
  name: 'Boss Key Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where Boss Keys can be',
  values: [
    { value: 'ownDungeon', name: 'Own Dungeon', description: 'Boss Keys can only be in their own dungeons' },
    { value: 'anywhere', name: 'Anywhere', description: 'Boss Keys can be found anywhere' },
  ],
  default: 'ownDungeon'
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
  key: 'strayFairyShuffle',
  name: 'Dungeon Stray Fairy Shuffle',
  category: 'main.shuffle',
  type: 'enum',
  description: 'Controls where the Dungeon Stray Fairies can be',
  values: [
    { value: 'vanilla', name: 'Vanilla', description: 'The Dungeon Stray Fairies will be at their original locations' },
    { value: 'mixed', name: 'Chests Own Dungeon', description: 'The Dungeon Stray Fairies within bubbles are vanilla, but the ones in chests are shuffled within their own dungeon' },
    { value: 'ownDungeon', name: 'Own Dungeon', description: 'All Dungeon Stray Fairies are shuffled within their own dungeon' },
    { value: 'anywhere', name: 'Anywhere', description: 'All Dungeon Stray Fairies are shuffled anywhere' },
  ],
  default: 'mixed'
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
  default: 'removed'
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
  key: 'eggShuffle',
  name: 'Weird / Pocket Egg Content Shuffle',
  category: 'main.shuffle',
  type: 'boolean',
  description: 'Fun setting: should using the Weird/Pocket Eggs give an item? If not, they\'re entirely removed from the game',
  default: false
}, {
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
  key: 'skipZelda',
  name: 'Skip Child Zelda',
  category: 'main.events',
  type: 'boolean',
  description: 'This changes the beginning of the child trade quest. True means you\'ll start having already met Zelda and got her item along the one from Impa. And the Chicken is also removed from the game',
  default: false,
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
  default: 'none'
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
  key: 'sunSongMm',
  name: 'Sun\'s Song in MM',
  category: 'main.cross',
  type: 'boolean',
  description: 'Enables Sun\'s Song as an item in MM.',
  default: false
}, {
  key: 'csmc',
  name: 'Chest Size Matches Content',
  category: 'main.misc',
  type: 'boolean',
  description: 'Modifies the chests appearance so they match their content. Works for unique items, keys and fairies.',
  default: false
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
  default: 'start'
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
  key: 'fairyOcarinaMm',
  name: 'Fairy Ocarina in MM',
  category: 'items.extensions',
  type: 'boolean',
  description: 'Functionally identical as the Ocarina of Time, but now there\'s 2 Ocarinas for Majora\'s Mask!',
  default: false
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
  description: 'If turned on, it meeans Bottom of the Well, Ice Cavern and Gerudo Training Grounds are also shuffled',
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
  description: 'Basic Overworld Shuffled',
  default: false
}] as const;

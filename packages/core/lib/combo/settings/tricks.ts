import { Game } from '../config';

/*
 * Each trick should have a user friendly name, a tooltip to explain the trick's function within the logic,
 * and if possible, some supporting documentation like a youtube video or article detailing the trick further for beginners.
 */

/* Typing for each individual trick's values */
export type Trick = {
  game: Game;
  name: string;
  glitch?: boolean;
  tooltip?: string;
  linkVideo?: string;
  linkText?: string;
};

/* Typing for the whole object of tricks */
export type Tricks = {
  [key: string]: Trick;
};

/** Typing for each key of the TRICKS object */
export type TrickKey = string;

/**
 * All the tricks in the randomizer.
 */
export const TRICKS: Tricks = {
  OOT_LENS: {
    game: 'oot',
    name: 'Fewer Lens Requirements (OoT)',
    tooltip: 'Doing things that would normally require Lens will no longer will, except for Treasure Chest Game and Haunted Wasteland.',
  },
  OOT_TUNICS: {
    game: 'oot',
    name: 'Fewer Tunic Requirements (OoT)',
    tooltip: 'Most things that would normally require Tunics no longer will. Fire Temple past the big lava room, Volvagia, Fire Trial without silver rupee shuffle, and Water Temple central pillar still require Tunics.',
  },
  OOT_HIDDEN_GROTTOS: {
    game: 'oot',
    name: 'Hidden Grottos (OoT) without Stone of Agony',
    tooltip: 'All hidden grottos will no longer require the Stone of Agony for logic.',
    linkText: 'https://www.zeldadungeon.net/wiki/Ocarina_of_Time_Secret_Grottos',
  },
  OOT_MIDO_SKIP: {
    game: 'oot',
    name: 'Backflip Over Mido',
    tooltip: 'As Adult, Mido can be backflipped over from the front with a specific angle. It can be done with nothing, but logic will expect a first-person item to make it easier.',
    linkVideo: 'https://www.youtube.com/watch?v=ll8e7h9vFag&ab_channel=ZeldaSpeedRuns',
  },
  OOT_MAN_ON_ROOF: {
    game: 'oot',
    name: 'Man on Roof with Nothing',
    tooltip: 'Child can sidehop off the watchtower to reach this man, and Adult can jump onto the Potion Shop roof from the gate to Death Mountain to then reach the man.',
    linkVideo: 'https://www.youtube.com/watch?v=3-1BY24Fu-Y&ab_channel=dotzo',
  },
  OOT_BLIND_WASTELAND: {
    game: 'oot',
    name: 'Haunted Wasteland Lensless',
    tooltip: 'Turning this on may expect the player to cross the wasteland both ways without the Poe Guide.',
    linkVideo: 'https://www.youtube.com/watch?v=h5_EiPTjdEk&ab_channel=DanielBaamonde',
  },
  OOT_DEKU_SKIP: {
    game: 'oot',
    name: 'Deku Tree B1 Skip',
    tooltip: 'Child is able to jump to the upper portion of the basement without going around.',
    linkVideo: 'https://www.youtube.com/watch?v=TSAk3x-zHcI&ab_channel=AnnoyedTortoise',
  },
  OOT_DC_JUMP: {
    game: 'oot',
    name: "Dodongo's Cavern Upper Adult Jump",
    tooltip: 'Adult can jump to the upper platforms in the Bomb Bag room, removing the need for a projectile.',
    linkVideo: 'https://www.youtube.com/watch?v=LEvlWAvpqfo&ab_channel=I_Buy_Slingshats',
  },
  OOT_FOREST_HOOK: {
    game: 'oot',
    name: 'Forest Temple Garden Vines with Hookshot',
    tooltip: 'Link is able to hookshot to the vines above the east courtyard using only Hookshot.',
    linkVideo: 'https://www.youtube.com/watch?v=Lr3TpVdNg2k&ab_channel=dotzo',
  },
  OOT_HAMMER_WALLS: {
    game: 'oot',
    name: 'Hammer Through Walls',
    tooltip: 'Using the Megaton Hammer against a wall will cause Link to hammer through it. This can be used to hit some switches through walls.',
    linkVideo: 'https://www.youtube.com/watch?v=R18uZdwXRPA&ab_channel=Numbers',
  },
  OOT_VOLCANO_HOVERS: {
    game: 'oot',
    name: 'Volcano Item with Hover Boots',
    tooltip: "Rolling with Hover Boots next to Darunia's room can get you to the volcano without the bean plant.",
    linkVideo: 'https://www.youtube.com/watch?v=rbxaO5lNUTU&ab_channel=StoneeBadLuck',
  },
  OOT_NIGHT_GS: {
    game: 'oot',
    name: "Nighttime Gold Skulltulas without Sun's Song",
    tooltip: "This will make it so that Gold Skulltulas that only appear at night will no longer expect Sun's Song to get, meaning you will need to wait for nightfall.",
  },
  OOT_DOMAIN_CUCCO: {
    game: 'oot',
    name: "Enter Child Zora's Domain with Cucco",
    tooltip: "Child can carry a cucco from the front or plateau of Zora's River and fly behind the waterfall.",
    linkVideo: 'https://www.youtube.com/watch?v=shR5yahUifs&ab_channel=EmptyGarbageCan',
  },
  OOT_DOMAIN_HOVER: {
    game: 'oot',
    name: "Enter Zora's Domain using Hover Boots",
    tooltip: 'Rolling once with Hover Boots at an angle can get you enough distance to get behind the waterfall.',
    linkVideo: 'https://www.youtube.com/watch?v=y4iK6uiw0Xs&ab_channel=ZeldaSpeedRuns',
  },
  OOT_WATER_LONGSHOT: {
    game: 'oot',
    name: 'Drain Water Temple using Longshot',
    tooltip: 'Using Longshot, Link can hook to the torches at the bottom in front of the room he meets Ruto.',
    linkVideo: 'Link Needed',
  },
  OOT_SAND_RIVER_NOTHING: {
    game: 'oot',
    name: 'Cross the River of Sand with Nothing',
    tooltip: 'By backwalking across the left side, one can cross the River of Sand forwards. Going backwards, one can backwalk at a slight angle.',
    linkVideo: 'https://www.youtube.com/watch?v=PLHJY1KRvtg&ab_channel=dotzo',
  },
  OOT_SHADOW_FIRE_ARROW: {
    game: 'oot',
    name: 'Enter Shadow Temple using Fire Arrows',
    tooltip: 'With a precise setup, it is possible to light all of the torches for the Shadow Temple Entrance with a mix of fire arrows and normal arrow. The timing is very quick.',
    linkVideo: 'https://www.youtube.com/watch?v=im3IXe__8ww&ab_channel=Exodus122',
  },
  OOT_KZ_SKIP: {
    game: 'oot',
    name: 'Skip King Zora as Adult',
    tooltip: 'Adult can jump onto the fence next to King Zora and then jump down behind him.',
    linkVideo: 'https://www.youtube.com/watch?v=rfp9MZPgme4&ab_channel=Kryal',
  },
  OOT_LOST_WOODS_ADULT_GS: {
    game: 'oot',
    name: 'Lost Woods Adult GS without Bean',
    tooltip: "This bean can be collected using Hookshot at a precise angle after killing it with Bow, Din's Fire, Bombchus, or Longshot.",
    linkVideo: 'https://www.youtube.com/watch?v=YFLxEw2_PBw&ab_channel=dotzo',
  },
  OOT_WINDMILL_HP_NOTHING: {
    game: 'oot',
    name: 'Windmill HP as Adult with Nothing',
    tooltip: 'Adult can jump off the small stone posts to reach the central spinning platform.',
    linkVideo: 'https://www.youtube.com/watch?v=vDl7lr4Ecv8&ab_channel=dotzo',
  },
  OOT_LAB_DIVE_NO_GOLD_SCALE: {
    game: 'oot',
    name: 'Laboratory Dive without Gold Scale',
    tooltip: 'Using Iron Boots and Hookshot, it is possible to obtain this check without the Golden Scale.',
    linkVideo: 'https://www.youtube.com/watch?v=7ihH-wBP0PM&ab_channel=dotzo',
  },
  OOT_LAB_WALL_GS: {
    game: 'oot',
    name: 'Laboratory Wall GS with Jump Slash',
    tooltip: 'It is possible to obtain this Gold Skulltula by jump slashing it, then jump slashing into the token.',
    linkVideo: 'https://www.youtube.com/watch?v=ThUyxj3psSM&ab_channel=Link98OoT',
  },
  OOT_PASS_COLLISION: {
    game: 'oot',
    name: 'Pass through Visible One-Way Collisions',
    tooltip: "Some walls and gates, despite their appearance, only have collision on one side. Turning this on may expect you to know of these and walk through them. Some examples are the Kakariko Gate as child and the awning behind Impa's House.",
    linkVideo: 'https://www.youtube.com/watch?v=OnnNUZVW8Gw&ab_channel=StoneeBadLuck',
  },
  OOT_DMT_RED_ROCK_GS: {
    game: 'oot',
    name: 'DMT Red Rock Skulls without Hammer',
    tooltip: 'Adult can obtain the Gold Skulltulas on Death Mountain without the Hammer. The upper one can be backflipped into after spin attacking it. The lower one can be hookshotted or jumped onto from above to collect.',
    linkVideo: 'https://www.youtube.com/playlist?list=PL7AtrmSRvw6LAJRTwCEb0VzuOFj9BpPZ-',
  },
  OOT_DEAD_HAND_STICKS: {
    game: 'oot',
    name: 'Child Dead Hand without Kokiri Sword',
    tooltip: 'Fighting Dead Hand as child with Deku Sticks requires all 10 if you only use slashes. This trick removes the mercy of fighting it with a sword.',
    linkVideo: 'https://www.youtube.com/watch?v=gROY5Y66xeA&ab_channel=dotzo',
  },
  OOT_BFA_MUDWALLS: {
    game: 'oot',
    name: 'Break Mud Walls with Blue Fire Arrows',
    tooltip: 'Blue Fire has the ability to break mudwalls. Turning this on will allow the Blue Fire Arrows to be logical for breaking these.',
    linkVideo: 'https://www.youtube.com/watch?v=XXAFs1cfRrc&ab_channel=ThatWelshKaren',
  },
  OOT_JABU_BOSS_HOVER: {
    game: 'oot',
    name: 'Access Jabu-Jabu Pre-Boss using Hover Boots',
    tooltip: 'By using Hover Boots, Link can backwalk off the rising platform while carrying the box to skip needing to kill the Parasites and Big Octo.',
  },
  OOT_SPIRIT_CHILD_HOVER: {
    game: 'oot',
    name: 'Access Adult Spirit as Child using Hover Boots',
    tooltip: 'Using Ageless Hover Boots, Child is able to backwalk off the Armos statue to reach the Adult side.',
  },
  OOT_VALLEY_GATE_HOVER: {
    game: 'oot',
    name: 'Reach Gerudo Fortress as Child using Hover Boots',
    tooltip: 'Using Ageless Hover Boots, it is possible to backwalk off the small pegs near the gate then backflip over it.',
  },
  OOT_MQ_ICE_SCARE_NOTHING: {
    game: 'oot',
    name: 'MQ Ice Cavern GS without Scarecrow or Hover Boots',
    tooltip: 'The Gold Skulltula next to Pierre can be reached using a precise jump.',
  },
  OOT_MQ_ICE_SCARE_HOVER: {
    game: 'oot',
    name: 'MQ Ice Cavern GS using only Hover Boots',
    tooltip: 'The Gold Skulltula next to Pierre can be reached using only Hover Boots and somewhat precise platforming.',
    linkVideo: 'https://www.youtube.com/watch?v=RaggmtTEBuM',
  },
  OOT_MQ_GTG_FLAMES: {
    game: 'oot',
    name: 'MQ Gerudo Training Grounds Lava Room Walk along Flame Circles',
    tooltip: 'It is possible to walk along and jump between the flame rings in the lava room. Footing is very tiny and precise. This can allow reaching the underwater rupees without Fire Arrows and Hookshot.',
  },
  OOT_FORTRESS_JUMPS: {
    game: 'oot',
    name: 'Jump up to higher ledges outside Gerudo Fortress',
    tooltip: 'Outside the Fortress, Adult can jump to the upper vines without needing to go through the kitchen. Child can do the same after going through the kitchen.',
  },
  OOT_COLOSSUS_GS_NO_BEAN: {
    game: 'oot',
    name: 'Desert Colossus Plateau GS without Bean Plant',
    tooltip: 'It is possible to hookshot the Gold Skulltula without using the bean plant, but the Leevers make it a bit difficult to do.',
  },
  OOT_ENTER_JABU: {
    game: 'oot',
    name: 'Enter Jabu-Jabu with Head Collision',
    tooltip: 'Jabu-Jabu has a piece of collision on his head that will trigger the loading zone for the dungeon. Turning this on may expect you to exploit this by using a jump attack, Ageless Hookshot Anywhere, or Climb Anywhere and Hover Boots.',
  },
  OOT_JABU_BOSS_HIGH_SWITCH: {
    game: 'oot',
    name: 'Jabu Boss Switch without Boomerang or Climb Anywhere',
    tooltip: "It is possible to hit the switch before Jabu-Jabu's boss using Bow, Slingshot, Longshot, Bombchu, and Hover Boots + Bombs. Having Climb Anywhere logical makes this trickless.",
  },
  OOT_WATER_REVERSE_RIVER: {
    game: 'oot',
    name: 'Water Temple Reverse River with Hookshot Anywhere',
    tooltip: 'Using Hookshot Anywhere, Link can hook up from the Dragon Statue room, and then hook through the gate blocking the river before it has loaded.',
  },
  OOT_WATER_PIT_GS_CHU: {
    game: 'oot',
    name: 'Water Temple Large Pit GS with Bombchu',
    tooltip: 'It is possible to kill the Gold Skulltula using a Bombchu with a precise angle and timing. This is only relevant if Climb Anywhere is logical.',
  },
  OOT_SHADOW_BOAT_EARLY: {
    game: 'oot',
    name: 'Reach Shadow Temple Boat with Climb Anywhere',
    tooltip: 'From the Spinning Scythes room, Link can climb the fence at the boat shortcut and pop up on top next to Pierre.',
  },
  OOT_SPIRIT_BOSS_CLIMB_NO_HOOK: {
    game: 'oot',
    name: 'Reach Twinrova using Climb Anywhere and Hover Boots',
    tooltip: 'It is possible to reach Twinrova without Hookshot by climbing to the top of the statue, then using Hover Boots and jump slashing to the gate.',
    linkVideo: 'https://www.youtube.com/watch?v=hobCEU1bY-s',
  },
  OOT_ZR_FALLS_HOOK: {
    game: 'oot',
    name: "Use Hookshot Anywhere to get past the Zora's River Waterfall",
    tooltip: 'Hookshot Anywhere can be used next to the shortcut to get behind the waterfall.',
  },
  OOT_DC_BOULDER: {
    game: 'oot',
    name: "Skip Dodongo Cavern's Boulder using Ageless Hookshot Anywhere",
    tooltip: 'Using Ageless Hookshot Anywhere, child can hookshot through the small hole at the bottom right next to the soft soil. Doing this can get you past the boulder without blowing it up.',
  },
  OOT_LAKE_SHORTCUT: {
    game: 'oot',
    name: "Move Between Lake Hylia and Zora's Domain with Ageless Hookshot Anywhere",
    tooltip: "Ageless Hookshot Anywhere can be used to access the shortcuts in Lake Hylia and Zora's Domain, but the angle is a bit precise. Doing this with Longshot is trickless.",
  },
  OOT_JJB_BOXLESS: {
    game: 'oot',
    name: 'Access Jabu-Jabu pre-boss without the box',
    tooltip: 'By doing a jump slash and precise roll, it is possible to get through the door without the box.',
  },
  OOT_TENT_CHILD: {
    game: 'oot',
    name: 'Enter the Gerudo Valley Tent as Child',
    tooltip: "The Gerudo Valley Tent is only present as Adult, however it's entrance is still there as child. Turning this on may expect the player to know where it is and use it for progress in Interior ER.",
  },
  OOT_WELL_ADULT_TT: {
    game: 'oot',
    name: 'Enter Bottom of the Well as Adult with Time Travel',
    tooltip: 'Intended for use with Boss ER and Full Dungeon ER. This trick will allow an Adult Boss to be placed in an MM Dungeon in this location.',
  },
  OOT_ADULT_GALLERY_TT: {
    game: 'oot',
    name: 'Enter Adult Shooting Gallery as Child with Time Travel',
    tooltip:
      'Inteded for use with Interior ER. This will allow interiors where you cannot age swap with Child-only checks to be placed in this location.',
  },
  OOT_GTG_CHILD_TT: {
    game: 'oot',
    name: 'Enter Gerudo Training Grounds as Child with Time Travel',
    tooltip: 'Intended for use with Boss ER and Full Dungeon ER. This trick will allow a Child Boss to be placed in an MM Dungeon in this location.',
  },
  OOT_REVERSE_DAMPE: {
    game: 'oot',
    name: "Navigate Dampe's Grave in Reverse",
    tooltip: "Using Climb Anywhere or Hookshot Anywhere and Age Swap, it is possible to get past the final door of Dampe's Grave and then navigate to the entrance in complete darkness.",
  },
  OOT_GANON_FAIRY_TT: {
    game: 'oot',
    name: "Ganon's Great Fairy with Age Swap and without Gold Gauntlets",
    tooltip: 'By using Age Swap in the child Great Fairy entrance, Adult can walk out of his entrance and then walk right back in to access his Great Fairy entrance.',
  },
  OOT_GANON_CASTLE_ENTRY: {
    game: 'oot',
    name: "Enter Ganon's Castle with Longshot Anywhere",
    tooltip: 'While easy, doing this can bypass a huge chunk of potential logic.',
  },
  OOT_SHADOW_TEMPLE_STICKS: {
    game: 'oot',
    name: 'Enter Shadow Temple with Sticks & Fire Arrows',
    tooltip: 'Turning this on will enable Fire Arrows and Ageless Sticks to enter Shadow Temple in logic.',
    linkVideo: 'Link Needed',
  },
  OOT_WATER_GOLD_SCALE: {
    game: 'oot',
    name: 'Enter Water Temple with Gold Scale and Longshot',
    tooltip: 'It is possible to enter Water Temple without Iron Boots by using the Golden Scale.',
  },
  OOT_COURTYARD_FROM_GANON: {
    game: 'oot',
    name: "Enter Zelda's Courtyard using Age Swap in Ganon's Castle",
    tooltip: "Exiting Ganon's Castle as Child puts you in front of the crawlspace to Zelda. This will allow Age Swap to be logical for doing this.",
  },
  MM_LENS: {
    game: 'mm',
    name: 'Fewer Lens Requirements (MM)',
    tooltip: 'Makes Lens of Truth not a required item for most checks, excluding Shiro (stone mask check) and climbing the wall to Darmani',
  },
  MM_TUNICS: {
    game: 'mm',
    name: 'Fewer Tunic Requirements (MM)',
    tooltip: 'Most things that would normally require Zora Tunic no longer will. Pirate Fortress Sewers and Pinnacle Rock will still require it.',
  },
  MM_PALACE_BEAN_SKIP: {
    game: 'mm',
    name: 'Skip Planting Beans in Deku Palace',
    tooltip: 'Jump onto on of the doorframes to skip planting the beans, removing bottle for this check',
    linkVideo: 'https://www.youtube.com/watch?v=sL30HLF9tAQ&ab_channel=PushyMisumi',
  },
  MM_DARMANI_WALL: {
    game: 'mm',
    name: 'Climb Mountain Village Wall Blind',
    tooltip: 'Climb the Mountain Village wall without Lens of Truth.',
    linkVideo: 'https://www.youtube.com/watch?v=X_IQYkJ49zA&ab_channel=dodo',
  },
  MM_NO_SEAHORSE: {
    game: 'mm',
    name: 'Pinnacle Rock without Seahorse',
    tooltip: 'Go to Pinnacle Rock blind. The Signs are your markers for turns.',
    linkVideo: 'https://www.youtube.com/watch?v=Ye7lu1kSxpQ&ab_channel=Rahylia',
  },
  MM_ZORA_HALL_HUMAN: {
    game: 'mm',
    name: 'Swim to Zora Hall as Human',
    tooltip: "Swim around the back of Zora Hall to get there without Zora' mask",
    linkVideo: 'https://www.youtube.com/watch?v=j4GiWM0xgtA&ab_channel=Rahylia',
  },
  MM_ICELESS_IKANA: {
    game: 'mm',
    name: 'Climb Ikana Canyon without Ice Arrows',
    tooltip: 'With a precise hookshot postition, you can hit the first tree instead of using the ice arrows to get closer',
    linkVideo: 'https://www.youtube.com/watch?v=VtMx8XIE0tA&ab_channel=yogamoanyo',
  },
  MM_ONE_MASK_STONE_TOWER: {
    game: 'mm',
    name: 'Climb Stone Tower with One Mask',
    tooltip: 'Play the Elegy of Emptiness on a specific block to climb a certain portion of Stone Tower',
    linkVideo: 'https://www.youtube.com/watch?v=I3xLE1X9Iiw&t=3s&ab_channel=dodo',
  },
  MM_ISTT_EYEGORE: {
    game: 'mm',
    name: 'Inverted Stone Tower Temple Eyegore Skips',
    tooltip: 'Using either hookshot and light arrows, or bombs for a recoil jump, skip portions of the dungeon using the central bridge with an Eyegore on it',
    linkVideo: 'https://www.youtube.com/watch?v=PxUhL6pombo&list=PLJoiLTDPjgenwpASzbu07cLnUSGZcIUnB&index=52&ab_channel=dodo',
  },
  MM_SCT_NOTHING: {
    game: 'mm',
    name: 'South Clock Town Chest with Nothing',
    tooltip: 'Climb the roof with a precise jump to access the South Clock town Chest',
    linkVideo: 'https://www.youtube.com/watch?v=8_tjO86vRsY&ab_channel=PushyMisumi',
  },
  MM_GORON_BOMB_JUMP: {
    game: 'mm',
    name: 'Bomb Jump Fences as Goron',
    tooltip: 'Using bombs or a keg, use the Goron Pound to leap into the air and get damaged by the explosion to hop over fences',
    linkVideo: 'https://www.youtube.com/watch?v=lLMT2-wNs9w&ab_channel=KokiriKid07',
  },
  MM_BOMBER_GUESS: {
    game: 'mm',
    name: 'Guess Bomber Code',
    tooltip: 'Guess the Bomber Code for Astral Observatory.  There are 120 Combinations. (The Video Link is a backflip over him)',
    linkVideo: 'https://www.youtube.com/watch?v=W6DhLXzJn0A&ab_channel=Tyler%2706',
  },
  MM_CAPTAIN_SKIP: {
    game: 'mm',
    name: 'Guess Oceanside Spider House Code',
    tooltip: 'Guess the code on the masks you hit with arrows.',
  },
  MM_ISTT_ENTRY_JUMP: {
    game: 'mm',
    name: 'Inverted Stone Tower Temple Long Jump to Death Armos',
    tooltip: 'Using a precise bomb long jump, you can make it to the switch on the left side of the room, normally blocked by the wall',
    linkVideo: 'https://www.youtube.com/watch?v=UAG9jbjKwek&ab_channel=Gigopler',
  },
  MM_HARD_HOOKSHOT: {
    game: 'mm',
    name: 'Precise Hookshot Usage',
    tooltip: 'Using the short hookshot gives you logical access to:<br>- the Deku Palace Bean Grotto chest<br>- the Road to Ikana chest and tree',
  },
  MM_PFI_BOAT_HOOK: {
    game: 'mm',
    name: 'Enter Pirate Fortress Interior using Hookshot from the Boats',
    tooltip: 'From the boats, you can make a precise hookshot to a barrel',
  },
  MM_PALACE_GUARD_SKIP: {
    game: 'mm',
    name: 'Backflip over Deku Palace Guards',
    tooltip: 'With a precise backflip on the fence, jump over the guards as human link',
    linkVideo: 'https://www.youtube.com/watch?v=-4F9lzES7tM&ab_channel=Pokemongenius88',
  },
  MM_SHT_HOT_WATER: {
    game: 'mm',
    name: 'Complete Snowhead Temple using Hot Spring Water',
    tooltip: 'Use Hot Spring Water to melt all the ice instead of Fire Arrows or Din\'s Fire.',
  },
  MM_SHT_STICKS_RUN: {
    game: 'mm',
    name: 'Access SHT Pillar Fireless with Precise Stick Run',
    tooltip: 'With a precise path, light the three torches.',
  },
  MM_SHT_PILLARLESS: {
    game: 'mm',
    name: 'Snowhead Temple Skip Raising Pillar',
    tooltip: 'Destroy the snowballs with fire arrows or bombs and then jump down.  A precise jump slash may help getting onto the platform.',
    linkVideo: 'https://youtube.com/clip/UgkxwCrliqwRiCbljfpozhmsnlzVwSUaAI0N',
  },
  MM_KEG_EXPLOSIVES: {
    game: 'mm',
    name: 'Use Powder Kegs as Explosives',
    tooltip: 'Allows Powder Keg to be an alternative to bombs in logic',
  },
  MM_DOG_RACE_CHEST_NOTHING: {
    game: 'mm',
    name: 'Doggy Racetrack Chest with Nothing',
    tooltip: 'Climb the fence and make a precise jump to get to the chest in the Doggy Racetrack area',
    linkVideo: 'https://www.youtube.com/watch?v=DlpBZq579Ds&ab_channel=PushyMisumi',
  },
  MM_MAJORA_LOGIC: {
    game: 'mm',
    name: 'Fight Majora to Reset Time',
    tooltip: 'Access to fight Majora logically counts as a time reset, as an alternative to playing Song of Time',
  },
  MM_SOUTHERN_SWAMP_SCRUB_HP_GORON: {
    game: 'mm',
    name: 'Southern Swamp Scrub HP as Goron',
    tooltip: "Use Goron's ground pound in front of the Tourist Center door to clip through the roof and reach the heart piece",
    linkVideo: 'https://www.youtube.com/watch?v=HvH5NaTsf5w&list=PLuteuW9RqeG0NfhzoiZlJ1HQM2BdHftzg&index=5&ab_channel=Pokemongenius88',
  },
  MM_ZORA_HALL_SCRUB_HP_NO_DEKU: {
    game: 'mm',
    name: 'Zora Hall Scrub HP without Deku',
    tooltip: 'As either Goron or Zora Link, jump up to the heart piece.  Linked playlist has both versions.',
    linkVideo: 'https://www.youtube.com/playlist?list=PL7AtrmSRvw6JxohRTRK61ZGSvuIL5rq0w',
  },
  MM_ZORA_HALL_DOORS: {
    game: 'mm',
    name: 'Access the doors in Zora Hall using Short Hookshot Anywhere',
    tooltip: 'Using Hookshot Anywhere, it is possible to hook behind the doors and open the doors behind them. It is somewhat precise but works with all doors.',
  },
  MM_IKANA_ROOF_PARKOUR: {
    game: 'mm',
    name: "Jump from Ikana Castle's Roof Interior to Exterior",
    tooltip: 'Jump off of a piece of rubble to the slope on the wall, and jump outside the fenced wall.  Walk around the edge to get to the other side.',
    linkVideo: 'https://www.youtube.com/watch?v=9af_q8oYBhA&t=1s',
  },
  MM_IKANA_PILLAR_ENTRANCE_FLOAT: {
    game: 'mm',
    name: "Float from Ikana Castle's Roof to Entrance",
    tooltip: 'Float from the Ikana Castle roof to the main entrance of the courtyard using Deku Mask or Hover Boots',
  },
  MM_IKANA_PILLAR_ENTRANCE_JUMP: {
    game: 'mm',
    name: "Jump from Ikana Castle's Roof to Entrance",
    tooltip: 'Jump from the Ikana Castle roof to the main entrance of the courtyard',
  },
  MM_POST_OFFICE_GAME: {
    game: 'mm',
    name: 'Post Office Timing Game without Bunny Hood',
    tooltip: 'Get good at counting to 10',
    linkVideo: 'https://www.youtube.com/watch?v=bAlGLPkp8go&ab_channel=ZZtheCat2',
  },
  MM_WELL_HSW: {
    game: 'mm',
    name: "Well's Hot Spring Water without Killing Dexihand",
    tooltip: 'Grab the water before the hand grabs you.',
  },
  MM_ISTT_CHUCHU_LESS: {
    game: 'mm',
    name: 'ISTT Block Room without Chuchu Jellies',
    tooltip: 'Normally the room contains Chuchu Jellies to restock your arrows and magic. With this trick on, logic can expect to do this room without the Soul of Chuchus.',
  },
  MM_GBT_WATERWHEEL_GORON: {
    game: 'mm',
    name: 'Cross GBT Waterwheel Room as Goron',
    tooltip: 'Skip the first yellow turnkey with a precise Goron Roll',
  },
  MM_GBT_ENTRANCE_BOW: {
    game: 'mm',
    name: 'Great Bay Temple Entrance Chest using only Bow',
    tooltip: 'Light the four torches using somewhat precise arrow shots',
    linkVideo: 'https://www.youtube.com/watch?v=McXOXYxYiqw',
  },
  MM_OOB_MOVEMENT: {
    game: 'mm',
    name: 'Walk Along Surfaces Out of Bounds',
    tooltip: 'In certain situations, it is required to walk on surfaces that are unable to be reached normally.',
  },
  MM_ST_UPDRAFTS: {
    game: 'mm',
    name: 'Stone Tower Updrafts without Deku Mask',
    tooltip: 'This room can be traversed using Recoil Flips or Goron Mask instead of Deku Mask.',
  },
  MM_ESCAPE_CAGE: {
    game: 'mm',
    name: 'Escape the Monkey Cage with Hookshot Anywhere',
    tooltip: 'For Interior ER, it is possible to hook over the fence and land in a spot where Deku Mask can then be used to leave out the front entrance.',
  },
  MM_GBT_FAIRY2_HOOK: {
    game: 'mm',
    name: 'GBT First Underwater Fairy with Short Hookshot Anywhere',
    tooltip: 'The first Stray Fairy in a bubble can be obtained with a precise angle.',
  },
  MM_GBT_CENTRAL_GEYSER: {
    game: 'mm',
    name: 'GBT Central Room without Zora using Fire & Ice Arrows or an OoT Magic Spell',
    tooltip: 'Using Fire and Ice Arrows or an OOT Spell, it is possible to sink down during the cutscene, then swim into one of the tunnels.',
  },
  MM_BANK_ONE_WALLET: {
    game: 'mm',
    name: 'Bank Rewards Require One Less Wallet',
    tooltip: 'The 500-Rupee item will not require a wallet, and the 1000-rupee item will require one wallet.',
  },
  MM_BANK_NO_WALLET: {
    game: 'mm',
    name: 'Bank Rewards Require No Wallets',
    tooltip: 'All bank items will not require a wallet.',
  },
  MM_CLOCK_TOWER_WAIT: {
    game: 'mm',
    name: 'Wait for the Clock Tower to Open When Shuffled',
    tooltip: 'With this enabled, it may be expected to wait for the Clock Tower to open without a way to quickly advance time.',
  },
  MM_WFT_RUPEES_ICE: {
    game: 'mm',
    name: 'Collect the Pillar Rupees in Woodfall Temple using Ice Arrows',
    tooltip: "The rupees next to Odolwa's door can be jumped to after creating ice platforms in the water.",
  },
  MM_ISTT_RUPEES_GORON: {
    game: 'mm',
    name: 'Collect the Floating Rupees in ISTT as Goron',
    tooltip: 'In the room before Twinmold, Goron can collect the rupees by rolling over them.',
  },
  MM_BOMBER_BACKFLIP: {
    game: 'mm',
    name: 'Backflip over the Bomber in East Clock Town',
    tooltip: 'Just backflip over the kid.',
  },
  MM_NCT_TINGLE: {
    game: 'mm',
    name: 'Jump slash Tingle in North Clock Town',
    tooltip: "Jump off the tree and jump slash Tingle's balloon. Sticks will not work.",
  },
  MM_GBT_FIRELESS: {
    game: 'mm',
    name: 'Great Bay Temple without Fire Arrows',
    tooltip: 'It is possible to traverse the final room in the reverse loop using Zora Mask or Adult Link with a jump slash.',
  },
  MM_IGOS_DINS: {
    game: 'mm',
    name: "Burn Igos' curtains with Din's Fire",
    tooltip: "Igos' curtains can be burned by standing at the top of the stairs next to his throne.",
  },
  MM_BIO_BABA_CHU: {
    game: 'mm',
    name: 'Destroy the Bio Baba Grotto Hive with a Bombchu',
    tooltip: 'The hive in this grotto can be destroyed with a precise bombchu toss.',
  },
  MM_BIO_BABA_LUCK: {
    game: 'mm',
    name: 'Bio Baba Grotto Lilypad Luck',
    tooltip: 'Hope the item lands on the lilypads.',
  },
  MM_WF_SHRINE_HOVERS: {
    game: 'mm',
    name: 'Woodfall Owl Chest with Hover Boots and Jump Slash',
    tooltip: 'The chest can be reached with a tight jump slash at the end of the hover duration.',
  },
  MM_WFT_LOBBY_HOVERS: {
    game: 'mm',
    name: 'Woodfall Temple Lobby with Damage Boost and Hover Boots',
    tooltip: 'The lobby can be traversed by damaging yourself with bombs at certain points. Very difficult.',
  },
  MM_SOARING_ZORA: {
    game: 'mm',
    name: 'Zora Long Jump to the Soaring Tablet',
    tooltip: 'The Soaring Tablet can be long jumped to as Zora from the back poisoned swamp or from the Woodfall entrance.',
  },
  MM_SOARING_HOVERS: {
    game: 'mm',
    name: 'Jump Slash or Damage Boost to the Soaring Tablet with Hover Boots and Bunny Hood',
    tooltip: 'Using Bunny Hood for extra speed, Hover Boots can be used to reach the Soaring Tablet with a tight jump slash or damage boost with explosives.',
  },
  MM_LULLABY_SKIP_IRONS: {
    game: 'mm',
    name: 'Skip playing Goron Lullaby by using Iron Boots',
    tooltip: 'By combining Iron Boots to not be blown by wind and Goron to roll up the slope, Goron Lullaby can be skipped.',
  },
  MM_PATH_SNOWHEAD_HOVERS: {
    game: 'mm',
    name: 'Cross Path to Snowhead using Hover Boots',
    tooltip: 'Hover Boots can be used to slide off the slopes on the side and get across the gaps.',
  },
  MM_GBT_WATERWHEEL_HOVERS: {
    game: 'mm',
    name: 'Cross GBT Waterwheel Room using Hover Boots',
    tooltip: 'Similar to using Goron, Hover Boots can be used to get on the water wheel and then to the central room.',
  },
  MM_GBT_CENTER_POT_IRONS: {
    game: 'mm',
    name: 'Great Bay Temple Center Underwater Pot using only Iron Boots',
    tooltip: 'The pot can be broken by rolling into it and the Stray Fairy collected.',
  },
  MM_GBT_RED1_HOVERS: {
    game: 'mm',
    name: 'Reach the first red pipe in GBT using Hover Boots, Bunny Hood, and a Jump Slash',
    tooltip: 'The red turnkey in front of Wart can be reached with a precise jump slash after the hover duration, using Bunny Hood for extra speed.',
  },
  MM_GBT_GREEN2_UPPER_HOVERS: {
    game: 'mm',
    name: "Reach the Upper Chest in GBT's Second Green Room using Hover Boots",
    tooltip: "The upper chest in the green water wheel room can be reached by using Hover Boots to move between the wheel's blades.",
  },
  MM_GYORG_IRONS: {
    game: 'mm',
    name: 'Fight Gyorg as Human using Iron Boots and Hookshot',
    tooltip: 'Gyorg can be damaged with Hookshot. Turning this on will enable the fight in logic with Iron Boots.',
  },
  MM_STT_LAVA_BLOCK_HOVERS: {
    game: 'mm',
    name: 'Stone Tower Temple Map Chest using Hover Boots',
    tooltip: 'After hooking to the chest near the entrance of the room, one can then hover on top of the sun block.',
  },
  MM_ISTT_ENTRY_HOVER: {
    game: 'mm',
    name: 'Inverted Stone Tower Temple Death Armos using Hover Boots and Bunny Hood',
    tooltip: 'The Death Armos can be reached by hovering and then sidehopping, using Bunny Hood for extra speed.',
  },
  MM_GYORG_POTS_DIVE: {
    game: 'mm',
    name: "Dive Down for Gyorg's Pots with Blast Mask",
    tooltip: 'After defeating Gyorg, dive down and blow up the pots with Blast Mask.',
  },
  MM_STT_POT_BOMBCHU_DIVE: {
    game: 'mm',
    name: 'STT Water Room Shallow Pots Dive with Bombchu',
    tooltip: 'The pots closer to the surface in the water room can be broken using Bombchus, then dived down to.',
  },
  MM_STOCK_POT_WAIT: {
    game: 'mm',
    name: "Wait outside Stock Pot Inn's roof for closing",
    tooltip: 'Normally, logic will not force you to wait on the roof for night inn checks. Enabling this will remove that restriction.',
  },
  MM_STAGE_LIGHTS_DIN: {
    game: 'mm',
    name: "Light the Zora Hall Stage Lights using Din's Fire, Bow, and Hookshot",
    tooltip: "The item can be obtained without Fire Arrows by first hooking to either torch, using Din's Fire, then shooting an arrow to the other torch.",
  },
  MM_RANCH_FARORE: {
    game: 'mm',
    name: "Romani's Ranch locations using Farore's Wind and Time Reset",
    tooltip: "Turning this on will let logic allow placing Farore's Wind then resetting time to obtain these items.",
  },
  MM_EVAN_FARORE: {
    game: 'mm',
    name: "Evan's Piece of Heart using Farore's Wind and Time Reset",
    tooltip: "Turning this on will let logic allow placing Farore's Wind then resetting time to obtain this item.",
  },
  MM_KEG_TRIAL_HEATLESS: {
    game: 'mm',
    name: 'Powder Keg Trial without Thawing Ice using Hookshot Anywhere',
  },
  GLITCH_OOT_EQUIP_SWAP: {
    game: 'oot',
    name: "Equip Swap (OoT)",
    glitch: true,
  },
  GLITCH_OOT_OCARINA_ITEMS: {
    game: 'oot',
    name: "Ocarina Items (OoT)",
    glitch: true,
  },
  GLITCH_OOT_MEGAFLIP: {
    game: 'oot',
    name: "Megaflips (OoT)",
    glitch: true,
  },
};

export const DEFAULT_TRICKS: TrickKey[] = ['OOT_NIGHT_GS'];

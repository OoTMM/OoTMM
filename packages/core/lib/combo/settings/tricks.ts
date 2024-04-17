/*
 * Each trick should have a user friendly name, a tooltip to explain the trick's function within the logic,
 * and if possible, some supporting documentation like a youtube video or article detailing the trick further for beginners.
 */

/* Typing for each individual trick's values */
export type Trick = {
  name: string;
  tooltip: string;
  youtubeLink: string;
  writtenDocumentationLink: string;
};

/* Typing for the whole object of tricks */
export type Tricks = {
  [key: string]: Trick;
};

/** Typing for each key of the TRICKS object */
export type TrickKey = string;

/** Object containing all of the Tricks in the Randomizer
 * Note: "" and "None" as values for tooltip and the links will not render those elements, but they have to be defined
 */
export const TRICKS: Tricks = {
  OOT_LENS: {
    name: 'Fewer Lens Requirements (OoT)',
    tooltip: 'Doing things that would normally require Lens will no longer will, except for Treasure Chest Game and Haunted Wasteland.',
    youtubeLink: 'None',
    writtenDocumentationLink: 'None',
  },
  OOT_TUNICS: {
    name: 'Fewer Tunic Requirements (OoT)',
    tooltip:
      'Most things that would normally require Tunics no longer will. Fire Temple past the big lava room, Volvagia, Fire Trial without silver rupee shuffle, and Water Temple central pillar still require Tunics.',
    youtubeLink: 'None',
    writtenDocumentationLink: 'None',
  },
  OOT_HIDDEN_GROTTOS: {
    name: 'Hidden Grottos (OoT) without Stone of Agony',
    tooltip: 'All hidden grottos will no longer require the Stone of Agony for logic.',
    youtubeLink: 'None',
    writtenDocumentationLink: 'https://www.zeldadungeon.net/wiki/Ocarina_of_Time_Secret_Grottos',
  },
  OOT_MIDO_SKIP: {
    name: 'Backflip Over Mido',
    tooltip:
      'As Adult, Mido can be backflipped over from the front with a specific angle. It can be done with nothing, but logic will expect a first-person item to make it easier.',
    youtubeLink: 'https://www.youtube.com/watch?v=ll8e7h9vFag&ab_channel=ZeldaSpeedRuns',
    writtenDocumentationLink: 'None',
  },
  OOT_MAN_ON_ROOF: {
    name: 'Man on Roof with Nothing',
    tooltip:
      'Child can sidehop off the watchtower to reach this man, and Adult can jump onto the Potion Shop roof from the gate to Death Mountain to then reach the man.',
    youtubeLink: 'https://www.youtube.com/watch?v=3-1BY24Fu-Y&ab_channel=dotzo',
    writtenDocumentationLink: 'None',
  },
  OOT_BLIND_WASTELAND: {
    name: 'Haunted Wasteland Lensless',
    tooltip: 'Turning this on may expect the player to cross the wasteland both ways without the Poe Guide.',
    youtubeLink: 'https://www.youtube.com/watch?v=h5_EiPTjdEk&ab_channel=DanielBaamonde',
    writtenDocumentationLink: 'None',
  },
  OOT_DEKU_SKIP: {
    name: 'Deku Tree B1 Skip',
    tooltip: 'Child is able to jump to the upper portion of the basement without going around.',
    youtubeLink: 'https://www.youtube.com/watch?v=TSAk3x-zHcI&ab_channel=AnnoyedTortoise',
    writtenDocumentationLink: 'None',
  },
  OOT_DC_JUMP: {
    name: "Dodongo's Cavern Upper Adult Jump",
    tooltip: 'Adult can jump to the upper platforms in the Bomb Bag room, removing the need for a projectile.',
    youtubeLink: 'https://www.youtube.com/watch?v=LEvlWAvpqfo&ab_channel=I_Buy_Slingshats',
    writtenDocumentationLink: 'None',
  },
  OOT_FOREST_HOOK: {
    name: 'Forest Temple Garden Vines with Hookshot',
    tooltip: 'Link is able to hookshot to the vines above the east courtyard using only Hookshot.',
    youtubeLink: 'https://www.youtube.com/watch?v=Lr3TpVdNg2k&ab_channel=dotzo',
    writtenDocumentationLink: 'None',
  },
  OOT_HAMMER_WALLS: {
    name: 'Hammer Through Walls',
    tooltip:
      'Using the Megaton Hammer against a wall will cause Link to hammer through it. This can be used to hit some switches through walls.',
    youtubeLink: 'https://www.youtube.com/watch?v=R18uZdwXRPA&ab_channel=Numbers',
    writtenDocumentationLink: 'None',
  },
  OOT_VOLCANO_HOVERS: {
    name: 'Volcano Item with Hover Boots',
    tooltip: "Rolling with Hover Boots next to Darunia's room can get you to the volcano without the bean plant.",
    youtubeLink: 'https://www.youtube.com/watch?v=rbxaO5lNUTU&ab_channel=StoneeBadLuck',
    writtenDocumentationLink: 'None',
  },
  OOT_NIGHT_GS: {
    name: "Nighttime Gold Skulltulas without Sun's Song",
    tooltip:
      "This will make it so that Gold Skulltulas that only appear at night will no longer expect Sun's Song to get, meaning you will need to wait for nightfall.",
    youtubeLink: 'None',
    writtenDocumentationLink: 'None',
  },
  OOT_DOMAIN_CUCCO: {
    name: "Enter Child Zora's Domain with Cucco",
    tooltip: "Child can carry a cucco from the front or plateau of Zora's River and fly behind the waterfall.",
    youtubeLink: 'https://www.youtube.com/watch?v=shR5yahUifs&ab_channel=EmptyGarbageCan',
    writtenDocumentationLink: 'None',
  },
  OOT_DOMAIN_HOVER: {
    name: "Enter Zora's Domain using Hover Boots",
    tooltip: 'Rolling once with Hover Boots at an angle can get you enough distance to get behind the waterfall.',
    youtubeLink: 'https://www.youtube.com/watch?v=y4iK6uiw0Xs&ab_channel=ZeldaSpeedRuns',
    writtenDocumentationLink: 'None',
  },
  OOT_WATER_LONGSHOT: {
    name: 'Drain Water Temple using Longshot',
    tooltip: 'Using Longshot, Link can hook to the torches at the bottom in front of the room he meets Ruto.',
    youtubeLink: 'Link Needed',
    writtenDocumentationLink: 'None',
  },
  OOT_SAND_RIVER_NOTHING: {
    name: 'Cross the River of Sand with Nothing',
    tooltip:
      'By backwalking across the left side, one can cross the River of Sand forwards. Going backwards, one can backwalk at a slight angle.',
    youtubeLink: 'https://www.youtube.com/watch?v=PLHJY1KRvtg&ab_channel=dotzo',
    writtenDocumentationLink: 'None',
  },
  OOT_SHADOW_FIRE_ARROW: {
    name: 'Enter Shadow Temple using Fire Arrows',
    tooltip:
      'With a precise setup, it is possible to light all of the torches for the Shadow Temple Entrance with a mix of fire arrows and normal arrow. The timing is very quick.',
    youtubeLink: 'https://www.youtube.com/watch?v=im3IXe__8ww&ab_channel=Exodus122',
    writtenDocumentationLink: 'None',
  },
  OOT_KZ_SKIP: {
    name: 'Skip King Zora as Adult',
    tooltip: 'Adult can jump onto the fence next to King Zora and then jump down behind him.',
    youtubeLink: 'https://www.youtube.com/watch?v=rfp9MZPgme4&ab_channel=Kryal',
    writtenDocumentationLink: 'None',
  },
  OOT_LOST_WOODS_ADULT_GS: {
    name: 'Lost Woods Adult GS without Bean',
    tooltip: "This bean can be collected using Hookshot at a precise angle after killing it with Bow, Din's Fire, Bombchus, or Longshot.",
    youtubeLink: 'https://www.youtube.com/watch?v=YFLxEw2_PBw&ab_channel=dotzo',
    writtenDocumentationLink: 'None',
  },
  OOT_WINDMILL_HP_NOTHING: {
    name: 'Windmill HP as Adult with Nothing',
    tooltip: 'Adult can jump off the small stone posts to reach the central spinning platform.',
    youtubeLink: 'https://www.youtube.com/watch?v=vDl7lr4Ecv8&ab_channel=dotzo',
    writtenDocumentationLink: 'None',
  },
  OOT_LAB_DIVE_NO_GOLD_SCALE: {
    name: 'Laboratory Dive without Gold Scale',
    tooltip: 'Using Iron Boots and Hookshot, it is possible to obtain this check without the Golden Scale.',
    youtubeLink: 'https://www.youtube.com/watch?v=7ihH-wBP0PM&ab_channel=dotzo',
    writtenDocumentationLink: 'None',
  },
  OOT_LAB_WALL_GS: {
    name: 'Laboratory Wall GS with Jump Slash',
    tooltip: 'It is possible to obtain this Gold Skulltula by jump slashing it, then jump slashing into the token.',
    youtubeLink: 'https://www.youtube.com/watch?v=ThUyxj3psSM&ab_channel=Link98OoT',
    writtenDocumentationLink: 'None',
  },
  OOT_PASS_COLLISION: {
    name: 'Pass through Visible One-Way Collisions',
    tooltip:
      "Some walls and gates, despite their appearance, only have collision on one side. Turning this on may expect you to know of these and walk through them. Some examples are the Kakariko Gate as child and the awning behind Impa's House.",
    youtubeLink: 'https://www.youtube.com/watch?v=OnnNUZVW8Gw&ab_channel=StoneeBadLuck',
    writtenDocumentationLink: 'None',
  },
  OOT_DMT_RED_ROCK_GS: {
    name: 'DMT Red Rock Skulls without Hammer',
    tooltip:
      'Adult can obtain the Gold Skulltulas on Death Mountain without the Hammer. The upper one can be backflipped into after spin attacking it. The lower one can be hookshotted or jumped onto from above to collect.',
    youtubeLink: 'https://www.youtube.com/playlist?list=PL7AtrmSRvw6LAJRTwCEb0VzuOFj9BpPZ-',
    writtenDocumentationLink: 'None',
  },
  OOT_DEAD_HAND_STICKS: {
    name: 'Child Dead Hand without Kokiri Sword',
    tooltip:
      'Fighting Dead Hand as child with Deku Sticks requires all 10 if you only use slashes. This trick removes the mercy of fighting it with a sword.',
    youtubeLink: 'https://www.youtube.com/watch?v=gROY5Y66xeA&ab_channel=dotzo',
    writtenDocumentationLink: 'None',
  },
  OOT_BFA_MUDWALLS: {
    name: 'Break Mud Walls with Blue Fire Arrows',
    tooltip:
      'Blue Fire has the ability to break mudwalls. Turning this on will allow the Blue Fire Arrows to be logical for breaking these.',
    youtubeLink: 'https://www.youtube.com/watch?v=XXAFs1cfRrc&ab_channel=ThatWelshKaren',
    writtenDocumentationLink: 'None',
  },
  OOT_JABU_BOSS_HOVER: {
    name: 'Access Jabu-Jabu Pre-Boss using Hover Boots',
    tooltip:
      'By using Hover Boots, Link can backwalk off the rising platform while carrying the box to skip needing to kill the Parasites and Big Octo.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_SPIRIT_CHILD_HOVER: {
    name: 'Access Adult Spirit as Child using Hover Boots',
    tooltip: 'Using Ageless Hover Boots, Child is able to backwalk off the Armos statue to reach the Adult side.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_VALLEY_GATE_HOVER: {
    name: 'Reach Gerudo Fortress as Child using Hover Boots',
    tooltip: 'Using Ageless Hover Boots, it is possible to backwalk off the small pegs near the gate then backflip over it.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_MQ_ICE_SCARE_NOTHING: {
    name: 'MQ Ice Cavern GS without Scarecrow or Hover Boots',
    tooltip: 'The Gold Skulltula next to Pierre can be reached using a precise jump.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_MQ_ICE_SCARE_HOVER: {
    name: 'MQ Ice Cavern GS using only Hover Boots',
    tooltip: 'The Gold Skulltula next to Pierre can be reached using only Hover Boots and somewhat precise platforming.',
    youtubeLink: 'https://www.youtube.com/watch?v=RaggmtTEBuM',
    writtenDocumentationLink: 'None',
  },
  OOT_MQ_GTG_FLAMES: {
    name: 'MQ Gerudo Training Grounds Lava Room Walk along Flame Circles',
    tooltip:
      'It is possible to walk along and jump between the flame rings in the lava room. Footing is very tiny and precise. This can allow reaching the underwater rupees without Fire Arrows and Hookshot.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_FORTRESS_JUMPS: {
    name: 'Jump up to higher ledges outside Gerudo Fortress',
    tooltip:
      'Outside the Fortress, Adult can jump to the upper vines without needing to go through the kitchen. Child can do the same after going through the kitchen.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_COLOSSUS_GS_NO_BEAN: {
    name: 'Desert Colossus Plateau GS without Bean Plant',
    tooltip: 'It is possible to hookshot the Gold Skulltula without using the bean plant, but the Leevers make it a bit difficult to do.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_ENTER_JABU: {
    name: 'Enter Jabu-Jabu with Head Collision',
    tooltip:
      'Jabu-Jabu has a piece of collision on his head that will trigger the loading zone for the dungeon. Turning this on may expect you to exploit this by using a jump attack, Ageless Hookshot Anywhere, or Climb Anywhere and Hover Boots.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_JABU_BOSS_HIGH_SWITCH: {
    name: 'Jabu Boss Switch without Boomerang or Climb Anywhere',
    tooltip:
      "It is possible to hit the switch before Jabu-Jabu's boss using Bow, Slingshot, Longshot, Bombchu, and Hover Boots + Bombs. Having Climb Anywhere logical makes this trickless.",
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_WATER_REVERSE_RIVER: {
    name: 'Water Temple Reverse River with Hookshot Anywhere',
    tooltip:
      'Using Hookshot Anywhere, Link can hook up from the Dragon Statue room, and then hook through the gate blocking the river before it has loaded.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_WATER_PIT_GS_CHU: {
    name: 'Water Temple Large Pit GS with Bombchu',
    tooltip:
      'It is possible to kill the Gold Skulltula using a Bombchu with a precise angle and timing. This is only relevant if Climb Anywhere is logical.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_SHADOW_BOAT_EARLY: {
    name: 'Reach Shadow Temple Boat with Climb Anywhere',
    tooltip: 'From the Spinning Scythes room, Link can climb the fence at the boat shortcut and pop up on top next to Pierre.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_SPIRIT_BOSS_CLIMB_NO_HOOK: {
    name: 'Reach Twinrova using Climb Anywhere and Hover Boots',
    tooltip:
      'It is possible to reach Twinrova without Hookshot by climbing to the top of the statue, then using Hover Boots and jump slashing to the gate.',
    youtubeLink: 'https://www.youtube.com/watch?v=hobCEU1bY-s',
    writtenDocumentationLink: 'None',
  },
  OOT_ZR_FALLS_HOOK: {
    name: "Use Hookshot Anywhere to get past the Zora's River Waterfall",
    tooltip: 'Hookshot Anywhere can be used next to the shortcut to get behind the waterfall.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_DC_BOULDER: {
    name: "Skip Dodongo Cavern's Boulder using Ageless Hookshot Anywhere",
    tooltip:
      'Using Ageless Hookshot Anywhere, child can hookshot through the small hole at the bottom right next to the soft soil. Doing this can get you past the boulder without blowing it up.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_LAKE_SHORTCUT: {
    name: "Move Between Lake Hylia and Zora's Domain with Ageless Hookshot Anywhere",
    tooltip:
      "Ageless Hookshot Anywhere can be used to access the shortcuts in Lake Hylia and Zora's Domain, but the angle is a bit precise. Doing this with Longshot is trickless.",
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_JJB_BOXLESS: {
    name: 'Access Jabu-Jabu pre-boss without the box',
    tooltip: 'By doing a jump slash and precise roll, it is possible to get through the door without the box.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_TENT_CHILD: {
    name: 'Enter the Gerudo Valley Tent as Child',
    tooltip:
      "The Gerudo Valley Tent is only present as Adult, however it's entrance is still there as child. Turning this on may expect the player to know where it is and use it for progress in Interior ER.",
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_WELL_ADULT_TT: {
    name: 'Enter Bottom of the Well as Adult with Time Travel',
    tooltip:
      'Intended for use with Boss ER and Full Dungeon ER. This trick will allow an Adult Boss to be placed in an MM Dungeon in this location.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_ADULT_GALLERY_TT: {
    name: 'Enter Adult Shooting Gallery as Child with Time Travel',
    tooltip:
      'Inteded for use with Interior ER. This will allow interiors where you cannot age swap with Child-only checks to be placed in this location.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_GTG_CHILD_TT: {
    name: 'Enter Gerudo Training Grounds as Child with Time Travel',
    tooltip:
      'Intended for use with Boss ER and Full Dungeon ER. This trick will allow a Child Boss to be placed in an MM Dungeon in this location.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_REVERSE_DAMPE: {
    name: "Navigate Dampe's Grave in Reverse",
    tooltip:
      "Using Climb Anywhere or Hookshot Anywhere and Age Swap, it is possible to get past the final door of Dampe's Grave and then navigate to the entrance in complete darkness.",
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_GANON_FAIRY_TT: {
    name: "Ganon's Great Fairy with Age Swap and without Gold Gauntlets",
    tooltip:
      'By using Age Swap in the child Great Fairy entrance, Adult can walk out of his entrance and then walk right back in to access his Great Fairy entrance.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_GANON_CASTLE_ENTRY: {
    name: "Enter Ganon's Castle with Longshot Anywhere",
    tooltip: 'While easy, doing this can bypass a huge chunk of potential logic.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_SHADOW_TEMPLE_STICKS: {
    name: 'Enter Shadow Temple with Sticks & Fire Arrows',
    tooltip: 'Turning this on will enable Fire Arrows and Ageless Sticks to enter Shadow Temple in logic.',
    youtubeLink: 'Link Needed',
    writtenDocumentationLink: 'None',
  },
  OOT_WATER_GOLD_SCALE: {
    name: 'Enter Water Temple with Gold Scale and Longshot',
    tooltip: 'It is possible to enter Water Temple without Iron Boots by using the Golden Scale.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  OOT_COURTYARD_FROM_GANON: {
    name: "Enter Zelda's Courtyard using Age Swap in Ganon's Castle",
    tooltip:
      "Exiting Ganon's Castle as Child puts you in front of the crawlspace to Zelda. This will allow Age Swap to be logical for doing this.",
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_LENS: {
    name: 'Fewer Lens Requirements (MM)',
    tooltip: 'Makes Lens of Truth not a required item for most checks, excluding Shiro (stone mask check) and climbing the wall to Darmani',
    youtubeLink: 'None',
    writtenDocumentationLink: 'None',
  },
  MM_TUNICS: {
    name: 'Fewer Tunic Requirements (MM)',
    tooltip:
      'Most things that would normally require Zora Tunic no longer will. Pirate Fortress Sewers and Pinnacle Rock will still require it.',
    youtubeLink: 'None',
    writtenDocumentationLink: 'None',
  },
  MM_PALACE_BEAN_SKIP: {
    name: 'Skip Planting Beans in Deku Palace',
    tooltip: 'Jump onto on of the doorframes to skip planting the beans, removing bottle for this check',
    youtubeLink: 'https://www.youtube.com/watch?v=sL30HLF9tAQ&ab_channel=PushyMisumi',
    writtenDocumentationLink: 'None',
  },
  MM_DARMANI_WALL: {
    name: 'Climb Mountain Village Wall Blind',
    tooltip: 'Climb the Mountain Village wall without Lens of Truth.',
    youtubeLink: 'https://www.youtube.com/watch?v=X_IQYkJ49zA&ab_channel=dodo',
    writtenDocumentationLink: 'None',
  },
  MM_NO_SEAHORSE: {
    name: 'Pinnacle Rock without Seahorse',
    tooltip: 'Go to Pinnacle Rock blind. The Signs are your markers for turns.',
    youtubeLink: 'https://www.youtube.com/watch?v=Ye7lu1kSxpQ&ab_channel=Rahylia',
    writtenDocumentationLink: 'None',
  },
  MM_ZORA_HALL_HUMAN: {
    name: 'Swim to Zora Hall as Human',
    tooltip: "Swim around the back of Zora Hall to get there without Zora' mask",
    youtubeLink: 'https://www.youtube.com/watch?v=j4GiWM0xgtA&ab_channel=Rahylia',
    writtenDocumentationLink: 'None',
  },
  MM_ICELESS_IKANA: {
    name: 'Climb Ikana Canyon without Ice Arrows',
    tooltip: 'With a precise hookshot postition, you can hit the first tree instead of using the ice arrows to get closer',
    youtubeLink: 'https://www.youtube.com/watch?v=VtMx8XIE0tA&ab_channel=yogamoanyo',
    writtenDocumentationLink: 'None',
  },
  MM_ONE_MASK_STONE_TOWER: {
    name: 'Climb Stone Tower with One Mask',
    tooltip: 'Play the Elegy of Emptiness on a specific block to climb a certain portion of Stone Tower',
    youtubeLink: 'https://www.youtube.com/watch?v=I3xLE1X9Iiw&t=3s&ab_channel=dodo',
    writtenDocumentationLink: 'None',
  },
  MM_ISTT_EYEGORE: {
    name: 'Inverted Stone Tower Temple Eyegore Skips',
    tooltip:
      'Using either hookshot and light arrows, or bombs for a recoil jump, skip portions of the dungeon using the central bridge with an Eyegore on it',
    youtubeLink: 'https://www.youtube.com/watch?v=PxUhL6pombo&list=PLJoiLTDPjgenwpASzbu07cLnUSGZcIUnB&index=52&ab_channel=dodo',
    writtenDocumentationLink: 'None',
  },
  MM_SCT_NOTHING: {
    name: 'South Clock Town Chest with Nothing',
    tooltip: 'Climb the roof with a precise jump to access the South Clock town Chest',
    youtubeLink: 'https://www.youtube.com/watch?v=8_tjO86vRsY&ab_channel=PushyMisumi',
    writtenDocumentationLink: 'None',
  },
  MM_GORON_BOMB_JUMP: {
    name: 'Bomb Jump Fences as Goron',
    tooltip: 'Using bombs or a keg, use the Goron Pound to leap into the air and get damaged by the explosion to hop over fences',
    youtubeLink: 'https://www.youtube.com/watch?v=lLMT2-wNs9w&ab_channel=KokiriKid07',
    writtenDocumentationLink: 'None',
  },
  MM_BOMBER_GUESS: {
    name: 'Guess Bomber Code',
    tooltip: 'Guess the Bomber Code for Astral Observatory.  There are 120 Combinations. (The Video Link is a backflip over him)',
    youtubeLink: 'https://www.youtube.com/watch?v=W6DhLXzJn0A&ab_channel=Tyler%2706',
    writtenDocumentationLink: 'None',
  },
  MM_CAPTAIN_SKIP: {
    name: 'Guess Oceanside Spider House Code',
    tooltip: 'Guess the code on the masks you hit with arrows.',
    youtubeLink: 'None',
    writtenDocumentationLink: 'None',
  },
  MM_ISTT_ENTRY_JUMP: {
    name: 'Inverted Stone Tower Temple Long Jump to Death Armos',
    tooltip: 'Using a precise bomb long jump, you can make it to the switch on the left side of the room, normally blocked by the wall',
    youtubeLink: 'https://www.youtube.com/watch?v=UAG9jbjKwek&ab_channel=Gigopler',
    writtenDocumentationLink: 'None',
  },
  MM_HARD_HOOKSHOT: {
    name: 'Precise Hookshot Usage',
    tooltip:
      'Using the short hookshot gives you logical access to:<br>- the Deku Palace Bean Grotto chest<br>- the Road to Ikana chest and tree',
    youtubeLink: 'Needs Link',
    writtenDocumentationLink: 'None',
  },
  MM_PFI_BOAT_HOOK: {
    name: 'Enter Pirate Fortress Interior using Hookshot from the Boats',
    tooltip: 'From the boats, you can make a precise hookshot to a barrel',
    youtubeLink: 'Needs Link',
    writtenDocumentationLink: 'None',
  },
  MM_PALACE_GUARD_SKIP: {
    name: 'Backflip over Deku Palace Guards',
    tooltip: 'With a precise backflip on the fence, jump over the guards as human link',
    youtubeLink: 'https://www.youtube.com/watch?v=-4F9lzES7tM&ab_channel=Pokemongenius88',
    writtenDocumentationLink: 'None',
  },
  MM_SHT_FIRELESS: {
    name: 'Complete Snowhead Temple without Fire Arrows',
    tooltip: 'Using torches and jumps, melt the ice with normal arrows',
    youtubeLink: 'https://youtu.be/arkzqsYWHWg?list=PL0NLeF-0sduxYgyvvamWsX9IfZpd4smNr',
    writtenDocumentationLink: 'None',
  },
  MM_SHT_STICKS_RUN: {
    name: 'Access SHT Pillar Fireless with Precise Stick Run',
    tooltip: 'With a precise path, light the three torches (or use flame storage for a glitched run)',
    youtubeLink: 'https://www.youtube.com/watch?v=vuP_vRb6ZTI&ab_channel=Pokemongenius88',
    writtenDocumentationLink: 'None',
  },
  MM_SHT_PILLARLESS: {
    name: 'Snowhead Temple Skip Raising Pillar',
    tooltip:
      'Destroy the snowballs with fire arrows or bombs and then jump down.  A precise jump slash may help getting onto the platform.',
    youtubeLink: 'https://youtube.com/clip/UgkxwCrliqwRiCbljfpozhmsnlzVwSUaAI0N',
    writtenDocumentationLink: 'None',
  },
  MM_KEG_EXPLOSIVES: {
    name: 'Use Powder Kegs as Explosives',
    tooltip: 'Allows Powder Keg to be an alternative to bombs in logic',
    youtubeLink: 'None',
    writtenDocumentationLink: 'None',
  },
  MM_DOG_RACE_CHEST_NOTHING: {
    name: 'Doggy Racetrack Chest with Nothing',
    tooltip: 'Climb the fence and make a precise jump to get to the chest in the Doggy Racetrack area',
    youtubeLink: 'https://www.youtube.com/watch?v=DlpBZq579Ds&ab_channel=PushyMisumi',
    writtenDocumentationLink: 'None',
  },
  MM_MAJORA_LOGIC: {
    name: 'Fight Majora to Reset Time',
    tooltip: 'Access to fight Majora logically counts as a time reset, as an alternative to playing Song of Time',
    youtubeLink: 'None',
    writtenDocumentationLink: 'None',
  },
  MM_SOUTHERN_SWAMP_SCRUB_HP_GORON: {
    name: 'Southern Swamp Scrub HP as Goron',
    tooltip: "Use Goron's ground pound in front of the Tourist Center door to clip through the roof and reach the heart piece",
    youtubeLink: 'https://www.youtube.com/watch?v=HvH5NaTsf5w&list=PLuteuW9RqeG0NfhzoiZlJ1HQM2BdHftzg&index=5&ab_channel=Pokemongenius88',
    writtenDocumentationLink: 'None',
  },
  MM_ZORA_HALL_SCRUB_HP_NO_DEKU: {
    name: 'Zora Hall Scrub HP without Deku',
    tooltip: 'As either Goron or Zora Link, jump up to the heart piece.  Linked playlist has both versions.',
    youtubeLink: 'https://www.youtube.com/playlist?list=PL7AtrmSRvw6JxohRTRK61ZGSvuIL5rq0w',
    writtenDocumentationLink: 'None',
  },
  MM_ZORA_HALL_DOORS: {
    name: 'Access the doors in Zora Hall using Short Hookshot Anywhere',
    tooltip:
      'Using Hookshot Anywhere, it is possible to hook behind the doors and open the doors behind them. It is somewhat precise but works with all doors.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_IKANA_ROOF_PARKOUR: {
    name: "Jump from Ikana Castle's Roof Interior to Exterior",
    tooltip:
      'Jump off of a piece of rubble to the slope on the wall, and jump outside the fenced wall.  Walk around the edge to get to the other side.',
    youtubeLink: 'https://www.youtube.com/watch?v=9af_q8oYBhA&t=1s',
    writtenDocumentationLink: 'None',
  },
  MM_IKANA_PILLAR_TO_ENTRANCE: {
    name: "Jump from Ikana Castle's Roof to Entrance",
    tooltip: 'Jump from the Ikana Castle roof to the main entrance of the courtyard',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_POST_OFFICE_GAME: {
    name: 'Post Office Timing Game without Bunny Hood',
    tooltip: 'Get good at counting to 10',
    youtubeLink: 'https://www.youtube.com/watch?v=bAlGLPkp8go&ab_channel=ZZtheCat2',
    writtenDocumentationLink: 'None',
  },
  MM_WELL_HSW: {
    name: "Well's Hot Spring Water without Killing Dexihand",
    tooltip: 'Grab the water before the hand grabs you.',
    youtubeLink: 'Needs Link',
    writtenDocumentationLink: 'None',
  },
  MM_ISTT_CHUCHU_LESS: {
    name: 'ISTT Block Room without Chuchu Jellies',
    tooltip:
      'Normally the room contains Chuchu Jellies to restock your arrows and magic. With this trick on, logic can expect to do this room without the Soul of Chuchus.',
    youtubeLink: 'Needs Link',
    writtenDocumentationLink: 'None',
  },
  MM_GBT_WATERWHEEL_GORON: {
    name: 'Cross GBT Waterwheel Room as Goron',
    tooltip: 'Skip the first yellow turnkey with a precise Goron Roll',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_GBT_ENTRANCE_BOW: {
    name: 'Great Bay Temple Entrance Chest using only Bow',
    tooltip: 'Light the four torches using somewhat precise arrow shots',
    youtubeLink: 'https://www.youtube.com/watch?v=McXOXYxYiqw',
    writtenDocumentationLink: 'None',
  },
  MM_OOB_MOVEMENT: {
    name: 'Walk Along Surfaces Out of Bounds',
    tooltip: 'In certain situations, it is required to walk on surfaces that are unable to be reached normally.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_ST_UPDRAFTS: {
    name: 'Stone Tower Updrafts without Deku Mask',
    tooltip: 'This room can be traversed using Recoil Flips or Goron Mask instead of Deku Mask.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_ESCAPE_CAGE: {
    name: 'Escape the Monkey Cage with Hookshot Anywhere',
    tooltip:
      'For Interior ER, it is possible to hook over the fence and land in a spot where Deku Mask can then be used to leave out the front entrance.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_GBT_FAIRY2_HOOK: {
    name: 'GBT First Underwater Fairy with Short Hookshot Anywhere',
    tooltip: 'The first Stray Fairy in a bubble can be obtained with a precise angle.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_GBT_CENTRAL_GEYSER: {
    name: 'GBT Central Room without Zora using Fire & Ice Arrows or an OoT Magic Spell',
    tooltip:
      'Using Fire and Ice Arrows or an OOT Spell, it is possible to sink down during the cutscene, then swim into one of the tunnels.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_BANK_ONE_WALLET: {
    name: 'Bank Rewards Require One Less Wallet',
    tooltip: 'The 500-Rupee item will not require a wallet, and the 1000-rupee item will require one wallet.',
    youtubeLink: 'None',
    writtenDocumentationLink: 'None',
  },
  MM_BANK_NO_WALLET: {
    name: 'Bank Rewards Require No Wallets',
    tooltip: 'All bank items will not require a wallet.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_CLOCK_TOWER_WAIT: {
    name: 'Wait for the Clock Tower to Open When Shuffled',
    tooltip: 'With this enabled, it may be expected to wait for the Clock Tower to open without a way to quickly advance time.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_WFT_RUPEES_ICE: {
    name: 'Collect the Pillar Rupees in Woodfall Temple using Ice Arrows',
    tooltip: "The rupees next to Odolwa's door can be jumped to after creating ice platforms in the water.",
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_ISTT_RUPEES_GORON: {
    name: 'Collect the Floating Rupees in ISTT as Goron',
    tooltip: 'In the room before Twinmold, Goron can collect the rupees by rolling over them.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_BOMBER_BACKFLIP: {
    name: 'Backflip over the Bomber in East Clock Town',
    tooltip: 'Just backflip over the kid.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_NCT_TINGLE: {
    name: 'Jump slash Tingle in North Clock Town',
    tooltip: "Jump off the tree and jump slash Tingle's balloon. Sticks will not work.",
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_GBT_FIRELESS: {
    name: 'Great Bay Temple without Fire Arrows',
    tooltip: 'It is possible to traverse the final room in the reverse loop using Zora Mask or Adult Link with a jump slash.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_IGOS_DINS: {
    name: "Burn Igos' curtains with Din's Fire",
    tooltip: "Igos' curtains can be burned by standing at the top of the stairs next to his throne.",
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_BIO_BABA_CHU: {
    name: 'Destroy the Bio Baba Grotto Hive with a Bombchu',
    tooltip: 'The hive in this grotto can be destroyed with a precise bombchu toss.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_BIO_BABA_LUCK: {
    name: 'Bio Baba Grotto Lilypad Luck',
    tooltip: 'Hope the item lands on the lilypads.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_WF_SHRINE_HOVERS: {
    name: 'Woodfall Owl Chest with Hover Boots and Jump Slash',
    tooltip: 'The chest can be reached with a tight jump slash at the end of the hover duration.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_WFT_LOBBY_HOVERS: {
    name: 'Woodfall Temple Lobby with Damage Boost and Hover Boots',
    tooltip: 'The lobby can be traversed by damaging yourself with bombs at certain points. Very difficult.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_SOARING_ZORA: {
    name: 'Zora Long Jump to the Soaring Tablet',
    tooltip: 'The Soaring Tablet can be long jumped to as Zora from the back poisoned swamp or from the Woodfall entrance.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_SOARING_HOVERS: {
    name: 'Jump Slash or Damage Boost to the Soaring Tablet with Hover Boots and Bunny Hood',
    tooltip:
      'Using Bunny Hood for extra speed, Hover Boots can be used to reach the Soaring Tablet with a tight jump slash or damage boost with explosives.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_LULLABY_SKIP_IRONS: {
    name: 'Skip playing Goron Lullaby by using Iron Boots',
    tooltip: 'By combining Iron Boots to not be blown by wind and Goron to roll up the slope, Goron Lullaby can be skipped.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_PATH_SNOWHEAD_HOVERS: {
    name: 'Cross Path to Snowhead using Hover Boots',
    tooltip: 'Hover Boots can be used to slide off the slopes on the side and get across the gaps.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_GBT_WATERWHEEL_HOVERS: {
    name: 'Cross GBT Waterwheel Room using Hover Boots',
    tooltip: 'Similar to using Goron, Hover Boots can be used to get on the water wheel and then to the central room.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_GBT_CENTER_POT_IRONS: {
    name: 'Great Bay Temple Center Underwater Pot using only Iron Boots',
    tooltip: 'The pot can be broken by rolling into it and the Stray Fairy collected.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_GBT_RED1_HOVERS: {
    name: 'Reach the first red pipe in GBT using Hover Boots, Bunny Hood, and a Jump Slash',
    tooltip:
      'The red turnkey in front of Wart can be reached with a precise jump slash after the hover duration, using Bunny Hood for extra speed.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_GBT_GREEN2_UPPER_HOVERS: {
    name: "Reach the Upper Chest in GBT's Second Green Room using Hover Boots",
    tooltip: "The upper chest in the green water wheel room can be reached by using Hover Boots to move between the wheel's blades.",
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_GYORG_IRONS: {
    name: 'Fight Gyorg as Human using Iron Boots and Hookshot',
    tooltip: 'Gyorg can be damaged with Hookshot. Turning this on will enable the fight in logic with Iron Boots.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_STT_LAVA_BLOCK_HOVERS: {
    name: 'Stone Tower Temple Map Chest using Hover Boots',
    tooltip: 'After hooking to the chest near the entrance of the room, one can then hover on top of the sun block.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_ISTT_ENTRY_HOVER: {
    name: 'Inverted Stone Tower Temple Death Armos using Hover Boots and Bunny Hood',
    tooltip: 'The Death Armos can be reached by hovering and then sidehopping, using Bunny Hood for extra speed.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_GYORG_POTS_DIVE: {
    name: "Dive Down for Gyorg's Pots with Blast Mask",
    tooltip: 'After defeating Gyorg, dive down and blow up the pots with Blast Mask.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_STT_POT_BOMBCHU_DIVE: {
    name: 'STT Water Room Shallow Pots Dive with Bombchu',
    tooltip: 'The pots closer to the surface in the water room can be broken using Bombchus, then dived down to.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_STOCK_POT_WAIT: {
    name: "Wait outside Stock Pot Inn's roof for closing",
    tooltip: 'Normally, logic will not force you to wait on the roof for night inn checks. Enabling this will remove that restriction.',
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_STAGE_LIGHTS_DIN: {
    name: "Light the Zora Hall Stage Lights using Din's Fire, Bow, and Hookshot",
    tooltip: "The item can be obtained without Fire Arrows by first hooking to either torch, using Din's Fire, then shooting an arrow to the other torch.",
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_RANCH_FARORE: {
    name: "Romani's Ranch locations using Farore's Wind and Time Reset",
    tooltip: "Turning this on will let logic allow placing Farore's Wind then resetting time to obtain these items.",
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_EVAN_FARORE: {
    name: "Evan's Piece of Heart using Farore's Wind and Time Reset",
    tooltip: "Turning this on will let logic allow placing Farore's Wind then resetting time to obtain this item.",
    youtubeLink: '',
    writtenDocumentationLink: 'None',
  },
  MM_KEG_TRIAL_HEATLESS: {
    name: 'Powder Keg Trial without Thawing Ice using Hookshot Anywhere',
    tooltip: '',
    youtubeLink: 'None',
    writtenDocumentationLink: 'None',
  },
};

export const DEFAULT_TRICKS: TrickKey[] = ['OOT_NIGHT_GS'];

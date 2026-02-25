#ifndef COMBO_CAMERA_H
#define COMBO_CAMERA_H

#if defined(GAME_OOT)
typedef enum CameraSettingType {
    /* 0x00 */ CAM_SET_NONE,
    /* 0x01 */ CAM_SET_NORMAL0,
    /* 0x02 */ CAM_SET_NORMAL1,
    /* 0x03 */ CAM_SET_DUNGEON0,
    /* 0x04 */ CAM_SET_DUNGEON1,
    /* 0x05 */ CAM_SET_NORMAL3,
    /* 0x06 */ CAM_SET_HORSE, // "HORSE0"
    /* 0x07 */ CAM_SET_BOSS_GOHMA, // "BOSS_GOMA" (unused)
    /* 0x08 */ CAM_SET_BOSS_DODONGO, // "BOSS_DODO" (unused)
    /* 0x09 */ CAM_SET_BOSS_BARINADE, // "BOSS_BARI" (unused)
    /* 0x0A */ CAM_SET_BOSS_PHANTOM_GANON, // "BOSS_FGANON"
    /* 0x0B */ CAM_SET_BOSS_VOLVAGIA, // "BOSS_BAL"
    /* 0x0C */ CAM_SET_BOSS_BONGO, // "BOSS_SHADES"
    /* 0x0D */ CAM_SET_BOSS_MORPHA, // "BOSS_MOFA" (unused)
    /* 0x0E */ CAM_SET_BOSS_TWINROVA_PLATFORM, // Upper main platform and 4 smaller platforms in the room of the Twinrova boss battle "TWIN0"
    /* 0x0F */ CAM_SET_BOSS_TWINROVA_FLOOR, // The floor in the room of the Twinrova boss battle "TWIN1"
    /* 0x10 */ CAM_SET_BOSS_GANONDORF, // "BOSS_GANON1"
    /* 0x11 */ CAM_SET_BOSS_GANON, // "BOSS_GANON2" (unused)
    /* 0x12 */ CAM_SET_TOWER_CLIMB, // Various climbing structures (collapse sequence stairs, spiral around sarias house, zora domain climb, etc...) "TOWER0"
    /* 0x13 */ CAM_SET_TOWER_UNUSED, // Unused but data is in Phantom Ganon's Lair (no surface uses it) "TOWER1"
    /* 0x14 */ CAM_SET_MARKET_BALCONY, // Activated in day child market by talking to NPC on balcony above bombchu bowling "FIXED0"
    /* 0x15 */ CAM_SET_CHU_BOWLING, // Fixes the camera to the bombchu bowling targets while playing the minigame "FIXED1"
    /* 0x16 */ CAM_SET_PIVOT_CRAWLSPACE, // Unknown. In scene data: closely associated with crawlspaces CIRCLE0"
    /* 0x17 */ CAM_SET_PIVOT_SHOP_BROWSING, // Shopping and browsing for items "CIRCLE2"
    /* 0x18 */ CAM_SET_PIVOT_IN_FRONT, // The camera used on Link's balcony in Kokiri forest. Data present in scene data for Deku Tree, GTG, Inside Ganon's Castle (TODO: may or may not be used) "CIRCLE3"
    /* 0x19 */ CAM_SET_PREREND_FIXED, // Camera is fixed in position and rotation "PREREND0"
    /* 0x1A */ CAM_SET_PREREND_PIVOT, // Camera is fixed in position with fixed pitch, but is free to rotate in the yaw direction 360 degrees "PREREND1"
    /* 0x1B */ CAM_SET_PREREND_SIDE_SCROLL, // Camera side-scrolls position to follow the player. Only used in castle courtyard with the guards "PREREND3"
    /* 0x1C */ CAM_SET_DOOR0, // Custom room door transitions, used in fire and royal family tomb
    /* 0x1D */ CAM_SET_DOORC, // Generic room door transitions, camera moves and follows player as the door is open and closed
    /* 0x1E */ CAM_SET_CRAWLSPACE, // Used in all crawlspaces "RAIL3"
    /* 0x1F */ CAM_SET_START0, // Data is given in Temple of Time, but no surface uses it
    /* 0x20 */ CAM_SET_START1, // Scene/room door transitions that snap the camera to a fixed location (example: ganon's towers doors climbing up)
    /* 0x21 */ CAM_SET_FREE0, // Full manual control is given over the camera
    /* 0x22 */ CAM_SET_FREE2, // Various OnePoint Cutscenes, 10 total (example: falling chest)
    /* 0x23 */ CAM_SET_PIVOT_CORNER, // Inside the carpenter jail cells from thieves hideout "CIRCLE4"
    /* 0x24 */ CAM_SET_PIVOT_WATER_SURFACE, // Player diving from the surface of the water to underwater "CIRCLE5"
    /* 0x25 */ CAM_SET_CS_0, // Various cutscenes "DEMO0"
    /* 0x26 */ CAM_SET_CS_TWISTED_HALLWAY, // Never set to, but checked in twisting hallway (Forest Temple) "DEMO1"
    /* 0x27 */ CAM_SET_FOREST_BIRDS_EYE, // Used in the falling ceiling room in forest temple "MORI1"
    /* 0x28 */ CAM_SET_SLOW_CHEST_CS, // Long cutscene when opening a big chest with a major item "ITEM0"
    /* 0x29 */ CAM_SET_ITEM_UNUSED, // Unused "ITEM1"
    /* 0x2A */ CAM_SET_CS_3, // Various cutscenes "DEMO3"
    /* 0x2B */ CAM_SET_CS_ATTENTION, // Attention cutscenes and the actor siofuki (water spout/jet) "DEMO4"
    /* 0x2C */ CAM_SET_BEAN_GENERIC, // All beans except lost woods "UFOBEAN"
    /* 0x2D */ CAM_SET_BEAN_LOST_WOODS, // Lost woods bean "LIFTBEAN"
    /* 0x2E */ CAM_SET_SCENE_UNUSED, // Unused "SCENE0"
    /* 0x2F */ CAM_SET_SCENE_TRANSITION, // Scene Transitions "SCENE1"
    /* 0x30 */ CAM_SET_ELEVATOR_PLATFORM, // All the fire temple platforms that rise. Also used in non-mq spirit temple entrance shortcut "HIDAN1"
    /* 0x31 */ CAM_SET_FIRE_STAIRCASE, // Used on fire temple staircase actor cutscene in shortcut room connecting vanilla hammer chest to the final goron small key "HIDAN2"
    /* 0x32 */ CAM_SET_FOREST_UNUSED, // Unused "MORI2"
    /* 0x33 */ CAM_SET_FOREST_DEFEAT_POE, // Used when defeating a poe sister "MORI3"
    /* 0x34 */ CAM_SET_BIG_OCTO, // Used by big octo miniboss in Jabu Jabu "TAKO"
    /* 0x35 */ CAM_SET_MEADOW_BIRDS_EYE, // Used only as child in Sacred Forest Meadow Maze "SPOT05A"
    /* 0x36 */ CAM_SET_MEADOW_UNUSED, // Unused from Sacred Forest Meadow "SPOT05B"
    /* 0x37 */ CAM_SET_FIRE_BIRDS_EYE, // Used in lower-floor maze in non-mq fire temple "HIDAN3"
    /* 0x38 */ CAM_SET_TURN_AROUND, // Put the camera in front of player and turn around to look at player from the front "ITEM2"
    /* 0x39 */ CAM_SET_PIVOT_VERTICAL, // Lowering platforms (forest temple bow room, Jabu final shortcut) "CIRCLE6"
    /* 0x3A */ CAM_SET_NORMAL2,
    /* 0x3B */ CAM_SET_FISHING, // Fishing pond by the lake
    /* 0x3C */ CAM_SET_CS_C, // Various cutscenes "DEMOC"
    /* 0x3D */ CAM_SET_JABU_TENTACLE, // Jabu-Jabu Parasitic Tentacle Rooms "UO_FIBER"
    /* 0x3E */ CAM_SET_DUNGEON2,
    /* 0x3F */ CAM_SET_DIRECTED_YAW, // Does not auto-update yaw, tends to keep the camera pointed at a certain yaw (used by biggoron and final spirit lowering platform) "TEPPEN"
    /* 0x40 */ CAM_SET_PIVOT_FROM_SIDE, // Fixed side view, allows rotation of camera (eg. Potion Shop, Meadow at fairy grotto) "CIRCLE7"
    /* 0x41 */ CAM_SET_NORMAL4,
    /* 0x42 */ CAM_SET_MAX
} CameraSettingType;
#endif

#if defined(GAME_MM)
typedef enum {
    /* 0x00 */ CAM_SET_NONE,
    /* 0x01 */ CAM_SET_NORMAL0, /* Generic camera 0, used in various places "NORMAL0" */
    /* 0x02 */ CAM_SET_NORMAL3, /* Generic camera 3, used in various places "NORMAL3" */
    /* 0x03 */ CAM_SET_PIVOT_DIVING, /* Player diving from the surface of the water to underwater not as zora "CIRCLE5" */
    /* 0x04 */ CAM_SET_HORSE, /* Reiding a horse "HORSE0" */
    /* 0x05 */ CAM_SET_ZORA_DIVING, /* Parallel's Pivot Diving, but as Zora. However, Zora does not dive like a human. So this setting appears to not be used "ZORA0" */
    /* 0x06 */ CAM_SET_PREREND_FIXED, /* Unused remnant of OoT: camera is fixed in position and rotation "PREREND0" */
    /* 0x07 */ CAM_SET_PREREND_PIVOT, /* Unused remnant of OoT: Camera is fixed in position with fixed pitch, but is free to rotate in the yaw direction 360 degrees "PREREND1" */
    /* 0x08 */ CAM_SET_DOORC, /* Generic room door transitions, camera moves and follows player as the door is open and closed "DOORC" */
    /* 0x09 */ CAM_SET_DEMO0, /* Unknown, possibly related to treasure chest game as goron? "DEMO0" */
    /* 0x0A */ CAM_SET_FREE0, /* Free Camera, manual control is given, no auto-updating eye or at "FREE0" */
    /* 0x0B */ CAM_SET_BIRDS_EYE_VIEW_0, /* Appears unused. Camera is a top-down view "FUKAN0" */
    /* 0x0C */ CAM_SET_NORMAL1, /* Generic camera 1, used in various places "NORMAL1" */
    /* 0x0D */ CAM_SET_NANAME, /* Unknown, slanted or tilted. Behaves identical to Normal0 except with added roll "NANAME" */
    /* 0x0E */ CAM_SET_CIRCLE0, /* Used in Curiosity Shop, Pirates Fortress, Mayor's Residence "CIRCLE0" */
    /* 0x0F */ CAM_SET_FIXED0, /* Used in Sakon's Hideout puzzle rooms, milk bar stage "FIXED0" */
    /* 0x10 */ CAM_SET_SPIRAL_DOOR, /* Exiting a Spiral Staircase "SPIRAL" */
    /* 0x11 */ CAM_SET_DUNGEON0, /* Generic dungeon camera 0, used in various places "DUNGEON0" */
    /* 0x12 */ CAM_SET_ITEM0, /* Getting an item and holding it above Player's head (from small chest, freestanding, npc, ...) "ITEM0" */
    /* 0x13 */ CAM_SET_ITEM1, /* Looking at player while playing the ocarina "ITEM1" */
    /* 0x14 */ CAM_SET_ITEM2, /* Bottles: drinking, releasing fairy, dropping fish "ITEM2" */
    /* 0x15 */ CAM_SET_ITEM3, /* Bottles: catching fish or bugs, showing an item "ITEM3" */
    /* 0x16 */ CAM_SET_NAVI, /* Song of Soaring, variations of playing Song of Time "NAVI" */
    /* 0x17 */ CAM_SET_WARP_PAD_MOON, /* Warp circles from Goron Trial on the moon "WARP0" */
    /* 0x18 */ CAM_SET_DEATH, /* Player death animation when health goes to 0 "DEATH" */
    /* 0x19 */ CAM_SET_REBIRTH, /* Unknown set with camDataId = -9 (it's not being revived by a fairy) "REBIRTH" */
    /* 0x1A */ CAM_SET_LONG_CHEST_OPENING, /* Long cutscene when opening a big chest with a major item "TREASURE" */
    /* 0x1B */ CAM_SET_MASK_TRANSFORMATION, /* Putting on a transformation mask "TRANSFORM" */
    /* 0x1C */ CAM_SET_ATTENTION, /* Unknown, set with camDataId = -15 "ATTENTION" */
    /* 0x1D */ CAM_SET_WARP_PAD_ENTRANCE, /* Warp pad from start of a dungeon to the boss-room "WARP1" */
    /* 0x1E */ CAM_SET_DUNGEON1, /* Generic dungeon camera 1, used in various places "DUNGEON1" */
    /* 0x1F */ CAM_SET_FIXED1, /* Fixes camera in place, used in various places eg. entering Stock Pot Inn, hiting a switch, giving witch a red potion, shop browsing "FIXED1" */
    /* 0x20 */ CAM_SET_FIXED2, /* Used in Pinnacle Rock after defeating Sea Monsters, and by Tatl in Fortress "FIXED2" */
    /* 0x21 */ CAM_SET_MAZE, /* Unused. Set to use Camera_Parallel2(), which is only Camera_Noop() "MAZE" */
    /* 0x22 */ CAM_SET_REMOTEBOMB, /* Unused. Set to use Camera_Parallel2(), which is only Camera_Noop(). But also related to Play_ChangeCameraSetting? "REMOTEBOMB" */
    /* 0x23 */ CAM_SET_CIRCLE1, /* Unknown "CIRCLE1" */
    /* 0x24 */ CAM_SET_CIRCLE2, /* Looking at far-away NPCs eg. Garo in Road to Ikana, Hungry Goron, Tingle "CIRCLE2" */
    /* 0x25 */ CAM_SET_CIRCLE3, /* Used in curiosity shop, goron racetrack, final room in Sakon's hideout, other places "CIRCLE3" */
    /* 0x26 */ CAM_SET_CIRCLE4, /* Used during the races on the doggy racetrack "CIRCLE4" */
    /* 0x27 */ CAM_SET_FIXED3, /* Used in Stock Pot Inn Toilet and Tatl cutscene after woodfall "FIXED3" */
    /* 0x28 */ CAM_SET_TOWER_ASCENT, /* Various climbing structures (Snowhead climb to the temple entrance) "TOWER0" */
    /* 0x29 */ CAM_SET_PARALLEL0, /* Unknown "PARALLEL0" */
    /* 0x2A */ CAM_SET_NORMALD, /* Unknown, set with camDataId = -20 "NORMALD" */
    /* 0x2B */ CAM_SET_SUBJECTD, /* Unknown, set with camDataId = -21 "SUBJECTD" */
    /* 0x2C */ CAM_SET_START0, /* Entering a room, either Dawn of a New Day reload, or entering a door where the camera is fixed on the other end "START0" */
    /* 0x2D */ CAM_SET_START2, /* Entering a scene, camera is put at a low angle eg. Grottos, Deku Palace, Stock Pot Inn "START2" */
    /* 0x2E */ CAM_SET_STOP0, /* Called in z_play "STOP0" */
    /* 0x2F */ CAM_SET_BOAT_CRUISE, /*  Koume's boat cruise "JCRUISING" */
    /* 0x30 */ CAM_SET_VERTICAL_CLIMB, /* Large vertical climbs, such as Mountain Village wall or Pirates Fortress ladder. "CLIMBMAZE" */
    /* 0x31 */ CAM_SET_SIDED, /* Unknown, set with camDataId = -24 "SIDED" */
    /* 0x32 */ CAM_SET_DUNGEON2, /* Generic dungeon camera 2, used in various places "DUNGEON2" */
    /* 0x33 */ CAM_SET_BOSS_ODOLWA, /* Odolwa's Lair, also used in GBT entrance: "BOSS_SHIGE" */
    /* 0x34 */ CAM_SET_KEEPBACK, /* Unknown. Possibly related to climbing something? "KEEPBACK" */
    /* 0x35 */ CAM_SET_CIRCLE6, /* Used in select regions from Ikana "CIRCLE6" */
    /* 0x36 */ CAM_SET_CIRCLE7, /* Unknown "CIRCLE7" */
    /* 0x37 */ CAM_SET_MINI_BOSS, /* Used during the various minibosses of the "CHUBOSS" */
    /* 0x38 */ CAM_SET_RFIXED1, /* Talking to Koume stuck on the floor in woods of mystery "RFIXED1" */
    /* 0x39 */ CAM_SET_TREASURE_CHEST_MINIGAME, /* Treasure Chest Shop in East Clock Town, minigame location "TRESURE1" */
    /* 0x3A */ CAM_SET_HONEY_AND_DARLING_1, /* Honey and Darling Minigames "BOMBBASKET" */
    /* 0x3B */ CAM_SET_CIRCLE8, /* Used by Stone Tower moving platforms, Falling eggs in Marine Lab, Bugs into soilpatch cutscene "CIRCLE8" */
    /* 0x3C */ CAM_SET_BIRDS_EYE_VIEW_1, /* Camera is a top-down view. Used in Fisherman's minigame and Deku Palace "FUKAN1" */
    /* 0x3D */ CAM_SET_DUNGEON3, /* Generic dungeon camera 3, used in various places "DUNGEON3" */
    /* 0x3E */ CAM_SET_TELESCOPE, /* Observatory telescope and Curiosity Shop Peep-Hole "TELESCOPE" */
    /* 0x3F */ CAM_SET_ROOM0, /* Certain rooms eg. inside the clock tower "ROOM0" */
    /* 0x40 */ CAM_SET_RCIRC0, /* Used by a few NPC cutscenes, focus close on the NPC "RCIRC0" */
    /* 0x41 */ CAM_SET_CIRCLE9, /* Used by Sakon Hideout entrance and Deku Palace Maze "CIRCLE9" */
    /* 0x42 */ CAM_SET_ONTHEPOLE, /* Somewhere in Snowhead Temple and Woodfall Temple "ONTHEPOLE" */
    /* 0x43 */ CAM_SET_INBUSH, /* Various bush environments eg. grottos, Swamp Spider House, Termina Field grass bushes, Deku Palace near bean "INBUSH" */
    /* 0x44 */ CAM_SET_BOSS_MAJORA, /* Majora's Lair: "BOSS_LAST" */
    /* 0x45 */ CAM_SET_BOSS_TWINMOLD, /* Twinmold's Lair: "BOSS_INI" */
    /* 0x46 */ CAM_SET_BOSS_GOHT, /* Goht's Lair: "BOSS_HAK" */
    /* 0x47 */ CAM_SET_BOSS_GYORG, /* Gyorg's Lair: "BOSS_KON" */
    /* 0x48 */ CAM_SET_CONNECT0, /* Smoothly and gradually return camera to Player after a cutscene "CONNECT0" */
    /* 0x49 */ CAM_SET_PINNACLE_ROCK, /* Pinnacle Rock pit "MORAY" */
    /* 0x4A */ CAM_SET_NORMAL2, /* Generic camera 2, used in various places "NORMAL2" */
    /* 0x4B */ CAM_SET_HONEY_AND_DARLING_2, /* "BOMBBOWL" */
    /* 0x4C */ CAM_SET_CIRCLEA, /* Unknown, Circle 10 "CIRCLEA" */
    /* 0x4D */ CAM_SET_WHIRLPOOL, /* Great Bay Temple Central Room Whirlpool "WHIRLPOOL" */
    /* 0x4E */ CAM_SET_CUCCO_SHACK, /* "KOKKOGAME" */
    /* 0x4F */ CAM_SET_GIANT, /* Giants Mask in Twinmold's Lair "GIANT" */
    /* 0x50 */ CAM_SET_SCENE0, /* Entering doors to a new scene "SCENE0" */
    /* 0x51 */ CAM_SET_ROOM1, /* Certain rooms eg. some rooms in Stock Pot Inn "ROOM1" */
    /* 0x52 */ CAM_SET_WATER2, /* Swimming as Zora in Great Bay Temple "WATER2" */
    /* 0x53 */ CAM_SET_WOODFALL_SWAMP, /* Woodfall inside the swamp, but not on the platforms, "SOKONASI" */
    /* 0x54 */ CAM_SET_FORCEKEEP, /* Unknown "FORCEKEEP" */
    /* 0x55 */ CAM_SET_PARALLEL1, /* Unknown "PARALLEL1" */
    /* 0x56 */ CAM_SET_START1, /* Used when entering the lens cave "START1" */
    /* 0x57 */ CAM_SET_ROOM2, /* Certain rooms eg. Deku King's Chamber, Ocean Spider House "ROOM2" */
    /* 0x58 */ CAM_SET_NORMAL4, /* Generic camera 4, used in Ikana Graveyard "NORMAL4" */
    /* 0x59 */ CAM_SET_ELEGY_SHELL, /* cutscene after playing elegy of emptyness and spawning a shell "SHELL" */
    /* 0x5A */ CAM_SET_DUNGEON4, /* Used in Pirates Fortress Interior, hidden room near hookshot "DUNGEON4" */
    /* 0x5B */ CAM_SET_MAX
} CameraSettingType;
#endif

s32 Camera_RequestSetting(Camera* camera, s16 setting);

#endif

#ifndef SAVE_MM_H
#define SAVE_MM_H

#include <PR/ultratypes.h>
#include <math_structs.h>

typedef struct MmItemEquips {
    /* 0x00 */ u8 buttonItems[4][4];                    // "register_item"
    /* 0x10 */ u8 cButtonSlots[4][4];                   // "register_item_pt"
    /* 0x20 */ u16 equipment;
} MmItemEquips; // size = 0x22

typedef struct MmInventory {
    /* 0x00 */ u8 items[48];                            // "item_register", first 24 elements are normal items and the other 24 are masks
    /* 0x30 */ s8 ammo[24];                             // "item_count"
    /* 0x48 */ u32 upgrades;                            // "non_equip_register" some bits are wallet upgrades
    /* 0x4C */ u32 questItems;                          // "collect_register"
    /* 0x50 */ u8 dungeonItems[10];                     // "key_compass_map"
    /* 0x5A */ s8 dungeonKeys[9];                       // "key_register"
    /* 0x63 */ s8 defenseHearts;
    /* 0x64 */ s8 strayFairies[10];                     // "orange_fairy"
    /* 0x6E */ char dekuPlaygroundPlayerName[3][8];     // "degnuts_memory_name" Stores playerName (8 char) over (3 days) when getting a new high score
} MmInventory; // size = 0x88

typedef struct MmHorseData {
    /* 0x0 */ s16 sceneId;                             // "spot_no"
    /* 0x2 */ Vec3s pos;                               // "horse_x", "horse_y" and "horse_z"
    /* 0x8 */ s16 yaw;                                 // "horse_a"
} MmHorseData; // size = 0xA

typedef struct MmPermanentSceneFlags {
    /* 0x00 */ u32 chest;
    /* 0x04 */ u32 switch0;
    /* 0x08 */ u32 switch1;
    /* 0x0C */ u32 clearedRoom;
    /* 0x10 */ u32 collectible;
    /* 0x14 */ u32 unk_14; // varies based on scene. For dungeons, floors visited.
    /* 0x18 */ u32 rooms;
} MmPermanentSceneFlags; // size = 0x1C

typedef struct MmSavePlayerData {
    /* 0x00 */ char newf[6];                          // "newf"               Will always be "ZELDA3 for a valid save
    /* 0x06 */ u16 threeDayResetCount;                // "savect"
    /* 0x08 */ char playerName[8];                    // "player_name"
    /* 0x10 */ s16 healthCapacity;                    // "max_life"
    /* 0x12 */ s16 health;                            // "now_life"
    /* 0x14 */ s8 magicLevel; // 0 for no magic/new load, 1 for magic, 2 for double magic "magic_max"
    /* 0x15 */ s8 magic; // current magic available for use "magic_now"
    /* 0x16 */ s16 rupees;                            // "lupy_count"
    /* 0x18 */ u16 swordHealth;                       // "long_sword_hp"
    /* 0x1A */ u16 tatlTimer;                         // "navi_timer"
    /* 0x1C */ u8 isMagicAcquired;                    // "magic_mode"
    /* 0x1D */ u8 isDoubleMagicAcquired;              // "magic_ability"
    /* 0x1E */ u8 doubleDefense;                      // "life_ability"
    /* 0x1F */ u8 unk_1F;                             // "ocarina_round"
    /* 0x20 */ u8 owlWarpId; // See `OwlWarpId`, "first_memory"
    /* 0x22 */ u16 owlActivationFlags;                // "memory_warp_point"
    /* 0x24 */ u8 unk_24;                             // "last_warp_pt"
    /* 0x26 */ s16 savedSceneId;                      // "scene_data_ID"
} MmSavePlayerData; // size = 0x28

typedef struct MmSaveInfo {
    /* 0x000 */ MmSavePlayerData playerData;
    /* 0x028 */ MmItemEquips equips;
    /* 0x04C */ MmInventory inventory;
    /* 0x0D4 */ MmPermanentSceneFlags permanentSceneFlags[120];
    /* 0xDF4 */ char unk_DF4[0x54];
    /* 0xE48 */ u32 dekuPlaygroundHighScores[3];
    /* 0xE54 */ u32 pictoFlags0;                       // Flags set by `PictoActor`s if pictograph is valid
    /* 0xE58 */ u32 pictoFlags1;                       // Flags set by Snap_ValidatePictograph() to record errors; volatile since that function is run many times in succession
    /* 0xE5C */ u32 unk_E5C;
    /* 0xE60 */ u32 unk_E60;
    /* 0xE64 */ u32 alienInfo[7];                      // Used by EnInvadepoh to hold alien spawn times and how many aliens the player has killed
    /* 0xE80 */ u32 scenesVisible[7];                  // tingle maps and clouded regions on pause map. Stores scenes bitwise for up to 224 scenes even though there are not that many scenes
    /* 0xE9C */ u32 skullTokenCount;                   // upper 16 bits store Swamp skulls, lower 16 bits store Ocean skulls
    /* 0xEA0 */ u32 unk_EA0;                           // Gossic stone heart piece flags
    /* 0xEA4 */ u32 unk_EA4;
    /* 0xEA8 */ u32 unk_EA8[2];                        // Related to blue warps
    /* 0xEB0 */ u32 stolenItems;                       // Items stolen by Takkuri and given to Curiosity Shop Man
    /* 0xEB4 */ u32 unk_EB4;
    /* 0xEB8 */ u32 highScores[7];
    /* 0xED4 */ u8 weekEventReg[100];                  // "week_event_reg"
    /* 0xF38 */ u32 regionsVisited;                    // "area_arrival"
    /* 0xF3C */ u32 worldMapCloudVisibility;           // "cloud_clear"
    /* 0xF40 */ u8 unk_F40;                            // "oca_rec_flag"                   has scarecrows song
    /* 0xF41 */ u8 scarecrowSpawnSongSet;              // "oca_rec_flag8"
    /* 0xF42 */ u8 scarecrowSpawnSong[128];
    /* 0xFC2 */ s8 bombersCaughtNum;                   // "aikotoba_index"
    /* 0xFC3 */ s8 bombersCaughtOrder[5];              // "aikotoba_table"
    /* 0xFC8 */ s8 lotteryCodes[3][3];                 // "numbers_table", Preset lottery codes
    /* 0xFD1 */ s8 spiderHouseMaskOrder[6];            // "kinsta_color_table"
    /* 0xFD7 */ s8 bomberCode[5];                      // "bombers_aikotoba_table"
    /* 0xFDC */ MmHorseData horseData;
    /* 0xFE6 */ u16 checksum;                          // "check_sum"
} MmSaveInfo; // size = 0xFE8

typedef struct MmSave {
    /* 0x00 */ s32 entrance;        // "scene_no"
    /* 0x04 */ u8 equippedMask;     // "player_mask"
    /* 0x05 */ u8 isFirstCycle;     // "opening_flag"
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 linkAge;          // "link_age"
    /* 0x08 */ s32 cutsceneIndex;   // "day_time"
    /* 0x0C */ u16 time;            // "zelda_time"
    /* 0x0E */ u16 owlWarpId;       // See `OwlWarpId` enum
    /* 0x10 */ s32 isNight;         // "asahiru_fg"
    /* 0x14 */ s32 timeSpeedOffset; // "change_zelda_time"
    /* 0x18 */ s32 day;             // "totalday"
    /* 0x1C */ s32 eventDayCount;   // "eventday"
    /* 0x20 */ u8 playerForm;       // "player_character"
    /* 0x21 */ u8 snowheadCleared;  // "spring_flag"
    /* 0x22 */ u8 hasTatl;          // "bell_flag"
    /* 0x23 */ u8 isOwlSave;
    /* 0x24 */ MmSaveInfo saveInfo;
} MmSave; // size = 0x100C

#endif

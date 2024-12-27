#ifndef SAVE_OOT_H
#define SAVE_OOT_H

#include <PR/ultratypes.h>
#include <math_structs.h>

typedef struct ItemEquips {
    /* 0x00 */ u8 buttonItems[4];
    /* 0x04 */ u8 cButtonSlots[3];
    /* 0x08 */ u16 equipment; // a mask where each nibble corresponds to a type of equipment `EquipmentType`, and each nibble is a piece `EquipValue*`
} OotItemEquips; // size = 0x0A

typedef struct Inventory {
    /* 0x00 */ u8 items[24];
    /* 0x18 */ s8 ammo[16];
    /* 0x28 */ u16 equipment; // a mask where each nibble corresponds to a type of equipment `EquipmentType`, and each bit to an owned piece `EquipInv*`
    /* 0x2C */ u32 upgrades;
    /* 0x30 */ u32 questItems;
    /* 0x34 */ u8 dungeonItems[20];
    /* 0x48 */ s8 dungeonKeys[19];
    /* 0x5B */ s8 defenseHearts;
    /* 0x5C */ s16 gsTokens;
} OotInventory; // size = 0x5E

typedef struct SavedSceneFlags {
    /* 0x00 */ u32 chest;
    /* 0x04 */ u32 swch;
    /* 0x08 */ u32 clear;
    /* 0x0C */ u32 collect;
    /* 0x10 */ u32 unk;
    /* 0x14 */ u32 rooms;
    /* 0x18 */ u32 floors;
} OotSavedSceneFlags; // size = 0x1C

typedef struct HorseData {
    /* 0x00 */ s16 sceneId;
    /* 0x02 */ Vec3s pos;
    /* 0x08 */ s16 angle;
} OotHorseData; // size = 0x0A

typedef struct SavePlayerData {
    /* 0x00  0x001C */ char newf[6]; // string "ZELDAZ"
    /* 0x06  0x0022 */ u16 deaths;
    /* 0x08  0x0024 */ char playerName[8];
    /* 0x10  0x002C */ s16 n64ddFlag;
    /* 0x12  0x002E */ s16 healthCapacity; // "max_life"
    /* 0x14  0x0030 */ s16 health; // "now_life"
    /* 0x16  0x0032 */ s8 magicLevel; // 0 for no magic/new load, 1 for magic, 2 for double magic
    /* 0x17  0x0033 */ s8 magic; // current magic available for use
    /* 0x18  0x0034 */ s16 rupees;
    /* 0x1A  0x0036 */ u16 swordHealth;
    /* 0x1C  0x0038 */ u16 naviTimer;
    /* 0x1E  0x003A */ u8 isMagicAcquired;
    /* 0x1F  0x003B */ char unk_3B[0x01];
    /* 0x20  0x003C */ u8 isDoubleMagicAcquired;
    /* 0x21  0x003D */ u8 isDoubleDefenseAcquired;
    /* 0x22  0x003E */ u8 bgsFlag;
    /* 0x23  0x003F */ u8 ocarinaGameRoundNum;
    /* 0x24  0x0040 */ OotItemEquips childEquips;
    /* 0x2E  0x004A */ OotItemEquips adultEquips;
    /* 0x38  0x0054 */ u32 unk_54; // this may be incorrect, currently used for alignment
    /* 0x3C  0x0058 */ char unk_58[0x0E];
    /* 0x4A  0x0066 */ s16 savedSceneId;
} OotSavePlayerData;

typedef struct FaroresWindData {
    /* 0x00 */ Vec3i pos;
    /* 0x0C */ s32 yaw;
    /* 0x10 */ s32 playerParams;
    /* 0x14 */ s32 entranceIndex;
    /* 0x18 */ s32 roomIndex;
    /* 0x1C */ s32 set;
    /* 0x20 */ s32 tempSwchFlags;
    /* 0x24 */ s32 tempCollectFlags;
} OotFaroresWindData; // size = 0x28

// offsets in SaveInfo and SaveContext/Save
typedef struct SaveInfo {
    /* 0x0000  0x001C */ OotSavePlayerData playerData; // "S_Private"
    /* 0x004C  0x0068 */ OotItemEquips equips;
    /* 0x0058  0x0074 */ OotInventory inventory;
    /* 0x00B8  0x00D4 */ OotSavedSceneFlags sceneFlags[124];
    /* 0x0E48  0x0E64 */ OotFaroresWindData fw;
    /* 0x0E70  0x0E8C */ char unk_E8C[0x10];
    /* 0x0E80  0x0E9C */ s32 gsFlags[6];
    /* 0x0E98  0x0EB4 */ char unk_EB4[0x4];
    /* 0x0E9C  0x0EB8 */ s32 highScores[7];
    /* 0x0EB8  0x0ED4 */ u16 eventChkInf[14]; // "event_chk_inf"
    /* 0x0ED4  0x0EF0 */ u16 itemGetInf[4]; // "item_get_inf"
    /* 0x0EDC  0x0EF8 */ u16 infTable[30]; // "inf_table"
    /* 0x0F18  0x0F34 */ char unk_F34[0x04];
    /* 0x0F1C  0x0F38 */ u32 worldMapAreaData; // "area_arrival"
    /* 0x0F20  0x0F3C */ char unk_F3C[0x4];
    /* 0x0F24  0x0F40 */ u8 scarecrowLongSongSet;
    /* 0x0F25  0x0F41 */ u8 scarecrowLongSong[0x360];
    /* 0x1285  0x12A1 */ char unk_12A1[0x24];
    /* 0x12A9  0x12C5 */ u8 scarecrowSpawnSongSet;
    /* 0x12AA  0x12C6 */ u8 scarecrowSpawnSong[0x80];
    /* 0x132A  0x1346 */ char unk_1346[0x02];
    /* 0x132C  0x1348 */ OotHorseData horseData;
    /* 0x1336  0x1352 */ u16 checksum; // "check_sum"
} OotSaveInfo;

typedef struct Save {
    /* 0x00 */ s32 entranceIndex;
    /* 0x04 */ s32 linkAge; // 0: Adult; 1: Child (see enum `LinkAge`)
    /* 0x08 */ s32 cutsceneIndex;
    /* 0x0C */ u16 dayTime; // "zelda_time"
    /* 0x10 */ s32 nightFlag;
    /* 0x14 */ s32 totalDays;
    /* 0x18 */ s32 bgsDayCount; // increments with totalDays, can be cleared with `Environment_ClearBgsDayCount`
    /* 0x1C */ OotSaveInfo info; // "information"
} OotSave;

#endif

#ifndef COMBO_SCENE_H
#define COMBO_SCENE_H

#include <combo.h>

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ u32 data2;
} SCmdBase; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdSpawnList; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  num;
    /* 0x4 */ void* segment;
} SCmdActorList; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdCsCameraList; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdColHeader; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  num;
    /* 0x4 */ void* segment;
} SCmdRoomList; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x2 */ char pad2[2];
    /* 0x4 */ s8  west;
    /* 0x5 */ s8  vertical;
    /* 0x6 */ s8  south;
    /* 0x7 */ u8  clothIntensity;
} SCmdWindSettings; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdEntranceList; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  naviQuestHintFileId;
    /* 0x4 */ u32 subKeepId;
} SCmdSpecialFiles; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  gpFlag1;
    /* 0x4 */ u32 gpFlag2;
} SCmdRoomBehavior; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdMesh; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  num;
    /* 0x4 */ void* segment;
} SCmdObjectList; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  num;
    /* 0x4 */ void* segment;
} SCmdLightList; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdPathList; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  num;
    /* 0x4 */ void* segment;
} SCmdTransitionActorList; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  num;
    /* 0x4 */ void* segment;
} SCmdLightSettingList; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x2 */ char pad2[2];
    /* 0x4 */ u8  hour;
    /* 0x5 */ u8  min;
    /* 0x6 */ u8  timeSpeed;
} SCmdTimeSettings; // size = 0x7

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x2 */ char pad2[2];
    /* 0x4 */ u8  skyboxId;
    /* 0x5 */ u8  skyboxConfig;
    /* 0x6 */ u8  envLightMode;
} SCmdSkyboxSettings; // size = 0x7

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x2 */ char pad2[2];
    /* 0x4 */ u8  unk4;
    /* 0x5 */ u8  unk5;
} SCmdSkyboxDisables; // size = 0x6

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdExitList; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ u32 data2;
} SCmdEndMarker; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  specId;
    /* 0x2 */ char unk_02[4];
#if defined(GAME_OOT)
    /* 0x6 */ u8  natureAmbienceId;
# else
    /* 0x6 */ u8  ambienceId;
#endif
    /* 0x7 */ u8  seqId;
} SCmdSoundSettings; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x2 */ char unk_02[5];
    /* 0x7 */ u8  echo;
} SCmdEchoSettings; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdCutsceneData; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  scriptListCount;
    /* 0x4 */ void* segment;
} SCmdCsScriptList; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdAltHeaders; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ char pad[7];
} SCmdRegionVisited; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdTextureAnimations; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  num;
    /* 0x4 */ void* segment;
} SCmdCutsceneList; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  data1;
    /* 0x4 */ void* segment;
} SCmdMapData; // size = 0x8

typedef struct {
    /* 0x0 */ u8  code;
    /* 0x1 */ u8  num;
    /* 0x4 */ void* segment;
} SCmdMapDataChests; // size = 0x8

typedef union {
    /* Command: N/A  */ SCmdBase                base;
    /* Command: 0x00 */ SCmdSpawnList           spawnList;
    /* Command: 0x01 */ SCmdActorList           actorList;
    /* Command: 0x02 */ SCmdCsCameraList        actorCsCamList;
    /* Command: 0x03 */ SCmdColHeader           colHeader;
    /* Command: 0x04 */ SCmdRoomList            roomList;
    /* Command: 0x05 */ SCmdWindSettings        windSettings;
    /* Command: 0x06 */ SCmdEntranceList        entranceList;
    /* Command: 0x07 */ SCmdSpecialFiles        specialFiles;
    /* Command: 0x08 */ SCmdRoomBehavior        roomBehavior;
    /* Command: 0x09 */ // Unused
    /* Command: 0x0A */ SCmdMesh                mesh;
    /* Command: 0x0B */ SCmdObjectList          objectList;
    /* Command: 0x0C */ SCmdLightList           lightList;
    /* Command: 0x0D */ SCmdPathList            pathList;
    /* Command: 0x0E */ SCmdTransitionActorList transitionActorList;
    /* Command: 0x0F */ SCmdLightSettingList    lightSettingList;
    /* Command: 0x10 */ SCmdTimeSettings        timeSettings;
    /* Command: 0x11 */ SCmdSkyboxSettings      skyboxSettings;
    /* Command: 0x12 */ SCmdSkyboxDisables      skyboxDisables;
    /* Command: 0x13 */ SCmdExitList            exitList;
    /* Command: 0x14 */ SCmdEndMarker           endMarker;
    /* Command: 0x15 */ SCmdSoundSettings       soundSettings;
    /* Command: 0x16 */ SCmdEchoSettings        echoSettings;
    /* Command: 0x17 */ SCmdCsScriptList        scriptList;
    /* Command: 0x18 */ SCmdAltHeaders          altHeaders;
    /* Command: 0x19 */ SCmdRegionVisited       regionVisited;
    /* Command: 0x1A */ SCmdTextureAnimations   textureAnimations;
    /* Command: 0x1B */ SCmdCutsceneList        cutsceneList;
    /* Command: 0x1C */ SCmdMapData             mapData;
    /* Command: 0x1D */ // Unused
    /* Command: 0x1E */ SCmdMapDataChests       mapDataChests;
} SceneCmd; // size = 0x8

#endif

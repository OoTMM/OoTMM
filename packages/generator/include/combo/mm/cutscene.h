#ifndef COMBO_MM_CUTSCENE_H
#define COMBO_MM_CUTSCENE_H

#include <combo.h>

typedef struct {
    /* 0x00 */ s16 csId;
    /* 0x02 */ s16 length;
    /* 0x04 */ s16 endCsId;
    /* 0x06 */ s16 subCamId;
    /* 0x08 */ Actor* targetActor;
    /* 0x0C */ s32 startMethod;
    /* 0x10 */ PlayState* play;
    /* 0x14 */ s16 retCamId;
    /* 0x16 */ s16 isCameraStored;
} CutsceneManager; // size = 0x18

extern CutsceneManager sCutsceneMgr;

typedef enum {
    /*  0 */ PLAYER_CS_ID_ITEM_OCARINA,
    /*  1 */ PLAYER_CS_ID_ITEM_GET,
    /*  2 */ PLAYER_CS_ID_ITEM_BOTTLE,
    /*  3 */ PLAYER_CS_ID_ITEM_SHOW,
    /*  4 */ PLAYER_CS_ID_WARP_PAD_MOON,
    /*  5 */ PLAYER_CS_ID_MASK_TRANSFORMATION,
    /*  6 */ PLAYER_CS_ID_DEATH,
    /*  7 */ PLAYER_CS_ID_REVIVE,
    /*  8 */ PLAYER_CS_ID_SONG_WARP, // Song of Time and Song of Soaring
    /*  9 */ PLAYER_CS_ID_WARP_PAD_ENTRANCE,
    /* 10 */ PLAYER_CS_ID_MAX
} PlayerCsId;

#endif

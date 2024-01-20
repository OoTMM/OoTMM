#ifndef COMBO_OOT_EN_DIVING_GAME_H
#define COMBO_OOT_EN_DIVING_GAME_H

#include <combo/common/actor.h>

typedef struct Actor_EnDivingGame Actor_EnDivingGame;

typedef void (*EnDivingGameActionFunc)(struct Actor_EnDivingGame*, GameState_Play*);

struct Actor_EnDivingGame
{
    /* 0x0000 */ Actor actor;
    /* 0x013C */ u8 skelAnime[0x44]; // SkelAnime
    /* 0x0180 */ Vec3s jointTable[20];
    /* 0x01F8 */ Vec3s morphTable[20];
    /* 0x0270 */ EnDivingGameActionFunc actionFunc;
    /* 0x0274 */ Vec3s headRot;
    /* 0x027A */ Vec3s torsoRot;
    /* 0x0280 */ s16 unk_290; // counter that only goes up
    /* 0x0282 */ s16 unk_292;
    /* 0x0284 */ s16 subCamTimer;
    /* 0x0286 */ s16 unk_296; // read by ExRuppy // timer?
    /* 0x0288 */ s16 eyeTimer;
    /* 0x028A */ s16 spawnRuppyTimer;
    /* 0x028C */ s16 phase;
    /* 0x028E */ s16 eyeTexIndex;
    /* 0x0290 */ s16 subCamId;
    /* 0x0292 */ s16 unk_2A2; // 0: , 1: , 2: Tells rupees to sink in water
    /* 0x0294 */ s16 grabbedRupeesCounter;
    /* 0x0296 */ s16 rupeesLeftToThrow;
    /* 0x0298 */ s16 state; // minigameState? 0: default, 1: waiting to give the scale, 2: minigame started
    /* 0x029A */ s16 extraWinCount; // counts how many times you have beaten the minigame **after** you got the scale. ExRuppy will reset it to zero if a 500 rupee is spawned.
    /* 0x029C */ char unk_2AC[0xC]; // probably another Vec3f, but unused.
    /* 0x02A8 */ Vec3f subCamAt;
    /* 0x02B4 */ Vec3f subCamEye;
    /* 0x02C0 */ Vec3f subCamEyeNext;
    /* 0x02CC */ Vec3f subCamEyeMaxVelFrac;
    /* 0x02D8 */ Vec3f subCamEyeVel;
    /* 0x02E4 */ Vec3f subCamAtNext;
    /* 0x02F0 */ Vec3f subCamAtMaxVelFrac;
    /* 0x02FC */ Vec3f subCamAtVel;
    /* 0x0308 */ f32 subCamVelFactor;
    /* 0x030C */ char unk_31C; // unused
    /* 0x030D */ u8 notPlayingMinigame; // flag
    /* 0x030E */ u8 allRupeesThrown; // flag
    /* 0x030F */ u8 unk_31F; // flag
    /* 0x0310 */ char unk_320[0x4]; // unused
    /* 0x0314 */ u8 interactInfo[0x28]; // NpcInteractInfo
    /* 0x033C */ ColliderCylinder collider;
} ;

_Static_assert(sizeof(Actor_EnDivingGame) == 0x0388, "OoT Actor_EnDivingGame size is wrong");

#endif

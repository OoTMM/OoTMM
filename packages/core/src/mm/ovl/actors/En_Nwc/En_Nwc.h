#ifndef COMBO_OVL_EN_NWC_H
#define COMBO_OVL_EN_NWC_H

#include <combo/actor.h>
#include "../src/mm/ovl/actors/En_Hs/En_Hs.h"

struct Actor_EnNwc;

typedef void (*Actor_EnNwc_ActionFunc)(struct Actor_EnNwc*, GameState_Play*);

#define NIW_LIMB_MAX 0x10

typedef struct Actor_EnNwc
{
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime niwSkeleton; // this skeleton is for when the chicks grow up into adults during the breman march
    /* 0x188 */ Vec3s jointTable[NIW_LIMB_MAX];
    /* 0x1E8 */ Vec3s morphTable[NIW_LIMB_MAX];
    /* 0x248 */ s8 niwObjectSlot;
    /* 0x249 */ s8 nwcObjectIndex;
    /* 0x24A */ s16 blinkState;
    /* 0x24C */ s16 blinkTimer;
    /* 0x250 */ Actor_EnHs* grog;
    /* 0x254 */ s16 stateTimer; // count frames to next state change
    /* 0x256 */ union {
                  s16 mixedValue256;  // default name for reset
                  s16 transformTimer; // frames since transformation sequence started
                  s16 fallingRotY;    // target rotation if off the ground in state 1
                };
    /* 0x258 */ s16 randomRot;
    /* 0x25A */ s16 state;
    /* 0x25C */ u16 hasGrownUp; // treated as both a boolean and a flag variable (0x1)
    /* 0x25E */ s16 footRotY;
    /* 0x260 */ s16 footRotZ;
    /* 0x262 */ s16 upperBodyRotY;
    /* 0x264 */ Actor_EnNwc_ActionFunc actionFunc;
}
Actor_EnNwc;

#endif
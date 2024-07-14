#ifndef COMBO_OVL_BG_INGATE_H
#define COMBO_OVL_BG_INGATE_H

#include <combo/actor.h>

struct Actor_BgIngate;

typedef void (*Actor_BgIngate_ActionFunc)(struct Actor_BgIngate*, GameState_Play*);

#define BGINGATE_GET_PATH_INDEX(thisx) ((thisx)->variable & 0xFF)

//TODO: move when this is used elsewhere
#define SUBS_TIME_PATHING_ORDER 3

typedef struct Actor_BgIngate {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ Actor_BgIngate_ActionFunc actionFunc;
    /* 0x160 */ u16 flags;
    /* 0x164 */ Path* timePath;
    /* 0x168 */ s16 timePathTimeSpeed;
    /* 0x16A */ s16 unk16A;
    /* 0x16C */ s16 unk16C;
    /* 0x16E */ s16 csId;
    /* 0x170 */ Vec3f timePathTargetPos;
    /* 0x17C */ f32 timePathProgress;
    /* 0x180 */ s32 timePathTotalTime;
    /* 0x184 */ s32 timePathWaypointTime;
    /* 0x188 */ s32 timePathWaypoint;
    /* 0x18C */ s32 timePathElapsedTime;
} Actor_BgIngate; // size = 0x190

#endif

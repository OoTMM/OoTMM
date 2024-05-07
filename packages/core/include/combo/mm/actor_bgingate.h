#ifndef COMBO_MM_ACTOR_BGINGATE_H
#define COMBO_MM_ACTOR_BGINGATE_H

#include <combo/actor.h>

typedef struct {
    /* 0x000 */ Actor base;
    /* 0x144 */ char pad144[0x18];
    /* 0x15C */ void* function;
    /* 0x160 */ u16 flags;
    /* 0x162 */ char pad162[0x2];
    /* 0x164 */ void* pathList;
    /* 0x168 */ s16 speed;
    /* 0x16A */ char pad16A[0x26];
} ActorBgIngate; // size = 0x190

#endif

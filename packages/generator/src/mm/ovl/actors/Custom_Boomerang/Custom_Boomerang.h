#ifndef CUSTOM_BOOMERANG_H
#define CUSTOM_BOOMERANG_H

#include <combo.h>
#include <combo/player.h>

struct Actor_CustomBoomerang;

typedef void (*CustomBoomerangActionFunc)(struct Actor_CustomBoomerang*, PlayState*);

typedef struct Actor_CustomBoomerang {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ ColliderQuad collider;
    /* 0x01CC */ Actor* moveTo;
    /* 0x01D0 */ Actor* grabbed;
    /* 0x01D4 */ u8 returnTimer;
    /* 0x01D5 */ u8 activeTimer;
    /* 0x01D8 */ s32 effectIndex;
    /* 0x01DC */ WeaponInfo weaponInfo;
    /* 0x01F8 */ CustomBoomerangActionFunc actionFunc;
    /* 0x01FC */ void* objectSegment;
} Actor_CustomBoomerang;

#endif
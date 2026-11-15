#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_FW_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_FW_H

#include <combo.h>

struct EnFw;

typedef void (*EnFwActionFunc)(struct EnFw* this, struct PlayState* play);

#define EN_FW_EFFECT_COUNT 20

typedef struct EnFwEffect {
    u8 type;
    u8 timer;
    u8 initialTimer;
    f32 scale;
    f32 scaleStep;
    Color_RGBA8 color;
    char unk_10[4];
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;
} EnFwEffect;

typedef struct EnFw {
    Actor actor;
    SkelAnime skelAnime;
    EnFwActionFunc actionFunc;
    ColliderJntSph collider;
    ColliderJntSphElement colliderElements[1];
    Vec3f bompPos;
    u8 lastDmgHook;
    s16 runDirection;
    s16 bounceCnt;
    char unk_206[0x2];
    s16 damageTimer;
    s16 explosionTimer;
    char unk_20C[0x2];
    s16 slideTimer;
    s16 slideSfxTimer;
    s16 returnToParentTimer;
    s16 turnAround;
    f32 runRadius;
    Vec3s jointTable[11];
    Vec3s morphTable[11];
    EnFwEffect effects[EN_FW_EFFECT_COUNT];
} EnFw;

#endif

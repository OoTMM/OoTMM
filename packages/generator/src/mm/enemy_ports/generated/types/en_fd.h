#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_FD_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_FD_H

#include <combo.h>

struct EnFd;

typedef void (*EnFdActionFunc)(struct EnFd* this, struct PlayState* play);

typedef enum FDEffectType {
    FD_EFFECT_NONE,
    FD_EFFECT_FLAME,
    FD_EFFECT_DOT
} FDEffectType;

#define EN_FD_EFFECT_COUNT 200

typedef struct EnFdEffect {
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
} EnFdEffect;

typedef struct EnFd {
    Actor actor;
    SkelAnime skelAnime;
    EnFdActionFunc actionFunc;
    ColliderJntSph collider;
    ColliderJntSphElement colliderElements[12];
    u8 coreActive;
    s16 initYawToInitPos;
    s16 curYawToInitPos;
    s16 runDir;
    s16 firstUpdateFlag;
    s16 spinTimer;
    s16 circlesToComplete;
    s16 invincibilityTimer;
    s16 attackTimer;
    f32 runRadius;
    f32 fadeAlpha;
    Vec3f corePos;
    Vec3s jointTable[27];
    Vec3s morphTable[27];
    EnFdEffect effects[EN_FD_EFFECT_COUNT];
} EnFd;

#endif

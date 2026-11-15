#ifndef COMBO_ENEMY_NATIVE_TYPES_OOT_EN_TP_H
#define COMBO_ENEMY_NATIVE_TYPES_OOT_EN_TP_H

#include <combo.h>

typedef enum EnTpType {
    ENTP_TYPE_HEAD = -1,
    ENTP_TYPE_BODYELEM,
    ENTP_TYPE_BODYELEMREMNANT = 10,
    ENTP_TYPE_BODYELEM_DYING,
    ENTP_TYPE_HEAD_DYING
} EnTpType;

struct EnTp;

typedef struct EnTp {
    Actor actor;
    s32 action;
    s32 modeVar;
    void (*actionFunc)(struct EnTp*, struct PlayState*);
    u8 unk_158;
    char pad_159[1];
    s16 timer;
    s16 timer2;
    s16 bodyElemAlpha;
    s16 redShineAmount;
    s16 sparkTimer;
    s16 unk_164;
    char pad_166[2];
    f32 unk_168;
    f32 bodyElemDist;
    f32 unk_170;
    ColliderJntSph collider;
    ColliderJntSphElement colliderElements[1];
    struct EnTp* headInstance;
} EnTp;

#endif

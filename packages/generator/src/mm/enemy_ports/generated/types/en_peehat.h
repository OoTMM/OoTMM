#ifndef Z_EN_PEEHAT_H
#define Z_EN_PEEHAT_H

typedef enum PeahatType {
    PEAHAT_TYPE_GROUNDED = -1,
    PEAHAT_TYPE_FLYING = 0,
    PEAHAT_TYPE_LARVA = 1
} PeahatType;

struct EnPeehat;

typedef void (*EnPeehatActionFunc)(struct EnPeehat*, struct PlayState*);

typedef struct EnPeehat {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[24];
    Vec3s morphTable[24];
    s32 state;
    s32 isStateDieFirstUpdate;
    EnPeehatActionFunc actionFunc;
    Vec3f bladeTip[2];
    s32 unk_2D4;
    f32 xzDistMax;
    f32 xzDistToRise;
    f32 unk_2E0;
    f32 jiggleRot;
    f32 jiggleRotInc;
    f32 scaleShift;
    s16 bladeRotVel;
    s16 bladeRot;
    s16 unk_2F4;
    s16 riseDelayTimer;
    s16 seekPlayerTimer;
    s16 unk_2FA;

    s16 animTimer;
    ColliderCylinder colliderCylinder;
    ColliderJntSph colliderJntSph;
    ColliderJntSphElement colliderJntSphElements[1];
    ColliderQuad colliderQuad;
} EnPeehat;

#endif

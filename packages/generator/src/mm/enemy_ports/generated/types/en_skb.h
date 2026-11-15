#ifndef Z_EN_SKB_H
#define Z_EN_SKB_H

struct EnSkb;

typedef void (*EnSkbActionFunc)(struct EnSkb*, struct PlayState*);

typedef struct EnSkb {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[20];
    Vec3s morphTable[20];
    u8 actionState;
    u8 setColliderAT;
    u8 lastDamageReaction;
    u8 breakFlags;
    EnSkbActionFunc actionFunc;
    s16 headlessYawOffset;
    BodyBreak bodyBreak;
    ColliderJntSph collider;
    ColliderJntSphElement colliderElements[2];
} EnSkb;

#endif

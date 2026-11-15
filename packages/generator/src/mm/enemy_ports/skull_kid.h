#ifndef COMBO_ENEMY_PORT_SKULL_KID_H
#define COMBO_ENEMY_PORT_SKULL_KID_H

#include <combo.h>

struct EnSkj;

typedef void (*EnSkjActionFunc)(struct EnSkj*, struct PlayState*);

typedef struct EnSkj {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[19];
    Vec3s morphTable[19];
    EnSkjActionFunc actionFunc;
    ColliderCylinder collider;
    u16 textId;
    s16 timer;
    s16 unk_2C8;
    s16 multiuseTimer;
    s16 needleShootTimer;
    s16 dodgeResetTimer;
    u8 animIndex;
    u8 action;
    u8 backflipFlag;
    u8 unk_2D3;
    u8 needlesToShoot;
    u8 hitsUntilDodge;
    u8 unk_2D6;
    u8 playerInRange;
    u8 minigameState;
    u32 alpha;
    Vec3f center;
    f32 unk_2EC;
    f32 unk_2F0;
    Vec3f posCopy;
} EnSkj;

#endif

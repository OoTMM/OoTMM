#ifndef CUSTOM_SLINGSHOT_PROJECTILE_H
#define CUSTOM_SLINGSHOT_PROJECTILE_H

#include <combo.h>
#include <combo/player.h>
#include <combo/collision.h>

struct Actor_CustomSlingshotProjectile;

typedef void (*CustomSlingshotProjectileActionFunc)(
    struct Actor_CustomSlingshotProjectile*,
    PlayState*
);

typedef struct Actor_CustomSlingshotProjectile {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ ColliderQuad collider;
    /* 0x01CC */ WeaponInfo weaponInfo;
    /* 0x01E8 */ CustomSlingshotProjectileActionFunc actionFunc;
    /* 0x01EC */ s16 timer;
    /* 0x01EE */ s16 activeTimer;
    /* 0x01F0 */ s32 touchedPoly;
} Actor_CustomSlingshotProjectile;

#endif
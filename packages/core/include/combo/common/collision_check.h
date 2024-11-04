#ifndef COMBO_COMMON_COLLISION_CHECK_H
#define COMBO_COMMON_COLLISION_CHECK_H

#include <combo/math/vec.h>
#include <combo/types.h>
#include <combo/misc.h>

struct Actor;
struct PlayState;

typedef struct Collider {
    /* 0x00 */ struct Actor* actor; // Attached actor
    /* 0x04 */ struct Actor* at; // Actor attached to what it collided with as an AT collider.
    /* 0x08 */ struct Actor* ac; // Actor attached to what it collided with as an AC collider.
    /* 0x0C */ struct Actor* oc; // Actor attached to what it collided with as an OC collider.
    /* 0x10 */ u8 atFlags;
    /* 0x11 */ u8 acFlags;
    /* 0x12 */ u8 ocFlags1;
    /* 0x13 */ u8 ocFlags2; // Flags related to which colliders it can OC collide with.
    /* 0x14 */ u8 colMaterial; // Determines hitmarks and sound effects during AC collisions. See `ColliderMaterial` enum
    /* 0x15 */ u8 shape; // See `ColliderShape` enum
} Collider; // size = 0x18

typedef struct ColliderInit {
    /* 0x00 */ u8 colMaterial;
    /* 0x01 */ u8 atFlags;
    /* 0x02 */ u8 acFlags;
    /* 0x03 */ u8 ocFlags1;
    /* 0x04 */ u8 ocFlags2;
    /* 0x05 */ u8 shape;
} ColliderInit; // size = 0x06

typedef struct ColliderInitType1 {
    /* 0x00 */ u8 colMaterial;
    /* 0x01 */ u8 atFlags;
    /* 0x02 */ u8 acFlags;
    /* 0x03 */ u8 ocFlags1;
    /* 0x04 */ u8 shape;
} ColliderInitType1; // size = 0x05

typedef struct ColliderInitToActor {
    /* 0x00 */ struct Actor* actor;
    /* 0x04 */ u8 atFlags;
    /* 0x05 */ u8 acFlags;
    /* 0x06 */ u8 ocFlags1;
    /* 0x07 */ u8 shape;
} ColliderInitToActor; // size = 0x08

typedef struct ColliderElementDamageInfoAT {
    /* 0x00 */ u32 dmgFlags; // Damage types dealt by this collider element as AT.
    /* 0x04 */ u8 effect; // Damage Effect (Knockback, Fire, etc.)
    /* 0x05 */ u8 damage; // Damage
} ColliderElementDamageInfoAT; // size = 0x08

typedef struct ColliderElementDamageInfoAC {
    /* 0x00 */ u32 dmgFlags; // Damage types that may affect this collider element as AC.
    /* 0x04 */ u8 effect;  // Damage Effect (Knockback, Fire, etc.)
    /* 0x05 */ u8 defense; // Damage Resistance
    /* 0x06 */ Vec3s hitPos; // Point of contact
} ColliderElementDamageInfoAC; // size = 0x0C

typedef struct ColliderElementDamageInfoACInit {
    /* 0x00 */ u32 dmgFlags; // Damage types that may affect this collider element as AC.
    /* 0x04 */ u8 effect; // Damage Effect (Knockback, Fire, etc.)
    /* 0x05 */ u8 defense; // Damage Resistance
} ColliderElementDamageInfoACInit; // size = 0x08

typedef struct DamageTable {
    /* 0x00 */ u8 attack[32];
} DamageTable; // size = 0x20

typedef struct CollisionCheckInfo {
    /* 0x00 */ DamageTable* damageTable;
    /* 0x04 */ Vec3f displacement; // Amount to correct actor velocity by when colliding into a body
    /* 0x10 */ s16 cylRadius; // Used for various purposes
    /* 0x12 */ s16 cylHeight; // Used for various purposes
    /* 0x14 */ s16 cylYShift; // Unused. Purpose inferred from Cylinder16 and CollisionCheck_CylSideVsLineSeg
    /* 0x16 */ u8 mass; // Used to compute displacement for OC collisions
    /* 0x17 */ u8 health; // Note: some actors may use their own health variable instead of this one
    /* 0x18 */ u8 damage; // Amount to decrement health by
    /* 0x19 */ u8 damageEffect; // Stores what effect should occur when hit by a weapon
    /* 0x1A */ u8 atHitEffect; // Stores what effect should occur when AT connects with an AC
    /* 0x1B */ u8 acHitEffect; // Stores what effect should occur when AC is touched by an AT
} CollisionCheckInfo; // size = 0x1C

#endif

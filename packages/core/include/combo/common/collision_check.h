#ifndef COMBO_COMMON_COLLISION_CHECK_H
#define COMBO_COMMON_COLLISION_CHECK_H

#include <combo/math/vec.h>
#include <combo/types.h>
#include <combo/misc.h>

#define SAC_ON (1 << 0) // CollisionContext SAC Flag

#define AT_NONE 0 // No flags set. Cannot have AT collisions when set as AT
#define AT_ON (1 << 0) // Can have AT collisions when set as AT
#define AT_HIT (1 << 1) // Had an AT collision
#define AT_BOUNCED (1 << 2) // Had an AT collision with an AC_HARD collider
#define AT_TYPE_PLAYER (1 << 3) // Has player-aligned damage
#define AT_TYPE_ENEMY (1 << 4) // Has enemy-aligned damage
#define AT_TYPE_OTHER (1 << 5) // Has non-aligned damage
#define AT_SELF (1 << 6) // Can have AT collisions with colliders attached to the same actor
#define AT_TYPE_ALL (AT_TYPE_PLAYER | AT_TYPE_ENEMY | AT_TYPE_OTHER) // Has all three damage alignments

#define AC_NONE 0 // No flags set. Cannot have AC collisions when set as AC
#define AC_ON (1 << 0) // Can have AC collisions when set as AC
#define AC_HIT (1 << 1) // Had an AC collision
#define AC_HARD (1 << 2) // Causes AT colliders to bounce off it
#define AC_TYPE_PLAYER AT_TYPE_PLAYER // Takes player-aligned damage
#define AC_TYPE_ENEMY AT_TYPE_ENEMY // Takes enemy-aligned damage
#define AC_TYPE_OTHER AT_TYPE_OTHER // Takes non-aligned damage
#define AC_NO_DAMAGE (1 << 6) // Collider does not take damage
#define AC_BOUNCED (1 << 7) // Caused an AT collider to bounce off it
#define AC_TYPE_ALL (AC_TYPE_PLAYER | AC_TYPE_ENEMY | AC_TYPE_OTHER) // Takes damage from all three alignments

#define OC1_NONE 0 // No flags set. Cannot have OC collisions when set as OC
#define OC1_ON (1 << 0) // Can have OC collisions when set as OC
#define OC1_HIT (1 << 1) // Had an OC collision
#define OC1_NO_PUSH (1 << 2) // Does not push other colliders away during OC collisions
#define OC1_TYPE_PLAYER (1 << 3) // Can have OC collisions with OC type player
#define OC1_TYPE_1 (1 << 4) // Can have OC collisions with OC type 1
#define OC1_TYPE_2 (1 << 5) // Can have OC collisions with OC type 2
#define OC1_TYPE_ALL (OC1_TYPE_PLAYER | OC1_TYPE_1 | OC1_TYPE_2) // Can have collisions with all three OC types

#define OC2_NONE 0 // No flags set. Has no OC type
#define OC2_HIT_PLAYER (1 << 0) // Had an OC collision with OC type player
#define OC2_UNK1 (1 << 1) // Prevents OC collisions with OC2_UNK2. Some horses and toki_sword have it.
#define OC2_UNK2 (1 << 2) // Prevents OC collisions with OC2_UNK1. Nothing has it.
#define OC2_TYPE_PLAYER OC1_TYPE_PLAYER // Has OC type player
#define OC2_TYPE_1 OC1_TYPE_1 // Has OC type 1
#define OC2_TYPE_2 OC1_TYPE_2 // Has OC type 2
#define OC2_FIRST_ONLY (1 << 6) // Skips AC checks on elements after the first collision. Only used by Ganon

#define ATELEM_NONE 0 // No flags set. Cannot have AT collisions
#define ATELEM_ON (1 << 0) // Can have AT collisions
#define ATELEM_HIT (1 << 1) // Had an AT collision
#define ATELEM_NEAREST (1 << 2) // For COLSHAPE_QUAD colliders, only collide with the closest AC element
#define ATELEM_SFX_MASK (3 << 3)
#define ATELEM_SFX_NORMAL (0 << 3) // Hit sound effect based on AC collider's type
#define ATELEM_SFX_HARD (1 << 3) // Always uses hard deflection sound
#define ATELEM_SFX_WOOD (2 << 3) // Always uses wood deflection sound
#define ATELEM_SFX_NONE (3 << 3) // No hit sound effect
#define ATELEM_AT_HITMARK (1 << 5) // Draw hitmarks for every AT collision
#define ATELEM_DREW_HITMARK (1 << 6) // Already drew hitmark for this frame
#define ATELEM_UNK7 (1 << 7) // Unknown purpose. Used by some enemy quads

#define ACELEM_NONE 0 // No flags set. Cannot have AC collisions
#define ACELEM_ON (1 << 0) // Can have AC collisions
#define ACELEM_HIT (1 << 1) // Had an AC collision
#define ACELEM_HOOKABLE (1 << 2) // Can be hooked if actor has hookability flags set.
#define ACELEM_NO_AT_INFO (1 << 3) // Does not give its info to the AT collider that hit it.
#define ACELEM_NO_DAMAGE (1 << 4) // Does not take damage.
#define ACELEM_NO_SWORD_SFX (1 << 5) // Does not have a sound effect when hit by player-attached AT colliders.
#define ACELEM_NO_HITMARK (1 << 6) // Skips hit effects.
#define ACELEM_DRAW_HITMARK (1 << 7) // Draw hitmark for AC collision this frame.

#define OCELEM_NONE 0 // No flags set. Cannot have OC collisions
#define OCELEM_ON (1 << 0) // Can have OC collisions
#define OCELEM_HIT (1 << 1) // Had an OC collision
#define OCELEM_UNK2 (1 << 2) // Unknown purpose.
#define OCELEM_UNK3 (1 << 3) // Unknown purpose. Used by Dead Hand element 0 and Dodongo element 5

#define OCLINE_NONE 0 // Did not have an OcLine collision
#define OCLINE_HIT (1 << 0) // Had an OcLine collision

#define DMG_ENTRY(damage, effect) ((damage) | ((effect) << 4))

struct Actor;
struct PlayState;

typedef enum ColliderMaterial {
    /*  0 */ COL_MATERIAL_HIT0, // Blue blood, white hitmark
    /*  1 */ COL_MATERIAL_HIT1, // No blood, dust hitmark
    /*  2 */ COL_MATERIAL_HIT2, // Green blood, dust hitmark
    /*  3 */ COL_MATERIAL_HIT3, // No blood, white hitmark
    /*  4 */ COL_MATERIAL_HIT4, // Water burst, no hitmark
    /*  5 */ COL_MATERIAL_HIT5, // No blood, red hitmark
    /*  6 */ COL_MATERIAL_HIT6, // Green blood, white hitmark
    /*  7 */ COL_MATERIAL_HIT7, // Red blood, white hitmark
    /*  8 */ COL_MATERIAL_HIT8, // Blue blood, red hitmark
    /*  9 */ COL_MATERIAL_METAL,
    /* 10 */ COL_MATERIAL_NONE,
    /* 11 */ COL_MATERIAL_WOOD,
    /* 12 */ COL_MATERIAL_HARD,
    /* 13 */ COL_MATERIAL_TREE
} ColliderMaterial;

typedef enum ElementMaterial {
    /* 0 */ ELEM_MATERIAL_UNK0,
    /* 1 */ ELEM_MATERIAL_UNK1,
    /* 2 */ ELEM_MATERIAL_UNK2,
    /* 3 */ ELEM_MATERIAL_UNK3,
    /* 4 */ ELEM_MATERIAL_UNK4,
    /* 5 */ ELEM_MATERIAL_UNK5,
    /* 6 */ ELEM_MATERIAL_UNK6,
    /* 7 */ ELEM_MATERIAL_UNK7
} ElementMaterial;

typedef enum HitSpecialEffect {
    HIT_SPECIAL_EFFECT_NONE,
    HIT_SPECIAL_EFFECT_FIRE,
    HIT_SPECIAL_EFFECT_ICE,
    HIT_SPECIAL_EFFECT_ELECTRIC,
    HIT_SPECIAL_EFFECT_KNOCKBACK,
    HIT_SPECIAL_EFFECT_7 = 7, // Same effect as `HIT_SPECIAL_EFFECT_NONE`
    HIT_SPECIAL_EFFECT_8, // Same effect as `HIT_SPECIAL_EFFECT_NONE`
    HIT_SPECIAL_EFFECT_9 // Same effect as `HIT_SPECIAL_EFFECT_NONE`
} HitSpecialEffect;

typedef enum HitBacklash {
    HIT_BACKLASH_NONE,
    HIT_BACKLASH_ELECTRIC
} HitBacklash;

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

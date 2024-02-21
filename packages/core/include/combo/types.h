#ifndef COMBO_TYPES_H
#define COMBO_TYPES_H

#include <ultra64.h>

#define NORETURN    __attribute__((noreturn))
#define PACKED      __attribute__((packed))
#define ALIGNED(x)  __attribute__((aligned(x)))
#define UNREACHABLE __builtin_unreachable
#define UNUSED(x)   ((void)x)

#define PALIGN(val, align)  ((void*)ALIGN(((u32)(val)), align))

#define ABS_ALT(x) ((x) < 0 ? -(x) : (x))

typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
} Vec3s; // size = 0x6

typedef struct {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 z;
} Vec3i; // size = 0xC

typedef struct {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 z;
} Vec3f; // size = 0xC

typedef struct {
    /* 0x0 */ u16 x;
    /* 0x2 */ u16 y;
    /* 0x4 */ u16 z;
} Vec3us; // size = 0x6

typedef struct {
    /* 0x0 */ f32 distance;
    /* 0x4 */ s16 angle;
} VecPolar; // size = 0x8

typedef struct {
    /* 0x0 */ Vec3s center;
    /* 0x6 */ s16   radius;
} Sphere16; // size = 0x8

typedef struct {
    /* 0x0 */ Vec3f center;
    /* 0xC */ f32   radius;
} Spheref; // size = 0x10

/*
The plane paramaters are of form `ax + by + cz + d = 0`
where `(a,b,c)` is the plane's normal vector and d is the originDist
 */
typedef struct {
    /* 0x00 */ Vec3f normal;
    /* 0x0C */ f32   originDist;
} Plane; // size = 0x10

typedef struct {
    /* 0x00 */ Vec3f vtx[3];
    /* 0x24 */ Plane plane;
} TriNorm; // size = 0x34

typedef struct {
    /* 0x0 */ s16   radius;
    /* 0x2 */ s16   height;
    /* 0x4 */ s16   yShift;
    /* 0x6 */ Vec3s pos;
} Cylinder16; // size = 0xC

typedef struct {
    /* 0x00 */ f32   radius;
    /* 0x04 */ f32   height;
    /* 0x08 */ f32   yShift;
    /* 0x0C */ Vec3f pos;
} Cylinderf; // size = 0x18

typedef struct {
    /* 0x00 */ Vec3f point;
    /* 0x0C */ Vec3f dir;
} InfiniteLine; // size = 0x18

typedef struct {
    /* 0x00 */ Vec3f a;
    /* 0x0C */ Vec3f b;
} Linef; // size = 0x18

typedef struct {
    /* 0x0 */ f32 r; // radius
    /* 0x4 */ s16 pitch; // depends on coordinate system. See below.
    /* 0x6 */ s16 yaw; // azimuthal angle
} VecSphGeo; // size = 0x8

typedef float MtxF_t[4][4];
typedef union {
    MtxF_t mf;
    struct {
        float xx, yx, zx, wx,
              xy, yy, zy, wy,
              xz, yz, zz, wz,
              xw, yw, zw, ww;
    };
} MtxF; // size = 0x40

#define TRUNCF_BINANG(f) (s16)(s32)(f)

// Angle conversion macros
#define DEG_TO_BINANG(degrees) (s16)TRUNCF_BINANG((degrees) * (0x8000 / 180.0f))
#define RAD_TO_BINANG(radians) (s16)TRUNCF_BINANG((radians) * (0x8000 / M_PI))
#define RAD_TO_DEG(radians) ((radians) * (180.0f / M_PI))
#define DEG_TO_RAD(degrees) ((degrees) * (M_PI / 180.0f))
#define BINANG_TO_DEG(binang) ((f32)(binang) * (180.0f / 0x8000))
#define BINANG_TO_RAD(binang) ((f32)(binang) * (M_PI / 0x8000))
#define BINANG_TO_RAD_ALT(binang) (((f32)(binang) / (f32)0x8000) * M_PI)
#define BINANG_TO_RAD_ALT2(binang) (((f32)(binang) * M_PI) / 0x8000)

struct Actor;
struct PlayState;

typedef enum {
    /*  0 */ COLTYPE_HIT0, // Blue blood, white hitmark
    /*  1 */ COLTYPE_HIT1, // No blood, dust hitmark
    /*  2 */ COLTYPE_HIT2, // Green blood, dust hitmark
    /*  3 */ COLTYPE_HIT3, // No blood, white hitmark
    /*  4 */ COLTYPE_HIT4, // Water burst, no hitmark
    /*  5 */ COLTYPE_HIT5, // No blood, red hitmark
    /*  6 */ COLTYPE_HIT6, // Green blood, white hitmark
    /*  7 */ COLTYPE_HIT7, // Red blood, white hitmark
    /*  8 */ COLTYPE_HIT8, // Blue blood, red hitmark
    /*  9 */ COLTYPE_METAL,
    /* 10 */ COLTYPE_NONE,
    /* 11 */ COLTYPE_WOOD,
    /* 12 */ COLTYPE_HARD,
    /* 13 */ COLTYPE_TREE
} ColliderType;

typedef enum {
    /* 0 */ COLSHAPE_JNTSPH,
    /* 1 */ COLSHAPE_CYLINDER,
    /* 2 */ COLSHAPE_TRIS,
    /* 3 */ COLSHAPE_QUAD,
    /* 4 */ COLSHAPE_MAX
} ColliderShape;

typedef enum {
    /* 0 */ ELEMTYPE_UNK0,
    /* 1 */ ELEMTYPE_UNK1,
    /* 2 */ ELEMTYPE_UNK2,
    /* 3 */ ELEMTYPE_UNK3,
    /* 4 */ ELEMTYPE_UNK4,
    /* 5 */ ELEMTYPE_UNK5,
    /* 6 */ ELEMTYPE_UNK6,
    /* 7 */ ELEMTYPE_UNK7
} ElementType;

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

#define TOUCH_NONE 0 // No flags set. Cannot have AT collisions
#define TOUCH_ON (1 << 0) // Can have AT collisions
#define TOUCH_HIT (1 << 1) // Had an AT collision
#define TOUCH_NEAREST (1 << 2) // If a Quad, only collides with the closest bumper
#define TOUCH_SFX_NORMAL (0 << 3) // Hit sound effect based on AC collider's type
#define TOUCH_SFX_HARD (1 << 3) // Always uses hard deflection sound
#define TOUCH_SFX_WOOD (2 << 3) // Always uses wood deflection sound
#define TOUCH_SFX_NONE (3 << 3) // No hit sound effect
#define TOUCH_AT_HITMARK (1 << 5) // Draw hitmarks for every AT collision
#define TOUCH_DREW_HITMARK (1 << 6) // Already drew hitmark for this frame
#define TOUCH_UNK7 (1 << 7) // Unknown purpose. Used by some enemy quads

#define BUMP_NONE 0 // No flags set. Cannot have AC collisions
#define BUMP_ON (1 << 0) // Can have AC collisions
#define BUMP_HIT (1 << 1) // Had an AC collision
#define BUMP_HOOKABLE (1 << 2) // Can be hooked if actor has hookability flags set.
#define BUMP_NO_AT_INFO (1 << 3) // Does not give its info to the AT collider that hit it.
#define BUMP_NO_DAMAGE (1 << 4) // Does not take damage.
#define BUMP_NO_SWORD_SFX (1 << 5) // Does not have a sound when hit by player-attached AT colliders.
#define BUMP_NO_HITMARK (1 << 6) // Skips hit effects.
#define BUMP_DRAW_HITMARK (1 << 7) // Draw hitmark for AC collision this frame.

#define OCELEM_NONE 0 // No flags set. Cannot have OC collisions
#define OCELEM_ON (1 << 0) // Can have OC collisions
#define OCELEM_HIT (1 << 1) // Had an OC collision
#define OCELEM_UNK2 (1 << 2) // Unknown purpose.
#define OCELEM_UNK3 (1 << 3) // Unknown purpose. Used by Dead Hand element 0 and Dodongo element 5

#define OCLINE_NONE 0 // Did not have an OcLine collision
#define OCLINE_HIT (1 << 0) // Had an OcLine collision

typedef struct {
    /* 0x00 */ struct Actor* actor; // Attached actor
    /* 0x04 */ struct Actor* at; // Actor attached to what it collided with as an AT collider.
    /* 0x08 */ struct Actor* ac; // Actor attached to what it collided with as an AC collider.
    /* 0x0C */ struct Actor* oc; // Actor attached to what it collided with as an OC collider.
    /* 0x10 */ u8 atFlags; // Information flags for AT collisions.
    /* 0x11 */ u8 acFlags; // Information flags for AC collisions.
    /* 0x12 */ u8 ocFlags1; // Information flags for OC collisions.
    /* 0x13 */ u8 ocFlags2; // Flags related to which colliders it can OC collide with.
    /* 0x14 */ u8 colType; // Determines hitmarks and sound effects during AC collisions.
    /* 0x15 */ u8 shape; // JntSph, Cylinder, Tris, or Quad
} Collider; // size = 0x18

typedef struct {
    /* 0x0 */ u8 colType; // Determines hitmarks and sound effects during AC collisions.
    /* 0x1 */ u8 atFlags; // Information flags for AT collisions.
    /* 0x2 */ u8 acFlags; // Information flags for OC collisions.
    /* 0x3 */ u8 ocFlags1; // Information flags for OC collisions.
    /* 0x4 */ u8 ocFlags2; // Flags related to which colliders it can OC collide with.
    /* 0x5 */ u8 shape; // JntSph, Cylinder, Tris, or Quad
} ColliderInit; // size = 0x6

typedef struct {
    /* 0x0 */ u8 colType; // Determines hitmarks and sound effects during AC collisions.
    /* 0x1 */ u8 atFlags; // Information flags for AT collisions.
    /* 0x2 */ u8 acFlags; // Information flags for AC collisions.
    /* 0x3 */ u8 ocFlags1; // Information flags for OC collisions.
    /* 0x4 */ u8 shape; // JntSph, Cylinder, Tris, or Quad
} ColliderInitType1; // size = 0x5

typedef struct {
    /* 0x0 */ struct Actor* actor; // Attached actor
    /* 0x4 */ u8 atFlags; // Information flags for AT collisions.
    /* 0x5 */ u8 acFlags; // Information flags for AC collisions.
    /* 0x6 */ u8 ocFlags1; // Information flags for OC collisions.
    /* 0x7 */ u8 shape; // JntSph, Cylinder, Tris, or Quad
} ColliderInitToActor; // size = 0x8

typedef struct {
    /* 0x0 */ u32 dmgFlags; // Toucher damage type flags.
    /* 0x4 */ u8 effect; // Damage Effect (Knockback, Fire, etc.)
    /* 0x5 */ u8 damage; // Damage or Stun Timer
} ColliderTouch; // size = 0x8

typedef struct {
    /* 0x0 */ u32 dmgFlags; // Toucher damage type flags.
    /* 0x4 */ u8 effect; // Damage Effect (Knockback, Fire, etc.)
    /* 0x5 */ u8 damage; // Damage or Stun Timer
} ColliderTouchInit; // size = 0x8

typedef struct {
    /* 0x0 */ u32 dmgFlags; // Bumper damage type flags.
    /* 0x4 */ u8 effect; // Damage Effect (Knockback, Fire, etc.)
    /* 0x5 */ u8 defense; // Damage Resistance
    /* 0x6 */ Vec3s hitPos; // Point of contact
} ColliderBump; // size = 0xC

typedef struct {
    /* 0x0 */ u32 dmgFlags; // Bumper exclusion mask
    /* 0x4 */ u8 effect; // Damage Effect (Knockback, Fire, etc.)
    /* 0x5 */ u8 defense; // Damage Resistance
} ColliderBumpInit; // size = 0x8

typedef struct ColliderInfo {
    /* 0x00 */ ColliderTouch toucher; // Damage properties when acting as an AT collider
    /* 0x08 */ ColliderBump bumper; // Damage properties when acting as an AC collider
    /* 0x14 */ u8 elemType; // Affects sfx reaction when attacked by Link and hookability. Full purpose unknown.
    /* 0x15 */ u8 toucherFlags; // Information flags for AT collisions
    /* 0x16 */ u8 bumperFlags; // Information flags for AC collisions
    /* 0x17 */ u8 ocElemFlags; // Information flags for OC collisions
    /* 0x18 */ Collider* atHit; // object touching this element's AT collider
    /* 0x1C */ Collider* acHit; // object touching this element's AC collider
    /* 0x20 */ struct ColliderInfo* atHitInfo; // element that hit the AT collider
    /* 0x24 */ struct ColliderInfo* acHitInfo; // element that hit the AC collider
} ColliderInfo; // size = 0x28

typedef struct {
    /* 0x00 */ u8 elemType; // Affects sfx reaction when attacked by Link and hookability. Full purpose unknown.
    /* 0x04 */ ColliderTouchInit toucher; // Damage properties when acting as an AT collider
    /* 0x0C */ ColliderBumpInit bumper; // Damage properties when acting as an AC collider
    /* 0x14 */ u8 toucherFlags; // Information flags for AT collisions
    /* 0x15 */ u8 bumperFlags; // Information flags for AC collisions
    /* 0x16 */ u8 ocElemFlags; // Information flags for OC collisions
} ColliderInfoInit; // size = 0x18

typedef struct {
    /* 0x00 */ Sphere16 modelSphere; // model space sphere
    /* 0x08 */ Sphere16 worldSphere; // world space sphere
    /* 0x10 */ f32 scale; // worldsphere = modelsphere * scale * 0.01
    /* 0x14 */ u8 limb; // attached limb
} ColliderJntSphElementDim; // size = 0x18

typedef struct {
    /* 0x0 */ u8 limb; // attached limb
    /* 0x2 */ Sphere16 modelSphere; // model space sphere
    /* 0xA */ s16 scale; // world space sphere = model * scale * 0.01
} ColliderJntSphElementDimInit; // size = 0xC

typedef struct {
    /* 0x00 */ ColliderInfo info;
    /* 0x28 */ ColliderJntSphElementDim dim;
} ColliderJntSphElement; // size = 0x40

typedef struct {
    /* 0x00 */ ColliderInfoInit info;
    /* 0x18 */ ColliderJntSphElementDimInit dim;
} ColliderJntSphElementInit; // size = 0x24

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ ColliderJntSphElement* elements;
} ColliderJntSph; // size = 0x20

typedef struct {
    /* 0x0 */ ColliderInit base;
    /* 0x8 */ s32 count;
    /* 0xC */ ColliderJntSphElementInit* elements;
} ColliderJntSphInit; // size = 0x10

typedef struct {
    /* 0x0 */ ColliderInitType1 base;
    /* 0x8 */ s32 count;
    /* 0xC */ ColliderJntSphElementInit* elements;
} ColliderJntSphInitType1; // size = 0x10

typedef struct {
    /* 0x0 */ ColliderInitToActor base;
    /* 0x8 */ s32 count;
    /* 0xC */ ColliderJntSphElementInit* elements;
} ColliderJntSphInitToActor; // size = 0x10

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ ColliderInfo info;
    /* 0x40 */ Cylinder16 dim;
} ColliderCylinder; // size = 0x4C

typedef struct {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ ColliderInfoInit info;
    /* 0x20 */ Cylinder16 dim;
} ColliderCylinderInit; // size = 0x2C

typedef struct {
    /* 0x00 */ ColliderInitType1 base;
    /* 0x08 */ ColliderInfoInit info;
    /* 0x20 */ Cylinder16 dim;
} ColliderCylinderInitType1; // size = 0x2C

typedef struct {
    /* 0x00 */ ColliderInitToActor base;
    /* 0x08 */ ColliderInfoInit info;
    /* 0x20 */ Cylinder16 dim;
} ColliderCylinderInitToActor; // size = 0x2C

typedef struct {
    /* 0x00 */ Vec3f vtx[3];
} ColliderTrisElementDimInit; // size = 0x24

typedef struct {
    /* 0x00 */ ColliderInfo info;
    /* 0x28 */ TriNorm dim;
} ColliderTrisElement; // size = 0x5C

typedef struct {
    /* 0x00 */ ColliderInfoInit info;
    /* 0x18 */ ColliderTrisElementDimInit dim;
} ColliderTrisElementInit; // size = 0x3C

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ ColliderTrisElement* elements;
} ColliderTris; // size = 0x20

typedef struct {
    /* 0x0 */ ColliderInit base;
    /* 0x8 */ s32 count;
    /* 0xC */ ColliderTrisElementInit* elements;
} ColliderTrisInit; // size = 0x10

typedef struct {
    /* 0x0 */ ColliderInitType1 base;
    /* 0x8 */ s32 count;
    /* 0xC */ ColliderTrisElementInit* elements;
} ColliderTrisInitType1; // size = 0x10

typedef struct {
    /* 0x00 */ Vec3f quad[4];
    /* 0x30 */ Vec3s dcMid; // midpoint of vectors d, c
    /* 0x36 */ Vec3s baMid; // midpoint of vectors b, a
    /* 0x3C */ f32 acDist; // distance to nearest AC collision this frame.
} ColliderQuadDim; // size = 0x40

typedef struct {
    /* 0x00 */ Vec3f quad[4];
} ColliderQuadDimInit; // size = 0x30

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ ColliderInfo info;
    /* 0x40 */ ColliderQuadDim dim;
} ColliderQuad; // size = 0x80

typedef struct {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ ColliderInfoInit info;
    /* 0x20 */ ColliderQuadDimInit dim;
} ColliderQuadInit; // size = 0x50

typedef struct {
    /* 0x00 */ ColliderInitType1 base;
    /* 0x08 */ ColliderInfoInit info;
    /* 0x20 */ ColliderQuadDimInit dim;
} ColliderQuadInitType1; // size = 0x50

typedef struct {
    /* 0x00 */ Collider base;
    /* 0x18 */ ColliderInfo info;
    /* 0x40 */ ColliderJntSphElementDim dim;
} ColliderSphere; // size = 0x58

typedef struct {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ ColliderInfoInit info;
    /* 0x20 */ ColliderJntSphElementDimInit dim;
} ColliderSphereInit; // size = 0x2C

typedef struct {
    /* 0x00 */ Linef line;
    /* 0x18 */ u16 ocFlags;
} OcLine; // size = 0x1C

typedef struct {
    /* 0x000 */ s16 colATCount;
    /* 0x002 */ u16 sacFlags; // Controls whether new collidors can be added or removed, or only swapped
    /* 0x004 */ Collider* colAT[50];
    /* 0x0CC */ s32 colACCount;
    /* 0x0D0 */ Collider* colAC[60];
    /* 0x1C0 */ s32 colOCCount;
    /* 0x1C4 */ Collider* colOC[50];
    /* 0x28C */ s32 colLineCount;
    /* 0x290 */ OcLine* colLine[3];
} CollisionCheckContext; // size = 0x29C

typedef struct {
    /* 0x00 */ s16 ambientColor[3];
    /* 0x06 */ s16 light1Color[3];
    /* 0x0C */ s16 light2Color[3];
    /* 0x12 */ s16 fogColor[3];
    /* 0x18 */ s16 fogNear;
    /* 0x1A */ s16 zFar;
} AdjLightSettings; // size = 0x1C

typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u8 color[3];
    /* 0x9 */ u8 drawGlow;
    /* 0xA */ s16 radius;
} LightPoint; // size = 0xC

typedef struct {
    /* 0x0 */ s8 x;
    /* 0x1 */ s8 y;
    /* 0x2 */ s8 z;
    /* 0x3 */ u8 color[3];
} LightDirectional; // size = 0x6

typedef union LightParams {
    LightPoint point;
    LightDirectional dir;
} LightParams; // size = 0xC

typedef struct LightInfo {
    /* 0x0 */ u8 type;
    /* 0x2 */ LightParams params;
} LightInfo; // size = 0xE

typedef struct Lights {
    /* 0x00 */ u8 enablePosLights;
    /* 0x01 */ u8 numLights;
    /* 0x08 */ Lightsn l;
} Lights; // size = 0x80

typedef struct LightNode {
    /* 0x0 */ LightInfo* info;
    /* 0x4 */ struct LightNode* prev;
    /* 0x8 */ struct LightNode* next;
} LightNode; // size = 0xC

#define LIGHTS_BUFFER_SIZE 32

typedef struct LightsBuffer {
    /* 0x000 */ s32 numOccupied;
    /* 0x004 */ s32 searchIndex;
    /* 0x008 */ LightNode lights[LIGHTS_BUFFER_SIZE];
} LightsBuffer; // size = 0x188

typedef struct LightContext {
    /* 0x0 */ LightNode* listHead;
    /* 0x4 */ u8 ambientColor[3];
    /* 0x7 */ u8 fogColor[3];
    /* 0xA */ s16 fogNear; // how close until fog starts taking effect. range 0 - 996
    /* 0xC */ s16 zFar; // draw distance. range 0 - 12800
} LightContext; // size = 0x10

typedef enum LightType {
    /* 0 */ LIGHT_POINT_NOGLOW,
    /* 1 */ LIGHT_DIRECTIONAL,
    /* 2 */ LIGHT_POINT_GLOW
} LightType;

typedef void (*LightsBindFunc)(Lights* lights, LightParams* params, Vec3f* vec);
typedef void (*LightsPosBindFunc)(Lights* lights, LightParams* params, struct PlayState* play);


// Model has limbs with only rigid meshes
typedef struct {
    /* 0x00 */ void** segment;
    /* 0x04 */ u8 limbCount;
} SkeletonHeader; // size = 0x8

// Model has limbs with flexible meshes
typedef struct {
    /* 0x00 */ SkeletonHeader sh;
    /* 0x08 */ u8 dListCount;
} FlexSkeletonHeader; // size = 0xC

typedef struct SkelAnime {
    /* 0x00 */ u8 limbCount;      // Number of limbs in the skeleton
    /* 0x01 */ u8 mode;           // 0: loop, 2: play once, 4: partial loop. +1 to interpolate between frames.
    /* 0x02 */ u8 dListCount;     // Number of display lists in a flexible skeleton
    /* 0x03 */ s8 taper;          // Tapering to use when morphing between animations. Only used by Door_Warp1.
    /* 0x04 */ void** skeleton;   // An array of pointers to limbs. Can be StandardLimb, LodLimb, or SkinLimb.
    /* 0x08 */ void* animation;   // Can be an AnimationHeader or PlayerAnimationHeader.
    /* 0x0C */ f32 startFrame;    // In mode 4, start of partial loop.
    /* 0x10 */ f32 endFrame;      // In mode 2, Update returns true when curFrame is equal to this. In mode 4, end of partial loop.
    /* 0x14 */ f32 animLength;    // Total number of frames in the current animation's file.
    /* 0x18 */ f32 curFrame;      // Current frame in the animation
    /* 0x1C */ f32 playSpeed;     // Multiplied by R_UPDATE_RATE / 3 to get the animation's frame rate.
    /* 0x20 */ Vec3s* jointTable; // Current translation of model and rotations of all limbs
    /* 0x24 */ Vec3s* morphTable; // Table of values used to morph between animations
    /* 0x28 */ f32 morphWeight;   // Weight of the current animation morph as a fraction in [0,1]
    /* 0x2C */ f32 morphRate;     // Reciprocal of the number of frames in the morph
    /* 0x30 */ union {
                    s32 (*normal)(struct SkelAnime*);// Can be Loop, Partial loop, Play once, Morph, or Tapered morph
                    s32 (*player)(struct PlayState*, struct SkelAnime*); // Loop, Play once, and Morph
                } update;
    /* 0x34 */ s8 initFlags;      // Flags used when initializing Player's skeleton
    /* 0x35 */ u8 moveFlags;      // Flags used for animations that move the actor in worldspace.
    /* 0x36 */ s16 prevRot;       // Previous rotation in worldspace.
    /* 0x38 */ Vec3s prevTransl;  // Previous modelspace translation.
    /* 0x3E */ Vec3s baseTransl;  // Base modelspace translation.
} SkelAnime; // size = 0x44

typedef struct {
    /* 0x00 */ s16   id;
    /* 0x02 */ Vec3s pos;
    /* 0x08 */ Vec3s rot;
    /* 0x0E */ s16   params;
} ActorEntry; // size = 0x10

typedef struct {
    struct {
        s8 room;    // Room to switch to
        s8 bgCamIndex; // How the camera reacts during the transition. See `Camera_ChangeDoorCam`
    } /* 0x00 */ sides[2]; // 0 = front, 1 = back
    /* 0x04 */ s16   id;
    /* 0x06 */ Vec3s pos;
    /* 0x0C */ s16   rotY;
    /* 0x0E */ s16   params;
} TransitionActorEntry; // size = 0x10

typedef struct {
    /* 0x00 */ u8 playerEntryIndex;
    /* 0x01 */ u8 room;
} Spawn;

typedef struct {
    /* 0x00 */ u8 count; // number of points in the path
    /* 0x04 */ Vec3s* points; // Segment Address to the array of points
} Path; // size = 0x8

typedef struct {
    /* 0x00 */ u8 byte0;
    /* 0x01 */ u8 byte1;
    /* 0x02 */ u8 byte2;
    /* 0x03 */ u8 byte3;
} QuestHintCmd; // size = 0x4

typedef struct {
    /* 0x00 */ OSContPad cur;
    /* 0x06 */ OSContPad prev;
    /* 0x0C */ OSContPad press; // X/Y store delta from last frame
    /* 0x12 */ OSContPad rel; // X/Y store adjusted
} Input; // size = 0x18

typedef struct {
    /* 0x00 */ u32* vromStart;
    /* 0x04 */ u32* vromEnd;
} RomFile; // size = 0x8

typedef struct {
    u8 unk_000[0x178];
} Camera;

typedef struct {
    /* 0x0 */ s16 frameCount;
} AnimationHeaderCommon; // size = 0x2

typedef struct {
    /* 0x0 */ AnimationHeaderCommon common;
    /* 0x4 */ union {
                void* segmentVoid;
                struct PlayerAnimationFrame* linkAnimSegment;
            };
} PlayerAnimationHeader; // size = 0x8

typedef struct {
    u32 cont:   1;
    u32 type:   4;
    u32 offset: 11;
    s32 value:  16;
} InitChainEntry;

typedef enum {
    /* 0x0 */ ICHAINTYPE_U8,            // sets byte
    /* 0x1 */ ICHAINTYPE_S8,
    /* 0x2 */ ICHAINTYPE_U16,           // sets short
    /* 0x3 */ ICHAINTYPE_S16,
    /* 0x4 */ ICHAINTYPE_U32,           // sets word
    /* 0x5 */ ICHAINTYPE_S32,
    /* 0x6 */ ICHAINTYPE_F32,           // sets float
    /* 0x7 */ ICHAINTYPE_F32_DIV1000,   // sets float divided by 1000
    /* 0x8 */ ICHAINTYPE_VEC3F,         // sets Vec3f members
    /* 0x9 */ ICHAINTYPE_VEC3F_DIV1000, // sets Vec3f members divided by 1000
    /* 0xA */ ICHAINTYPE_VEC3S          // sets Vec3s members
} InitChainType;

/**
 * ICHAIN macros generate an init chain entry of the following form:
 * * (e >> 31) & 0x0001 == Continue Parsing after this entry
 * * (e >> 27) & 0x000F == Type
 * * (e >> 16) & 0x07FF == Offset from start of instance to write initial value
 * *  e        & 0xFFFF == Initial Value
 *
 * Arguments:
 * * type ----- value from enum `InitChainType`
 * * member --- name of member inside `Actor` structure to use as the offset
 * * value ---- s16 value to use
 * * cont ----- ICHAIN_CONTINUE (or ICHAIN_STOP) to continue (or stop) parsing
 */
#define ICHAIN(type, member, value, cont)      \
        { cont, type, offsetof(Actor, member), value }

#define ICHAIN_U8(member, val, cont)            ICHAIN(ICHAINTYPE_U8, member, val, cont)
#define ICHAIN_S8(member, val, cont)            ICHAIN(ICHAINTYPE_S8, member, val, cont)
#define ICHAIN_U16(member, val, cont)           ICHAIN(ICHAINTYPE_U16, member, val, cont)
#define ICHAIN_S16(member, val, cont)           ICHAIN(ICHAINTYPE_S16, member, val, cont)
#define ICHAIN_U32(member, val, cont)           ICHAIN(ICHAINTYPE_U32, member, val, cont)
#define ICHAIN_S32(member, val, cont)           ICHAIN(ICHAINTYPE_S32, member, val, cont)
#define ICHAIN_F32(member, val, cont)           ICHAIN(ICHAINTYPE_F32, member, val, cont)
#define ICHAIN_F32_DIV1000(member, val, cont)   ICHAIN(ICHAINTYPE_F32_DIV1000, member, val, cont)
#define ICHAIN_VEC3F(member, val, cont)         ICHAIN(ICHAINTYPE_VEC3F, member, val, cont)
#define ICHAIN_VEC3F_DIV1000(member, val, cont) ICHAIN(ICHAINTYPE_VEC3F_DIV1000, member, val, cont)
#define ICHAIN_VEC3S(member, val, cont)         ICHAIN(ICHAINTYPE_VEC3S, member, val, cont)

#define ICHAIN_CONTINUE 1
#define ICHAIN_STOP     0

#define VTX(x,y,z,s,t,crnx,cgny,cbnz,a) { { { x, y, z }, 0, { s, t }, { crnx, cgny, cbnz, a } } }

#define MASS_IMMOVABLE 0xFF // Cannot be pushed by OC colliders
#define MASS_HEAVY 0xFE // Can only be pushed by OC colliders from actors with IMMOVABLE or HEAVY mass.

typedef enum {
    /* 0x00 */ CAM_SET_NONE,
    /* 0x01 */ CAM_SET_NORMAL0, // Generic camera 0, used in various places "NORMAL0"
    /* 0x02 */ CAM_SET_NORMAL3, // Generic camera 3, used in various places "NORMAL3"
    /* 0x03 */ CAM_SET_PIVOT_DIVING, // Player diving from the surface of the water to underwater not as zora "CIRCLE5"
    /* 0x04 */ CAM_SET_HORSE, // Reiding a horse "HORSE0"
    /* 0x05 */ CAM_SET_ZORA_DIVING, // Parallel's Pivot Diving, but as Zora. However, Zora does not dive like a human. So this setting appears to not be used "ZORA0"
    /* 0x06 */ CAM_SET_PREREND_FIXED, // Unused remnant of OoT: camera is fixed in position and rotation "PREREND0"
    /* 0x07 */ CAM_SET_PREREND_PIVOT, // Unused remnant of OoT: Camera is fixed in position with fixed pitch, but is free to rotate in the yaw direction 360 degrees "PREREND1"
    /* 0x08 */ CAM_SET_DOORC, // Generic room door transitions, camera moves and follows player as the door is open and closed "DOORC"
    /* 0x09 */ CAM_SET_DEMO0, // Unknown, possibly related to treasure chest game as goron? "DEMO0"
    /* 0x0A */ CAM_SET_FREE0, // Free Camera, manual control is given, no auto-updating eye or at "FREE0"
    /* 0x0B */ CAM_SET_BIRDS_EYE_VIEW_0, // Appears unused. Camera is a top-down view "FUKAN0"
    /* 0x0C */ CAM_SET_NORMAL1, // Generic camera 1, used in various places "NORMAL1"
    /* 0x0D */ CAM_SET_NANAME, // Unknown, slanted or tilted. Behaves identical to Normal0 except with added roll "NANAME"
    /* 0x0E */ CAM_SET_CIRCLE0, // Used in Curiosity Shop, Pirates Fortress, Mayor's Residence "CIRCLE0"
    /* 0x0F */ CAM_SET_FIXED0, // Used in Sakon's Hideout puzzle rooms, milk bar stage "FIXED0"
    /* 0x10 */ CAM_SET_SPIRAL_DOOR, // Exiting a Spiral Staircase "SPIRAL"
    /* 0x11 */ CAM_SET_DUNGEON0, // Generic dungeon camera 0, used in various places "DUNGEON0"
    /* 0x12 */ CAM_SET_ITEM0, // Getting an item and holding it above Player's head (from small chest, freestanding, npc, ...) "ITEM0"
    /* 0x13 */ CAM_SET_ITEM1, // Looking at player while playing the ocarina "ITEM1"
    /* 0x14 */ CAM_SET_ITEM2, // Bottles: drinking, releasing fairy, dropping fish "ITEM2"
    /* 0x15 */ CAM_SET_ITEM3, // Bottles: catching fish or bugs, showing an item "ITEM3"
    /* 0x16 */ CAM_SET_NAVI, // Song of Soaring, variations of playing Song of Time "NAVI"
    /* 0x17 */ CAM_SET_WARP_PAD_MOON, // Warp circles from Goron Trial on the moon "WARP0"
    /* 0x18 */ CAM_SET_DEATH, // Player death animation when health goes to 0 "DEATH"
    /* 0x19 */ CAM_SET_REBIRTH, // Unknown set with camDataId = -9 (it's not being revived by a fairy) "REBIRTH"
    /* 0x1A */ CAM_SET_LONG_CHEST_OPENING, // Long cutscene when opening a big chest with a major item "TREASURE"
    /* 0x1B */ CAM_SET_MASK_TRANSFORMATION, // Putting on a transformation mask "TRANSFORM"
    /* 0x1C */ CAM_SET_ATTENTION, // Unknown, set with camDataId = -15 "ATTENTION"
    /* 0x1D */ CAM_SET_WARP_PAD_ENTRANCE, // Warp pad from start of a dungeon to the boss-room "WARP1"
    /* 0x1E */ CAM_SET_DUNGEON1, // Generic dungeon camera 1, used in various places "DUNGEON1"
    /* 0x1F */ CAM_SET_FIXED1, // Fixes camera in place, used in various places eg. entering Stock Pot Inn, hiting a switch, giving witch a red potion, shop browsing "FIXED1"
    /* 0x20 */ CAM_SET_FIXED2, // Used in Pinnacle Rock after defeating Sea Monsters, and by Tatl in Fortress "FIXED2"
    /* 0x21 */ CAM_SET_MAZE, // Unused. Set to use Camera_Parallel2(), which is only Camera_Noop() "MAZE"
    /* 0x22 */ CAM_SET_REMOTEBOMB, // Unused. Set to use Camera_Parallel2(), which is only Camera_Noop(). But also related to Play_ChangeCameraSetting? "REMOTEBOMB"
    /* 0x23 */ CAM_SET_CIRCLE1, // Unknown "CIRCLE1"
    /* 0x24 */ CAM_SET_CIRCLE2, // Looking at far-away NPCs eg. Garo in Road to Ikana, Hungry Goron, Tingle "CIRCLE2"
    /* 0x25 */ CAM_SET_CIRCLE3, // Used in curiosity shop, goron racetrack, final room in Sakon's hideout, other places "CIRCLE3"
    /* 0x26 */ CAM_SET_CIRCLE4, // Used during the races on the doggy racetrack "CIRCLE4"
    /* 0x27 */ CAM_SET_FIXED3, // Used in Stock Pot Inn Toilet and Tatl cutscene after woodfall "FIXED3"
    /* 0x28 */ CAM_SET_TOWER_ASCENT, // Various climbing structures (Snowhead climb to the temple entrance) "TOWER0"
    /* 0x29 */ CAM_SET_PARALLEL0, // Unknown "PARALLEL0"
    /* 0x2A */ CAM_SET_NORMALD, // Unknown, set with camDataId = -20 "NORMALD"
    /* 0x2B */ CAM_SET_SUBJECTD, // Unknown, set with camDataId = -21 "SUBJECTD"
    /* 0x2C */ CAM_SET_START0, // Entering a room, either Dawn of a New Day reload, or entering a door where the camera is fixed on the other end "START0"
    /* 0x2D */ CAM_SET_START2, // Entering a scene, camera is put at a low angle eg. Grottos, Deku Palace, Stock Pot Inn "START2"
    /* 0x2E */ CAM_SET_STOP0, // Called in z_play "STOP0"
    /* 0x2F */ CAM_SET_BOAT_CRUISE, //  Koume's boat cruise "JCRUISING"
    /* 0x30 */ CAM_SET_VERTICAL_CLIMB, // Large vertical climbs, such as Mountain Village wall or Pirates Fortress ladder. "CLIMBMAZE"
    /* 0x31 */ CAM_SET_SIDED, // Unknown, set with camDataId = -24 "SIDED"
    /* 0x32 */ CAM_SET_DUNGEON2, // Generic dungeon camera 2, used in various places "DUNGEON2"
    /* 0x33 */ CAM_SET_BOSS_ODOLWA, // Odolwa's Lair, also used in GBT entrance: "BOSS_SHIGE"
    /* 0x34 */ CAM_SET_KEEPBACK, // Unknown. Possibly related to climbing something? "KEEPBACK"
    /* 0x35 */ CAM_SET_CIRCLE6, // Used in select regions from Ikana "CIRCLE6"
    /* 0x36 */ CAM_SET_CIRCLE7, // Unknown "CIRCLE7"
    /* 0x37 */ CAM_SET_MINI_BOSS, // Used during the various minibosses of the "CHUBOSS"
    /* 0x38 */ CAM_SET_RFIXED1, // Talking to Koume stuck on the floor in woods of mystery "RFIXED1"
    /* 0x39 */ CAM_SET_TREASURE_CHEST_MINIGAME, // Treasure Chest Shop in East Clock Town, minigame location "TRESURE1"
    /* 0x3A */ CAM_SET_HONEY_AND_DARLING_1, // Honey and Darling Minigames "BOMBBASKET"
    /* 0x3B */ CAM_SET_CIRCLE8, // Used by Stone Tower moving platforms, Falling eggs in Marine Lab, Bugs into soilpatch cutscene "CIRCLE8"
    /* 0x3C */ CAM_SET_BIRDS_EYE_VIEW_1, // Camera is a top-down view. Used in Fisherman's minigame and Deku Palace "FUKAN1"
    /* 0x3D */ CAM_SET_DUNGEON3, // Generic dungeon camera 3, used in various places "DUNGEON3"
    /* 0x3E */ CAM_SET_TELESCOPE, // Observatory telescope and Curiosity Shop Peep-Hole "TELESCOPE"
    /* 0x3F */ CAM_SET_ROOM0, // Certain rooms eg. inside the clock tower "ROOM0"
    /* 0x40 */ CAM_SET_RCIRC0, // Used by a few NPC cutscenes, focus close on the NPC "RCIRC0"
    /* 0x41 */ CAM_SET_CIRCLE9, // Used by Sakon Hideout entrance and Deku Palace Maze "CIRCLE9"
    /* 0x42 */ CAM_SET_ONTHEPOLE, // Somewhere in Snowhead Temple and Woodfall Temple "ONTHEPOLE"
    /* 0x43 */ CAM_SET_INBUSH, // Various bush environments eg. grottos, Swamp Spider House, Termina Field grass bushes, Deku Palace near bean "INBUSH"
    /* 0x44 */ CAM_SET_BOSS_MAJORA, // Majora's Lair: "BOSS_LAST"
    /* 0x45 */ CAM_SET_BOSS_TWINMOLD, // Twinmold's Lair: "BOSS_INI"
    /* 0x46 */ CAM_SET_BOSS_GOHT, // Goht's Lair: "BOSS_HAK"
    /* 0x47 */ CAM_SET_BOSS_GYORG, // Gyorg's Lair: "BOSS_KON"
    /* 0x48 */ CAM_SET_CONNECT0, // Smoothly and gradually return camera to Player after a cutscene "CONNECT0"
    /* 0x49 */ CAM_SET_PINNACLE_ROCK, // Pinnacle Rock pit "MORAY"
    /* 0x4A */ CAM_SET_NORMAL2, // Generic camera 2, used in various places "NORMAL2"
    /* 0x4B */ CAM_SET_HONEY_AND_DARLING_2, // "BOMBBOWL"
    /* 0x4C */ CAM_SET_CIRCLEA, // Unknown, Circle 10 "CIRCLEA"
    /* 0x4D */ CAM_SET_WHIRLPOOL, // Great Bay Temple Central Room Whirlpool "WHIRLPOOL"
    /* 0x4E */ CAM_SET_CUCCO_SHACK, // "KOKKOGAME"
    /* 0x4F */ CAM_SET_GIANT, // Giants Mask in Twinmold's Lair "GIANT"
    /* 0x50 */ CAM_SET_SCENE0, // Entering doors to a new scene "SCENE0"
    /* 0x51 */ CAM_SET_ROOM1, // Certain rooms eg. some rooms in Stock Pot Inn "ROOM1"
    /* 0x52 */ CAM_SET_WATER2, // Swimming as Zora in Great Bay Temple "WATER2"
    /* 0x53 */ CAM_SET_WOODFALL_SWAMP, // Woodfall inside the swamp, but not on the platforms, "SOKONASI"
    /* 0x54 */ CAM_SET_FORCEKEEP, // Unknown "FORCEKEEP"
    /* 0x55 */ CAM_SET_PARALLEL1, // Unknown "PARALLEL1"
    /* 0x56 */ CAM_SET_START1, // Used when entering the lens cave "START1"
    /* 0x57 */ CAM_SET_ROOM2, // Certain rooms eg. Deku King's Chamber, Ocean Spider House "ROOM2"
    /* 0x58 */ CAM_SET_NORMAL4, // Generic camera 4, used in Ikana Graveyard "NORMAL4"
    /* 0x59 */ CAM_SET_ELEGY_SHELL, // cutscene after playing elegy of emptyness and spawning a shell "SHELL"
    /* 0x5A */ CAM_SET_DUNGEON4, // Used in Pirates Fortress Interior, hidden room near hookshot "DUNGEON4"
    /* 0x5B */ CAM_SET_MAX
} CameraSettingType;

typedef enum {
    /* 0 */ ROOM_BEHAVIOR_TYPE1_0,
    /* 1 */ ROOM_BEHAVIOR_TYPE1_1,
    /* 2 */ ROOM_BEHAVIOR_TYPE1_2,
    /* 3 */ ROOM_BEHAVIOR_TYPE1_3, // unused
    /* 4 */ ROOM_BEHAVIOR_TYPE1_4,
    /* 5 */ ROOM_BEHAVIOR_TYPE1_5
} RoomBehaviorType1;

typedef enum {
    /* 0 */ ROOM_BEHAVIOR_TYPE2_0,
    /* 1 */ ROOM_BEHAVIOR_TYPE2_1,
    /* 2 */ ROOM_BEHAVIOR_TYPE2_2,
    /* 3 */ ROOM_BEHAVIOR_TYPE2_HOT,
    /* 4 */ ROOM_BEHAVIOR_TYPE2_4,
    /* 5 */ ROOM_BEHAVIOR_TYPE2_5,
    /* 6 */ ROOM_BEHAVIOR_TYPE2_6
} RoomBehaviorType2;

#define SQ(x) ((x)*(x))
#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define DECR(x) ((x) == 0 ? 0 : --(x))
#define CLAMP(x, min, max) ((x) < (min) ? (min) : (x) > (max) ? (max) : (x))
#define CLAMP_MAX(x, max) ((x) > (max) ? (max) : (x))
#define CLAMP_MIN(x, min) ((x) < (min) ? (min) : (x))

//! @TODO: Verify from OoT (may not be the same)
typedef enum {
    /* 0 */ PRECIP_RAIN_MAX, // max number of raindrops that can draw; uses this or SOS_MAX, whichever is larger
    /* 1 */ PRECIP_RAIN_CUR, // current number of rain drops being drawn on screen
    /* 2 */ PRECIP_SNOW_CUR, // current number of snowflakes being drawn on screen
    /* 3 */ PRECIP_SNOW_MAX, // max number of snowflakes that can draw
    /* 4 */ PRECIP_SOS_MAX, // max number of rain drops requested from song of storms specifically
    /* 5 */ PRECIP_MAX
} PrecipitationData;

typedef struct
{
    u32 vstart;
    u32 vend;
    u32 pstart;
    u32 pend;
}
DmaEntry;

typedef struct
{
    u32     vromAddr;
    void*   dramAddr;
    u32     size;
    char*   name;
    s32     line;
    u32     unk[3];
}
DmaRequest;

#define GET_ACTIVE_CAM(play) ((play)->cameraPtrs[(play)->activeCamId])

typedef struct {
    /* 0x0 */ u16 flags;         // Only the bottom two bits are used, although others are set in objects
    /* 0x2 */ s16 abscissa;      // knot input value
    /* 0x4 */ s16 leftGradient;  // left derivative at the point
    /* 0x6 */ s16 rightGradient; // right derivative at the point
    /* 0x8 */ f32 ordinate;      // output value
} CurveInterpKnot; // size = 0xC

typedef struct {
    /* 0x0 */ u8* knotCounts;
    /* 0x4 */ CurveInterpKnot* interpolationData;
    /* 0x8 */ s16* constantData;
    /* 0xC */ s16 unk_0C;     // Set but not used, always 1 in objects
    /* 0xE */ s16 frameCount; // Not used, inferred from use in objects
} CurveAnimationHeader; // size = 0x10

typedef struct {
    /* 0x0 */ u8 child;
    /* 0x1 */ u8 sibling;
    /* 0x4 */ Gfx* dList[2];
} SkelCurveLimb; // size = 0xC

typedef struct {
    /* 0x0 */ SkelCurveLimb** limbs;
    /* 0x4 */ u8 limbCount;
} CurveSkeletonHeader; // size = 0x8

typedef struct {
    /* 0x00 */ u8 limbCount;
    /* 0x04 */ SkelCurveLimb** skeleton;
    /* 0x08 */ CurveAnimationHeader* animation;
    /* 0x0C */ f32 unk_0C; // set but not used
    /* 0x10 */ f32 endFrame;
    /* 0x14 */ f32 playSpeed;
    /* 0x18 */ f32 curFrame;
    /* 0x1C */ s16 (*jointTable)[9];
} SkelCurve; // size = 0x20

#define LIMB_DONE 0xFF

typedef struct Viewport {
    /* 0x00 */ s32 topY;    // uly (upper left y)
    /* 0x04 */ s32 bottomY; // lry (lower right y)
    /* 0x08 */ s32 leftX;   // ulx (upper left x)
    /* 0x0C */ s32 rightX;  // lrx (lower right x)
} Viewport; // size = 0x10

typedef union {
    struct {
        /* 0x00 */ u16 id; // "dousa"
        /* 0x02 */ u16 startFrame;
        /* 0x04 */ u16 endFrame;
        /* 0x06 */ Vec3us rot;
        /* 0x0C */ Vec3i startPos;
        /* 0x18 */ Vec3i endPos;
        /* 0x24 */ Vec3f normal;
    };
    s32 _words[12];
} CsCmdActorCue; // size = 0x30

typedef union CutsceneData {
    s32 i;
    f32 f;
    s16 s[2];
    s8  b[4];
} CutsceneData;

typedef struct {
    /* 0x0 */ CutsceneData* script;
    /* 0x4 */ s16 nextEntrance;
    /* 0x6 */ u8 spawn;
    /* 0x7 */ u8 spawnFlags; // See `CS_SPAWN_FLAG_`
} CutsceneScriptEntry; // size = 0x8

// ZAPD compatibility typedefs
// TODO: Remove when ZAPD adds support for them
typedef CutsceneScriptEntry CutsceneEntry;

#if defined(GAME_MM)
// TODO consolidate type with oot's type
typedef struct {
    /* 0x00 */ u8 scriptListCount;
    /* 0x04 */ CutsceneData* script;
    /* 0x08 */ u8 state;
    /* 0x0C */ f32 timer;
    /* 0x10 */ u16 curFrame;
    /* 0x12 */ u16 scriptIndex;
    /* 0x14 */ s32 subCamId;
    /* 0x18 */ u16 camEyeSplinePointsAppliedFrame; // Remnant of OoT. Set but never used.
    /* 0x1C */ u8 unk_1C[0xA]; // Remnant of cam data from OoT
    /* 0x24 */ CsCmdActorCue* playerCue;
    /* 0x28 */ CsCmdActorCue* actorCues[10]; // "npcdemopnt"
    /* 0x50 */ CutsceneScriptEntry* scriptList;
} CutsceneContext; // size = 0x54
#endif

typedef enum {
    /* 0 */ CS_STATE_IDLE,
    /* 1 */ CS_STATE_START,
    /* 2 */ CS_STATE_RUN,
    /* 3 */ CS_STATE_STOP,
    /* 4 */ CS_STATE_RUN_UNSTOPPABLE
} CutsceneState;

typedef enum RespawnMode {
    /* 0 */ RESPAWN_MODE_DOWN,                          // "RESTART_MODE_DOWN"
    /* 1 */ RESPAWN_MODE_RETURN,                        // "RESTART_MODE_RETURN"
    /* 2 */ RESPAWN_MODE_TOP,                           // "RESTART_MODE_TOP"
    /* 3 */ RESPAWN_MODE_UNK_3,                         // Related to grottos
    /* 4 */ RESPAWN_MODE_GORON,                         // "RESTART_MODE_GORON"
    /* 5 */ RESPAWN_MODE_ZORA,                          // "RESTART_MODE_ZORA"
    /* 6 */ RESPAWN_MODE_DEKU,                          // "RESTART_MODE_NUTS"
    /* 7 */ RESPAWN_MODE_HUMAN,                         // "RESTART_MODE_CHILD"
    /* 8 */ RESPAWN_MODE_MAX
} RespawnMode;

#define GROTTO_CHEST_FLAG(save) (save.respawn[RESPAWN_MODE_UNK_3].data)

typedef enum {
    /*  0 */ TRANS_TYPE_WIPE,
    /*  1 */ TRANS_TYPE_TRIFORCE,
    /*  2 */ TRANS_TYPE_FADE_BLACK,
    /*  3 */ TRANS_TYPE_FADE_WHITE,
    /*  4 */ TRANS_TYPE_FADE_BLACK_FAST,
    /*  5 */ TRANS_TYPE_FADE_WHITE_FAST,
    /*  6 */ TRANS_TYPE_FADE_BLACK_SLOW,
    /*  7 */ TRANS_TYPE_FADE_WHITE_SLOW,
    /*  8 */ TRANS_TYPE_WIPE_FAST,
    /*  9 */ TRANS_TYPE_FILL_WHITE_FAST, // Not actually fast due to bug in Play_UpdateTransition
    /* 10 */ TRANS_TYPE_FILL_WHITE,
    /* 11 */ TRANS_TYPE_INSTANT,
    /* 12 */ TRANS_TYPE_FILL_BROWN,
    /* 13 */ TRANS_TYPE_FADE_WHITE_CS_DELAYED,
    /* 14 */ TRANS_TYPE_SANDSTORM_PERSIST,
    /* 15 */ TRANS_TYPE_SANDSTORM_END,
    /* 16 */ TRANS_TYPE_CS_BLACK_FILL,
    /* 17 */ TRANS_TYPE_FADE_WHITE_INSTANT,
    /* 18 */ TRANS_TYPE_FADE_GREEN,
    /* 19 */ TRANS_TYPE_FADE_BLUE,
    /* 20 */ TRANS_TYPE_FADE_DYNAMIC, // Chooses Black or White based on time of day
    /* 21 */ TRANS_TYPE_CIRCLE,
    /* 22 */ TRANS_TYPE_WIPE5,
    // transition types 23 - 31 are unused
    // transition types 32 - 39 are Wipe4 TODO needs macro
    /* 38 */ TRANS_TYPE_38 = 38,
    // transition types 40 - 63 are unused
    // transition types 64 - 127 are Wipe3 TODO needs macro
    /* 64 */ TRANS_TYPE_64 = 64,
    /* 70 */ TRANS_TYPE_70 = 70,
    /* 72 */ TRANS_TYPE_72 = 72,
    /* 73 */ TRANS_TYPE_73,
    /* 80 */ TRANS_TYPE_80 = 80,
    /* 86 */ TRANS_TYPE_86 = 86
} TransitionType;

typedef struct {
    /* 0x00 */ u16 type;
    union {
        u16 vtxData[3];
        struct {
            /* 0x02 */ u16 flags_vIA; // 0xE000 is poly exclusion flags (xpFlags), 0x1FFF is vtxId
            /* 0x04 */ u16 flags_vIB; // 0xE000 is flags, 0x1FFF is vtxId
                                      // 0x2000 = poly IsFloorConveyor surface
            /* 0x06 */ u16 vIC;
        };
    };
    /* 0x08 */ Vec3s normal; // Unit normal vector
                             // Value ranges from -0x7FFF to 0x7FFF, representing -1.0 to 1.0; 0x8000 is invalid

    /* 0x0E */ s16 dist; // Plane distance from origin along the normal
} CollisionPoly; // size = 0x10

typedef struct DamageTable {
    /* 0x00 */ u8 attack[32];
} DamageTable; // size = 0x20

typedef struct CollisionCheckInfoInit {
    /* 0x0 */ u8 health;
    /* 0x2 */ s16 cylRadius;
    /* 0x4 */ s16 cylHeight;
    /* 0x6 */ u8 mass;
} CollisionCheckInfoInit; // size = 0x8

typedef struct CollisionCheckInfoInit2 {
    /* 0x0 */ u8 health;
    /* 0x2 */ s16 cylRadius;
    /* 0x4 */ s16 cylHeight;
    /* 0x6 */ s16 cylYShift;
    /* 0x8 */ u8 mass;
} CollisionCheckInfoInit2; // size = 0xC

typedef struct CollisionCheckInfo {
    /* 0x00 */ DamageTable* damageTable;
    /* 0x04 */ Vec3f displacement;
    /* 0x10 */ s16 cylRadius;
    /* 0x12 */ s16 cylHeight;
    /* 0x14 */ s16 cylYShift;
    /* 0x16 */ u8 mass;
    /* 0x17 */ u8 health;
    /* 0x18 */ u8 damage;
    /* 0x19 */ u8 damageEffect;
    /* 0x1A */ u8 atHitEffect;
    /* 0x1B */ u8 acHitEffect;
} CollisionCheckInfo; // size = 0x1C

#define BGCHECKFLAG_GROUND (1 << 0) // Standing on the ground
#define BGCHECKFLAG_GROUND_TOUCH (1 << 1) // Has touched the ground (only active for 1 frame)
#define BGCHECKFLAG_GROUND_LEAVE (1 << 2) // Has left the ground (only active for 1 frame)
#define BGCHECKFLAG_WALL (1 << 3) // Touching a wall
#define BGCHECKFLAG_CEILING (1 << 4) // Touching a ceiling
#define BGCHECKFLAG_WATER (1 << 5) // In water
#define BGCHECKFLAG_WATER_TOUCH (1 << 6) // Has touched water (reset when leaving water)
#define BGCHECKFLAG_GROUND_STRICT (1 << 7) // Strictly on ground (BGCHECKFLAG_GROUND has some leeway)
#define BGCHECKFLAG_CRUSHED (1 << 8) // Crushed between a floor and ceiling (triggers a void for player)
#define BGCHECKFLAG_PLAYER_WALL_INTERACT (1 << 9) // Only set/used by player, related to interacting with walls

typedef enum {
    /* 0x00 */ CAM_MODE_NORMAL, // "NORMAL"
    /* 0x01 */ CAM_MODE_JUMP, // "JUMP"
    /* 0x02 */ CAM_MODE_GORONDASH, // "GORONDASH"
    /* 0x03 */ CAM_MODE_DEKUSHOOT, // "NUTSSHOT"
    /* 0x04 */ CAM_MODE_BOWARROWZ, // "BOWARROWZ"
    /* 0x05 */ CAM_MODE_DEKUFLY, // "NUTSFLY"
    /* 0x06 */ CAM_MODE_FIRSTPERSON, // "SUBJECT"
    /* 0x07 */ CAM_MODE_FOLLOWBOOMERANG, // "BOOKEEPON"
    /* 0x08 */ CAM_MODE_ZORAFIN, // "ZORAFIN"
    /* 0x09 */ CAM_MODE_FOLLOWTARGET, // "KEEPON"
    /* 0x0A */ CAM_MODE_TARGET, // "PARALLEL"
    /* 0x0B */ CAM_MODE_TALK, // "TALK"
    /* 0x0C */ CAM_MODE_SLINGSHOT, // "PACHINCO"
    /* 0x0D */ CAM_MODE_BOWARROW, // "BOWARROW"
    /* 0x0E */ CAM_MODE_BATTLE, // "BATTLE"
    /* 0x0F */ CAM_MODE_DEKUHIDE, // "NUTSHIDE"
    /* 0x10 */ CAM_MODE_STILL, // "STILL"
    /* 0x11 */ CAM_MODE_CHARGE, // "CHARGE"
    /* 0x12 */ CAM_MODE_CLIMB, // "CLIMB"
    /* 0x13 */ CAM_MODE_CLIMBZ, // "CLIMBZ"
    /* 0x14 */ CAM_MODE_HOOKSHOT, // "FOOKSHOT"
    /* 0x15 */ CAM_MODE_FREEFALL, // "FREEFALL"
    /* 0x16 */ CAM_MODE_HANG, // "HANG"
    /* 0x17 */ CAM_MODE_HANGZ, // "HANGZ
    /* 0x18 */ CAM_MODE_PUSHPULL, // "PUSHPULL"
    /* 0x19 */ CAM_MODE_DEKUFLYZ, // "CNUTSFLYZ"
    /* 0x1A */ CAM_MODE_GORONJUMP, // "GORONJUMP"
    /* 0x1B */ CAM_MODE_BOOMERANG, // "BOOMERANG"
    /* 0x1C */ CAM_MODE_CHARGEZ, // "CHARGEZ"
    /* 0x1D */ CAM_MODE_ZORAFINZ, // "ZORAFINZ"
    /* 0x1E */ CAM_MODE_MAX
} CameraModeType;

typedef enum {
    /* 0x00 */ DO_ACTION_ATTACK,
    /* 0x01 */ DO_ACTION_CHECK,
    /* 0x02 */ DO_ACTION_ENTER,
    /* 0x03 */ DO_ACTION_RETURN,
    /* 0x04 */ DO_ACTION_OPEN,
    /* 0x05 */ DO_ACTION_JUMP,
    /* 0x06 */ DO_ACTION_DECIDE,
    /* 0x07 */ DO_ACTION_DIVE,
    /* 0x08 */ DO_ACTION_FASTER,
    /* 0x09 */ DO_ACTION_THROW,
    /* 0x0A */ DO_ACTION_NONE,  // in do_action_static, the texture at this position is NAVI, however this value is in practice the "No Action" value
    /* 0x0B */ DO_ACTION_CLIMB,
    /* 0x0C */ DO_ACTION_DROP,
    /* 0x0D */ DO_ACTION_DOWN,
    /* 0x0E */ DO_ACTION_QUIT,
    /* 0x0F */ DO_ACTION_SPEAK,
    /* 0x10 */ DO_ACTION_NEXT,
    /* 0x11 */ DO_ACTION_GRAB,
    /* 0x12 */ DO_ACTION_STOP,
    /* 0x13 */ DO_ACTION_PUTAWAY,
    /* 0x14 */ DO_ACTION_REEL,
    /* 0x15 */ DO_ACTION_INFO,
    /* 0x16 */ DO_ACTION_WARP,
    /* 0x17 */ DO_ACTION_SNAP,
    /* 0x18 */ DO_ACTION_EXPLODE,
    /* 0x19 */ DO_ACTION_DANCE,
    /* 0x1A */ DO_ACTION_MARCH,
    /* 0x1B */ DO_ACTION_1,
    /* 0x1C */ DO_ACTION_2,
    /* 0x1D */ DO_ACTION_3,
    /* 0x1E */ DO_ACTION_4,
    /* 0x1F */ DO_ACTION_5,
    /* 0x20 */ DO_ACTION_6,
    /* 0x21 */ DO_ACTION_7,
    /* 0x22 */ DO_ACTION_8,
    /* 0x23 */ DO_ACTION_CURL,
    /* 0x24 */ DO_ACTION_SURFACE,
    /* 0x25 */ DO_ACTION_SWIM,
    /* 0x26 */ DO_ACTION_PUNCH,
    /* 0x27 */ DO_ACTION_POUND,
    /* 0x28 */ DO_ACTION_HOOK,
    /* 0x29 */ DO_ACTION_SHOOT,
    /* 0x2A */ DO_ACTION_MAX,
    /* 0x2A */ TATL_STATE_2A = DO_ACTION_MAX,
    /* 0x2B */ TATL_STATE_2B,
    /* 0x2C */ TATL_STATE_2C
} DoAction;

typedef struct {
    /* 0x0 */ s8 sceneId;
    /* 0x1 */ s8 spawnNum;
    /* 0x2 */ u16 flags;
} EntranceTableEntry; // size = 0x4

#define UPDBGCHECKINFO_FLAG_0 (1 << 0) // check wall
#define UPDBGCHECKINFO_FLAG_1 (1 << 1) // check ceiling
#define UPDBGCHECKINFO_FLAG_2 (1 << 2) // check floor and water
#define UPDBGCHECKINFO_FLAG_3 (1 << 3)
#define UPDBGCHECKINFO_FLAG_4 (1 << 4)
#define UPDBGCHECKINFO_FLAG_5 (1 << 5) // unused
#define UPDBGCHECKINFO_FLAG_6 (1 << 6) // disable water ripples
#define UPDBGCHECKINFO_FLAG_7 (1 << 7) // alternate wall check?

// MM

#define UPDBGCHECKINFO_FLAG_8 (1 << 8)
#define UPDBGCHECKINFO_FLAG_9 (1 << 9)
#define UPDBGCHECKINFO_FLAG_10 (1 << 10) // check water
#define UPDBGCHECKINFO_FLAG_11 (1 << 11)

#endif /* TYPES_H */

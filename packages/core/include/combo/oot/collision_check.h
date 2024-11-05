#ifndef COMBO_OOT_COLLISION_CHECK_H
#define COMBO_OOT_COLLISION_CHECK_H

#include <combo/common/collision_check.h>

#define COLLISION_CHECK_AT_MAX 50
#define COLLISION_CHECK_AC_MAX 60
#define COLLISION_CHECK_OC_MAX 50
#define COLLISION_CHECK_OC_LINE_MAX 3

struct Actor;
struct GraphicsContext;
struct PlayState;

/*
 * Bases for all shapes of colliders
 */

typedef enum ColliderShape {
    /* 0 */ COLSHAPE_JNTSPH,
    /* 1 */ COLSHAPE_CYLINDER,
    /* 2 */ COLSHAPE_TRIS,
    /* 3 */ COLSHAPE_QUAD,
    /* 4 */ COLSHAPE_MAX
} ColliderShape;

typedef struct ColliderElement {
    /* 0x00 */ ColliderElementDamageInfoAT atDmgInfo; // Damage properties when acting as an AT collider
    /* 0x08 */ ColliderElementDamageInfoAC acDmgInfo; // Damage properties when acting as an AC collider
    /* 0x14 */ u8 elemMaterial; // Affects sfx when attacked by Player, and interaction with hookshot and arrows.
    /* 0x15 */ u8 atElemFlags; // Information flags for AT collisions
    /* 0x16 */ u8 acElemFlags; // Information flags for AC collisions
    /* 0x17 */ u8 ocElemFlags; // Information flags for OC collisions
    /* 0x18 */ Collider* atHit; // object touching this element's AT collider
    /* 0x1C */ Collider* acHit; // object touching this element's AC collider
    /* 0x20 */ struct ColliderElement* atHitElem; // element that hit the AT collider
    /* 0x24 */ struct ColliderElement* acHitElem; // element that hit the AC collider
} ColliderElement; // size = 0x28

typedef struct ColliderElementInit {
    /* 0x00 */ u8 elemMaterial; // Affects sfx when attacked by Player, and interaction with hookshot and arrows.
    /* 0x04 */ ColliderElementDamageInfoAT atDmgInfo; // Damage properties when acting as an AT collider
    /* 0x0C */ ColliderElementDamageInfoACInit acDmgInfo; // Damage properties when acting as an AC collider
    /* 0x14 */ u8 atElemFlags; // Information flags for AT collisions
    /* 0x15 */ u8 acElemFlags;  // Information flags for AC collisions
    /* 0x16 */ u8 ocElemFlags; // Information flags for OC collisions
} ColliderElementInit; // size = 0x18

/*
 * JntSph - A collider made of sphere shaped elements. Each sphere can attach to a skeleton joint (limb).
 */

// collider structs

typedef struct ColliderJntSphElementDim {
    /* 0x00 */ Sphere16 modelSphere; // model space sphere
    /* 0x08 */ Sphere16 worldSphere; // world space sphere
    /* 0x10 */ f32 scale; // world space sphere = model * scale * 0.01
    /* 0x14 */ u8 limb; // attached limb
} ColliderJntSphElementDim; // size = 0x18

typedef struct ColliderJntSphElement {
    /* 0x00 */ ColliderElement base;
    /* 0x28 */ ColliderJntSphElementDim dim;
} ColliderJntSphElement; // size = 0x40

typedef struct ColliderJntSph {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ ColliderJntSphElement* elements;
} ColliderJntSph; // size = 0x20

// init data structs

typedef struct ColliderJntSphElementDimInit {
    /* 0x00 */ u8 limb; // attached limb
    /* 0x02 */ Sphere16 modelSphere; // model space sphere
    /* 0x0A */ s16 scale; // world space sphere = model * scale * 0.01
} ColliderJntSphElementDimInit; // size = 0x0C

typedef struct ColliderJntSphElementInit {
    /* 0x00 */ ColliderElementInit base;
    /* 0x18 */ ColliderJntSphElementDimInit dim;
} ColliderJntSphElementInit; // size = 0x24

typedef struct ColliderJntSphInit {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ s32 count;
    /* 0x0C */ ColliderJntSphElementInit* elements;
} ColliderJntSphInit; // size = 0x10

typedef struct ColliderJntSphInitType1 {
    /* 0x00 */ ColliderInitType1 base;
    /* 0x08 */ s32 count;
    /* 0x0C */ ColliderJntSphElementInit* elements;
} ColliderJntSphInitType1; // size = 0x10

typedef struct ColliderJntSphInitToActor {
    /* 0x00 */ ColliderInitToActor base;
    /* 0x08 */ s32 count;
    /* 0x0C */ ColliderJntSphElementInit* elements;
} ColliderJntSphInitToActor; // size = 0x10

/*
 * Cylinder - A single cylinder shaped collider
 */

// collider structs

typedef struct ColliderCylinder {
    /* 0x00 */ Collider base;
    /* 0x18 */ ColliderElement elem;
    /* 0x40 */ Cylinder16 dim;
} ColliderCylinder; // size = 0x4C

// init data structs

typedef struct ColliderCylinderInit {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ ColliderElementInit elem;
    /* 0x20 */ Cylinder16 dim;
} ColliderCylinderInit; // size = 0x2C

typedef struct ColliderCylinderInitType1 {
    /* 0x00 */ ColliderInitType1 base;
    /* 0x08 */ ColliderElementInit elem;
    /* 0x20 */ Cylinder16 dim;
} ColliderCylinderInitType1; // size = 0x2C

typedef struct ColliderCylinderInitToActor {
    /* 0x00 */ ColliderInitToActor base;
    /* 0x08 */ ColliderElementInit elem;
    /* 0x20 */ Cylinder16 dim;
} ColliderCylinderInitToActor; // size = 0x2C

/*
 * Tris - A collider made of triangle shaped elements
 */

// collider structs

typedef struct ColliderTrisElement {
    /* 0x00 */ ColliderElement base;
    /* 0x28 */ TriNorm dim;
} ColliderTrisElement; // size = 0x5C

typedef struct ColliderTris {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ ColliderTrisElement* elements;
} ColliderTris; // size = 0x20

// init data structs

typedef struct ColliderTrisElementDimInit {
    /* 0x00 */ Vec3f vtx[3];
} ColliderTrisElementDimInit; // size = 0x24

typedef struct ColliderTrisElementInit {
    /* 0x00 */ ColliderElementInit base;
    /* 0x18 */ ColliderTrisElementDimInit dim;
} ColliderTrisElementInit; // size = 0x3C

typedef struct ColliderTrisInit {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ s32 count;
    /* 0x0C */ ColliderTrisElementInit* elements;
} ColliderTrisInit; // size = 0x10

typedef struct ColliderTrisInitType1 {
    /* 0x00 */ ColliderInitType1 base;
    /* 0x08 */ s32 count;
    /* 0x0C */ ColliderTrisElementInit* elements;
} ColliderTrisInitType1; // size = 0x10

/*
 * Quad - A single quad shaped collider
 */

// collider structs

typedef struct ColliderQuadDim {
    /* 0x00 */ Vec3f quad[4];
    /* 0x30 */ Vec3s dcMid; // midpoint of vectors d, c
    /* 0x36 */ Vec3s baMid; // midpoint of vectors b, a
    /* 0x3C */ f32 acDistSq; // distance to nearest AC collision this frame, squared.
} ColliderQuadDim; // size = 0x40

typedef struct ColliderQuad {
    /* 0x00 */ Collider base;
    /* 0x18 */ ColliderElement elem;
    /* 0x40 */ ColliderQuadDim dim;
} ColliderQuad; // size = 0x80

// init data structs

typedef struct ColliderQuadDimInit {
    /* 0x00 */ Vec3f quad[4];
} ColliderQuadDimInit; // size = 0x30

typedef struct ColliderQuadInit {
    /* 0x00 */ ColliderInit base;
    /* 0x08 */ ColliderElementInit elem;
    /* 0x20 */ ColliderQuadDimInit dim;
} ColliderQuadInit; // size = 0x50

typedef struct ColliderQuadInitType1 {
    /* 0x00 */ ColliderInitType1 base;
    /* 0x08 */ ColliderElementInit elem;
    /* 0x20 */ ColliderQuadDimInit dim;
} ColliderQuadInitType1; // size = 0x50

/*
 * Line collider
 */

typedef struct OcLine {
    /* 0x00 */ Linef line;
    /* 0x18 */ u16 ocFlags;
} OcLine; // size = 0x1C

typedef struct CollisionCheckContext {
    /* 0x000 */ s16 colATCount;
    /* 0x002 */ u16 sacFlags;
    /* 0x004 */ Collider* colAT[COLLISION_CHECK_AT_MAX];
    /* 0x0CC */ s32 colACCount;
    /* 0x0D0 */ Collider* colAC[COLLISION_CHECK_AC_MAX];
    /* 0x1C0 */ s32 colOCCount;
    /* 0x1C4 */ Collider* colOC[COLLISION_CHECK_OC_MAX];
    /* 0x28C */ s32 colLineCount;
    /* 0x290 */ OcLine* colLine[COLLISION_CHECK_OC_LINE_MAX];
} CollisionCheckContext; // size = 0x29C

/*
 * Collider properties, for all shapes
 */

#define DMG_DEKU_NUT     (1 << 0)
#define DMG_DEKU_STICK   (1 << 1)
#define DMG_SLINGSHOT    (1 << 2)
#define DMG_EXPLOSIVE    (1 << 3)
#define DMG_BOOMERANG    (1 << 4)
#define DMG_ARROW_NORMAL (1 << 5)
#define DMG_HAMMER_SWING (1 << 6)
#define DMG_HOOKSHOT     (1 << 7)
#define DMG_SLASH_KOKIRI (1 << 8)
#define DMG_SLASH_MASTER (1 << 9)
#define DMG_SLASH_GIANT  (1 << 10)
#define DMG_ARROW_FIRE   (1 << 11)
#define DMG_ARROW_ICE    (1 << 12)
#define DMG_ARROW_LIGHT  (1 << 13)
#define DMG_ARROW_UNK1   (1 << 14)
#define DMG_ARROW_UNK2   (1 << 15)
#define DMG_ARROW_UNK3   (1 << 16)
#define DMG_MAGIC_FIRE   (1 << 17)
#define DMG_MAGIC_ICE    (1 << 18)
#define DMG_MAGIC_LIGHT  (1 << 19)
#define DMG_SHIELD       (1 << 20)
#define DMG_MIR_RAY      (1 << 21)
#define DMG_SPIN_KOKIRI  (1 << 22)
#define DMG_SPIN_GIANT   (1 << 23)
#define DMG_SPIN_MASTER  (1 << 24)
#define DMG_JUMP_KOKIRI  (1 << 25)
#define DMG_JUMP_GIANT   (1 << 26)
#define DMG_JUMP_MASTER  (1 << 27)
#define DMG_UNKNOWN_1    (1 << 28)
#define DMG_UNBLOCKABLE  (1 << 29)
#define DMG_HAMMER_JUMP  (1 << 30)
#define DMG_UNKNOWN_2    (1 << 31)

#define DMG_SLASH (DMG_SLASH_KOKIRI | DMG_SLASH_MASTER | DMG_SLASH_GIANT)
#define DMG_SPIN_ATTACK (DMG_SPIN_KOKIRI | DMG_SPIN_MASTER | DMG_SPIN_GIANT)
#define DMG_JUMP_SLASH (DMG_JUMP_KOKIRI | DMG_JUMP_MASTER | DMG_JUMP_GIANT)
#define DMG_SWORD (DMG_SLASH | DMG_SPIN_ATTACK | DMG_JUMP_SLASH)
#define DMG_HAMMER (DMG_HAMMER_SWING | DMG_HAMMER_JUMP)
#define DMG_FIRE (DMG_ARROW_FIRE | DMG_MAGIC_FIRE)
#define DMG_ARROW (DMG_ARROW_NORMAL | DMG_ARROW_FIRE | DMG_ARROW_ICE | DMG_ARROW_LIGHT | DMG_ARROW_UNK1 | DMG_ARROW_UNK2 | DMG_ARROW_UNK3)
#define DMG_RANGED (DMG_ARROW | DMG_HOOKSHOT | DMG_SLINGSHOT)
#define DMG_DEFAULT ~(DMG_SHIELD | DMG_MIR_RAY)

typedef struct CollisionCheckInfoInit {
    /* 0x00 */ u8 health;
    /* 0x02 */ s16 cylRadius;
    /* 0x04 */ s16 cylHeight;
    /* 0x06 */ u8 mass;
} CollisionCheckInfoInit;

typedef struct CollisionCheckInfoInit2 {
    /* 0x00 */ u8 health;
    /* 0x02 */ s16 cylRadius;
    /* 0x04 */ s16 cylHeight;
    /* 0x06 */ s16 cylYShift;
    /* 0x08 */ u8 mass;
} CollisionCheckInfoInit2;

DamageTable* DamageTable_Get(s32 index);
void DamageTable_Clear(DamageTable* table);
s32 Collider_InitJntSph(struct PlayState* play, ColliderJntSph* jntSph);
s32 Collider_FreeJntSph(struct PlayState* play, ColliderJntSph* jntSph);
s32 Collider_DestroyJntSph(struct PlayState* play, ColliderJntSph* jntSph);
s32 Collider_SetJntSphToActor(struct PlayState* play, ColliderJntSph* dest, ColliderJntSphInitToActor* src);
s32 Collider_SetJntSphAllocType1(struct PlayState* play, ColliderJntSph* dest, struct Actor* actor,
                                 ColliderJntSphInitType1* src);
s32 Collider_SetJntSphAlloc(struct PlayState* play, ColliderJntSph* dest, struct Actor* actor, ColliderJntSphInit* src);
s32 Collider_SetJntSph(struct PlayState* play, ColliderJntSph* dest, struct Actor* actor, ColliderJntSphInit* src,
                       ColliderJntSphElement* jntSphElements);
s32 Collider_ResetJntSphAT(struct PlayState* play, Collider* col);
s32 Collider_ResetJntSphAC(struct PlayState* play, Collider* col);
s32 Collider_ResetJntSphOC(struct PlayState* play, Collider* col);
s32 Collider_InitCylinder(struct PlayState* play, ColliderCylinder* cyl);
s32 Collider_DestroyCylinder(struct PlayState* play, ColliderCylinder* cyl);
s32 Collider_SetCylinderToActor(struct PlayState* play, ColliderCylinder* dest, ColliderCylinderInitToActor* src);
s32 Collider_SetCylinderType1(struct PlayState* play, ColliderCylinder* dest, struct Actor* actor, ColliderCylinderInitType1* src);
s32 Collider_SetCylinder(struct PlayState* play, ColliderCylinder* dest, struct Actor* actor, ColliderCylinderInit* src);
s32 Collider_ResetCylinderAT(struct PlayState* play, Collider* col);
s32 Collider_ResetCylinderAC(struct PlayState* play, Collider* col);
s32 Collider_ResetCylinderOC(struct PlayState* play, Collider* col);
s32 Collider_InitTris(struct PlayState* play, ColliderTris* tris);
s32 Collider_FreeTris(struct PlayState* play, ColliderTris* tris);
s32 Collider_DestroyTris(struct PlayState* play, ColliderTris* tris);
s32 Collider_SetTrisAllocType1(struct PlayState* play, ColliderTris* dest, struct Actor* actor, ColliderTrisInitType1* src);
s32 Collider_SetTrisAlloc(struct PlayState* play, ColliderTris* dest, struct Actor* actor, ColliderTrisInit* src);
s32 Collider_SetTris(struct PlayState* play, ColliderTris* dest, struct Actor* actor, ColliderTrisInit* src,
                     ColliderTrisElement* trisElements);
s32 Collider_ResetTrisAT(struct PlayState* play, Collider* col);
s32 Collider_ResetTrisAC(struct PlayState* play, Collider* col);
s32 Collider_ResetTrisOC(struct PlayState* play, Collider* col);
s32 Collider_InitQuad(struct PlayState* play, ColliderQuad* quad);
s32 Collider_DestroyQuad(struct PlayState* play, ColliderQuad* quad);
s32 Collider_SetQuadType1(struct PlayState* play, ColliderQuad* dest, struct Actor* actor, ColliderQuadInitType1* src);
s32 Collider_SetQuad(struct PlayState* play, ColliderQuad* dest, struct Actor* actor, ColliderQuadInit* src);
s32 Collider_ResetQuadAT(struct PlayState* play, Collider* col);
s32 Collider_ResetQuadAC(struct PlayState* play, Collider* col);
s32 Collider_ResetQuadOC(struct PlayState* play, Collider* col);
s32 Collider_InitLine(struct PlayState* play, OcLine* line);
s32 Collider_DestroyLine(struct PlayState* play, OcLine* line);
s32 Collider_SetLinePoints(struct PlayState* play, OcLine* ocLine, Vec3f* a, Vec3f* b);
s32 Collider_SetLine(struct PlayState* play, OcLine* dest, OcLine* src);
s32 Collider_ResetLineOC(struct PlayState* play, OcLine* line);
void CollisionCheck_InitContext(struct PlayState* play, CollisionCheckContext* colChkCtx);
void CollisionCheck_DestroyContext(struct PlayState* play, CollisionCheckContext* colChkCtx);
void CollisionCheck_ClearContext(struct PlayState* play, CollisionCheckContext* colChkCtx);
void CollisionCheck_EnableSAC(struct PlayState* play, CollisionCheckContext* colChkCtx);
void CollisionCheck_DisableSAC(struct PlayState* play, CollisionCheckContext* colChkCtx);
s32 CollisionCheck_SetAT(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
s32 CollisionCheck_SetAT_SAC(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider, s32 index);
s32 CollisionCheck_SetAC(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
s32 CollisionCheck_SetAC_SAC(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider, s32 index);
s32 CollisionCheck_SetOC(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
s32 CollisionCheck_SetOC_SAC(struct PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider, s32 index);
s32 CollisionCheck_SetOCLine(struct PlayState* play, CollisionCheckContext* colChkCtx, OcLine* collider);
void CollisionCheck_BlueBlood(struct PlayState* play, Collider* collider, Vec3f* v);
void CollisionCheck_AT(struct PlayState* play, CollisionCheckContext* colChkCtx);
void CollisionCheck_OC(struct PlayState* play, CollisionCheckContext* colChkCtx);
void CollisionCheck_InitInfo(CollisionCheckInfo* info);
void CollisionCheck_ResetDamage(CollisionCheckInfo* info);
void CollisionCheck_SetInfoNoDamageTable(CollisionCheckInfo* info, CollisionCheckInfoInit* init);
void CollisionCheck_SetInfo(CollisionCheckInfo* info, DamageTable* damageTable, CollisionCheckInfoInit* init);
void CollisionCheck_SetInfo2(CollisionCheckInfo* info, DamageTable* damageTable, CollisionCheckInfoInit2* init);
void CollisionCheck_SetInfoGetDamageTable(CollisionCheckInfo* info, s32 index, CollisionCheckInfoInit2* init);
void CollisionCheck_Damage(struct PlayState* play, CollisionCheckContext* colChkCtx);
s32 CollisionCheck_LineOCCheckAll(struct PlayState* play, CollisionCheckContext* colChkCtx, Vec3f* a, Vec3f* b);
s32 CollisionCheck_LineOCCheck(struct PlayState* play, CollisionCheckContext* colChkCtx, Vec3f* a, Vec3f* b,
                               struct Actor** exclusions, s32 numExclusions);
void Collider_UpdateCylinder(struct Actor* actor, ColliderCylinder* cyl);
void Collider_SetCylinderPosition(ColliderCylinder* cyl, Vec3s* pos);
void Collider_SetQuadVertices(ColliderQuad* quad, Vec3f* a, Vec3f* b, Vec3f* c, Vec3f* d);
void Collider_SetTrisVertices(ColliderTris* tris, s32 elemIndex, Vec3f* a, Vec3f* b, Vec3f* c);
void Collider_SetTrisDim(struct PlayState* play, ColliderTris* tris, s32 elemIndex, ColliderTrisElementDimInit* src);
void Collider_UpdateSpheres(s32 limb, ColliderJntSph* jntSph);
void CollisionCheck_SpawnRedBlood(struct PlayState* play, Vec3f* v);
void CollisionCheck_SpawnWaterDroplets(struct PlayState* play, Vec3f* v);
void CollisionCheck_SpawnShieldParticles(struct PlayState* play, Vec3f* v);
void CollisionCheck_SpawnShieldParticlesMetal(struct PlayState* play, Vec3f* v);
void CollisionCheck_SpawnShieldParticlesMetalSfx(struct PlayState* play, Vec3f* v, Vec3f* pos);
void CollisionCheck_SpawnShieldParticlesMetal2(struct PlayState* play, Vec3f* v);
void CollisionCheck_SpawnShieldParticlesWood(struct PlayState* play, Vec3f* v, Vec3f* actorPos);
s32 CollisionCheck_CylSideVsLineSeg(f32 radius, f32 height, f32 offset, Vec3f* actorPos, Vec3f* itemPos,
                                    Vec3f* itemProjPos, Vec3f* out1, Vec3f* out2);
u8 CollisionCheck_GetSwordDamage(s32 dmgFlags);

#endif

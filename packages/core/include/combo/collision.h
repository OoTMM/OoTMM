#ifndef COMBO_COLLISION_H
#define COMBO_COLLISION_H

#include <combo/collision_check.h>
#include <combo/types.h>
#include <combo/math/vec.h>

#define BG_ACTOR_MAX        50
#define BGCHECK_SCENE       BG_ACTOR_MAX
#define BGCHECK_Y_MIN       -32000.0f
#define BGCHECK_XYZ_ABSMAX  32760.0f

#if defined(GAME_OOT)
# define DMG_DEKU_NUT     (1 << 0)
# define DMG_DEKU_STICK   (1 << 1)
# define DMG_SLINGSHOT    (1 << 2)
# define DMG_EXPLOSIVE    (1 << 3)
# define DMG_BOOMERANG    (1 << 4)
# define DMG_ARROW_NORMAL (1 << 5)
# define DMG_HAMMER_SWING (1 << 6)
# define DMG_HOOKSHOT     (1 << 7)
# define DMG_SLASH_KOKIRI (1 << 8)
# define DMG_SLASH_MASTER (1 << 9)
# define DMG_SLASH_GIANT  (1 << 10)
# define DMG_ARROW_FIRE   (1 << 11)
# define DMG_ARROW_ICE    (1 << 12)
# define DMG_ARROW_LIGHT  (1 << 13)
# define DMG_ARROW_UNK1   (1 << 14)
# define DMG_ARROW_UNK2   (1 << 15)
# define DMG_ARROW_UNK3   (1 << 16)
# define DMG_MAGIC_FIRE   (1 << 17)
# define DMG_MAGIC_ICE    (1 << 18)
# define DMG_MAGIC_LIGHT  (1 << 19)
# define DMG_SHIELD       (1 << 20)
# define DMG_MIR_RAY      (1 << 21)
# define DMG_SPIN_KOKIRI  (1 << 22)
# define DMG_SPIN_GIANT   (1 << 23)
# define DMG_SPIN_MASTER  (1 << 24)
# define DMG_JUMP_KOKIRI  (1 << 25)
# define DMG_JUMP_GIANT   (1 << 26)
# define DMG_JUMP_MASTER  (1 << 27)
# define DMG_UNKNOWN_1    (1 << 28)
# define DMG_UNBLOCKABLE  (1 << 29)
# define DMG_HAMMER_JUMP  (1 << 30)
# define DMG_UNKNOWN_2    (1 << 31)

# define DMG_SLASH (DMG_SLASH_KOKIRI | DMG_SLASH_MASTER | DMG_SLASH_GIANT)
# define DMG_SPIN_ATTACK (DMG_SPIN_KOKIRI | DMG_SPIN_MASTER | DMG_SPIN_GIANT)
# define DMG_JUMP_SLASH (DMG_JUMP_KOKIRI | DMG_JUMP_MASTER | DMG_JUMP_GIANT)
# define DMG_SWORD (DMG_SLASH | DMG_SPIN_ATTACK | DMG_JUMP_SLASH)
# define DMG_HAMMER (DMG_HAMMER_SWING | DMG_HAMMER_JUMP)
# define DMG_FIRE (DMG_ARROW_FIRE | DMG_MAGIC_FIRE)
# define DMG_ARROW (DMG_ARROW_NORMAL | DMG_ARROW_FIRE | DMG_ARROW_ICE | DMG_ARROW_LIGHT | DMG_ARROW_UNK1 | DMG_ARROW_UNK2 | DMG_ARROW_UNK3)
# define DMG_RANGED (DMG_ARROW | DMG_HOOKSHOT | DMG_SLINGSHOT)
# define DMG_DEFAULT ~(DMG_SHIELD | DMG_MIR_RAY)
#endif

#if defined(GAME_MM)
# define DMG_ENTRY(damage, effect) ((damage) | ((effect) << 4))

#define DMG_DEKU_NUT       (1 << 0x00)
#define DMG_DEKU_STICK     (1 << 0x01)
#define DMG_HORSE_TRAMPLE  (1 << 0x02)
#define DMG_EXPLOSIVES     (1 << 0x03)
#define DMG_ZORA_BOOMERANG (1 << 0x04)
#define DMG_NORMAL_ARROW   (1 << 0x05)
#define DMG_UNK_0x06       (1 << 0x06)
#define DMG_HOOKSHOT       (1 << 0x07)
#define DMG_GORON_PUNCH    (1 << 0x08)
#define DMG_SWORD          (1 << 0x09)
#define DMG_GORON_POUND    (1 << 0x0A)
#define DMG_FIRE_ARROW     (1 << 0x0B)
#define DMG_ICE_ARROW      (1 << 0x0C)
#define DMG_LIGHT_ARROW    (1 << 0x0D)
#define DMG_GORON_SPIKES   (1 << 0x0E)
#define DMG_DEKU_SPIN      (1 << 0x0F)
#define DMG_DEKU_BUBBLE    (1 << 0x10)
#define DMG_DEKU_LAUNCH    (1 << 0x11)
#define DMG_UNK_0x12       (1 << 0x12)
#define DMG_ZORA_BARRIER   (1 << 0x13)
#define DMG_NORMAL_SHIELD  (1 << 0x14)
#define DMG_LIGHT_RAY      (1 << 0x15)
#define DMG_THROWN_OBJECT  (1 << 0x16)
#define DMG_ZORA_PUNCH     (1 << 0x17)
#define DMG_SPIN_ATTACK    (1 << 0x18)
#define DMG_SWORD_BEAM     (1 << 0x19)
#define DMG_NORMAL_ROLL    (1 << 0x1A)
#define DMG_UNK_0x1B       (1 << 0x1B)
#define DMG_UNK_0x1C       (1 << 0x1C)
#define DMG_UNBLOCKABLE    (1 << 0x1D)
#define DMG_UNK_0x1E       (1 << 0x1E)
#define DMG_POWDER_KEG     (1 << 0x1F)

#endif

/* MM bgcheck things */
#if defined(GAME_MM)
typedef enum FloorType {
    /*  0 */ FLOOR_TYPE_0,
    /*  1 */ FLOOR_TYPE_1,
    /*  2 */ FLOOR_TYPE_2,
    /*  3 */ FLOOR_TYPE_3,
    /*  4 */ FLOOR_TYPE_4,
    /*  5 */ FLOOR_TYPE_5,
    /*  6 */ FLOOR_TYPE_6,
    /*  7 */ FLOOR_TYPE_7,
    /*  8 */ FLOOR_TYPE_8,
    /*  9 */ FLOOR_TYPE_9,
    /* 10 */ FLOOR_TYPE_10,
    /* 11 */ FLOOR_TYPE_11,
    /* 12 */ FLOOR_TYPE_12,
    /* 13 */ FLOOR_TYPE_13,
    /* 14 */ FLOOR_TYPE_14,
    /* 15 */ FLOOR_TYPE_15
} FloorType;

typedef enum SurfaceSfxOffset {
    /*  0 */ SURFACE_SFX_OFFSET_DIRT,
    /*  1 */ SURFACE_SFX_OFFSET_SAND,
    /*  2 */ SURFACE_SFX_OFFSET_STONE,
    /*  3 */ SURFACE_SFX_OFFSET_DIRT_SHALLOW,
    /*  4 */ SURFACE_SFX_OFFSET_WATER_SHALLOW,
    /*  5 */ SURFACE_SFX_OFFSET_WATER_DEEP,
    /*  6 */ SURFACE_SFX_OFFSET_TALL_GRASS,
    /*  7 */ SURFACE_SFX_OFFSET_LAVA, // MAGMA?
    /*  8 */ SURFACE_SFX_OFFSET_GRASS,
    /*  9 */ SURFACE_SFX_OFFSET_CARPET,
    /* 10 */ SURFACE_SFX_OFFSET_WOOD,
    /* 11 */ SURFACE_SFX_OFFSET_BRIDGE, // WOOD_PLANK?
    /* 12 */ SURFACE_SFX_OFFSET_VINE, // METAL?
    /* 13 */ SURFACE_SFX_OFFSET_DIRT_DEEP,
    /* 14 */ SURFACE_SFX_OFFSET_SNOW,
    /* 15 */ SURFACE_SFX_OFFSET_ICE
} SurfaceSfxOffset;

typedef enum SurfaceMaterial {
    /*  0 */ SURFACE_MATERIAL_DIRT,
    /*  1 */ SURFACE_MATERIAL_SAND,
    /*  2 */ SURFACE_MATERIAL_STONE,
    /*  3 */ SURFACE_MATERIAL_DIRT_SHALLOW,
    /*  4 */ SURFACE_MATERIAL_WATER_SHALLOW,
    /*  5 */ SURFACE_MATERIAL_WATER_DEEP,
    /*  6 */ SURFACE_MATERIAL_TALL_GRASS,
    /*  7 */ SURFACE_MATERIAL_LAVA, // MAGMA?
    /*  8 */ SURFACE_MATERIAL_GRASS,
    /*  9 */ SURFACE_MATERIAL_BRIDGE, // WOOD_PLANK?
    /* 10 */ SURFACE_MATERIAL_WOOD,
    /* 11 */ SURFACE_MATERIAL_DIRT_SOFT,
    /* 12 */ SURFACE_MATERIAL_ICE,
    /* 13 */ SURFACE_MATERIAL_CARPET,
    /* 14 */ SURFACE_MATERIAL_SNOW,
    /* 15 */ SURFACE_MATERIAL_MAX
} SurfaceMaterial;

#define COLPOLY_NORMAL_FRAC (1.0f / 32767.0f)
#define COLPOLY_SNORMAL(x) ((s16)((x) * 32767.0f))
#define COLPOLY_GET_NORMAL(n) ((n)*COLPOLY_NORMAL_FRAC)
#define COLPOLY_VIA_FLAG_TEST(vIA, flags) ((vIA) & (((flags)&7) << 13))
#define COLPOLY_VTX_INDEX(vI) ((vI)&0x1FFF)

#endif

typedef struct Actor Actor;
typedef struct Collider Collider;
typedef struct CollisionCheckInfo CollisionCheckInfo;
typedef struct PlayState PlayState;
typedef struct CollisionContext CollisionContext;
typedef struct CollisionHeader CollisionHeader;
typedef struct CollisionPoly CollisionPoly;
typedef struct CollisionCheckInfoInit CollisionCheckInfoInit;
typedef struct DamageTable DamageTable;
typedef struct CollisionCheckInfoInit2 CollisionCheckInfoInit2;
typedef struct DynaPolyActor DynaPolyActor;

void    BgCheck_Allocate(CollisionContext* colCtx, PlayState* play, CollisionHeader* colHeader);
float   BgCheck_EntityRaycastDown1(CollisionContext* colCtx, CollisionPoly** outGroundPoly, Vec3f* pos);
float   BgCheck_EntityRaycastDown2(PlayState* play, CollisionContext* colCtx, CollisionPoly** outGroundPoly, Vec3f* pos);
float   BgCheck_EntityRaycastDown3(CollisionContext* colCtx, CollisionPoly** outGroundPoly, s32* bgId, Vec3f* pos);
float   BgCheck_EntityRaycastDown4(CollisionContext* colCtx, CollisionPoly** outGroundPoly, s32* bgId, Actor* actor, Vec3f* pos);
float   BgCheck_EntityRaycastDown5(PlayState* play, CollisionContext* colCtx, CollisionPoly** outGroundPoly, s32* bgId, Actor* actor, Vec3f* pos);
float   BgCheck_EntityRaycastDown6(CollisionContext* colCtx, CollisionPoly** outGroundPoly, s32* bgId, Actor* actor, Vec3f* pos, float chkDist);
float   BgCheck_EntityRaycastDown7(CollisionContext* colCtx, CollisionPoly** outPoly, s32* bgId, Actor* actor, Vec3f* pos);
float   BgCheck_AnyRaycastDown1(CollisionContext* colCtx, CollisionPoly* outGroundPoly, Vec3f* pos);
float   BgCheck_AnyRaycastDown2(CollisionContext* colCtx, CollisionPoly* outGroundPoly, s32* bgId, Vec3f* pos);
float   BgCheck_CameraRaycastDown2(CollisionContext* colCtx, CollisionPoly** outPoly, s32* bgId, Vec3f* pos);
float   BgCheck_EntityRaycastDownWalls(CollisionContext* colCtx, CollisionPoly** outPoly, s32* bgId, Actor* actor, Vec3f* pos);
float   BgCheck_EntityRaycastDown9(CollisionContext* colCtx, CollisionPoly** outPoly, s32* bgId, Vec3f* pos);
s32     BgCheck_EntitySphVsWall1(CollisionContext* colCtx, Vec3f* posResult, Vec3f* posNext, Vec3f* posPrev, float radius, CollisionPoly** outPoly, float checkHeight);
s32     BgCheck_EntitySphVsWall2(CollisionContext* colCtx, Vec3f* posResult, Vec3f* posNext, Vec3f* posPrev, float radius, CollisionPoly** outPoly, s32* outBgId, float checkHeight);
s32     BgCheck_EntitySphVsWall3(CollisionContext* colCtx, Vec3f* posResult, Vec3f* posNext, Vec3f* posPrev, float radius, CollisionPoly** outPoly, s32* outBgId, Actor* actor, float checkHeight);
s32     BgCheck_EntitySphVsWall4(CollisionContext* colCtx, Vec3f* posResult, Vec3f* posNext, Vec3f* posPrev, float radius, CollisionPoly** outPoly, s32* outBgId, Actor* actor, float checkHeight);
s32     BgCheck_AnyCheckCeiling(CollisionContext* colCtx, float* outY, Vec3f* pos, float checkHeight);
s32     BgCheck_EntityCheckCeiling(CollisionContext* colCtx, float* outY, Vec3f* pos, float checkHeight, CollisionPoly** outPoly, s32* outBgId, Actor* actor);
s32     BgCheck_CameraLineTest1(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 chkWall, s32 chkFloor, s32 chkCeil, s32 chkOneFace, s32* bgId);
s32     BgCheck_CameraLineTest2(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 chkWall, s32 chkFloor, s32 chkCeil, s32 chkOneFace, s32* bgId);
s32     BgCheck_EntityLineTest1(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 chkWall, s32 chkFloor, s32 chkCeil, s32 chkOneFace, s32* bgId);
s32     BgCheck_EntityLineTest2(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 chkWall, s32 chkFloor, s32 chkCeil, s32 chkOneFace, s32* bgId, Actor* actor);
s32     BgCheck_EntityLineTest3(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 chkWall, s32 chkFloor, s32 chkCeil, s32 chkOneFace, s32* bgId, Actor* actor, float chkDist);
s32     BgCheck_ProjectileLineTest(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 chkWall, s32 chkFloor, s32 chkCeil, s32 chkOneFace, s32* bgId);
s32     BgCheck_AnyLineTest1(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 chkOneFace);
s32     BgCheck_AnyLineTest2(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 chkWall, s32 chkFloor, s32 chkCeil, s32 chkOneFace);
s32     BgCheck_AnyLineTest3(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 chkWall, s32 chkFloor, s32 chkCeil, s32 chkOneFace, s32* bgId);
s32     BgCheck_SphVsFirstPoly(CollisionContext* colCtx, Vec3f* center, float radius);

s32 CollisionCheck_SetAT(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
s32 CollisionCheck_SetAT_SAC(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider, s32 index);
s32 CollisionCheck_SetAC(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
s32 CollisionCheck_SetAC_SAC(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider, s32 index);
s32 CollisionCheck_SetOC(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
s32 CollisionCheck_SetOC_SAC(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider, s32 index);
s32 CollisionCheck_SetOCLine(PlayState* play, CollisionCheckContext* colChkCtx, OcLine* collider);
void CollisionCheck_BlueBlood(PlayState* play, Collider* collider, Vec3f* v);
void CollisionCheck_AT(PlayState* play, CollisionCheckContext* colChkCtx);
void CollisionCheck_OC(PlayState* play, CollisionCheckContext* colChkCtx);
void CollisionCheck_InitInfo(CollisionCheckInfo* info);
void CollisionCheck_ResetDamage(CollisionCheckInfo* info);
void CollisionCheck_SetInfoNoDamageTable(CollisionCheckInfo* info, CollisionCheckInfoInit* init);
void CollisionCheck_SetInfo(CollisionCheckInfo* info, DamageTable* damageTable, CollisionCheckInfoInit* init);
void CollisionCheck_SetInfo2(CollisionCheckInfo* info, DamageTable* damageTable, CollisionCheckInfoInit2* init);
void CollisionCheck_SetInfoGetDamageTable(CollisionCheckInfo* info, s32 index, CollisionCheckInfoInit2* init);
void CollisionCheck_Damage(PlayState* play, CollisionCheckContext* colChkCtx);
s32 CollisionCheck_LineOCCheckAll(PlayState* play, CollisionCheckContext* colChkCtx, Vec3f* a, Vec3f* b);
s32 CollisionCheck_LineOCCheck(PlayState* play, CollisionCheckContext* colChkCtx, Vec3f* a, Vec3f* b, Actor** exclusions, s32 numExclusions);

/* Unsure if that same function exists in OoT */
#if defined(GAME_MM)
void CollisionCheck_SpawnShieldParticles(PlayState* play, Vec3f* v);
f32 BgCheck_EntityRaycastFloor5(CollisionContext* colCtx, CollisionPoly** outPoly, s32* outBgId, Actor* actor, Vec3f* pos);
s32 SurfaceType_IsIgnoredByEntities(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId);
SurfaceMaterial SurfaceType_GetMaterial(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId);
void CollisionCheck_SpawnShieldParticlesWood(PlayState* play, Vec3f* v, Vec3f* pos);
s32 BgCheck_EntityLineTest2(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly, s32 checkWall, s32 checkFloor, s32 checkCeil, s32 checkOneFace, s32* bgId, Actor* actor);
void EffectSsBlast_SpawnWhiteShockwave(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel);
s32 Collider_InitAndSetTris(PlayState* play, ColliderTris* dest, Actor* actor, ColliderTrisInit* src, ColliderTrisElement* trisElements);
s32 Collider_InitAndSetQuad(PlayState* play, ColliderQuad* dest, Actor* actor, ColliderQuadInit* src);
s32 Collider_DestroyQuad(PlayState* play, ColliderQuad* quad);
s32 Collider_DestroyTris(PlayState* play, ColliderTris* tris);
void func_800BE504(Actor* actor, ColliderCylinder* cyl);
void Collider_SetTrisVertices(ColliderTris* tris, s32 elemIndex, Vec3f* a, Vec3f* b, Vec3f* c);
void Collider_SetQuadVertices(ColliderQuad* quad, Vec3f* a, Vec3f* b, Vec3f* c, Vec3f* d);

#endif

DynaPolyActor* DynaPoly_GetActor(CollisionContext* colCtx, s32 bgId);

#endif

#ifndef GENERATED_MM_NATIVE_COMPAT_H
#define GENERATED_MM_NATIVE_COMPAT_H

#include <combo/enemy_port.h>
#include <combo/actor_ovl.h>
#include <combo/global.h>
#include <combo/save.h>
#include <combo/sfx.h>
#include "types/native_sphere.h"
#include "actors/En_Bom/En_Bom.h"

void comboEnemySfxWorld(PlayState*, Vec3f*, s32, u16);
void comboEnemyDeathFlamePreset(PlayState*, Vec3f*, Vec3f*, Vec3f*, s16, s16, s32);
s16 comboEnemyCurrentCutscene(void);
#define func_800B3030 comboEnemyDeathFlamePreset

#define ACTOR_FLAG_HOOKSHOT_PULLS_ACTOR (1 << 9)
#define ACTOR_FLAG_HOOKSHOT_PULLS_PLAYER (1 << 10)
#define ACTOR_FLAG_HOOKSHOT_ATTACHED (1 << 13)
#define BODYPART_NONE (-1)
#define PLAYER_STATE1_400000 (1u << 22)
#define PLAYER_STATE1_800000 (1u << 23)
#define PLAYER_STATE1_8000000 (1u << 27)
#define PLAYER_STATE2_80 (1u << 7)
#define EVENTINF_41 0x41
#define PLAYER_FORM_MAX 5
#define PLAYER_FORM_HUMAN 4
#define GET_PLAYER_FORM PLAYER_FORM_HUMAN
#define PLAYER_MASK_STONE 0x10
#define BGCHECK_Y_MAX 32000.0f
#define COLORFILTER_COLORFLAG_NONE 0
#define FALLTHROUGH __attribute__((fallthrough))
#define CHECK_FLAG_ALL(a, b) (((a) & (b)) == (b))
#define UPDBGCHECKINFO_FLAG_40 UPDBGCHECKINFO_FLAG_MM_40
enum {
    ACTOR_DRAW_DMGEFF_FIRE = 0,
    ACTOR_DRAW_DMGEFF_BLUE_FIRE = 1,
    ACTOR_DRAW_DMGEFF_FROZEN_NO_SFX = 10,
    ACTOR_DRAW_DMGEFF_FROZEN_SFX = 11,
    ACTOR_DRAW_DMGEFF_LIGHT_ORBS = 20,
    ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_SMALL = 30,
    ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_MEDIUM = 31,
    ACTOR_DRAW_DMGEFF_ELECTRIC_SPARKS_LARGE = 32
};
#define BOMB_EXPLOSIVE_TYPE_BOMB 0
#define BOMB_TYPE_BODY 0
#define PLAYER_CSACTION_WAIT 7
#define CLEAR_TAG_PARAMS(x) (x)
#define CLEAR_TAG_LARGE_LIGHT_RAYS 4
#define CLEAR_TAG_SMALL_LIGHT_RAYS 3
#define CLEAR_TAG_POP 2

u32 comboEnemySourceHalfDays(Actor*);
s16 comboEnemyCutsceneIsNext(s16);
void comboEnemyCutsceneQueue(s16);
s16 comboEnemyCutsceneStartPlayer(s16, Actor*);
s16 comboEnemyCutsceneStop(s16);
#define CutsceneManager_IsNext comboEnemyCutsceneIsNext
#define CutsceneManager_Queue comboEnemyCutsceneQueue
#define CutsceneManager_StartWithPlayerCs comboEnemyCutsceneStartPlayer
#define CutsceneManager_Stop comboEnemyCutsceneStop
void comboEnemyDrawDamageEffects(PlayState*, Actor*, Vec3f*, s16, f32, f32, f32, u8);
void comboEnemyIceShatter(PlayState*, Actor*, Vec3f*, s32, s32, f32, f32);
void comboEnemyDeathFlame(PlayState*, Vec3f*, Vec3f*, Vec3f*, Color_RGBA8*, Color_RGBA8*, s16, s16, s32);
#undef EffectSsDeadDb_Spawn
#define EffectSsDeadDb_Spawn comboEnemyDeathFlame
#define Actor_DrawDamageEffects comboEnemyDrawDamageEffects
#define Actor_SpawnIceEffects comboEnemyIceShatter
static inline void generatedMmJoint(PlayState* p, ColliderJntSph* c, Actor* a, ColliderJntSphInit* i, ColliderJntSphElement* e)
{
    Collider_InitJntSph(p, c);
    Collider_SetJntSph(p, c, a, i, e);
}
#define Collider_InitAndSetJntSph generatedMmJoint

void comboEnemySetDropFlag(Actor*, ColliderElement*);
void comboEnemySetDropFlagJoint(Actor*, ColliderJntSph*);
#undef Actor_SetDropFlag
#undef Actor_SetDropFlagJntSph
#define Actor_SetDropFlag comboEnemySetDropFlag
#define Actor_SetDropFlagJntSph comboEnemySetDropFlagJoint
#define Actor_Spawn comboEnemyActorSpawn
#define Actor_SpawnAsChild comboEnemyActorSpawnAsChild
#define AudioSfx_StopByPosAndId comboEnemySfxStopPosition
#define AudioSfx_StopById comboEnemySfxStopId
#define Audio_PlaySfx_AtPos comboEnemySfxPosition
#define Math_Vec3s_Copy(a, b) (*(a) = *(b))
#define UPDBGCHECKINFO_FLAG_1 UPDBGCHECKINFO_FLAG_MM_1
#define UPDBGCHECKINFO_FLAG_2 UPDBGCHECKINFO_FLAG_MM_2
#define UPDBGCHECKINFO_FLAG_4 UPDBGCHECKINFO_FLAG_MM_4
#define UPDBGCHECKINFO_FLAG_8 UPDBGCHECKINFO_FLAG_MM_8
#define UPDBGCHECKINFO_FLAG_10 UPDBGCHECKINFO_FLAG_MM_10

static inline void generatedMmCylinder(PlayState* p, ColliderCylinder* c, Actor* a, ColliderCylinderInit* i)
{
    Collider_InitCylinder(p, c);
    Collider_SetCylinder(p, c, a, i);
}
static inline void generatedMmQuad(PlayState* p, ColliderQuad* c, Actor* a, ColliderQuadInit* i)
{
    Collider_InitQuad(p, c);
    Collider_SetQuad(p, c, a, i);
}
#define Collider_InitAndSetCylinder generatedMmCylinder
#define Collider_InitAndSetQuad generatedMmQuad

typedef s32 (*GeneratedMmOverrideLimbDraw)(PlayState*, s32, Gfx**, Vec3f*, Vec3s*, Actor*);
typedef void (*GeneratedMmPostLimbDraw)(PlayState*, s32, Gfx**, Vec3s*, Actor*);
typedef s32 (*OverrideLimbDraw)(PlayState*, s32, Gfx**, Vec3f*, Vec3s*, Actor*, Gfx**);
typedef void (*PostLimbDraw)(PlayState*, s32, Gfx**, Vec3s*, Actor*, Gfx**);
typedef void (*TransformLimbDrawOpa)(PlayState*, s32, Actor*);
typedef s32 SurfaceMaterial;
#define SURFACE_MATERIAL_DIRT 0
#define SURFACE_MATERIAL_SAND 1
#define SURFACE_MATERIAL_SNOW 14
#define SUBS_SHADOW_TEX_SIZE 4096
#define WALL_FLAG_4 (1 << 4)
#define WALL_FLAG_5 (1 << 5)
#define ITEM00_MAGIC_JAR_SMALL 0x0f
#define EFF_BLURE_DRAW_MODE_SIMPLE 0
_Static_assert(sizeof(EffectBlureInit2) == 0x24,"Native blade-trail initializer");

#include <combo/enemy_keep_gbi.h>

extern u8 FOREIGN_ENEMY_API_ATAN2S[];
extern u8 FOREIGN_ENEMY_API_ATAN2S_XY[];
extern u8 FOREIGN_ENEMY_API_CATEGORY[];
extern u8 FOREIGN_ENEMY_API_COLLISION_AC[];
extern u8 FOREIGN_ENEMY_API_COLLISION_AT[];
extern u8 FOREIGN_ENEMY_API_COLLISION_OC[];
extern u8 FOREIGN_ENEMY_API_COLOR_FILTER[];
extern u8 FOREIGN_ENEMY_API_CUTSCENE_IS_NEXT_NONE[];
extern u8 FOREIGN_ENEMY_API_CUTSCENE_QUEUE_NONE[];
extern u8 FOREIGN_ENEMY_API_CUTSCENE_START_PLAYER_NONE[];
extern u8 FOREIGN_ENEMY_API_CUTSCENE_STOP_NONE[];
extern u8 FOREIGN_ENEMY_API_CYLINDER_DESTROY[];
extern u8 FOREIGN_ENEMY_API_CYLINDER_INIT_SET[];
extern u8 FOREIGN_ENEMY_API_DAMAGE_EFFECTS[];
extern u8 FOREIGN_ENEMY_API_DEATH_FLAME_PRESET[];
extern u8 FOREIGN_ENEMY_API_DROP[];
extern u8 FOREIGN_ENEMY_API_DROP_FLAG[];
extern u8 FOREIGN_ENEMY_API_HIPLOOP_EFFECT_DESTROY[];
extern u8 FOREIGN_ENEMY_API_HIT_YAW_CYLINDER[];
extern u8 FOREIGN_ENEMY_API_ICE_SHATTER[];
extern u8 FOREIGN_ENEMY_API_JOINT_DESTROY[];
extern u8 FOREIGN_ENEMY_API_JOINT_INIT_SET[];
extern u8 FOREIGN_ENEMY_API_KNOCKBACK_LARGE[];
extern u8 FOREIGN_ENEMY_API_LILYPAD_COLLISION[];
extern u8 FOREIGN_ENEMY_API_MATERIAL_DRAW[];
extern u8 FOREIGN_ENEMY_API_MATRIX_MULT[];
extern u8 FOREIGN_ENEMY_API_MATRIX_MULT_X[];
extern u8 FOREIGN_ENEMY_API_MATRIX_MULT_Y[];
extern u8 FOREIGN_ENEMY_API_MATRIX_MULT_Z[];
extern u8 FOREIGN_ENEMY_API_MATRIX_MULT_ZERO[];
extern u8 FOREIGN_ENEMY_API_MATRIX_ROTATE_XF_APPLY[];
extern u8 FOREIGN_ENEMY_API_MATRIX_ROTATE_XS[];
extern u8 FOREIGN_ENEMY_API_MATRIX_ROTATE_YS[];
extern u8 FOREIGN_ENEMY_API_MATRIX_ROTATE_ZF[];
extern u8 FOREIGN_ENEMY_API_MATRIX_ROTATE_ZS[];
extern u8 FOREIGN_ENEMY_API_MATRIX_ROTATE_ZYX[];
extern u8 FOREIGN_ENEMY_API_MATRIX_SCALE[];
extern u8 FOREIGN_ENEMY_API_MATRIX_TRANSLATE[];
extern u8 FOREIGN_ENEMY_API_PLAYER_MASK[];
extern u8 FOREIGN_ENEMY_API_POLY_SHADOW_MATRIX[];
extern u8 FOREIGN_ENEMY_API_QUAD_DESTROY[];
extern u8 FOREIGN_ENEMY_API_RANDOM_DROP[];
extern u8 FOREIGN_ENEMY_API_RENDER_XLU[];
extern u8 FOREIGN_ENEMY_API_SEGMENTED_TO_VIRTUAL[];
extern u8 FOREIGN_ENEMY_API_SFX_ACTOR[];
extern u8 FOREIGN_ENEMY_API_SFX_FLAGGED[];
extern u8 FOREIGN_ENEMY_API_SFX_WORLD[];
extern u8 FOREIGN_ENEMY_API_SFX_WORLD_LOOP[];
extern u8 FOREIGN_ENEMY_API_SPAWN[];
extern u8 FOREIGN_ENEMY_API_SPAWN_CHILD[];
extern u8 FOREIGN_ENEMY_API_SPAWN_CUTSCENE[];
extern u8 FOREIGN_ENEMY_API_SPHERE_DESTROY[];
extern u8 FOREIGN_ENEMY_API_SPHERE_INIT_SET[];
extern u8 FOREIGN_ENEMY_API_TRANSFORM_FLEX_OPA[];
extern u8 FOREIGN_ENEMY_API_UNOWNED_MM_SWITCH[];
extern u8 FOREIGN_ENEMY_API_VEC3S_COPY[];

#endif

#ifndef COMBO_GENERATED_OOT_PORT_COMPAT_H
#define COMBO_GENERATED_OOT_PORT_COMPAT_H

#include <combo.h>
#include <combo/enemy_port.h>
#include <combo/actor_ovl.h>
#include <combo/enemy_native.h>
#include <combo/effect.h>
#include <combo/matrix.h>
#include <combo/common/animation.h>
#include <combo/common/actors/En_Item00.h>

#define naviEnemyId hintId
#define lockOnArrowOffset targetArrowOffset
#define Actor_Spawn comboEnemyActorSpawn
#define Actor_SpawnAsChild comboEnemyActorSpawnAsChild
#define COLLECTIBLE_DROP_RANDOM_PARAMS(t, p) (((t) * 16) | ((p) ? 0x8000 : 0))
#define COLLECTIBLE_DROP_TABLE_3 3
#define COLLECTIBLE_DROP_TABLE_4 4
#define COLLECTIBLE_DROP_TABLE_5 5
#define COLLECTIBLE_DROP_TABLE_9 9
#define COLLECTIBLE_DROP_TABLE_10 10
#define COLLECTIBLE_DROP_TABLE_12 12
#define COLLECTIBLE_DROP_TABLE_14 14
#define OOT_VERSION 0
#define PAL_1_0 100
#define SFX_FLAG 0x800
#define UPDBGCHECKINFO_FLAG_0 1
#define UPDBGCHECKINFO_FLAG_1 2
#define UPDBGCHECKINFO_FLAG_2 4
#define UPDBGCHECKINFO_FLAG_3 8
#define UPDBGCHECKINFO_FLAG_4 16
#define UPDBGCHECKINFO_FLAG_8 256
#define UPDBGCHECKINFO_FLAG_10 1024
#define Math_Vec3f_Copy(dst, src) (*(dst) = *(src))
#define Animation_ChangeByInfo Actor_ChangeAnimationByInfo
#define Audio_PlaySfx_AtPos comboEnemySfxPosition
#define Audio_PlaySfx_AtPosWithReverb comboEnemySfxReverb
#define ActorShadow_DrawFeet ((void*)0x800b42f8)
#define Actor_SetFeetPos ((void (*)(Actor*, s32, s32, Vec3f*, s32, Vec3f*))0x800b4a98)
#include <combo/enemy_actor_layout.h>

typedef EnemyNativeBomb EnBom;

#undef DMG_DEKU_NUT
#define DMG_DEKU_NUT (1 << 0)

#undef DMG_DEKU_STICK
#define DMG_DEKU_STICK (1 << 1)

#undef DMG_SLINGSHOT
#define DMG_SLINGSHOT (1 << 2)

#undef DMG_EXPLOSIVE
#define DMG_EXPLOSIVE (1 << 3)

#undef DMG_BOOMERANG
#define DMG_BOOMERANG (1 << 4)

#undef DMG_ARROW_NORMAL
#define DMG_ARROW_NORMAL (1 << 5)

#undef DMG_HAMMER_SWING
#define DMG_HAMMER_SWING (1 << 6)

#undef DMG_HOOKSHOT
#define DMG_HOOKSHOT (1 << 7)

#undef DMG_SLASH_KOKIRI
#define DMG_SLASH_KOKIRI (1 << 8)

#undef DMG_SLASH_MASTER
#define DMG_SLASH_MASTER (1 << 9)

#undef DMG_SLASH_GIANT
#define DMG_SLASH_GIANT (1 << 10)

#undef DMG_ARROW_FIRE
#define DMG_ARROW_FIRE (1 << 11)

#undef DMG_ARROW_ICE
#define DMG_ARROW_ICE (1 << 12)

#undef DMG_ARROW_LIGHT
#define DMG_ARROW_LIGHT (1 << 13)

#undef DMG_ARROW_UNK1
#define DMG_ARROW_UNK1 (1 << 14)

#undef DMG_ARROW_UNK2
#define DMG_ARROW_UNK2 (1 << 15)

#undef DMG_ARROW_UNK3
#define DMG_ARROW_UNK3 (1 << 16)

#undef DMG_MAGIC_FIRE
#define DMG_MAGIC_FIRE (1 << 17)

#undef DMG_MAGIC_ICE
#define DMG_MAGIC_ICE (1 << 18)

#undef DMG_MAGIC_LIGHT
#define DMG_MAGIC_LIGHT (1 << 19)

#undef DMG_SHIELD
#define DMG_SHIELD (1 << 20)

#undef DMG_MIR_RAY
#define DMG_MIR_RAY (1 << 21)

#undef DMG_SPIN_KOKIRI
#define DMG_SPIN_KOKIRI (1 << 22)

#undef DMG_SPIN_GIANT
#define DMG_SPIN_GIANT (1 << 23)

#undef DMG_SPIN_MASTER
#define DMG_SPIN_MASTER (1 << 24)

#undef DMG_JUMP_KOKIRI
#define DMG_JUMP_KOKIRI (1 << 25)

#undef DMG_JUMP_GIANT
#define DMG_JUMP_GIANT (1 << 26)

#undef DMG_JUMP_MASTER
#define DMG_JUMP_MASTER (1 << 27)

#undef DMG_UNKNOWN_1
#define DMG_UNKNOWN_1 (1 << 28)

#undef DMG_UNBLOCKABLE
#define DMG_UNBLOCKABLE (1 << 29)

#undef DMG_HAMMER_JUMP
#define DMG_HAMMER_JUMP (1 << 30)

#undef DMG_UNKNOWN_2
#define DMG_UNKNOWN_2 (1 << 31)

#undef DMG_SLASH
#define DMG_SLASH (DMG_SLASH_KOKIRI | DMG_SLASH_MASTER | DMG_SLASH_GIANT)

#undef DMG_SPIN_ATTACK
#define DMG_SPIN_ATTACK (DMG_SPIN_KOKIRI | DMG_SPIN_MASTER | DMG_SPIN_GIANT)

#undef DMG_JUMP_SLASH
#define DMG_JUMP_SLASH (DMG_JUMP_KOKIRI | DMG_JUMP_MASTER | DMG_JUMP_GIANT)

#undef DMG_SWORD
#define DMG_SWORD (DMG_SLASH | DMG_SPIN_ATTACK | DMG_JUMP_SLASH)

#undef DMG_HAMMER
#define DMG_HAMMER (DMG_HAMMER_SWING | DMG_HAMMER_JUMP)

#undef DMG_FIRE
#define DMG_FIRE (DMG_ARROW_FIRE | DMG_MAGIC_FIRE)

#undef DMG_ARROW
#define DMG_ARROW (DMG_ARROW_NORMAL | DMG_ARROW_FIRE | DMG_ARROW_ICE | DMG_ARROW_LIGHT | DMG_ARROW_UNK1 | DMG_ARROW_UNK2 | DMG_ARROW_UNK3)

#undef DMG_RANGED
#define DMG_RANGED (DMG_ARROW | DMG_HOOKSHOT | DMG_SLINGSHOT)

#undef DMG_DEFAULT
#define DMG_DEFAULT ~(DMG_SHIELD | DMG_MIR_RAY)

#define FALLTHROUGH __attribute__((fallthrough))
#define ACTOR_FLAG_HOOKSHOT_PULLS_ACTOR (1u << 9)
#define ACTOR_FLAG_HOOKSHOT_ATTACHED (1u << 13)
#define ACTOR_FLAGS_CHECK_ALL(actor, mask) (((actor)->flags & (mask)) == (mask))
#define PLAYER_STATE1_13 PLAYER_STATE1_MM_2000
#define PLAYER_STATE1_14 PLAYER_STATE1_MM_4000
#define PLAYER_STATE1_26 (1u << 26)
#define HAHEN_OBJECT_DEFAULT -1
#define BOMB_BODY 0
#define SETUPDL_0 0
#define BgCheck_GetWaterSurface ((s32 (*)(PlayState*, CollisionContext*, f32, f32, f32*, WaterBox**))0x800ca1e8)
#define ITEM00_ARROWS_SMALL 8
typedef Actor EnItem00;
#define SFX_PLAY_AT_POS comboEnemySfxPosition
static Gfx gActorSetupXluDL[] __attribute__((unused)) = {
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2 | Z_UPD),
    gsDPSetAlphaCompare(G_AC_THRESHOLD),
    gsSPEndDisplayList(),
};
#define ACTOR_SETUP_OPA_DL (&gActorSetupXluDL[2])
typedef struct
{
    AnimationHeader* animation;
    f32 playSpeed, startFrame, frameCount;
    u8 mode;
    f32 morphFrames;
} AnimationInfo;

_Static_assert(sizeof(AnimationInfo) == 0x18, "shared animation info ABI");
#include <combo/enemy_keep_gbi.h>

extern Actor* FOREIGN_ENEMY_API_SPAWN(ActorContext* actorCtx, PlayState* play, s16 actorId, f32 posX, f32 posY, f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params);
extern DamageTable* FOREIGN_ENEMY_API_DAMAGE_TABLE(s32 index);
extern s32 FOREIGN_ENEMY_API_COLLISION_AC(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
extern s32 FOREIGN_ENEMY_API_COLLISION_AT(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
extern s32 FOREIGN_ENEMY_API_COLLISION_OC(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
extern s32 FOREIGN_ENEMY_API_CYLINDER_DESTROY(PlayState* play, ColliderCylinder* cyl);
extern s32 FOREIGN_ENEMY_API_JOINT_DESTROY(PlayState* play, ColliderJntSph* jntSph);
extern void FOREIGN_ENEMY_API_CATEGORY(PlayState* play, ActorContext* actorCtx, Actor* actor, u8 actorCategory);
extern void FOREIGN_ENEMY_API_COLOR_FILTER(Actor* actor, s16 colorFlag, s16 colorIntensityMax, s16 bufFlag, s16 duration);
extern void FOREIGN_ENEMY_API_DEFAULT_FRAGMENT(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 unused, s16 scale, s16 objId, s16 life, Gfx* dList);
extern void FOREIGN_ENEMY_API_DROP_FLAG(Actor* actor, ColliderElement* elem, s32 freezeFlag);
extern void FOREIGN_ENEMY_API_FLYING_ICE_F(PlayState* play, Actor* actor, Vec3f* pos, s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, f32 scale);
extern void FOREIGN_ENEMY_API_KILL(Actor* actor);
extern void FOREIGN_ENEMY_API_KNOCKBACK_LARGE_NO_DAMAGE(PlayState* play, Actor* actor, f32 speed, s16 rot, f32 yVelocity);
extern void FOREIGN_ENEMY_API_MATRIX_ROTATE_XF(f32 x, u8 mode);
extern void FOREIGN_ENEMY_API_MATRIX_ROTATE_YF(f32 y, u8 mode);
extern void FOREIGN_ENEMY_API_MATRIX_ROTATE_ZF(f32 z, u8 mode);
extern void FOREIGN_ENEMY_API_MATRIX_SCALE(f32 x, f32 y, f32 z, u8 mode);
extern void FOREIGN_ENEMY_API_MATRIX_TRANSLATE(f32 x, f32 y, f32 z, u8 mode);
extern void FOREIGN_ENEMY_API_OOT_BGM_STORE(u16 seqId);
extern void FOREIGN_ENEMY_API_RANDOM_DROP(PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params);
extern void FOREIGN_ENEMY_API_SFX_ACTOR(Actor* actor, u16 sfxId);
extern void FOREIGN_ENEMY_API_SFX_FLAGGED(Actor* actor, u16 sfxId);
extern void FOREIGN_ENEMY_API_SFX_GENERAL(u16 sfxId, Vec3f* pos, u8 token, f32* freqScale, f32* vol, s8* reverbAdd);
extern void FOREIGN_ENEMY_API_SFX_WORLD(PlayState* play, Vec3f* worldPos, s32 duration, u16 sfxId);

#endif

#ifndef COMBO_ENEMY_PORT_H
#define COMBO_ENEMY_PORT_H

#include <combo.h>
#include <combo/enemy.h>

s32 comboEnemyCollisionSetAT(PlayState*, CollisionCheckContext*, Collider*);
s32 comboEnemyCollisionSetAC(PlayState*, CollisionCheckContext*, Collider*);
s32 comboEnemyCollisionSetOC(PlayState*, CollisionCheckContext*, Collider*);
s32 comboEnemyDestroyCylinder(PlayState*, ColliderCylinder*);
s32 comboEnemyDestroyQuad(PlayState*, ColliderQuad*);
s32 comboEnemyDestroyJoint(PlayState*, ColliderJntSph*);
s32 comboEnemyDestroyTris(PlayState*, ColliderTris*);
void comboEnemySfxActor(Actor*, u16);
void comboEnemySfxPosition(Vec3f*, u16);
void comboEnemySfxStopPosition(Vec3f*, u16);
void comboEnemySfxStopId(u16);
void comboEnemySetColorFilter(Actor*, u16, u16, u16, u16);
void comboEnemyRandomDrop(PlayState*, Actor*, Vec3f*, s16);
Actor* comboEnemyDropCollectible(PlayState*, Vec3f*, s16);
Actor* comboEnemyDropCollectible2(PlayState*, Vec3f*, s16);
void enemySetOotDropFlag(Actor*, ColliderElement*, s32);
void enemySetOotJointDropFlag(Actor*, ColliderJntSph*, s32);
void enemyActorKill(Actor*);
void enemyActorChangeCategory(PlayState*, ActorContext*, Actor*, u8);
void Actor_SpawnFloorDustRing(PlayState*, Actor*, Vec3f*, float, s32, float, s16, s16, u8);
void enemyFixedHitMark(PlayState*, s32, Vec3f*);
void enemyFlame(PlayState*, Vec3f*, Vec3f*, Vec3f*, s16, s16, s16, s16, s16, s16, s16, s16, s16, s16, s32, s16);

#define CollisionCheck_SetAT comboEnemyCollisionSetAT
#define CollisionCheck_SetAC comboEnemyCollisionSetAC
#define CollisionCheck_SetOC comboEnemyCollisionSetOC
#define Collider_DestroyCylinder comboEnemyDestroyCylinder
#define Collider_DestroyQuad comboEnemyDestroyQuad
#define Collider_DestroyJntSph comboEnemyDestroyJoint
#define Collider_DestroyTris comboEnemyDestroyTris
#define Actor_PlaySfx comboEnemySfxActor
#define Actor_SetColorFilter comboEnemySetColorFilter
#define Item_DropCollectibleRandom comboEnemyRandomDrop
#define Item_DropCollectible comboEnemyDropCollectible
#define Item_DropCollectible2 comboEnemyDropCollectible2
#define Actor_SetDropFlag enemySetOotDropFlag
#define Actor_SetDropFlagJntSph enemySetOotJointDropFlag
#define Actor_Kill enemyActorKill
#define Actor_ChangeCategory enemyActorChangeCategory
#define EffectSsHitMark_SpawnFixedScale enemyFixedHitMark
#define EffectSsDeadDb_Spawn enemyFlame

void enemyFlyingIceS(PlayState*, Actor*, Vec3s*, s16, s16, s16, s16, s16, s16, s16, f32);
void enemyFlyingIce(PlayState*, Actor*, Vec3f*, s16, s16, s16, s16, s16, s16, s16, f32);
Actor* comboEnemyActorSpawn(ActorContext*, PlayState*, s16, float, float, float, s16, s16, s16, s32);
Actor* comboEnemyActorSpawnAsChild(ActorContext*, Actor*, PlayState*, s16, float, float, float, s16, s16, s16, s32);
#define EffectSsEnIce_SpawnFlyingVec3s enemyFlyingIceS
#define EffectSsEnIce_SpawnFlyingVec3f enemyFlyingIce
#define Audio_StopSfxByPosAndId comboEnemySfxStopPosition

void enemyFootSparkle(PlayState*, Vec3f*);
#define func_800355B8 enemyFootSparkle

#endif

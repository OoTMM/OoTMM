#ifndef COMBO_ENEMY_PLACEMENT_H
#define COMBO_ENEMY_PLACEMENT_H

#include <combo.h>

#define ENEMY_NATIVE_RESOURCE       1u
#define ENEMY_NATIVE_SUPPRESS       2u
#define ENEMY_NATIVE_ARENA          8u
#define ENEMY_NATIVE_SKULL_KID      16u
#define ENEMY_NATIVE_GARAHNA        24u
#define ENEMY_NATIVE_DRY_AQUATIC    28u
#define ENEMY_NATIVE_DRY_MM_AQUATIC 29u
#define ENEMY_NATIVE_FLOOR          32u
#define ENEMY_NATIVE_AIR            256u
#define ENEMY_NATIVE_UPRIGHT        512u
#define ENEMY_NATIVE_GROUND         1024u
#define ENEMY_NATIVE_IRON_KNUCKLE   64u
int comboEnemyFlyingAttackInRange(Actor*, PlayState*, f32);
void comboEnemyFlyingAttackAim(Actor*, PlayState*, int);
void comboEnemyRedeadUpdate(Actor*, PlayState*, ActorFunc);
int comboEnemyNativeAirBound(Actor*);
int comboEnemyNativePlacementActive(Actor*);
int comboEnemyShuffledDeadHand(Actor*, PlayState*);
int comboEnemyNativeMusicSuppressed(void);
int comboEnemyNativePlacementOrigin(Actor*, Vec3f*);
int comboEnemyNativePlacementIsShuffled(Actor*);
void comboEnemyNativePlacementReset(void);
void comboEnemyNativePlacementBeginRoom(PlayState*);
void comboEnemyNativePlacementSetActorList(PlayState*, ActorEntry*, u32);
int comboEnemyNativePlacementSpawn(ActorContext*, ActorEntry*, PlayState*);
void comboEnemyNativePlacementUpdate(PlayState*);
int comboEnemyNativePlacementPending(s32 room);
#if defined(GAME_OOT)
Actor* comboEnemyNativeDryAquaticOwner(PlayState*, f32*);
#endif
#if defined(GAME_MM)
void comboEnemyNativePlacementBlock(s32 room);
void comboEnemyDependenciesReset(void);
void comboEnemyDependenciesBeginRoom(PlayState*, ActorEntry*, u32);
void comboEnemyDependenciesSpawnActors(PlayState*);
Actor* comboEnemyExistingDekuFlower(PlayState*, f32, f32, f32);
int comboEnemyFairyHost(Actor*, PlayState*, Vec3f*);
int comboEnemyRoomSlotComplete(s32, u32);

int comboEnemyNativeScriptedCurrent(Actor* monitor, PlayState* play);
#endif
Actor* comboEnemyNativeArenaOwner(PlayState*);
int comboEnemyNativeArenaCurrent(PlayState*);
int comboEnemyNativeArenaTarget(PlayState*, s16*, u16*);
u32 comboEnemyNativeSplitKey(PlayState*, s16 actorId);
#if defined(GAME_OOT)
void comboEnemyNativeFloorEntry(PlayState*, const ActorEntry*, ActorEntry*);
int comboMqEnemyPlacementKey(PlayState*, const ActorEntry*, u32*);
Actor* comboEnemyNativeSkullKidSpawn(ActorContext*, ActorEntry*, PlayState*);
int comboEnemyNativeArenaTrackTarget(PlayState*, Actor*);
int comboEnemyNativeArenaDefeated(PlayState*);
#endif

#endif

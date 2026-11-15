#ifndef COMBO_ENEMY_NATIVE_H
#define COMBO_ENEMY_NATIVE_H

#include <combo/types.h>
#include <combo/common/actor_init.h>
#include <combo/math/vec.h>

#define ENEMY_NATIVE_KEEP_MAIN 1u
#define ENEMY_NATIVE_KEEP_SUB 2u

typedef struct
{
    u32 saved[2];
    u8 active;
} EnemyNativeSegments;

struct Actor;
ActorProfile* comboEnemyNativeProfile(struct Actor*, s16, s16);
struct ActorContext;
struct ActorEntry;
struct PlayState;

struct Actor* comboEnemyNativeStandaloneSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*, u32);
int comboEnemyNativeEnter(struct PlayState* play, EnemyNativeSegments* scope, u32 required);
void comboEnemyNativeLeave(EnemyNativeSegments* scope);
int comboEnemyTentacleCeiling(struct PlayState*, Vec3f*);
u16 comboEnemyNativeIronKnuckleParams(u16);
void comboEnemyNativeIronKnuckleActivate(struct Actor*, u16);

struct Actor* comboEnemyNativeBigPoeSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*);
struct Actor* comboEnemyNativeBubbleSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*);
struct Actor* comboEnemyNativeEyegoreSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*);
struct Actor* comboEnemyNativeFighterSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*);
struct Actor* comboEnemyNativeGaroSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*);
struct Actor* comboEnemyNativeGuaySpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*);
struct Actor* comboEnemyNativeIronKnuckleSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*);
struct Actor* comboEnemyNativeLeeverSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*);
struct Actor* comboEnemyNativeRenewableSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*);
struct Actor* comboEnemyNativeLilySpawn(struct ActorContext* ctx, struct ActorEntry* entry, struct PlayState* play);
struct Actor* comboEnemyNativePeahatSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*);
struct Actor* comboEnemyNativePoeSisterSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*);
struct Actor* comboEnemyNativeRenewableBabaSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*);
struct Actor* comboEnemyNativeSkullwalltulaSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*);
struct Actor* comboEnemyNativeStalchildSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*);
struct Actor* comboEnemyNativeTakkuriSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*);
struct Actor* comboEnemyNativeTentacleSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*);

#endif

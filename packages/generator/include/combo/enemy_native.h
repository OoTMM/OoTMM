#ifndef COMBO_ENEMY_NATIVE_H
#define COMBO_ENEMY_NATIVE_H

#include <combo/types.h>
#include <combo/common/actor_init.h>
#include <combo/math/vec.h>
#include <combo/player.h>

#define ENEMY_NATIVE_KEEP_MAIN 1u
#define ENEMY_NATIVE_KEEP_SUB  2u

static inline int comboEnemyDarkLinkClashedWithPlayer(Player* fighter, Player* player)
{
    Collider* shield = &fighter->shieldQuad.base;
    if (shield->ac == &player->actor && (shield->acFlags & AC_BOUNCED))
        return 1;
    for (int i = 0; i < 2; ++i)
    {
        Collider* sword = &fighter->meleeWeaponQuads[i].base;
        if ((sword->ac == &player->actor && (sword->acFlags & AC_BOUNCED)) ||
            (sword->at == &player->actor && (sword->atFlags & AT_BOUNCED)))
            return 1;
    }
    return 0;
}

extern int gEnemyFrozenWallmasterIceSpawn;

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

#if defined(GAME_OOT)
int comboEnemyNativeSoulCheck(struct PlayState*, s16, u16);
#endif
struct Actor* comboEnemyDynamicSpawn(ActorContext*, ActorEntry*, PlayState*, u32 operation);
typedef struct EnemyFamily
{
    u16 live;
    u8 started, blocked, flags;
    void (*complete)(struct EnemyFamily*, PlayState*);
} EnemyFamily;

#define ENEMY_FAMILY_GOLD 1
#define ENEMY_FAMILY_NO_CLEAR 2
#define ENEMY_FAMILY_FIELD_STALCHILD 4
#define ENEMY_FAMILY_DESERT_LEEVER 8
Actor* comboEnemyFamilySpawn(EnemyFamily*, ActorContext*, ActorEntry*, PlayState*, u32);
Actor* comboEnemyFamilySpawnVanillaChild(EnemyFamily*, ActorContext*, Actor*, PlayState*, s16,
                                        f32, f32, f32, s16, s16, s16, s32);
void comboEnemyFamilyClear(EnemyFamily*);
int comboEnemyFamilySetVirtualFloor(EnemyFamily*, PlayState*, const ActorEntry*);
typedef struct {
    EnemyFamily* family;
    EnemyFamily* previous;
    int collecting;
} EnemyVirtualFloorScope;
int comboEnemyVirtualFloorBegin(EnemyVirtualFloorScope*, PlayState*, const ActorEntry*);
void comboEnemyVirtualFloorEnd(EnemyVirtualFloorScope*, Actor*);
int comboEnemyVirtualFloorActive(void);
void comboEnemyNativeCancelInit(Actor*, PlayState*);
void comboEnemyFamilySpawned(Actor*);
int comboEnemyFamilySpawning(void);
int comboEnemyFamilyIsGold(Actor*);
int comboEnemyFamilyNoClear(Actor*);
int comboEnemyFamilyDone(const EnemyFamily*);
int comboEnemyFamilyRewardReady(const EnemyFamily*);

Actor* comboEnemyNativeArenaSpawn(ActorContext*, ActorEntry*, PlayState*);
Actor* comboEnemyNativeStandaloneSpawn(struct ActorContext*, struct ActorEntry*, struct PlayState*, u32);
int comboEnemyNativeEnter(struct PlayState* play, EnemyNativeSegments* scope, u32 required);
void comboEnemyNativeLeave(EnemyNativeSegments* scope);
int comboEnemyTentacleCeiling(struct PlayState*, Vec3f*);
u16 comboEnemyNativeIronKnuckleParams(u16);
void comboEnemyNativeIronKnuckleActivate(struct Actor*, struct PlayState*, u16);

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

#ifndef COMBO_ENEMY_H
#define COMBO_ENEMY_H

#include <combo/types.h>
#include <combo/common/actor_init.h>

typedef struct ColliderElement ColliderElement;
typedef struct Player Player;

u32 comboEnemyCrc32(const u8* data, u32 size);
int comboEnemyLoadRomData(void);

#define ENEMY_ARCHIVE_VROM_START 0xd0000000u
#define ENEMY_ARCHIVE_VROM_END   0xd1000000u

typedef struct
{
    u32 sourceVrom;
    u32 fileSize;
    u32 vramStart;
    u32 memorySize;
    u32 profileOffset;
    u32 reserved;
    u32 sourceCrc32;
}
ForeignEnemyOverlay;

_Static_assert(sizeof(ForeignEnemyOverlay) == 28, "Foreign enemy overlay format");

ActorProfile* comboLoadForeignEnemyOverlay(void* dst, u32 capacity, const ForeignEnemyOverlay* overlay);

#define FOREIGN_ENEMY_ACTOR_BASE 0xc00
#define FOREIGN_ENEMY_ACTOR_COUNT 0x400
#define FOREIGN_ENEMY_ROOT 1u
#define FOREIGN_ENEMY_PART 2u
#define FOREIGN_ENEMY_KEEP_MAIN 4u
#define FOREIGN_ENEMY_KEEP_FIELD 8u
#define FOREIGN_ENEMY_KEEP_DUNGEON 16u
#define FOREIGN_ENEMY_SOURCE_DAMAGE 64u
#define FOREIGN_ENEMY_SOURCE_AUDIO 128u
#define FOREIGN_ENEMY_NATIVE_PORT 256u
#define FOREIGN_ENEMY_GROUND_BOUND 512u
#define FOREIGN_ENEMY_AIR_BOUND 1024u
#define FOREIGN_ENEMY_HOST_SOUL 2048u
int comboEnemyGroundBound(Actor* actor);

#if defined(GAME_MM)
typedef struct
{
    u32 flags;
    u16 sfx;
    u16 reserved;
}
ForeignEnemyAudioState;
ForeignEnemyAudioState* comboEnemySourceAudioState(Actor* actor);
u32 comboEnemySfxEnter(Actor* actor, ForeignEnemyAudioState* state, u32 callback);
void comboEnemySfxLeave(Actor* actor, ForeignEnemyAudioState* state, u32 nativeFlags);

#endif

typedef struct {
    u32 field;
    u32 from;
    u32 to;
} EnemyDefeatTransition;

typedef struct
{
    u16 actorId;
    u16 soulGi;
    u32 flags;
    u32 manifestVrom;
    EnemyDefeatTransition defeat;
}
ForeignEnemyRegistration;

_Static_assert(sizeof(ForeignEnemyRegistration) == 24, "Foreign enemy registration format");
struct ActorContext;
struct ActorEntry;
extern u32 gForeignEnemyRegistryVrom;
extern u32 gForeignEnemySpawnsVrom;
u32 comboEnemySourceHalfDays(Actor* actor);
int comboEnemyUsesSourceDamage(Actor* actor);
int comboEnemyUsesNativePort(Actor* actor);
u32 comboEnemyDamageFlags(u32 nativeFlags);
u32 comboEnemyDamageFlagsForHit(u32 nativeFlags, u32 provenance);
#if defined(GAME_MM)
void comboEnemyAttackRecord(Actor* owner, ColliderElement* element, u32 sourceFlags);
u32 comboEnemyAttackProvenance(ColliderElement* element);
void comboEnemyAttackForget(Actor* owner);
void comboEnemyAttackReset(void);
u32 comboEnemyMeleeFlags(Player* player, u32 nativeFlags, int strong, int spin);
#endif
void comboEnemyCaptureHit(ColliderElement* target);
u32 comboEnemyAttackFlags(ColliderElement* attack);
u8 comboEnemyAttackDamage(ColliderElement* attack);
#if defined(GAME_OOT)
u8 CollisionCheck_GetSwordDamageWrapper(u32 flags);
#endif
u32 comboEnemyDamageMask(u32 sourceMask);
int comboEnemySfxId(u16 source, u16* target);
struct Collider;
struct ColliderElement;
int comboEnemyApplySourceDamage(struct Collider* col, struct ColliderElement* elem);
#if defined(GAME_OOT)
float comboEnemySourceMmDamage(struct Collider* ac, Actor* attacker, struct ColliderElement* attack, u32* effect);
#endif
void comboEnemyHintEnter(Actor* actor, u8 source);
u8 comboEnemyHintLeave(Actor* actor);
Actor* comboEnemyCurrentActor(void);
void comboEnemyOverlayReset(void);
void comboEnemyCollisionEnter(Actor* actor);
void comboEnemyCollisionLeave(Actor* actor);
void comboEnemyCollisionRelease(Actor* actor);
void comboEnemyCollisionReset(void);
u32 comboEnemyCurrentCallback(void);
typedef struct {
    u32 listVrom;
    u32 visit;
    u8 room;
    u8 slot;
} EnemyPlacement;

int comboEnemyDefeatTransitionValid(const EnemyDefeatTransition* t, u32 root, u32 privateEnd, u32 profileOffset);
int comboEnemyDefeatState(const Actor* actor, const EnemyDefeatTransition* t, const void* overlay, u32 action);
void comboEnemyEncounterAfterUpdate(Actor* actor, PlayState* play, const EnemyDefeatTransition* t,
    const void* overlay, int inDefeatAction);
void comboEnemyEncounterReset(void);
void comboEnemyEncounterBeginRoom(PlayState* play, const void* cmd, struct ActorEntry* list, u32 count);
void comboEnemyBeginSpawnBatch(void);
void comboEnemyEndSpawnBatch(struct ActorContext*, PlayState*);
void comboEnemySpawnRoomEntry(struct ActorContext* ctx, struct ActorEntry* entry, PlayState* play);
int comboEnemyTakePlacement(PlayState* play, s16 actorId, EnemyPlacement* out);
int comboEnemyEncounterRegister(Actor* actor, const EnemyPlacement* placement);
u32 comboEnemyEncounterSplitKey(PlayState*, const EnemyPlacement*);
int comboEnemyEncounterSplitPlacement(Actor* owner, EnemyPlacement* out);
int comboEnemyEncounterRegisterMember(Actor* actor, const EnemyPlacement* placement);
void comboEnemyEncounterBlock(const EnemyPlacement* placement);
void comboEnemyEncounterRelease(Actor* actor);
int comboEnemyEncounterPending(s32 room);
void comboEnemyEncounterComplete(Actor* actor, PlayState* play);
void comboEnemyEncounterReconcile(PlayState* play);
void comboEnemyEncounterPostFrame(PlayState* play);
int comboEnemyEncounterRetryAllocation(const EnemyPlacement* placement);
int comboIsForeignEnemyActor(s16 id);
int comboEnemyForeignSoulCheck(s16 actorId);
int comboEnemyFamilySpawning(void);
void comboEnemyFamilySpawned(Actor* actor);
Actor* comboSpawnForeignEnemy(struct ActorContext* actorCtx, PlayState* play, s16 actorId,
    float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 params,
    u32 csId, u32 halfDays, Actor* parent);

#endif

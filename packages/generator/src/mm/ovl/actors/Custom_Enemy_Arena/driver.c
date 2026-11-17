#include <combo.h>
#include <combo/enemy_scripted.h>
#include <combo/enemy_placement.h>
#include <combo/enemy_resource.h>
#include <combo/global.h>

#define CHILD_LIMIT 128
#define PHASE_INTRO 0
#define PHASE_ACTIVE 1
#define PHASE_RETURN 2
#define PHASE_DONE 3
#define PHASE_BLOCKED 4
#ifndef ScriptedNativeSpawnEntry
#define ScriptedNativeSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)
#define ScriptedSetSegment ((void (*)(PlayState*, Actor*))0x800b67fc)
#define ScriptedNativeTempClear ((s32 (*)(PlayState*, s32))0x800b5d18)
#endif

typedef struct {
    Actor* actor;
    ActorFunc update, draw, fini;
    u32 flags;
} HeldChild;
typedef struct EnemyScriptedArena {
    EnemyScriptedBinding binding;
    struct EnemyScriptedArena* next;
    PlayState* play;
    const EnemyScriptedDescriptor* descriptor;
    Actor* root;
    Actor* target;
    ActorFunc rootUpdate, rootDraw, rootFini;
    EnemyFamily family;
    u32 rootFlags;
    u16 childCount;
    u8 phase, rootBound, rootHeld, rootHadHealth, targetHadHealth, targetDefeated, sourceTerminal;
    HeldChild children[CHILD_LIMIT];
} EnemyScriptedArena;
static EnemyScriptedArena* sArenas;

_Static_assert(sizeof(EnemyScriptedBinding) == 0x16c, "Scripted MM binding ABI");
_Static_assert(__builtin_offsetof(EnemyScriptedBinding, placementVrom) == 0x160, "Scripted MM lease ABI");
const u32 gEnemyScriptedLeaseAbiV1[] __attribute__((used)) = {
    0x45534c41,
    1,
    sizeof(EnemyScriptedBinding),
    __builtin_offsetof(EnemyScriptedBinding, placementVrom),
    __builtin_offsetof(EnemyScriptedBinding, placementVisit),
    __builtin_offsetof(EnemyScriptedArena, next),
    sizeof(EnemyScriptedArena),
    0x4c534531,
};
EXPORT_SYMBOL(ENEMY_SCRIPTED_MM_DRIVER_LEASE_ABI_V1, gEnemyScriptedLeaseAbiV1);

#define FAMILY(name) \
 int Scripted##name##_Ready(Actor*, PlayState*); \
 void Scripted##name##_Begin(Actor*, PlayState*); \
 void Scripted##name##_Complete(Actor*, PlayState*); \
 int Scripted##name##_Completed(PlayState*, const ActorEntry*);
FAMILY(Egol)
FAMILY(Jso2) FAMILY(Death) FAMILY(Pametfrog) FAMILY(Bigslime) FAMILY(Wart) FAMILY(Igos) FAMILY(Keeta) FAMILY(Wiz)
#define DESCRIPTOR(id, name) {id, Scripted##name##_Ready, Scripted##name##_Begin, Scripted##name##_Complete, Scripted##name##_Completed}
static const EnemyScriptedDescriptor kDescriptors[] = {
        DESCRIPTOR(0x184, Egol),  DESCRIPTOR(0x182, Jso2),    DESCRIPTOR(0x43, Death),
        DESCRIPTOR(7, Pametfrog), DESCRIPTOR(0x65, Bigslime), DESCRIPTOR(0x12c, Wart),
        DESCRIPTOR(0x115, Igos),  DESCRIPTOR(0x28f, Keeta),   DESCRIPTOR(0x15d, Wiz),
    };

static int live(PlayState* play, Actor* wanted)
{
    if (!wanted)
        return 0;
    for (int category = 0; category < 12; ++category)
        for (Actor* actor = play->actorCtx.actors[category].first; actor; actor = actor->next)
            if (actor == wanted)
                return 1;
    return 0;
}

static int current(EnemyScriptedArena* arena)
{
    return arena->play && arena->binding.actor.room == arena->play->roomCtx.curRoom.num &&
           comboEnemyNativeScriptedCurrent(&arena->binding.actor, arena->play);
}

static EnemyScriptedArena* owner(Actor* root)
{
    for (EnemyScriptedArena* arena = sArenas; arena; arena = arena->next)
        if (arena->root == root)
            return arena;
    return NULL;
}

int comboEnemyScriptedRoomComplete(PlayState* play)
{
    return Flags_GetClear(play, play->roomCtx.curRoom.num) || ScriptedNativeTempClear(play, play->roomCtx.curRoom.num);
}

void comboEnemyScriptedBlock(Actor* root)
{
    EnemyScriptedArena* arena = owner(root);
    if (arena)
    {
        arena->phase = PHASE_BLOCKED;
        arena->binding.pending = 1;
    }
}

static void childPause(Actor* actor, PlayState* play) {}
static void childFini(Actor* actor, PlayState* play)
{
    for (EnemyScriptedArena* arena = sArenas; arena; arena = arena->next)
        for (u32 i = 0; i < arena->childCount; ++i)
        {
            HeldChild* child = &arena->children[i];
            if (child->actor != actor)
                continue;
            ActorFunc fini = child->fini;
            child->actor = NULL;
            if (current(arena) && arena->phase != PHASE_RETURN && arena->phase != PHASE_DONE)
                arena->phase = PHASE_BLOCKED;
            actor->fini = fini;
            if (fini)
                fini(actor, play);
            return;
        }
}

int comboEnemyScriptedHoldChild(Actor* root, Actor* actor)
{
    EnemyScriptedArena* arena = owner(root);
    if (!arena || !current(arena) || !live(arena->play, actor) || actor == root || actor->init || !actor->update ||
        actor->room != root->room)
    {
        comboEnemyScriptedBlock(root);
        return 0;
    }
    for (u32 i = 0; i < arena->childCount; ++i)
        if (arena->children[i].actor == actor)
            return 1;
    if (arena->childCount == CHILD_LIMIT)
    {
        comboEnemyScriptedBlock(root);
        return 0;
    }
    HeldChild* child = &arena->children[arena->childCount++];
    *child = (HeldChild){ actor, actor->update, actor->draw, actor->fini, actor->flags };
    actor->update = childPause;
    actor->draw = NULL;
    actor->fini = childFini;
    actor->flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
    return 1;
}

void comboEnemyScriptedReleaseChildren(Actor* root)
{
    EnemyScriptedArena* arena = owner(root);
    if (!arena)
        return;
    for (u32 i = 0; i < arena->childCount; ++i)
    {
        HeldChild* child = &arena->children[i];
        if (!live(arena->play, child->actor))
            continue;
        if (child->actor->update == childPause)
            child->actor->update = child->update;
        if (!child->actor->draw)
            child->actor->draw = child->draw;
        if (child->actor->fini == childFini)
            child->actor->fini = child->fini;
        child->actor->flags = child->flags;
        child->actor = NULL;
    }
    arena->childCount = 0;
}

static void rootUpdate(Actor*, PlayState*);
static void rootFini(Actor* actor, PlayState* play)
{
    EnemyScriptedArena* arena = owner(actor);
    if (!arena)
        return;
    ActorFunc fini = arena->rootFini;
    comboEnemyScriptedReleaseChildren(actor);
    arena->root = NULL;
    if (!arena->sourceTerminal && arena->phase != PHASE_DONE)
        arena->phase = PHASE_BLOCKED;
    actor->fini = fini;
    if (fini)
        fini(actor, play);
}

static void syncRootCallbacks(EnemyScriptedArena* arena)
{
    Actor* root = arena->root;
    if (root->update != rootUpdate)
    {
        arena->rootUpdate = root->update;
        root->update = rootUpdate;
    }
    if (root->fini != rootFini)
    {
        arena->rootFini = root->fini;
        root->fini = rootFini;
    }
}

static void tryBegin(EnemyScriptedArena* arena)
{
    if (arena->phase != PHASE_INTRO || !current(arena))
        return;
    u32 segment = gSegments[6];
    ScriptedSetSegment(arena->play, arena->root);
    int ready = arena->descriptor->ready(arena->root, arena->play);
    syncRootCallbacks(arena);
    gSegments[6] = segment;
    if (!arena->rootUpdate)
    {
        arena->phase = PHASE_BLOCKED;
        return;
    }
    if (!ready || arena->phase != PHASE_INTRO)
        return;
    arena->rootDraw = arena->root->draw;
    arena->rootHadHealth = arena->root->colChkInfo.health != 0;
    arena->rootFlags = arena->root->flags;
    arena->rootHeld = 1;
    arena->phase = PHASE_ACTIVE;
    ScriptedSetSegment(arena->play, arena->root);
    arena->descriptor->begin(arena->root, arena->play);
    syncRootCallbacks(arena);
    gSegments[6] = segment;
    if (!arena->rootUpdate)
        arena->phase = PHASE_BLOCKED;
    if (arena->phase == PHASE_BLOCKED)
        return;
    arena->root->draw = NULL;
    arena->root->flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
}

static void rootUpdate(Actor* actor, PlayState* play)
{
    EnemyScriptedArena* arena = owner(actor);
    if (!arena || !current(arena))
        return;
    tryBegin(arena);
    if (arena->phase == PHASE_ACTIVE || arena->phase == PHASE_BLOCKED)
        return;
    actor->update = arena->rootUpdate;
    actor->fini = arena->rootFini;
    if (arena->rootUpdate)
        arena->rootUpdate(actor, play);
    arena->rootUpdate = actor->update;
    arena->rootFini = actor->fini;
    if (!actor->update)
    {
        if (arena->phase == PHASE_RETURN && ((arena->rootHadHealth && !actor->colChkInfo.health) ||
                                             arena->descriptor->completed(play, &arena->binding.original)))
        {
            arena->sourceTerminal = 1;
            arena->binding.pending = 0;
        }
        else
            arena->phase = PHASE_BLOCKED;
    }
    if (actor->update)
        actor->update = rootUpdate;
    actor->fini = rootFini;
}

static Actor* spawnOriginal(EnemyScriptedArena* arena, PlayState* play)
{
    u8 index = gCurrentSpawnActorNum;
    gCurrentSpawnActorNum = arena->binding.actorIndex;
    u32 segment = gSegments[6];
    Actor* actor = ScriptedNativeSpawnEntry(&play->actorCtx, &arena->binding.original, play);
    gCurrentSpawnActorNum = index;
    gSegments[6] = segment;
    return actor;
}

static void ArenaInit(Actor* actor, PlayState* play)
{
    EnemyScriptedArena* arena = (EnemyScriptedArena*)actor;
    arena->play = play;
    arena->binding.pending = 1;
    arena->next = sArenas;
    sArenas = arena;
}

static void ArenaDestroy(Actor* actor, PlayState* play)
{
    EnemyScriptedArena* arena = (EnemyScriptedArena*)actor;
    if (arena->rootBound && live(play, arena->root))
    {
        comboEnemyScriptedReleaseChildren(arena->root);
        if (arena->root->update == rootUpdate)
            arena->root->update = arena->rootUpdate;
        if (arena->rootHeld)
            arena->root->draw = arena->rootDraw;
        if (arena->root->fini == rootFini)
            arena->root->fini = arena->rootFini;
        if (arena->rootHeld)
            arena->root->flags = arena->rootFlags;
    }
    comboEnemyFamilyClear(&arena->family);
    EnemyScriptedArena** link = &sArenas;
    while (*link && *link != arena)
        link = &(*link)->next;
    if (*link)
        *link = arena->next;
}

static void ArenaUpdate(Actor* actor, PlayState* play)
{
    EnemyScriptedArena* arena = (EnemyScriptedArena*)actor;
    if (!current(arena))
        return;
    if (arena->binding.binding != ENEMY_SCRIPTED_MAGIC)
    {
        arena->phase = PHASE_BLOCKED;
        return;
    }
    if (!arena->descriptor)
    {
        s16 id = arena->binding.original.id & 0x1fff;
        for (u32 i = 0; i < ARRAY_COUNT(kDescriptors); ++i)
            if (kDescriptors[i].sourceId == id)
                arena->descriptor = &kDescriptors[i];
        if (!arena->descriptor)
        {
            arena->phase = PHASE_BLOCKED;
            return;
        }
        if (arena->descriptor->completed(play, &arena->binding.original))
        {
            spawnOriginal(arena, play);
            comboEnemyScriptedCancelPlacement(actor, play);
            arena->binding.pending = 0;
            arena->phase = PHASE_DONE;
            Actor_Kill(actor);
            return;
        }
    }
    if (arena->phase == PHASE_BLOCKED)
        return;
    if (arena->phase == PHASE_RETURN &&
        (arena->sourceTerminal || arena->descriptor->completed(play, &arena->binding.original)))
    {
        arena->binding.pending = 0;
        arena->phase = PHASE_DONE;
        Actor_Kill(actor);
        return;
    }
    if (!arena->root)
    {
        arena->root = spawnOriginal(arena, play);
        return;
    }
    if (!live(play, arena->root))
    {
        arena->phase = PHASE_BLOCKED;
        return;
    }
    if (!arena->rootBound)
    {
        if (arena->root->init)
            return;
        if (!arena->root->update)
        {
            arena->phase = PHASE_BLOCKED;
            return;
        }
        arena->rootUpdate = arena->root->update;
        arena->rootDraw = arena->root->draw;
        arena->rootFini = arena->root->fini;
        arena->root->update = rootUpdate;
        arena->root->fini = rootFini;
        arena->rootBound = 1;
    }
    u32 segment = gSegments[6];
    tryBegin(arena);
    if (arena->phase == PHASE_ACTIVE)
    {
        if (!arena->family.started)
        {
            CollisionPoly* poly = NULL;
            s32 bgId;
            Vec3f pos = arena->root->world.pos;
            pos.y += 100.f;
            float floor = BgCheck_EntityRaycastFloor5_2(play, &play->colCtx, &poly, &bgId, arena->root, &pos);
            if (floor == BGCHECK_Y_MIN)
            {
                arena->phase = PHASE_BLOCKED;
                return;
            }
            arena->target = comboEnemyScriptedSpawnTarget(actor, play, pos.x,
                                                          floor + (arena->binding.targetId == 0x16f ? 1.f : 8.f), pos.z,
                                                          arena->root->shape.rot.y, &arena->family);
            gSegments[6] = segment;
            if (!arena->target)
                return;
            if (!arena->target->update && !arena->target->init)
            {
                Actor_Kill(arena->target);
                arena->target = NULL;
                arena->phase = PHASE_BLOCKED;
                return;
            }

        }
        if (comboEnemyFamilyDone(&arena->family))
        {
            comboEnemyScriptedReleaseChildren(arena->root);
            arena->root->draw = arena->rootDraw;
            arena->root->flags = arena->rootFlags;
            arena->rootHeld = 0;
            arena->phase = PHASE_RETURN;
            ScriptedSetSegment(play, arena->root);
            arena->descriptor->complete(arena->root, play);
            gSegments[6] = segment;
            syncRootCallbacks(arena);
        }
    }
    if (arena->phase == PHASE_RETURN &&
        (arena->sourceTerminal || arena->descriptor->completed(play, &arena->binding.original)))
    {
        arena->binding.pending = 0;
        arena->phase = PHASE_DONE;
        Actor_Kill(actor);
    }
}

ActorProfile Enemy_Arena_Profile = {
    ACTOR_CUSTOM_ENEMY_ARENA,
    ACTORCAT_MISC,
    ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_UPDATE_DURING_OCARINA,
    1,
    sizeof(EnemyScriptedArena),
    ArenaInit,
    ArenaDestroy,
    ArenaUpdate,
    NULL,
};
OVL_INFO_ACTOR(ACTOR_CUSTOM_ENEMY_ARENA, Enemy_Arena_Profile);

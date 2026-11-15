#include <combo.h>
#include <combo/enemy_placement.h>

typedef struct {
    Actor* actor;
    Actor* parent;
    ActorFunc init, update, fini;
    u8 hadHealth;
} KeetaGuard;
static KeetaGuard sGuards[16];
static u32 sGuardTarget __attribute__((section(".data"))) = 0;
EXPORT_SYMBOL(ENEMY_MM_KEETA_GUARDS_V1, sGuardTarget);
void comboEnemyKeetaGuardsReset(void)
{
    memset(sGuards, 0, sizeof(sGuards));
}

static KeetaGuard* guardState(Actor* actor)
{
    for (u32 i = 0; i < ARRAY_COUNT(sGuards); ++i)
        if (sGuards[i].actor == actor)
            return &sGuards[i];
    return NULL;
}

static void guardUpdate(Actor* actor, PlayState* play)
{
    KeetaGuard* state = guardState(actor);
    if (!state)
        return;
    actor->update = state->update;
    if (state->update)
        state->update(actor, play);
    state->hadHealth |= actor->colChkInfo.health != 0;
    state->update = actor->update;
    if (actor->update)
        actor->update = guardUpdate;
}

static void guardInit(Actor* actor, PlayState* play)
{
    KeetaGuard* state = guardState(actor);
    if (!state)
        return;
    actor->init = state->init;
    actor->update = state->update;
    if (state->init)
        state->init(actor, play);
    actor->init = state->init = NULL;
    state->hadHealth = actor->colChkInfo.health != 0;
    state->update = actor->update;
    if (actor->update)
        actor->update = guardUpdate;
}

static void guardFini(Actor* actor, PlayState* play)
{
    KeetaGuard* state = guardState(actor);
    if (!state)
        return;
    KeetaGuard saved = *state;
    memset(state, 0, sizeof(*state));
    for (Actor* parent = play->actorCtx.actors[ACTORCAT_PROP].first; parent; parent = parent->next)
    {
        if (parent != saved.parent || parent->id != ACTOR_EN_ENCOUNT4 || !parent->update)
            continue;
        s16* live = (s16*)((u8*)parent + 0x14c);
        s16* defeated = (s16*)((u8*)parent + 0x14e);
        if (*live > 0)
            --*live;
        if (saved.hadHealth && actor->colChkInfo.health == 0)
            ++*defeated;
        else
            *live = *defeated = 0;
        break;
    }
    actor->fini = saved.fini;
    if (!saved.init && saved.fini)
        saved.fini(actor, play);
}

static Actor* keetaGuardSpawn(ActorContext* ctx, Actor* parent, PlayState* play, s16 id,
                              f32 x, f32 y, f32 z, s16 rx, s16 ry, s16 rz, s32 params)
{
    if (!sGuardTarget || play->sceneId != 0x0c || !parent || parent->id != ACTOR_EN_ENCOUNT4 || id != ACTOR_EN_SKB)
        return Actor_SpawnAsChild(ctx, parent, play, id, x, y, z, rx, ry, rz, params);
    for (u32 i = 0; i < ARRAY_COUNT(sGuards); ++i)
    {
        KeetaGuard* state = &sGuards[i];
        if (state->actor)
            continue;
        Actor* actor = Actor_Spawn(ctx, play, sGuardTarget >> 16, x, y, z, rx, ry, rz, (u16)sGuardTarget);
        if (!actor || (!actor->init && !actor->update))
            return NULL;
        *state = (KeetaGuard){ actor, parent, actor->init, actor->update, actor->fini, actor->colChkInfo.health != 0 };
        if (actor->init)
            actor->init = guardInit;
        else if (actor->update)
            actor->update = guardUpdate;
        actor->fini = guardFini;
        return actor;
    }
    return NULL;
}

PATCH_CALL(0x809c454c, keetaGuardSpawn);

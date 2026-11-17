#include <combo.h>
#include <combo/enemy_native.h>
#include <combo/enemy_placement.h>

typedef struct
{
    EnemyFamily family;
    Actor* parent;
} KeetaGuard;

static KeetaGuard sGuards[16];
static u16 sGuardTarget[4] __attribute__((section(".data")));
EXPORT_SYMBOL(ENEMY_MM_KEETA_GUARDS_V1, sGuardTarget);

void comboEnemyKeetaGuardsReset(void)
{
    for (u32 i = 0; i < ARRAY_COUNT(sGuards); ++i)
        comboEnemyFamilyClear(&sGuards[i].family);
    memset(sGuards, 0, sizeof(sGuards));
}

static void guardComplete(EnemyFamily* family, PlayState* play)
{
    KeetaGuard* guard = (KeetaGuard*)family;
    for (Actor* parent = play->actorCtx.actors[ACTORCAT_PROP].first; parent; parent = parent->next)
    {
        if (parent != guard->parent || parent->id != ACTOR_EN_ENCOUNT4 || !parent->update)
            continue;
        s16* live = (s16*)((u8*)parent + 0x14c);
        s16* defeated = (s16*)((u8*)parent + 0x14e);
        if (*live > 0)
            --*live;
        if (comboEnemyFamilyDone(family))
            ++*defeated;
        else
            *live = *defeated = 0;
        break;
    }
    memset(guard, 0, sizeof(*guard));
}

static Actor* keetaGuardSpawn(ActorContext* ctx, Actor* parent, PlayState* play, s16 id,
                              f32 x, f32 y, f32 z, s16 rx, s16 ry, s16 rz, s32 params)
{
    if (!sGuardTarget[3] || play->sceneId != 0x0c || !parent || parent->id != ACTOR_EN_ENCOUNT4 || id != ACTOR_EN_SKB)
        return Actor_SpawnAsChild(ctx, parent, play, id, x, y, z, rx, ry, rz, params);
    for (u32 i = 0; i < ARRAY_COUNT(sGuards); ++i)
    {
        KeetaGuard* guard = &sGuards[i];
        if (guard->parent)
            continue;
        ActorEntry entry = { .id = sGuardTarget[0], .params = sGuardTarget[1],
            .pos = { x, y, z }, .rot = { 7, ((u16)(s16)(ry * (360.f / 65536.f)) << 7) | 0x7f, 0x7f } };
        guard->family.complete = guardComplete;
        Actor* actor = comboEnemyFamilySpawn(&guard->family, ctx, &entry, play, sGuardTarget[2]);
        if (!actor || (!actor->init && !actor->update))
            return NULL;
        guard->parent = parent;
        return actor;
    }
    return NULL;
}

PATCH_CALL(0x809c454c, keetaGuardSpawn);

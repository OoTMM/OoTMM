#include <combo.h>
#include <combo/enemy_native.h>
#if defined(GAME_OOT)
#define LEEVER_ID              0x1c
#define LEEVER_LARGE           1
#define NativeLeeverSpawnEntry ((Actor * (*)(ActorContext*, ActorEntry*, PlayState*))0x800255c4)

static Actor* sLeeverActor;

s32 comboEnemyNativeLeeverFloor(CollisionContext* ctx, CollisionPoly* poly, s32 bgId)
{
    if (sLeeverActor && (!sLeeverActor->floorPoly || sLeeverActor->floorPoly == poly) && poly)
        return 4;
    return SurfaceType_GetFloorType(ctx, poly, bgId);
}

EXPORT_SYMBOL(ENEMY_NATIVE_LEEVER_FLOOR_V1, comboEnemyNativeLeeverFloor);
#else
#define LEEVER_ID              0x216
#define LEEVER_LARGE           0x8000
#define NativeLeeverSpawnEntry ((Actor * (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)
#endif

#if defined(GAME_OOT)

static void leeverUpdate(Actor* actor, PlayState* play)
{
    ActorProfile* profile = comboEnemyNativeProfile(actor, LEEVER_ID, -1);
    Actor* previous = sLeeverActor;
    sLeeverActor = actor;
    if (profile && profile->update)
        profile->update(actor, play);
    sLeeverActor = previous;
}
#else
static void leeverDraw(Actor* actor, PlayState* play)
{
    ActorProfile* profile = comboEnemyNativeProfile(actor, LEEVER_ID, -1);
    Gfx* first = play->state.gfxCtx->polyOpa.append;
    if (!profile || !profile->draw)
        return;
    profile->draw(actor, play);
    for (Gfx* gfx = first; gfx < play->state.gfxCtx->polyOpa.append; ++gfx)
    {
        if ((gfx->words.w0 >> 24) == G_SETPRIMCOLOR)
        {
            gfx->words.w1 = 0x9b37ffff;
            break;
        }
    }
}

static void leeverUpdate(Actor* actor, PlayState* play)
{
    ActorProfile* profile = comboEnemyNativeProfile(actor, LEEVER_ID, -1);
    if (profile && profile->update)
        profile->update(actor, play);
    if (actor->draw)
        actor->draw = leeverDraw;
}
#endif

static void leeverActivate(Actor* actor, PlayState* play)
{
    if (!actor || !actor->update)
        return;
    actor->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
#if defined(GAME_OOT)
    Actor_ChangeCategory(play, &play->actorCtx, actor, ACTORCAT_ENEMY);
    actor->update = leeverUpdate;
#else
    void (*rise)(Actor*) = actorAddr(LEEVER_ID, 0x80b7ce34);
    if (rise)
        rise(actor);
    if ((u16)actor->params & LEEVER_LARGE)
    {
        actor->update = leeverUpdate;
        if (actor->draw)
            actor->draw = leeverDraw;
    }
#endif
}

static void leeverInit(Actor* actor, PlayState* play)
{
    ActorProfile* profile = comboEnemyNativeProfile(actor, LEEVER_ID, -1);
    ActorFunc init = profile ? profile->init : NULL;
#if defined(GAME_OOT)
    Actor* previous = sLeeverActor;
    sLeeverActor = actor;
#endif
    actor->init = NULL;
    if (init && init != leeverInit)
        init(actor, play);
#if defined(GAME_OOT)
    sLeeverActor = previous;
#endif
    leeverActivate(actor, play);
}

Actor* comboEnemyNativeLeeverSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    Actor* actor;
    if (!ctx || !entry || !play || ((u16)entry->id & 0x1fff) != LEEVER_ID || (entry->params != 0 && (u16)entry->params != LEEVER_LARGE))
        return NULL;
#if defined(GAME_OOT)
    Actor sentinel;
    Actor* previous = sLeeverActor;
    memset(&sentinel, 0, sizeof(sentinel));
    sLeeverActor = &sentinel;
#endif
    actor = NativeLeeverSpawnEntry(ctx, entry, play);
#if defined(GAME_OOT)
    sLeeverActor = previous;
#endif
    if (actor && actor->init)
        actor->init = leeverInit;
    else
        leeverActivate(actor, play);
    return actor;
}

EXPORT_SYMBOL(ENEMY_NATIVE_LEEVER_V1, comboEnemyNativeLeeverSpawn);

#if defined(GAME_OOT)
#include <combo/enemy_actor_layout.h>
#include <combo/enemy.h>

typedef struct
{
    u16 id, params, operation, enabled;
} DesertLeeverChoice;

static DesertLeeverChoice sDesertChoices[8] __attribute__((section(".data")));
EXPORT_SYMBOL(ENEMY_DESERT_LEEVER_CHOICES, sDesertChoices);

typedef struct
{
    EnemyFamily family;
    Actor* spawner;
} DesertLeeverChild;

static DesertLeeverChild sDesertChildren[8];

static void desertComplete(EnemyFamily* family, PlayState* play)
{
    DesertLeeverChild* child = (DesertLeeverChild*)family;
    for (Actor* parent = play->actorCtx.actors[ACTORCAT_PROP].first; parent; parent = parent->next)
        if (parent == child->spawner && parent->id == ACTOR_EN_ENCOUNT1 && parent->update)
        {
            EnemyNativeEncounterSpawner* spawner = (EnemyNativeEncounterSpawner*)parent;
            if (spawner->curNumSpawn > 0)
                --spawner->curNumSpawn;
            break;
        }
    memset(child, 0, sizeof(*child));
}

static void desertSpawnerFini(Actor* actor, PlayState* play)
{
    for (u32 i = 0; i < ARRAY_COUNT(sDesertChildren); ++i)
        if (sDesertChildren[i].spawner == actor)
        {
            comboEnemyFamilyClear(&sDesertChildren[i].family);
            sDesertChildren[i].spawner = NULL;
        }
}

static Actor* desertLeeverSpawn(ActorContext* ctx, Actor* parent, PlayState* play, s16 id,
                                f32 x, f32 y, f32 z, s16 rx, s16 ry, s16 rz, s32 params)
{
    EnemyNativeEncounterSpawner* spawner = (EnemyNativeEncounterSpawner*)parent;
    int first = play->sceneId == 0x5c ? 0 : 5;
    int count = first == 0 ? 5 : 3;
    if ((play->sceneId == 0x5c || play->sceneId == 0x5e) && id == ACTOR_EN_REEBA &&
        sDesertChoices[first].enabled)
    {
        for (int i = first; i < first + count; ++i)
        {
            if (sDesertChildren[i].spawner)
                continue;
            const DesertLeeverChoice* choice = &sDesertChoices[i];
            ActorEntry entry = { .id = choice->id, .params = choice->params, .pos = { x, y, z }, .rot = { rx, ry, rz } };
            u8 lackSoul = g.roomEnemyLackSoul;
            u32 segment = gSegments[6];
            int missingSoul = (comboIsForeignEnemyActor(choice->id) ? comboEnemyForeignSoulCheck(choice->id) :
                comboEnemyNativeSoulCheck(play, choice->id, choice->params)) <= 0;
            sDesertChildren[i].family.complete = desertComplete;
            Actor* actor = comboEnemyFamilySpawn(&sDesertChildren[i].family, ctx, &entry, play, choice->operation);
            g.roomEnemyLackSoul = lackSoul;
            gSegments[6] = segment;
            if (!actor && missingSoul)
            {
                sDesertChildren[i].spawner = parent;
                parent->fini = desertSpawnerFini;
                return parent;
            }
            if (!actor || (!actor->init && !actor->update))
            {
                spawner->timer = 30;
                return NULL;
            }
            actor->room = parent->room;
            sDesertChildren[i].spawner = parent;
            parent->fini = desertSpawnerFini;
            return actor;
        }
        return NULL;
    }
    Actor* actor = Actor_SpawnAsChild(ctx, parent, play, id, x, y, z, rx, ry, rz, params);
    if (actor)
        ((EnemyNativeLeeverAim*)actor)->aimType = spawner->leeverIndex;
    return actor;
}

PATCH_CALL(0x8096f830, desertLeeverSpawn);
#endif

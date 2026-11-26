#include <combo.h>
#include <combo/enemy_native.h>

typedef struct
{
    Actor* actor;
    ActorFunc init, update, fini, draw;
    EnemyFamily* family;
    u8 hadHealth, defeated;
} FamilyChild;

static FamilyChild sChildren[256];
static EnemyFamily* sFamily;
static int sCollecting;

static void familyInit(Actor*, PlayState*);
static void familyUpdate(Actor*, PlayState*);
static void familyFini(Actor*, PlayState*);
static void familyDraw(Actor*, PlayState*);

static FamilyChild* familyChild(Actor* actor, int vacant)
{
    u32 start = ((u32)actor >> 4) & (ARRAY_COUNT(sChildren) - 1);
    FamilyChild* free = NULL;
    for (u32 i = 0; i < ARRAY_COUNT(sChildren); ++i)
    {
        FamilyChild* child = &sChildren[(start + i) & (ARRAY_COUNT(sChildren) - 1)];
        if (child->actor == actor)
            return child;
        if (!child->actor)
            return vacant ? (free ? free : child) : NULL;
        if (child->actor == (Actor*)1 && !free)
            free = child;
    }
    return vacant ? free : NULL;
}

#if defined(GAME_OOT)
typedef struct
{
    EnemyFamily* family;
    Actor* root;
    CollisionHeader* header;
    CollisionPoly* surface;
    Vec3f origin;
    s16 targetId;
} FamilyVirtualFloor;

static FamilyVirtualFloor sVirtualFloors[8];
static u8 sVirtualFloorCount;

typedef f32 (*FamilyRaycast)(PlayState*, CollisionContext*, u16, CollisionPoly**, s32*, Vec3f*, Actor*, u32, f32);
static FamilyRaycast volatile sOriginalRaycast __attribute__((section(".data"))) = NULL;
EXPORT_SYMBOL(ENEMY_VIRTUAL_FLOOR_ORIGINAL_V1, sOriginalRaycast);

static FamilyVirtualFloor* familyFloor(EnemyFamily* family, int vacant)
{
    FamilyVirtualFloor* free = NULL;
    if (!family)
        return NULL;
    for (u32 i = 0; i < ARRAY_COUNT(sVirtualFloors); ++i)
    {
        FamilyVirtualFloor* floor = &sVirtualFloors[i];
        if (floor->family == family)
            return floor;
        if (!floor->family && !free)
            free = floor;
    }
    return vacant ? free : NULL;
}

void comboEnemyFamilySetVirtualFloor(EnemyFamily* family, PlayState* play, const ActorEntry* entry)
{
    FamilyVirtualFloor* floor = familyFloor(family, 1);
    if (!floor || !play || !entry)
        return;
    if (!floor->family)
        ++sVirtualFloorCount;
    *floor = (FamilyVirtualFloor){ .family = family, .header = play->colCtx.colHeader,
        .origin = { entry->pos.x, entry->pos.y, entry->pos.z }, .targetId = entry->id };
    static const s16 dirs[][2] = {
        { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 },
        { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 },
    };
    for (s32 radius = 0; radius <= 384 && !floor->surface; radius += 32)
        for (u32 i = 0; i < ARRAY_COUNT(dirs); ++i)
        {
            Vec3f probe = { floor->origin.x + radius * dirs[i][0], floor->origin.y + 50.f,
                floor->origin.z + radius * dirs[i][1] };
            CollisionPoly* poly = NULL;
            s32 bgId = BGCHECK_SCENE;
            f32 y = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, NULL, &probe);
            if (poly && bgId == BGCHECK_SCENE && poly->normal.y > 0x6000 &&
                y >= floor->origin.y - 120.f && y <= floor->origin.y + 50.f)
            {
                floor->surface = poly;
                break;
            }
        }
}

static FamilyVirtualFloor* familyActorFloor(Actor* actor)
{
    if (!actor || !sVirtualFloorCount)
        return NULL;
    FamilyChild* child = familyChild(actor, 0);
    FamilyVirtualFloor* floor = child ? familyFloor(child->family, 0) : NULL;
    if (floor && (actor == floor->root || actor->category == ACTORCAT_ENEMY || actor->category == ACTORCAT_BOSS))
        return floor;
    if (!child && sCollecting && sFamily)
    {
        floor = familyFloor(sFamily, 0);
        if (floor && actor->id == floor->targetId && actor->home.pos.x == floor->origin.x &&
            actor->home.pos.z == floor->origin.z &&
            (actor->category == ACTORCAT_ENEMY || actor->category == ACTORCAT_BOSS))
            return floor;
    }
    return NULL;
}

static f32 familyVirtualRaycast(PlayState* play, CollisionContext* col, u16 xpFlags,
    CollisionPoly** outPoly, s32* outBgId, Vec3f* pos, Actor* actor, u32 flags, f32 distance)
{
    f32 y = sOriginalRaycast(play, col, xpFlags, outPoly, outBgId, pos, actor, flags, distance);
    if (!actor || !(xpFlags & 2) || !(flags & 4))
        return y;
    FamilyVirtualFloor* floor = familyActorFloor(actor);
    if (!floor || !floor->surface || col->colHeader != floor->header ||
        !(pos->y >= floor->origin.y) || !(y < floor->origin.y))
        return y;
    *outPoly = floor->surface;
    *outBgId = BGCHECK_SCENE;
    return floor->origin.y;
}
EXPORT_SYMBOL(ENEMY_VIRTUAL_FLOOR_RAYCAST_V1, familyVirtualRaycast);

static void familyFloorClamp(Actor* actor, PlayState* play)
{
    FamilyVirtualFloor* floor = familyActorFloor(actor);
    if (!floor || !play || play->colCtx.colHeader != floor->header)
        return;
    f32 y = floor->origin.y;
    if (actor->world.pos.y <= y)
    {
        actor->world.pos.y = y;
        if (actor->velocity.y < 0.f)
            actor->velocity.y = 0.f;
    }
    if (actor->floorHeight < y)
        actor->floorHeight = y;
}
#endif

static void familyBind(FamilyChild* child)
{
    Actor* actor = child->actor;
    child->init = actor->init;
    child->update = actor->update;
    child->fini = actor->fini;
    child->draw = actor->draw;
    if (actor->init)
        actor->init = familyInit;
    if (actor->update)
        actor->update = familyUpdate;
    actor->fini = familyFini;
    if (actor->draw)
        actor->draw = familyDraw;
}

static void familyUnbind(FamilyChild* child)
{
    Actor* actor = child->actor;
    if (actor->init == familyInit)
        actor->init = child->init;
    if (actor->update == familyUpdate)
        actor->update = child->update;
    if (actor->fini == familyFini)
        actor->fini = child->fini;
    if (actor->draw == familyDraw)
        actor->draw = child->draw;
}

int comboEnemyFamilySpawning(void)
{
    return sFamily != NULL;
}

int comboEnemyFamilyIsGold(Actor* actor)
{
    FamilyChild* child = familyChild(actor, 0);
    return child && (child->family->flags & ENEMY_FAMILY_GOLD);
}

int comboEnemyFamilyNoClear(Actor* actor)
{
    FamilyChild* child = familyChild(actor, 0);
    return child && (child->family->flags & ENEMY_FAMILY_NO_CLEAR);
}

int comboEnemyFamilyDone(const EnemyFamily* family)
{
    return family->started && !family->live && !family->blocked;
}

int comboEnemyFamilyRewardReady(const EnemyFamily* family)
{
    if (!family->started || family->blocked)
        return 0;
    for (u32 i = 0; i < ARRAY_COUNT(sChildren); ++i)
    {
        const FamilyChild* child = &sChildren[i];
        if (child->actor && child->actor != (Actor*)1 && child->family == family && !child->defeated)
            return 0;
    }
    return 1;
}

void comboEnemyFamilySpawned(Actor* actor)
{
    if (!sFamily || !actor || (actor->category == ACTORCAT_ITEMACTION && !(sFamily->flags & ENEMY_FAMILY_GOLD)) || actor->id == ACTOR_EN_ELF || actor->id == ACTOR_EN_ITEM00)
        return;
#if defined(GAME_OOT)
    if (actor->id == 0x60 && actor->params == 10)
        return;
#else
    if ((actor->id == 0xc60 && actor->params == 10) || (actor->id == 0x183 && actor->params == 0x80))
        return;
#endif
    FamilyChild* child = familyChild(actor, 1);
    if (!child)
    {
        sFamily->blocked = 1;
        return;
    }
    if (child->actor == actor)
        return;
    *child = (FamilyChild){ .actor = actor, .family = sFamily };
    ++sFamily->live;
    if (!sCollecting)
        familyBind(child);
}

static void familyRun(Actor* actor, PlayState* play, int action)
{
    FamilyChild* child = familyChild(actor, 0);
    if (!child)
        return;
    ActorFunc callback = action == 1 ? child->init : action == 2 ? child->draw : child->update;
    familyUnbind(child);
    EnemyFamily* previous = sFamily;
    sFamily = child->family;
#if defined(GAME_OOT)
    if (action != 2)
        familyFloorClamp(actor, play);
#endif
    if (callback)
        callback(actor, play);
#if defined(GAME_OOT)
    if (action != 2 && actor->update)
        familyFloorClamp(actor, play);
#endif
    sFamily = previous;
    if (action == 1)
    {
        actor->init = NULL;
        if (!actor->update)
            child->family->blocked = 1;
    }
    if ((child->family->flags & ENEMY_FAMILY_NO_CLEAR) &&
        (actor->category == ACTORCAT_ENEMY || actor->category == ACTORCAT_BOSS))
        Actor_ChangeCategory(play, &play->actorCtx, actor, ACTORCAT_NPC);
    if (action != 2)
    {
        child->hadHealth |= actor->colChkInfo.health != 0;
#if defined(GAME_OOT)
        int terminalHealth = actor->id == 0x90 || actor->id == 0xc4c ||
            actor->id == 0xa4 || actor->id == 0xdd || actor->id == 0xc6c;
#else
        int terminalHealth = actor->id == 0x4c || actor->id == 0xc90 ||
            actor->id == 0xca4 || actor->id == 0x6c || actor->id == 0xcdd;
#endif
        if (terminalHealth && (child->family->flags & ENEMY_FAMILY_GOLD) && child->hadHealth &&
            !actor->colChkInfo.health && !(actor->flags & ACTOR_FLAG_ATTENTION_ENABLED))
            child->defeated = 1;
    }
    familyBind(child);
}

static void familyInit(Actor* actor, PlayState* play)
{
    familyRun(actor, play, 1);
}

#if defined(GAME_OOT)
static int familyScriptedChildOutOfRange(Actor* actor, PlayState* play)
{
    FamilyChild* child = familyChild(actor, 0);
    if (!child || !child->family || !play)
        return 0;
    u8 flags = child->family->flags;
    if (!(flags & (ENEMY_FAMILY_FIELD_STALCHILD | ENEMY_FAMILY_DESERT_LEEVER)))
        return 0;
    Player* player = GET_PLAYER(play);
    if (!player)
        return 0;
    const Vec3f* pos = flags & ENEMY_FAMILY_FIELD_STALCHILD ? &actor->home.pos : &actor->world.pos;
    f32 dx = pos->x - player->actor.world.pos.x;
    f32 dz = pos->z - player->actor.world.pos.z;
    f32 radius = flags & ENEMY_FAMILY_FIELD_STALCHILD ? 800.f : 400.f;
    return dx * dx + dz * dz > radius * radius;
}
#endif

static void familyUpdate(Actor* actor, PlayState* play)
{
#if defined(GAME_OOT)
    if (familyScriptedChildOutOfRange(actor, play))
    {
        Actor_Kill(actor);
        return;
    }
#endif
    familyRun(actor, play, 0);
}

static void familyDraw(Actor* actor, PlayState* play)
{
    familyRun(actor, play, 2);
}

static void familyFini(Actor* actor, PlayState* play)
{
    FamilyChild* child = familyChild(actor, 0);
    if (!child)
        return;
    EnemyFamily* family = child->family;
#if defined(GAME_OOT)
    FamilyVirtualFloor* floor = familyFloor(family, 0);
    if (floor && floor->root == actor)
        floor->root = NULL;
#endif
    ActorFunc fini = child->fini;
    if (actor->init || actor->update)
        family->blocked = 1;
    familyUnbind(child);
    *child = (FamilyChild){ .actor = (Actor*)1 };
    EnemyFamily* previous = sFamily;
    sFamily = family;
    actor->fini = fini;
    if (fini)
        fini(actor, play);
    sFamily = previous;
    if (family->live && !--family->live && family->complete)
        family->complete(family, play);
}

void comboEnemyFamilyClear(EnemyFamily* family)
{
    for (u32 i = 0; i < ARRAY_COUNT(sChildren); ++i)
    {
        FamilyChild* child = &sChildren[i];
        if (!child->actor || child->actor == (Actor*)1 || child->family != family)
            continue;
        Actor* actor = child->actor;
        familyUnbind(child);
        ActorFunc init = actor->init;
        *child = (FamilyChild){ .actor = (Actor*)1 };
        if (init)
        {
            comboEnemyNativeCancelInit(actor, gPlay);
            actor->init = NULL;
            actor->fini = NULL;
        }
        Actor_Kill(actor);
    }
#if defined(GAME_OOT)
    FamilyVirtualFloor* floor = familyFloor(family, 0);
    if (floor)
    {
        memset(floor, 0, sizeof(*floor));
        --sVirtualFloorCount;
    }
#endif
    memset(family, 0, sizeof(*family));
}

static Actor* familyFinishSpawn(EnemyFamily* family, Actor* actor, EnemyFamily* previous, int collecting)
{
    comboEnemyFamilySpawned(actor);
#if defined(GAME_OOT)
    FamilyVirtualFloor* floor = familyFloor(family, 0);
    if (floor && actor)
    {
        floor->root = actor;
        if (gPlay)
            familyFloorClamp(actor, gPlay);
    }
#endif
    for (u32 i = 0; i < ARRAY_COUNT(sChildren); ++i)
        if (sChildren[i].actor && sChildren[i].actor != (Actor*)1 && sChildren[i].family == family &&
            sChildren[i].actor->fini != familyFini)
            familyBind(&sChildren[i]);
    sCollecting = collecting;
    sFamily = previous;
    if (actor)
        family->started = 1;
    return actor;
}

Actor* comboEnemyFamilySpawn(EnemyFamily* family, ActorContext* ctx, ActorEntry* entry, PlayState* play, u32 operation)
{
    EnemyFamily* previous = sFamily;
    int collecting = sCollecting;
    if (previous)
        family->flags |= previous->flags;
    sFamily = family;
    sCollecting = 1;
    Actor* actor = comboEnemyDynamicSpawn(ctx, entry, play, operation);
    return familyFinishSpawn(family, actor, previous, collecting);
}

Actor* comboEnemyFamilySpawnVanillaChild(EnemyFamily* family, ActorContext* ctx, Actor* parent,
                                        PlayState* play, s16 id, f32 x, f32 y, f32 z,
                                        s16 rx, s16 ry, s16 rz, s32 params)
{
    EnemyFamily* previous = sFamily;
    int collecting = sCollecting;
    if (previous)
        family->flags |= previous->flags;
    sFamily = family;
    sCollecting = 1;
    Actor* actor = Actor_SpawnAsChild(ctx, parent, play, id, x, y, z, rx, ry, rz, params);
    return familyFinishSpawn(family, actor, previous, collecting);
}

#if defined(GAME_MM)
static void familyActorDestroy(Actor* actor, PlayState* play)
{
    if (!actor->init)
    {
        if (actor->fini)
        {
            ((ActorFunc)actor->fini)(actor, play);
            actor->fini = NULL;
        }
        return;
    }
    FamilyChild* child = familyChild(actor, 0);
    if (!child)
        return;
    EnemyFamily* family = child->family;
    family->blocked = 1;
    familyUnbind(child);
    *child = (FamilyChild){ .actor = (Actor*)1 };
    comboEnemyNativeCancelInit(actor, play);
    if (family->live && !--family->live && family->complete)
        family->complete(family, play);
}

PATCH_FUNC(0x800b6948, familyActorDestroy);
#endif

#include <combo.h>
#include <combo/enemy_native.h>
#include <combo/enemy_placement.h>

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

typedef struct FamilyVirtualFloor
{
    struct FamilyVirtualFloor* next;
    EnemyFamily standalone;
    EnemyFamily* family;
    Actor* root;
    CollisionHeader* header;
    CollisionPoly* surface;
    Vec3f origin;
    s16 targetId;
} FamilyVirtualFloor;

#define ENEMY_FAMILY_FLOOR_ONLY 16
static FamilyVirtualFloor* sVirtualFloors;

#if defined(GAME_OOT)
typedef f32 (*FamilyRaycast)(PlayState*, CollisionContext*, u16, CollisionPoly**, s32*, Vec3f*, Actor*, u32, f32);
#else
/* MM BgCheck_RaycastFloorImpl has an additional static/dynamic query flag. */
typedef f32 (*FamilyRaycast)(PlayState*, CollisionContext*, u16, CollisionPoly**, s32*, Vec3f*, Actor*, u32, f32, s32);
#endif
static FamilyRaycast volatile sOriginalRaycast __attribute__((section(".data"))) = NULL;
#if defined(GAME_OOT)
EXPORT_SYMBOL(ENEMY_VIRTUAL_FLOOR_ORIGINAL_V1, sOriginalRaycast);
#else
EXPORT_SYMBOL(ENEMY_VIRTUAL_FLOOR_ORIGINAL_MM_V1, sOriginalRaycast);
#endif

static FamilyVirtualFloor* familyFloor(EnemyFamily* family, int vacant)
{
    if (family)
        for (FamilyVirtualFloor* floor = sVirtualFloors; floor; floor = floor->next)
            if (floor->family == family)
                return floor;
    if (!vacant)
        return NULL;
    FamilyVirtualFloor* floor = malloc(sizeof(*floor));
    if (!floor)
        return NULL;
    memset(floor, 0, sizeof(*floor));
    floor->family = family ? family : &floor->standalone;
    floor->next = sVirtualFloors;
    sVirtualFloors = floor;
    return floor;
}

int comboEnemyFamilySetVirtualFloor(EnemyFamily* family, PlayState* play, const ActorEntry* entry)
{
    if (!family || !play || !entry || !sOriginalRaycast)
        return 0;
    FamilyVirtualFloor* floor = familyFloor(family, 1);
    if (!floor)
        return 0;
    floor->root = NULL;
    floor->surface = NULL;
    floor->header = play->colCtx.colHeader;
    floor->origin = (Vec3f){ entry->pos.x, entry->pos.y, entry->pos.z };
    floor->targetId = entry->id;
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
#if defined(GAME_OOT)
            f32 y = BgCheck_EntityRaycastDown4(&play->colCtx, &poly, &bgId, NULL, &probe);
#else
            f32 y = BgCheck_EntityRaycastFloor5(&play->colCtx, &poly, &bgId, NULL, &probe);
#endif
            if (poly && bgId == BGCHECK_SCENE && poly->normal.y > 0x6000 &&
                y >= floor->origin.y - 120.f && y <= floor->origin.y + 50.f)
            {
                floor->surface = poly;
                break;
            }
        }
    return 1;
}

static FamilyVirtualFloor* familyActorFloor(Actor* actor)
{
    if (!actor || !sVirtualFloors)
        return NULL;
    FamilyChild* child = familyChild(actor, 0);
    FamilyVirtualFloor* floor = child ? familyFloor(child->family, 0) : NULL;
    if (floor && (actor == floor->root || actor->category == ACTORCAT_ENEMY || actor->category == ACTORCAT_BOSS))
        return floor;
    if (!child && sCollecting && sFamily)
    {
        floor = familyFloor(sFamily, 0);
        if (floor && actor->home.pos.x == floor->origin.x &&
            actor->home.pos.z == floor->origin.z &&
            (actor->category == ACTORCAT_ENEMY || actor->category == ACTORCAT_BOSS))
            return floor;
    }
    return NULL;
}

static f32 familyVirtualRaycast(PlayState* play, CollisionContext* col, u16 xpFlags,
    CollisionPoly** outPoly, s32* outBgId, Vec3f* pos, Actor* actor, u32 flags, f32 distance
#if defined(GAME_MM)
    , s32 queryFlags
#endif
    )
{
#if defined(GAME_OOT)
    f32 y = sOriginalRaycast(play, col, xpFlags, outPoly, outBgId, pos, actor, flags, distance);
#else
    f32 y = sOriginalRaycast(play, col, xpFlags, outPoly, outBgId, pos, actor, flags, distance, queryFlags);
#endif
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
#if defined(GAME_OOT)
EXPORT_SYMBOL(ENEMY_VIRTUAL_FLOOR_RAYCAST_V1, familyVirtualRaycast);
#else
EXPORT_SYMBOL(ENEMY_VIRTUAL_FLOOR_RAYCAST_MM_V1, familyVirtualRaycast);
#endif

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

static void familyReleaseFloor(EnemyFamily* family)
{
    for (FamilyVirtualFloor** link = &sVirtualFloors; *link; link = &(*link)->next)
    {
        FamilyVirtualFloor* floor = *link;
        if (floor->family == family)
        {
            *link = floor->next;
            free(floor);
            return;
        }
    }
}

int comboEnemyVirtualFloorActive(void)
{
    return familyFloor(sFamily, 0) != NULL;
}

int comboEnemyVirtualFloorBegin(EnemyVirtualFloorScope* scope, PlayState* play, const ActorEntry* entry)
{
    memset(scope, 0, sizeof(*scope));
    if (!play || !entry || !sOriginalRaycast)
        return 0;
    FamilyVirtualFloor* floor = familyFloor(NULL, 1);
    if (!floor)
        return 0;
    EnemyFamily* family = floor->family;
    family->flags = ENEMY_FAMILY_FLOOR_ONLY;
    if (!comboEnemyFamilySetVirtualFloor(family, play, entry))
    {
        familyReleaseFloor(family);
        return 0;
    }
    scope->family = family;
    scope->previous = sFamily;
    scope->collecting = sCollecting;
    sFamily = family;
    sCollecting = 1;
    return 1;
}

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
    return sFamily != NULL && !(sFamily->flags & ENEMY_FAMILY_FLOOR_ONLY);
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
    FamilyVirtualFloor* floor = familyFloor(sFamily, 0);
    if (floor && !floor->root && (actor->id == floor->targetId || actor->category == ACTORCAT_ENEMY || actor->category == ACTORCAT_BOSS) &&
        actor->home.pos.x == floor->origin.x && actor->home.pos.z == floor->origin.z)
        floor->root = actor;
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
    if (action != 2)
        familyFloorClamp(actor, play);
    if (callback)
        callback(actor, play);
    if (action != 2 && actor->update)
        familyFloorClamp(actor, play);
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
    FamilyVirtualFloor* floor = familyFloor(family, 0);
    if (floor && floor->root == actor)
        floor->root = NULL;
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
    if (family->live && !--family->live)
    {
        if (family->complete)
            family->complete(family, play);
        if (!family->live && sFamily != family)
            familyReleaseFloor(family);
    }
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
    memset(family, 0, sizeof(*family));
    familyReleaseFloor(family);
}

static Actor* familyFinishSpawn(EnemyFamily* family, Actor* actor, EnemyFamily* previous, int collecting)
{
    comboEnemyFamilySpawned(actor);
    FamilyVirtualFloor* floor = familyFloor(family, 0);
    if (floor && actor)
    {
        floor->root = actor;
        if (gPlay)
            familyFloorClamp(actor, gPlay);
    }
    for (u32 i = 0; i < ARRAY_COUNT(sChildren); ++i)
        if (sChildren[i].actor && sChildren[i].actor != (Actor*)1 && sChildren[i].family == family &&
            sChildren[i].actor->fini != familyFini)
            familyBind(&sChildren[i]);
    sCollecting = collecting;
    sFamily = previous;
    if (actor)
        family->started = 1;
    if (!family->live)
        familyReleaseFloor(family);
    return actor;
}

void comboEnemyVirtualFloorEnd(EnemyVirtualFloorScope* scope, Actor* actor)
{
    if (scope->family)
    {
        familyFinishSpawn(scope->family, actor, scope->previous, scope->collecting);
        scope->family = NULL;
    }
}

Actor* comboEnemyFamilySpawn(EnemyFamily* family, ActorContext* ctx, ActorEntry* entry, PlayState* play, u32 operation)
{
    EnemyFamily* previous = sFamily;
    int collecting = sCollecting;
    if (previous)
        family->flags |= previous->flags & ~ENEMY_FAMILY_FLOOR_ONLY;
    if (operation != 0xffff && (operation & ENEMY_DYNAMIC_VIRTUAL_FLOOR))
    {
        if (!comboEnemyFamilySetVirtualFloor(family, play, entry))
            return NULL;
        operation &= ~ENEMY_DYNAMIC_VIRTUAL_FLOOR;
    }
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
        family->flags |= previous->flags & ~ENEMY_FAMILY_FLOOR_ONLY;
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
    FamilyVirtualFloor* floor = familyFloor(family, 0);
    if (floor && floor->root == actor)
        floor->root = NULL;
    family->blocked = 1;
    familyUnbind(child);
    *child = (FamilyChild){ .actor = (Actor*)1 };
    comboEnemyNativeCancelInit(actor, play);
    if (family->live && !--family->live)
    {
        if (family->complete)
            family->complete(family, play);
        if (!family->live && sFamily != family)
            familyReleaseFloor(family);
    }
}

PATCH_FUNC(0x800b6948, familyActorDestroy);
#endif

#include <combo.h>
#include <combo/global.h>
#include <combo/enemy_native.h>
#define NativeWallSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)

static void nativeWallReject(Actor* actor)
{
    g.roomEnemyLackSoul = 1;
    if (actor)
    {
        actor->init = NULL;
        Actor_Kill(actor);
    }
}

static ActorProfile* nativeWallProfile(Actor* actor)
{
    ActorOvl* overlay = actor->ovl;
    ActorProfile* profile;
    if (actor->id != 0x50 || (u16)actor->params != 0xff00 || !overlay || !overlay->data ||
        overlay->vramEnd < overlay->vramStart || (u32)overlay->dataInit < overlay->vramStart ||
        (u32)overlay->dataInit > overlay->vramEnd ||
        overlay->vramEnd - (u32)overlay->dataInit < sizeof(*profile))
        return NULL;
    profile = (ActorProfile*)((u8*)overlay->data + (u32)overlay->dataInit - overlay->vramStart);
    return profile->id == 0x50 && profile->objectId == 0x20 ? profile : NULL;
}

static int nativeWallFind(PlayState* play, Vec3f* position, Vec3f* result, CollisionPoly** polygon, s32* bgId)
{
    static const s16 directions[4][2] = { { 64, 0 }, { -64, 0 }, { 0, 64 }, { 0, -64 } };
    Vec3f end, hit;
    CollisionPoly* poly;
    s32 bg;
    f32 distance, nearest = 4097.f;
    int found = 0;
    int i;
    for (i = 0; i < 4; ++i)
    {
        end = *position;
        end.x += directions[i][0];
        end.z += directions[i][1];
        if (!BgCheck_EntityLineTest1(&play->colCtx, position, &end, &hit, &poly, 1, 0, 0, 1, &bg) ||
            !poly || poly->normal.y < -6553 || poly->normal.y > 6553 ||
            (((s32(*)(CollisionContext*, CollisionPoly*, s32))0x800c9a4c)(&play->colCtx, poly, bg) & 0x30))
            continue;
        distance = (hit.x - position->x) * (hit.x - position->x) + (hit.z - position->z) * (hit.z - position->z);
        if (distance >= nearest)
            continue;
        nearest = distance;
        *result = hit;
        *polygon = poly;
        *bgId = bg;
        found = 1;
    }
    return found;
}

static int nativeWallAttach(Actor* actor, PlayState* play)
{
    Vec3f hit;
    CollisionPoly* poly;
    s32 bg;
    s32 (*align)(Actor*, CollisionPoly*) = actorAddr(0x50, 0x808d91c4);
    void (*rotate)(Actor*) = actorAddr(0x50, 0x808d93bc);
    if (!align || !rotate || !nativeWallFind(play, &actor->home.pos, &hit, &poly, &bg))
        return 0;
    align(actor, poly);
    rotate(actor);
    actor->shape.rot.x = -actor->world.rot.x;
    actor->shape.rot.y = actor->world.rot.y;
    actor->shape.rot.z = actor->world.rot.z;
    actor->world.pos.x = hit.x + poly->normal.x * (2.f / 32767.f);
    actor->world.pos.y = hit.y + poly->normal.y * (2.f / 32767.f);
    actor->world.pos.z = hit.z + poly->normal.z * (2.f / 32767.f);
    actor->home.pos = actor->world.pos;
    actor->prevPos = actor->world.pos;
    actor->floorPoly = poly;
    actor->floorBgId = bg;
    return 1;
}

static void nativeWallInit(Actor* actor, PlayState* play)
{
    ActorProfile* profile = nativeWallProfile(actor);
    ActorFunc init = profile ? profile->init : NULL;
    if (!init || init == nativeWallInit)
    {
        nativeWallReject(actor);
        return;
    }
    actor->init = init;
    init(actor, play);
    actor->init = NULL;
    if (!actor->update || !nativeWallAttach(actor, play))
        nativeWallReject(actor);
}

Actor* comboEnemyNativeSkullwalltulaSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    ActorEntry adjusted;
    Vec3f pos, hit;
    CollisionPoly* poly;
    s32 bg;
    Actor* actor;
    if (!ctx || !entry || !play || ((u16)entry->id & 0x1fff) != 0x50 ||
        (u16)entry->params != 0xff00)
        return NULL;
    pos.x = entry->pos.x;
    pos.y = entry->pos.y;
    pos.z = entry->pos.z;
    if (!nativeWallFind(play, &pos, &hit, &poly, &bg))
    {
        nativeWallReject(NULL);
        return NULL;
    }
    adjusted = *entry;
    adjusted.rot.x &= 0x7f;
    adjusted.rot.z &= 0x7f;
    actor = NativeWallSpawnEntry(ctx, &adjusted, play);
    if (!actor)
    {
        nativeWallReject(NULL);
        return NULL;
    }
    if (actor->init)
        actor->init = nativeWallInit;
    else if (!actor->update || !nativeWallProfile(actor) || !nativeWallAttach(actor, play))
        nativeWallReject(actor);
    return actor;
}
EXPORT_SYMBOL(ENEMY_NATIVE_SKULLWALLTULA_V1, comboEnemyNativeSkullwalltulaSpawn);
EXPORT_SYMBOL(ENEMY_NATIVE_SKULLWALLTULA_WALL_V2, comboEnemyNativeSkullwalltulaSpawn);

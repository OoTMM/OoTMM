#include <combo.h>
#include <combo/enemy_native.h>
#include <combo/common/actors/En_Item00.h>
#define NativeTakkuriSpawnEntry ((Actor* (*)(ActorContext*, ActorEntry*, PlayState*))0x800bb2d0)
typedef struct {
    Actor actor;
    u8 padding[0x3ec - sizeof(Actor)];
    Actor_EnItem00* target;
} NativeTakkuri;
_Static_assert(__builtin_offsetof(NativeTakkuri, target) == 0x3ec, "Takkuri collectible target");
static Actor* sSelectedTakkuri;
void comboEnemyNativeTakkuriItems(Actor* actor, PlayState* play)
{
    NativeTakkuri* bird = (NativeTakkuri*)actor;
    f32 nearest = 25000000.f;
    Player* player = GET_PLAYER(play);
    if (actor != sSelectedTakkuri)
    {
        ActorFunc original = actorAddr(0x291, 0x80c11338);
        if (original)
            original(actor, play);
        return;
    }
    bird->target = NULL;
    for (Actor* entry = play->actorCtx.actors[ACTORCAT_MISC].first; entry; entry = entry->next)
    {
        if (entry->id != ACTOR_EN_ITEM00 || !entry->update)
            continue;
        Actor_EnItem00* item = (Actor_EnItem00*)entry;
        s16 type = entry->params;
        if (item->isExtended || item->timer <= 0 ||
            (type != ITEM00_ARROWS_10 && type != ITEM00_BOMBS_5 && type != ITEM00_RUPEE_GREEN &&
             type != ITEM00_RUPEE_BLUE && type != ITEM00_RUPEE_RED && type != ITEM00_RUPEE_PURPLE))
            continue;
        f32 dx = entry->world.pos.x - player->actor.world.pos.x, dz = entry->world.pos.z - player->actor.world.pos.z;
        if (dx * dx + dz * dz <= 100.f)
            continue;
        dx = entry->world.pos.x - actor->world.pos.x;
        dz = entry->world.pos.z - actor->world.pos.z;
        f32 dy = entry->world.pos.y - actor->world.pos.y, distance = dx * dx + dy * dy + dz * dz;
        if (distance < nearest)
        {
            nearest = distance;
            bird->target = item;
        }
    }
}

static void takkuriUpdate(Actor* actor, PlayState* play)
{
    ActorFunc update = actorAddr(0x291, 0x80c12ed4);
    s32* questTheft = actorAddr(0x291, 0x80c1392c);
    s32 saved = *questTheft;
    Actor* previous = sSelectedTakkuri;
    sSelectedTakkuri = actor;
    *questTheft = 0;
    if (update)
        update(actor, play);
    *questTheft = saved;
    sSelectedTakkuri = previous;
    if (actor->update)
        actor->update = takkuriUpdate;
}

static void takkuriInit(Actor* actor, PlayState* play)
{
    ActorFunc init = actorAddr(0x291, 0x80c10770);
    u32 stolen = gSave.info.stolenItems;
    actor->init = NULL;
    gSave.info.stolenItems &= 0x00ffffff;
    if (init)
        init(actor, play);
    gSave.info.stolenItems = stolen;
    if (actor->update)
        actor->update = takkuriUpdate;
}

Actor* comboEnemyNativeTakkuriSpawn(ActorContext* ctx, ActorEntry* entry, PlayState* play)
{
    Actor* actor;
    u32 stolen;
    if (!ctx || !entry || !play || ((u16)entry->id & 0x1fff) != 0x291 || entry->params != 0)
        return NULL;
    stolen = gSave.info.stolenItems;
    gSave.info.stolenItems &= 0x00ffffff;
    actor = NativeTakkuriSpawnEntry(ctx, entry, play);
    gSave.info.stolenItems = stolen;
    if (actor && actor->init)
        actor->init = takkuriInit;
    else if (actor && actor->update)
        actor->update = takkuriUpdate;
    return actor;
}
EXPORT_SYMBOL(ENEMY_NATIVE_TAKKURI_V1, comboEnemyNativeTakkuriSpawn);

PATCH_CALL(0x80c11c0c, comboEnemyNativeTakkuriItems);
PATCH_CALL(0x80c128b0, comboEnemyNativeTakkuriItems);
EXPORT_SYMBOL(ENEMY_NATIVE_TAKKURI_ITEMS_V1, comboEnemyNativeTakkuriItems);

PATCH_VROM_U32(0x1078738, 0);
PATCH_VROM_U32(0x1078804, 0);

#include <combo.h>
#include <combo/souls.h>
#include <combo/enemy_placement.h>
#include <combo/enemy_native.h>

static Actor* sArenaOwner;
static Actor* sArenaTarget;
static s16 sArenaTargetId;
static int sArenaSpawnAttempted;
static int sArenaSpawnFailed;

static int En_Blkobj_ArenaBinding(PlayState* play, Actor** owner, s16* id, u16* params)
{
    *owner = comboEnemyNativeArenaOwner(play);
    return *owner && (*owner)->id == 0x136 && comboEnemyNativeArenaTarget(play, id, params);
}

static Actor* En_Blkobj_Actor_Spawn_Hook(ActorContext* ctx, PlayState* play, s16 actorId,
                                         float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 params)
{
    Actor* owner;
    s16 targetId;
    u16 targetParams;
    if (actorId != 0x33 || !En_Blkobj_ArenaBinding(play, &owner, &targetId, &targetParams))
        return Actor_Spawn(ctx, play, actorId, x, y, z, rx, ry, rz, params);
    if (!comboEnemyNativeArenaCurrent(play) || owner->room != play->roomCtx.curRoom.num)
        return NULL;
    sArenaOwner = owner;
    sArenaTarget = NULL;
    sArenaTargetId = targetId;
    sArenaSpawnAttempted = 1;
    sArenaSpawnFailed = 1;
    ActorEntry entry = { .id = targetId, .params = targetParams, .pos = { x, y, z }, .rot = { rx, ry, rz } };
    sArenaTarget = comboEnemyNativeArenaSpawn(ctx, &entry, play);
    if (sArenaTarget && (sArenaTarget->update || sArenaTarget->init) && comboEnemyNativeArenaTrackTarget(play, sArenaTarget))
        sArenaSpawnFailed = 0;
    return sArenaTarget;
}

static Actor* En_Blkobj_Actor_Find_Hook(void* actorCtx, s32 actorId, s32 actorCategory)
{
    Actor* owner;
    s16 targetId;
    u16 targetParams;
    if (actorId == 0x33 && En_Blkobj_ArenaBinding(gPlay, &owner, &targetId, &targetParams))
    {
        if (!comboEnemyNativeArenaCurrent(gPlay) || owner->room != gPlay->roomCtx.curRoom.num)
            return (Actor*)1;
        if (comboEnemyNativeArenaDefeated(gPlay))
            return NULL;
        if (owner != sArenaOwner || !sArenaSpawnAttempted || sArenaSpawnFailed || sArenaTargetId != targetId)
            return (Actor*)1;
        for (int category = 0; category < 12; ++category)
        {
            for (Actor* actor = ((ActorContext*)actorCtx)->actors[category].first; actor; actor = actor->next)
            {
                if (actor != sArenaTarget)
                    continue;
                if (actor->id != targetId)
                    return (Actor*)1;
                return actor->update && (category == ACTORCAT_ENEMY || category == ACTORCAT_BOSS) ? actor : (Actor*)1;
            }
        }
        return (Actor*)1;
    }
    if (!comboHasSoulOot(GI_OOT_SOUL_ENEMY_DARK_LINK))
        return (Actor*)1;
    return ActorFind(actorCtx, actorId, actorCategory);
}

EXPORT_SYMBOL(FOREIGN_ENEMY_DARK_LINK_ARENA_V1, En_Blkobj_Actor_Spawn_Hook);
PATCH_CALL(0x80a8ebd4, En_Blkobj_Actor_Spawn_Hook);
PATCH_CALL(0x80a8ec2c, En_Blkobj_Actor_Find_Hook);

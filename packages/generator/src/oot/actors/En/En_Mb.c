#include <combo.h>
#include <combo/enemy_placement.h>

static s16 EnMb_ChargeDuration(Actor* actor)
{
    return MAX(60, MIN(200, (s16)(actor->xzDistToPlayer / 10.f) + 30));
}

static void EnMb_RunAction(Actor* actor, PlayState* play)
{
    ActorFunc* action = (ActorFunc*)((u8*)actor + 0x314);
    ActorFunc before = *action;

    if (!comboEnemyNativePlacementActive(actor))
    {
        before(actor, play);
        return;
    }

    ActorFunc guard = actorAddr(ACTOR_EN_MB, 0x808e7478);
    ActorFunc patrolPrepare = actorAddr(ACTOR_EN_MB, 0x808e7958);
    ActorFunc patrol = actorAddr(ACTOR_EN_MB, 0x808e7c9c);
    ActorFunc clubIdle = actorAddr(ACTOR_EN_MB, 0x808e8a54);

    s16* chargeTicks = (s16*)((u8*)actor + 0x31c);
    s16* chargeHit = (s16*)((u8*)actor + 0x322);
    s16* attack = (s16*)((u8*)actor + 0x324);
    Collider* spear = (Collider*)((u8*)actor + 0x3a4);
    Player* player = GET_PLAYER(play);
    int charging = before == guard || before == patrolPrepare || before == patrol;
    if (before == clubIdle && actor->xzDistToPlayer < 500.f && fabsf(actor->yDistanceFromLink) < 120.f)
    {
        Math_SmoothStepToS(&actor->world.rot.y, actor->yawTowardsPlayer, 1, 0x600, 0);
        actor->shape.rot.y = actor->world.rot.y;
    }
    if (charging)
    {
        if (!*attack)
        {
            *chargeTicks = EnMb_ChargeDuration(actor);
            *chargeHit = 0;
        }
        else
        {
            if (((spear->atFlags & AT_HIT) && spear->at == &player->actor) || player->actor.parent == actor)
                *chargeHit = 1;
            s16 yaw = actor->yawTowardsPlayer - actor->world.rot.y;
            if (!*chargeHit && yaw > -0x4000 && yaw < 0x4000)
                Math_SmoothStepToS(&actor->world.rot.y, actor->yawTowardsPlayer, 8, 0x80, 0);
            if (*chargeTicks > 0)
                --*chargeTicks;
        }
        actor->shape.rot.y = actor->world.rot.y;
        if (*attack && player->actor.parent != actor &&
            (*chargeTicks <= 0 || (actor->bgCheckFlags & BGCHECKFLAG_WALL)))
        {
            void (*stop)(Actor*) = actorAddr(ACTOR_EN_MB, before == guard ? 0x808e69b8 : 0x808e6a18);
            *attack = 0;
            stop(actor);
            return;
        }
    }

    if (before == guard)
        actor->shape.rot.y = actor->yawTowardsPlayer;
    before(actor, play);
    if (charging && *action == before)
        actor->shape.rot.y = actor->world.rot.y;
    if (*action != before && (*action == guard || *action == patrolPrepare || *action == patrol))
    {
        *chargeTicks = EnMb_ChargeDuration(actor);
        *chargeHit = 0;
    }
}

PATCH_CALL(0x808e9308, EnMb_RunAction);

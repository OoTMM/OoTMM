#include <combo.h>
#include <combo/enemy_placement.h>

#ifndef NativeBigOctoSetPermanent
#define NativeBigOctoSetPermanent ((void (*)(PlayState*, s32))0x80020658)
#define NativeBigOctoCameraState ((s16 (*)(Camera*, s16))0x80049a94)
#endif

typedef struct {
    Actor actor;
    char nativeDyna[0x154 - sizeof(Actor)];
    ActorFunc action;
} BigOctoPlatform;
_Static_assert(__builtin_offsetof(BigOctoPlatform, action) == 0x154, "NTSC 1.0 BgBdanObjects action offset");

static void BigOcto_Suspend(Actor* actor, PlayState* play)
{
}

static Actor* BigOcto_LiveChild(Actor* owner, PlayState* play)
{
    for (int category = 0; category < 12; ++category)
        for (Actor* actor = play->actorCtx.actors[category].first; actor; actor = actor->next)
            if (actor == owner->child && actor->id == 0xc6 && actor->parent == owner && !actor->init && actor->update &&
                ((u16)actor->params == 2 || (u16)actor->params == 3))
                return actor;
    return NULL;
}

static void BgBdanObjects_Update_Hook(Actor* actor, PlayState* play)
{
    BigOctoPlatform* platform = (BigOctoPlatform*)actor;
    Actor* owner = comboEnemyNativeArenaOwner(play);
    s16 id;
    u16 params;
    actor->focus.pos = actor->world.pos;
    actor->focus.pos.y += 50.f;
    actor->focus.rot = actor->world.rot;
    if (owner == actor && actor->id == 0xc8 && comboEnemyNativeArenaTarget(play, &id, &params))
    {
        if (!comboEnemyNativeArenaCurrent(play) || actor->room != play->roomCtx.curRoom.num)
            return;
        if (id != 0x34 || params != 0)
            return;
        if (platform->action == (ActorFunc)actorAddr(0xc8, 0x809bc39c))
        {
            if (comboEnemyNativeArenaDefeated(play))
            {
                NativeBigOctoSetPermanent(play, actor->room);
                Camera_RequestSetting(play->cameraPtrs[0], CAM_SET_DUNGEON0);
                NativeBigOctoCameraState(play->cameraPtrs[0], 4);
            }
            else
            {
                Actor* source = BigOcto_LiveChild(actor, play);
                if (source)
                {
                    source->update = BigOcto_Suspend;
                    source->draw = NULL;
                    Actor* target = Actor_Spawn(&play->actorCtx, play, id,
                                                source->world.pos.x, source->world.pos.y, source->world.pos.z,
                                                0, source->shape.rot.y, 0, params);
                    if (target && (target->update || target->init) && comboEnemyNativeArenaTrackTarget(play, target))
                    {
                        actor->child = NULL;
                        source->parent = NULL;
                        Actor_Kill(source);
                    }
                    else if (target)
                    {
                        Actor_Kill(target);
                    }
                }
            }
        }
    }
    if (platform->action)
        platform->action(actor, play);
}

EXPORT_SYMBOL(FOREIGN_ENEMY_BIG_OCTO_ARENA_V1, BgBdanObjects_Update_Hook);
PATCH_FUNC(0x809bc9e4, BgBdanObjects_Update_Hook);

#include <combo.h>
#include <combo/mm/actor_arms_hook.h>

_Static_assert(__builtin_offsetof(Actor_ArmsHook, grabbed) == 0x1f8, "MM hook attachment");
_Static_assert(__builtin_offsetof(Actor_ArmsHook, unk1FC) == 0x1fc, "MM hook offset");
_Static_assert(sizeof(((Actor_ArmsHook*)0)->unk1FC) == sizeof(Vec3f), "MM hook offset width");

static void enemyTransferHook(PlayState* play, Actor* from, Actor* to)
{
    for (Actor* actor = play->actorCtx.actors[ACTORCAT_ITEMACTION].first; actor; actor = actor->next)
    {
        if (actor->id != ACTOR_ARMS_HOOK)
            continue;
        Actor_ArmsHook* hook = (Actor_ArmsHook*)actor;
        if (hook->grabbed != from)
            continue;
        hook->grabbed = to;
        memset(hook->unk1FC, 0, sizeof(hook->unk1FC));
        to->flags |= 1u << 13;
        from->flags &= ~(1u << 13);
        return;
    }
}

EXPORT_LINKABLE_SYMBOL(FOREIGN_ENEMY_API_TRANSFER_HOOK, enemyTransferHook);

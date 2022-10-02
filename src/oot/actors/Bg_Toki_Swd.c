#include <combo.h>

void BgTokiSwd_Handler(Actor* actor, GameState_Play* play)
{
    TransitionContext* t;

    if (Actor_HasParent(actor))
    {
        /* Time Travel */
        t = &play->transition;
        t->type = TRANS_TYPE_NORMAL;
        t->gfx = TRANS_GFX_SHORTCUT;
        t->entrance = 0x02ca;
        t->age = !(gSave.age);
    }
    else
    {
        /* Needs the Master Sword to become adult */
        if (gSave.equipment.swords & EQ_OOT_SWORD_MASTER)
            ActorEnableInteraction(actor, play);
    }
}

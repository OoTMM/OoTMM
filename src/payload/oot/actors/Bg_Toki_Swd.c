#include <combo.h>

void BgTokiSwd_Handler(Actor* actor, GameState_Play* play)
{
    TransitionContext* t;

    if (HasActorGivenItem(actor))
    {
        /* Time Travel */
        t = &play->transition;
        t->type = TRANS_NORMAL;
        t->entrance = 0x0053;
        t->age = !(gSave.age);
    }
    else
    {
        /* Needs the Master Sword to become adult */
        if (gSave.equipment.swords & EQ_OOT_SWORD_MASTER)
            ActorEnableInteraction(actor, play);
    }
}

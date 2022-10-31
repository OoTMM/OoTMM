#include <combo.h>

void EnGg_HandlerGiveItem(Actor* this, GameState_Play* play)
{
    s16 gi;

    if (Actor_HasParent(this))
    {
        gMmExtraFlags.maskGoron = 1;
        ActorDestroy(this);
    }
    else
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_GORON, GI_MM_MASK_GORON);
        GiveItem(this, play, gi, 9999.f, 9999.f);
    }
}

void EnGg_MaybeDestroy(Actor* actor)
{
    if (gMmExtraFlags.maskGoron)
        ActorDestroy(actor);
}

#include <combo.h>
#include <combo/item.h>

void EnGg_HandlerGiveItem(Actor* this, GameState_Play* play)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags.maskGoron = 1;
        ActorDestroy(this);
        return;
    }
    comboGiveItemNpc(this, play, GI_MM_MASK_GORON, NPC_MM_MASK_GORON, 9999.f, 9999.f);
}

void EnGg_MaybeDestroy(Actor* actor)
{
    if (gMmExtraFlags.maskGoron)
        ActorDestroy(actor);
}

#include <combo.h>
#include <combo/item.h>

void EnGg_HandlerGiveItem(Actor* this, GameState_Play* play)
{
    if (Actor_HasParentZ(this))
    {
        gMmExtraFlags.maskGoron = 1;
        Actor_Kill(this);
        return;
    }
    comboGiveItemNpc(this, play, GI_MM_MASK_GORON, NPC_MM_MASK_GORON, 9999.f, 9999.f);
}

void EnGg_MaybeDestroy(Actor* actor)
{
    if (gMmExtraFlags.maskGoron)
        Actor_Kill(actor);
}

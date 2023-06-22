#include <combo.h>
#include <combo/item.h>

void EnHg_HandlerGiveMask(Actor* this, GameState_Play* play)
{
    if (gMmExtraFlags.maskGibdo || Actor_HasParent(this))
    {
        gMmExtraFlags.maskGibdo = 1;
        MM_SET_EVENT_WEEK(EV_MM_WEEK_PAMELA_FATHER);
        ActorDestroy(this);
        return;
    }

    comboGiveItemNpc(this, play, GI_MM_MASK_GIBDO, NPC_MM_MASK_GIBDO, 9999.f, 9999.f);
}

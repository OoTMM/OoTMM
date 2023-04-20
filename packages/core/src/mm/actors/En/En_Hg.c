#include <combo.h>

void EnHg_HandlerGiveMask(Actor* this, GameState_Play* play)
{
    s16 gi;

    if (gMmExtraFlags.maskGibdo || Actor_HasParent(this))
    {
        gMmExtraFlags.maskGibdo = 1;
        MM_SET_EVENT_WEEK(EV_MM_WEEK_PAMELA_FATHER);
        ActorDestroy(this);
        return;
    }

    gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_GIBDO, GI_MM_MASK_GIBDO);
    GiveItem(this, play, gi, 9999.f, 9999.f);
}

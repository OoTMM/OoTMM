#include <combo.h>

void EnTk_AfterInit(void)
{
    /* Unset the "won game" flag is the HP was not collected */
    if (!(gSave.perm[SCE_OOT_GRAVEYARD].collectibles & (1 << 0x1f)))
        BITMAP16_CLEAR(gSave.eventsItem, EV_OOT_ITEM_DAMPE_GRAVEYARD_HP);
}

float EnTk_RandFloat(void)
{
    if (BITMAP16_GET(gSave.eventsItem, EV_OOT_ITEM_DAMPE_GRAVEYARD_HP))
        return RandFloat();
    return 1.f;
}

PATCH_CALL(0x80925ff4, EnTk_RandFloat);

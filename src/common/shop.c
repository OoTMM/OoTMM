#include <combo.h>

#if defined(GAME_OOT)
int comboShopPrecond(GameState_Play* play, Actor_EnGirlA* girlA)
{
    if (comboIsItemUnavailable(girlA->gi))
        return SC_ERR_CANNOTBUY;

    if (gSave.playerData.rupees < girlA->price)
        return SC_ERR_NORUPEES;

    if (comboIsItemMinor(girlA->gi))
        return SC_OK_NOCUTSCENE;

    return SC_OK;
}
#endif

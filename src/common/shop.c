#include <combo.h>

#if defined(GAME_OOT)
# define SOLD_OUT -1
#else
# define SOLD_OUT GI_MM_SOLD_OUT
#endif

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

void comboShopDisplayTextBox(GameState_Play* play, Actor_EnGirlA* girlA)
{
    DisplayTextBox2(play, girlA->base.messageId);
    if (girlA->gi == SOLD_OUT)
    {
        girlA->disabled = 1;
    }
    comboTextHijackItemShop(play, girlA->gi, girlA->price, 0);
}

void comboShopDisplayTextBoxConfirm(GameState_Play* play, Actor_EnGirlA* girlA)
{
    DisplayTextBox2(play, girlA->messageId2);
    comboTextHijackItemShop(play, girlA->gi, girlA->price, 1);
}

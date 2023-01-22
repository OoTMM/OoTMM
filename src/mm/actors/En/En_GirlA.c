#include <combo.h>

void EnGirlA_Draw(Actor_EnGirlA* girlA, GameState_Play* play)
{
    if (girlA->gi != GI_MM_SOLD_OUT)
        MatrixRotation(girlA->angle, 1);
    comboDrawGI(play, &girlA->base, girlA->gi, 0);
}

void EnGirlA_PostHandler(Actor_EnGirlA* girlA, GameState_Play* play)
{
    int soldOut;

    soldOut = 0;
    switch (girlA->base.variable)
    {
    case 0x13:
        girlA->gi = comboOverrideEx(OV_NPC, 0, NPC_MM_MASK_ALL_NIGHT, GI_MM_MASK_ALL_NIGHT, OVF_PROGRESSIVE);
        soldOut = gMmExtraFlags2.maskAllNight;
        break;
    /* Bomb Shop: Bomb Bag */
    case 0x14:
    case 0x17:
        girlA->gi = comboOverrideEx(OV_NPC, 0, NPC_MM_SHOP_BOMB_BAG, GI_MM_BOMB_BAG, OVF_PROGRESSIVE);
        soldOut = gMmExtraFlags2.shopBombBag;
        break;
    case 0x15:
    case 0x18:
        girlA->gi = comboOverrideEx(OV_NPC, 0, NPC_MM_SHOP_BOMB_BAG2, GI_MM_BOMB_BAG2, OVF_PROGRESSIVE);
        soldOut = gMmExtraFlags2.shopBombBag2;
        break;
    }
    if (soldOut)
    {
        girlA->gi = GI_MM_SOLD_OUT;
    }
}

void comboShopDisplayTextBox(GameState_Play* play, Actor_EnGirlA* girlA, int price)
{
    DisplayTextBox2(play, girlA->base.messageId);
    if (girlA->gi == GI_MM_SOLD_OUT)
    {
        girlA->disabled = 1;
    }
    comboTextHijackItemShop(play, girlA->gi, price, 0);
}

void comboShopDisplayTextBoxConfirm(GameState_Play* play, Actor_EnGirlA* girlA, int price)
{
    DisplayTextBox2(play, girlA->messageId2);
    comboTextHijackItemShop(play, girlA->gi, price, 1);
}

void comboAfterBuy(Actor_EnGirlA* girlA, GameState_Play* play)
{
    int soldOut;

    soldOut = 0;
    switch (girlA->base.variable)
    {
    /* All Night Mask */
    case 0x13:
        gMmExtraFlags2.maskAllNight = 1;
        soldOut = 1;
        break;
    /* Bomb Bag */
    case 0x14:
    case 0x17:
        gMmExtraFlags2.shopBombBag = 1;
        soldOut = 1;
        break;
    /* Bomb Bag 2 */
    case 0x15:
    case 0x18:
        gMmExtraFlags2.shopBombBag2 = 1;
        soldOut = 1;
        break;
    }

    if (soldOut)
    {
        girlA->gi = GI_MM_SOLD_OUT;
    }
}

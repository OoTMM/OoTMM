#include <combo.h>

void EnGirlA_Draw(Actor_EnGirlA* girlA, GameState_Play* play)
{
    if (girlA->gi != GI_MM_SOLD_OUT)
        MatrixRotation(girlA->angle, 1);
    comboDrawGI(play, &girlA->base, girlA->gi);
}

void EnGirlA_PostHandler(Actor_EnGirlA* girlA, GameState_Play* play)
{
    int override;
    int soldOut;

    override = -1;
    soldOut = 0;
    switch (girlA->base.variable)
    {
    /* Bomb Shop: Bomb Bag */
    case 0x17:
        override = comboGetNpcOverride(0x02);
        soldOut = GetCollectibleFlag(play, 0x1f);
        break;
    }
    if (soldOut)
    {
        girlA->gi = GI_MM_SOLD_OUT;
    }
    else if (override >= 0)
    {
        girlA->gi = comboProgressive(override);
    }
}

void comboShopDisplayTextBox(GameState_Play* play, Actor_EnGirlA* girlA, int price)
{
    DisplayTextBox2(play, girlA->base.messageId);
    if (girlA->gi == GI_MM_SOLD_OUT)
    {
        girlA->disabled = 1;
    }
    comboTextHijackItemShop(play, comboItemFromGI(girlA->gi), price, 0);
}

void comboShopDisplayTextBoxConfirm(GameState_Play* play, Actor_EnGirlA* girlA, int price)
{
    DisplayTextBox2(play, girlA->messageId2);
    comboTextHijackItemShop(play, comboItemFromGI(girlA->gi), price, 1);
}

void comboAfterBuy(Actor_EnGirlA* girlA, GameState_Play* play)
{
    int soldOut;

    soldOut = 0;
    switch (girlA->base.variable)
    {
    /* Bomb Shop: Bomb Bag */
    case 0x17:
        SetCollectibleFlag(play, 0x1f);
        soldOut = 1;
        break;
    }

    if (soldOut)
    {
        girlA->gi = GI_MM_SOLD_OUT;
    }
}

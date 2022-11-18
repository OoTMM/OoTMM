#include <combo.h>

static int itemPrice(Actor_EnGirlA* girlA)
{
    switch (girlA->base.variable)
    {
    case 0x14:
    case 0x17:
        return 50;
    case 0x15:
    case 0x18:
        return 90;
    case 0x19:
        return 40;
    case 0x1a:
        return 30;
    case 0x1b:
        return 90;
    case 0x1c:
        return 20;
    case 0x1d:
        return 60;
    case 0x1e:
        return 40;
    case 0x1f:
        return 40;
    case 0x20:
        return 80;
    case 0x21:
        return 10;
    case 0x22:
        return 20;
    case 0x23:
        return 50;
    }
    UNREACHABLE();
}

void EnSob1_AfterBuy(Actor_EnSob1* sob1, GameState_Play* play)
{
    if (Actor_HasParent(&sob1->base))
    {
        comboAfterBuy(sob1->items[sob1->itemIndex], play);
        sob1->base.attachedA = NULL;
    }
}

void EnSob1_DisplayTextBox(GameState_Play* play, Actor_EnGirlA* girlA)
{
    comboShopDisplayTextBox(play, girlA, itemPrice(girlA));
}

void EnSob1_DisplayTextBoxConfirm(GameState_Play* play, Actor_EnGirlA* girlA)
{
    comboShopDisplayTextBoxConfirm(play, girlA, itemPrice(girlA));
}

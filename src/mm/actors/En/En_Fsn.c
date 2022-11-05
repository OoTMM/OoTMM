#include <combo.h>

static int itemPrice(Actor_EnGirlA* girlA)
{
    switch (girlA->base.variable)
    {
    case 0x13:
        return 500;
    case 0x15:
    case 0x18:
        return 100;
    }
    UNREACHABLE();
}

int EnFsn_HasGivenShopItem(Actor_EnFsn* this, GameState_Play* play)
{
    if (Actor_HasParent(&this->base))
    {
        if (this->mode == 1)
            comboAfterBuy(this->items[this->itemIndex], play);
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80ae3be0, EnFsn_HasGivenShopItem);

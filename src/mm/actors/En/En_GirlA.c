#include <combo.h>

static int itemPriceRegular(Actor_EnGirlA* girlA)
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

static int itemPriceCuriosity(Actor_EnGirlA* girlA)
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

void EnGirlA_Draw(Actor_EnGirlA* girlA, GameState_Play* play)
{
    if (girlA->gi != GI_MM_SOLD_OUT)
        MatrixRotation(girlA->angle, 1);
    comboDrawGI(play, &girlA->base, girlA->gi, 0);
}

void EnGirlA_AfterHandler(Actor_EnGirlA* this, GameState_Play* play)
{
    if (play->sceneId == SCE_MM_CURIOSITY_SHOP)
    {
        this->price = itemPriceCuriosity(this);
    }
    else
    {
        this->price = itemPriceRegular(this);
    }

    comboShopSetupItem(play, this);
}

#include <combo.h>

static s16 kitemPrices[] = {
    30,
    40,
    50,
    90,
    500,
    30,
    30,
    80,
    50,
    10,
    30,
    30,
    40,
    60,
    10,
    20,
};

static int itemPrice(GameState_Play* play, Actor_EnGirlA* girlA)
{
    if (play->sceneId == SCE_MM_CURIOSITY_SHOP && girlA->shopId == 0x03)
        return 100;
    return kitemPrices[girlA->shopId];
}

void EnGirlA_Draw(Actor_EnGirlA* girlA, GameState_Play* play)
{
    if (girlA->gi != GI_MM_SOLD_OUT)
        MatrixRotation(girlA->angle, 1);
    comboDrawGI(play, &girlA->base, girlA->gi, 0);
}

void EnGirlA_AfterHandler(Actor_EnGirlA* this, GameState_Play* play)
{
    comboShopSetupItem(play, this);
    this->price = itemPrice(play, this);
}

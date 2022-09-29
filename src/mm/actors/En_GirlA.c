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

    override = -1;
    switch (girlA->base.variable)
    {
    /* Bomb Shop: Bomb Bag */
    case 0x17:
        override = comboGetNpcOverride(0x02);
        break;
    }
    if (override >= 0)
    {
        girlA->gi = comboProgressive(override);
    }
}

void comboAfterBuy(Actor_EnGirlA* girlA, GameState_Play* play)
{
    girlA->gi = GI_MM_SOLD_OUT;
}

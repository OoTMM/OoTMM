#include <combo.h>

void EnGirlA_Draw(Actor_EnGirlA* girlA, GameState_Play* play)
{
    MatrixRotation(girlA->angle, 1);
    comboDrawGI(play, &girlA->base, girlA->gi - 1);
}

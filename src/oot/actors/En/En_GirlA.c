#include <combo.h>

void EnGirlA_AfterInit2(Actor_EnGirlA* this)
{
    this->precond = comboShopPrecond;
}

void EnGirlA_Draw(Actor_EnGirlA* this, GameState_Play* play)
{
    ModelViewRotateY(((this->angle * 360.f) / 65536.f) * 0.017453292f, MAT_MUL);
    comboDrawGI(play, &this->base, this->gi, 0);
}

PATCH_FUNC(0x80864910, EnGirlA_Draw);

#include <combo.h>

void EnGirlA_AfterHandler(Actor_EnGirlA* this, GameState_Play* play)
{
    comboShopSetupItem(play, this);
}

void EnGirlA_Draw(Actor_EnGirlA* this, GameState_Play* play)
{
    if (this->gi != (GI_MM_SOLD_OUT | MASK_FOREIGN_GI))
        ModelViewRotateY(((this->angle * 360.f) / 65536.f) * 0.017453292f, MAT_MUL);
    comboDrawGI(play, &this->base, this->gi, 0);
}

PATCH_FUNC(0x80864910, EnGirlA_Draw);

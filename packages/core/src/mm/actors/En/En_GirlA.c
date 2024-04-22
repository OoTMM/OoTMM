#include <combo.h>
#include <combo/item.h>
#include <combo/shop.h>

void EnGirlA_Draw(Actor_EnGirlA* this, GameState_Play* play)
{
    ComboItemOverride o;

    /* KLUDGE: Should not be here but the update func is annoying to patch */
    Shop_UpdateItem(play, this);
    EnGirlA_ItemOverride(&o, this);
    if (o.gi != GI_MM_SOLD_OUT)
        MatrixRotation(this->angle, 1);
    comboDrawGI(play, &this->base, o.gi, 0);
}

void EnGirlA_AfterHandler(Actor_EnGirlA* this, GameState_Play* play)
{
    Shop_SetupItem(play, this);
}

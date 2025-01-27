#include <combo.h>
#include <combo/item.h>
#include <combo/shop.h>
#include <combo/draw.h>

void EnGirlA_AfterHandler(Actor_EnGirlA* this, PlayState* play)
{
    Shop_SetupItem(play, this);
}

void EnGirlA_Draw(Actor_EnGirlA* this, PlayState* play)
{
    ComboItemOverride o;

    /* KLUDGE: Should not be here but the update func is annoying to patch */
    Shop_UpdateItem(play, this);
    EnGirlA_ItemOverride(&o, this);

    if (o.gi != GI_MM_SOLD_OUT)
    {
        if (!o.cloakGi)
        {
            Matrix_RotateY(((this->angle * 360.f) / 65536.f) * 0.017453292f, MTXMODE_APPLY);
            Draw_Gi(play, &this->base, o.gi, 0);
        }
        else
        {
            Matrix_RotateY(((-this->angle * 360.f) / 65536.f) * 0.017453292f, MTXMODE_APPLY);
            Draw_Gi(play, &this->base, o.cloakGi, 0);
        }
    }
    else
        Draw_Gi(play, &this->base, o.gi, 0);
}

PATCH_FUNC(0x80864910, EnGirlA_Draw);

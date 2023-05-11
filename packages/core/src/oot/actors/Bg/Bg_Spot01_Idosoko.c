#include <combo.h>

void BgSpot01Idosoko_Handler(Actor* this, GameState_Play* play)
{
    if (comboConfig(CFG_OOT_ADULT_WELL) && gSave.age == AGE_ADULT && GetEventChk(EV_OOT_CHK_WELL_DRAINED))
        ActorDestroy(this);
}

PATCH_FUNC(0x80aa8a90, BgSpot01Idosoko_Handler);

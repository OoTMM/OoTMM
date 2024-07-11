#include <combo.h>
#include <combo/config.h>

void BgSpot01Idosoko_Handler(Actor* this, GameState_Play* play)
{
    if (Config_Flag(CFG_OOT_ADULT_WELL) && gSave.age == AGE_ADULT && GetEventChk(EV_OOT_CHK_WELL_DRAINED))
        Actor_Kill(this);
}

PATCH_FUNC(0x80aa8a90, BgSpot01Idosoko_Handler);

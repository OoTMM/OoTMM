#include <combo.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x29c) = (h); } while (0)

void EnFu_HandlerNull(Actor* actor, GameState_Play* play)
{
}

void EnFu_HandlerAdultOcarina2(Actor* actor, GameState_Play* play)
{
    s16 gi;

    if (!Actor_HasParent(actor))
    {
        gi = comboOverride(OV_NPC, 0, NPC_OOT_SONG_STORMS, GI_OOT_SONG_STORMS);
        GiveItem(actor, play, gi, 10000.f, 500.f);
    }
    else
    {
        SetEventChk(EV_OOT_CHK_SONG_STORMS);
        SET_HANDLER(actor, EnFu_HandlerNull);
    }
}

void EnFu_HandlerAdultOcarina(Actor* actor, GameState_Play* play)
{
    play->msgCtx.ocarinaMode = 4;
    SET_HANDLER(actor, EnFu_HandlerAdultOcarina2);
    EnFu_HandlerAdultOcarina2(actor, play);
}

PATCH_FUNC(0x80ac2d5c, EnFu_HandlerAdultOcarina);

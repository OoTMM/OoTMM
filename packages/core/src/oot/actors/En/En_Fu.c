#include <combo.h>
#include <combo/item.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x29c) = (h); } while (0)

void EnFu_HandlerNull(Actor* actor, GameState_Play* play)
{
}

void EnFu_HandlerAdultOcarina2(Actor* actor, GameState_Play* play)
{
    if (Actor_HasParent(actor))
    {
        SetEventChk(EV_OOT_CHK_SONG_STORMS);
        SET_HANDLER(actor, EnFu_HandlerNull);
        return;
    }
    comboGiveItemNpc(actor, play, GI_OOT_SONG_STORMS, NPC_OOT_SONG_STORMS, 10000.f, 500.f);
}

void EnFu_HandlerAdultOcarina(Actor* actor, GameState_Play* play)
{
    play->msgCtx.ocarinaMode = 4;
    SET_HANDLER(actor, EnFu_HandlerAdultOcarina2);
    EnFu_HandlerAdultOcarina2(actor, play);
}

PATCH_FUNC(0x80ac2d5c, EnFu_HandlerAdultOcarina);

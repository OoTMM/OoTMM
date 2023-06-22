#include <combo.h>
#include <combo/item.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x180) = (h); } while (0)

void EnMa1_HandlerNull(Actor* actor, GameState_Play* play)
{
}

void EnMa1_HandlerOcarina2(Actor* actor, GameState_Play* play)
{
    if (Actor_HasParent(actor))
    {
        SetEventChk(EV_OOT_CHK_SONG_EPONA);
        SET_HANDLER(actor, EnMa1_HandlerNull);
        return;
    }
    comboGiveItemNpc(actor, play, GI_OOT_SONG_EPONA, NPC_OOT_MALON_SONG, 10000.f, 500.f);
}

void EnMa1_HandlerOcarina(Actor* actor, GameState_Play* play)
{
    play->msgCtx.ocarinaMode = 4;
    SET_HANDLER(actor, EnMa1_HandlerOcarina2);
    EnMa1_HandlerOcarina2(actor, play);
}

PATCH_FUNC(0x809f1414, EnMa1_HandlerOcarina);

void EnMa1_HandlerEgg(Actor* this, GameState_Play* play)
{
    if (Actor_HasParent(this))
    {
        if (Message_IsClosed(this, play))
        {
            SetEventChk(EV_OOT_CHK_MALON_SPOKEN_HYRULE);
            SetEventChk(EV_OOT_CHK_MALON_EGG);
            SET_HANDLER(this, EnMa1_HandlerNull);
        }
        return;
    }
    comboGiveItemNpc(this, play, GI_OOT_CHICKEN, NPC_OOT_MALON_EGG, 200.f, 200.f);
}

PATCH_FUNC(0x809f1240, EnMa1_HandlerEgg);

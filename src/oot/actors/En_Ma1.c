#include <combo.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x180) = (h); } while (0)

void EnMa1_HandlerNull(Actor* actor, GameState_Play* play)
{
}

void EnMa1_HandlerOcarina2(Actor* actor, GameState_Play* play)
{
    if (!Actor_HasParent(actor))
    {
        GiveItem(actor, play, GI_OOT_SONG_EPONA, 10000.f, 500.f);
    }
    else
    {
        SetEventChk(EV_OOT_CHK_SONG_EPONA);
        SET_HANDLER(actor, EnMa1_HandlerNull);
    }
}

void EnMa1_HandlerOcarina(Actor* actor, GameState_Play* play)
{
    play->msgCtx.ocarinaMode = 4;
    SET_HANDLER(actor, EnMa1_HandlerOcarina2);
    EnMa1_HandlerOcarina2(actor, play);
}

PATCH_FUNC(0x809f1414, EnMa1_HandlerOcarina);

void EnMa1_HandlerEgg(Actor* actor, GameState_Play* play)
{
    s16 gi;

    if (Actor_HasParent(actor))
    {
        if (Message_GetState(&play->msgCtx) == 2)
        {
            SetEventChk(EV_OOT_CHK_MALON_SPOKEN_HYRULE);
            SetEventChk(EV_OOT_CHK_MALON_EGG);
            ActorDestroy(actor);
        }
    }
    else
    {
        gi = comboOverride(OV_NPC, 0, NPC_OOT_MALON_EGG, GI_OOT_CHICKEN);
        GiveItem(actor, play, gi, 200.f, 200.f);
    }
}

void EnMa1_AfterHandler(Actor* actor, GameState_Play* play)
{
    if (actor->messageId == 0x2043)
    {
        Message_Close(play);
        actor->update = EnMa1_HandlerEgg;
        EnMa1_HandlerEgg(actor, play);
    }
}

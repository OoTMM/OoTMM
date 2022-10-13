#include <combo.h>

void EnMa1_HandlerEgg(Actor* actor, GameState_Play* play)
{
    s16 gi;

    if (Actor_HasParent(actor))
    {
        if (Message_GetState((char*)play + 0x20d8) == 2)
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

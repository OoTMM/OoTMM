#include <combo.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x180) = (h); } while (0)

void EnSa_HandlerNull(Actor* this, GameState_Play* play)
{
}

void EnSa_HandlerMeadow_GiveItem(Actor* this, GameState_Play* play)
{
    s16 gi;

    if (Actor_HasParent(this))
    {
        SetEventChk(EV_OOT_CHK_SONG_SARIA);
        SET_HANDLER(this, EnSa_HandlerNull);
    }
    else
    {
        gi = comboOverride(OV_NPC, 0, NPC_OOT_SARIA_SONG, GI_OOT_SONG_SARIA);
        GiveItem(this, play, gi, 10000.f, 5000.f);
    }
}

void EnSa_HandlerMeadow(Actor* this, GameState_Play* play)
{
    if (GetEventChk(EV_OOT_CHK_SONG_SARIA))
    {
        SET_HANDLER(this, EnSa_HandlerNull);
    }
    else if (GET_LINK(play)->base.position.z < -2220.f)
    {
        SET_HANDLER(this, EnSa_HandlerMeadow_GiveItem);
        EnSa_HandlerMeadow_GiveItem(this, play);
    }
}

PATCH_FUNC(0x80aa9ecc, EnSa_HandlerMeadow);

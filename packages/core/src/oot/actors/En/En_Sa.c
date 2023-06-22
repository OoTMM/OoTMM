#include <combo.h>
#include <combo/item.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x180) = (h); } while (0)

void EnSa_HandlerNull(Actor* this, GameState_Play* play)
{
}

void EnSa_HandlerMeadow_GiveItem(Actor* this, GameState_Play* play)
{
    if (Actor_HasParent(this))
    {
        SetEventChk(EV_OOT_CHK_SONG_SARIA);
        SET_HANDLER(this, EnSa_HandlerNull);
        return;
    }
    comboGiveItemNpc(this, play, GI_OOT_SONG_SARIA, NPC_OOT_SARIA_SONG, 10000.f, 5000.f);
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

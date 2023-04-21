#include <combo.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x13c) = (h); } while (0)

void EnOkarinaTag_GiveSunSong(Actor* this, GameState_Play* play)
{
    s16 gi;

    if (Actor_HasParent(this))
    {
        SetEventChk(EV_OOT_CHK_SONG_SUN);
        ActorDestroy(this);
    }
    else
    {
        gi = comboOverride(OV_NPC, 0, NPC_OOT_ROYAL_TOMB_SONG, GI_OOT_SONG_SUN);
        GiveItem(this, play, gi, 10000.f, 500.f);
    }
}

void EnOkarinaTag_HandlerTombRoyal(Actor* this, GameState_Play* play)
{
    if (GetEventChk(EV_OOT_CHK_SONG_SUN))
        ActorDestroy(this);

    this->messageId = 0x5021;
    if (ActorTalkedTo(this))
    {
        Message_Close(play);
        SET_HANDLER(this, EnOkarinaTag_GiveSunSong);
        EnOkarinaTag_GiveSunSong(this, play);
    }
    else
    {
        ActorEnableTalk(this, play, 70.f);
    }
}

PATCH_FUNC(0x80a87510, EnOkarinaTag_HandlerTombRoyal);

static void EnOkarinaTag_HandleWindmill(Actor* this, GameState_Play* play)
{
    SetEventChk(EV_OOT_CHK_WINDMILL_FAST);
    SetEventChk(EV_OOT_CHK_WELL_DRAINED);
}

PATCH_CALL(0x80a873bc, EnOkarinaTag_HandleWindmill);

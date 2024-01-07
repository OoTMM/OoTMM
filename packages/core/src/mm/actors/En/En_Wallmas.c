#include <combo.h>
#include <combo/entrance.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x188) = (h); } while (0)

static int EnWallmas_GetDestEr(Actor* this, GameState_Play* play)
{
    u8 room;

    room = this->room;
    switch (play->sceneId)
    {
    case SCE_MM_DAMPE_HOUSE: return ENTR_MM_WALLMASTER_DAMPE;
    case SCE_MM_BENEATH_THE_WELL:
        switch (room)
        {
        case 1: return ENTR_MM_WALLMASTER_BTW_ENTRANCE;
        case 5: return ENTR_MM_WALLMASTER_BTW_FOUNTAIN;
        case 7: return ENTR_MM_WALLMASTER_BTW_EXIT;
        default: UNREACHABLE();
        }
    default: UNREACHABLE();
    }
}

static void EnWallmas_Nop(Actor* this, GameState_Play* play)
{
}

void EnWallmas_TakePlayerEr(Actor* this, GameState_Play* play)
{
    int entrance;

    entrance = comboEntranceOverride(EnWallmas_GetDestEr(this, play));
    comboTransition(play, entrance);
    SET_HANDLER(this, EnWallmas_Nop);
}

#include <combo.h>
#include <combo/entrance.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x180) = (h); } while (0)

static int EnWallmas_GetDestEr(Actor* this, GameState_Play* play)
{
    u8 room;

    room = this->room;

    switch (play->sceneId)
    {
    case SCE_OOT_TEMPLE_FOREST:
        return room == 0x14 ? ENTR_OOT_WALLMASTER_FOREST_CORRIDOR_EAST : ENTR_OOT_WALLMASTER_FOREST_CORRIDOR_WEST;
    case SCE_OOT_TEMPLE_SHADOW:
        return ENTR_OOT_WALLMASTER_SHADOW;
    case SCE_OOT_TEMPLE_SPIRIT:
        return room == 0x02 ? ENTR_OOT_WALLMASTER_SPIRIT_CHILD : ENTR_OOT_WALLMASTER_SPIRIT_ADULT;
    case SCE_OOT_BOTTOM_OF_THE_WELL:
        return ENTR_OOT_WALLMASTER_BOTW;
    case SCE_OOT_GERUDO_TRAINING_GROUND:
        return ENTR_OOT_WALLMASTER_GTG;
    case SCE_OOT_INSIDE_GANON_CASTLE:
        return ENTR_OOT_WALLMASTER_GANON;
    default:
        UNREACHABLE();
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

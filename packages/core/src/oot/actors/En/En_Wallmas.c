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
        switch (room)
        {
        case 0x02: return ENTR_OOT_WALLMASTER_SPIRIT_CHILD_RUPEES;
        case 0x05: return ENTR_OOT_WALLMASTER_SPIRIT_STATUE;
        case 0x08: return ENTR_OOT_WALLMASTER_SPIRIT_CHILD_SUN;
        case 0x0f: return ENTR_OOT_WALLMASTER_SPIRIT_ADULT_CLIMB;
        default: UNREACHABLE();
        }
    case SCE_OOT_BOTTOM_OF_THE_WELL:
        switch (room)
        {
        case 0x00: return ENTR_OOT_WALLMASTER_BOTW_MAIN;
        case 0x01: return ENTR_OOT_WALLMASTER_BOTW_BASEMENT;
        case 0x03: return ENTR_OOT_WALLMASTER_BOTW_PIT;
        default: UNREACHABLE();
        }
    case SCE_OOT_GERUDO_TRAINING_GROUND:
        return ENTR_OOT_WALLMASTER_GTG;
    case SCE_OOT_INSIDE_GANON_CASTLE:
        switch (room)
        {
        case 0x08: return ENTR_OOT_WALLMASTER_GANON_LIGHT;
        default: return ENTR_OOT_WALLMASTER_GANON_SPIRIT;
        }
    default:
        UNREACHABLE();
    }
}

void EnWallmas_TakePlayerEr(GameState_Play* play, Actor* this)
{
    play->nextEntranceIndex = EnWallmas_GetDestEr(this, play);
    play->transitionTrigger = TRANS_TYPE_NORMAL;
    play->transitionType = TRANS_GFX_BLACK;
    gIsEntranceOverride = 1;
}

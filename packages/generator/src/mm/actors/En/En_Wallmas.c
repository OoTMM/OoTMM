#include <combo.h>
#include <combo/entrance.h>
#include <combo/enemy_placement.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x188) = (h); } while (0)

static int EnWallmas_GetDestEr(Actor* this, PlayState* play)
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
        default: return ENTR_INVALID;
        }
    default: return ENTR_INVALID;
    }
}

void EnWallmas_TakePlayerEr(PlayState* play, Actor* this)
{
    int destination = comboEnemyNativePlacementIsShuffled(this) ? ENTR_INVALID : EnWallmas_GetDestEr(this, play);
    if (destination == ENTR_INVALID)
    {
        gIsEntranceOverride = 0;
        comboTransition(play, gLastEntrance);
        return;
    }
    comboClearCustomRespawn(CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE);
    play->nextEntrance = destination;
    play->transitionTrigger = TRANS_TRIGGER_START;
    play->transitionType = TRANS_TYPE_FADE_BLACK;
    gIsEntranceOverride = 1;
}

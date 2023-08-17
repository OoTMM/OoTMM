#include <combo.h>
#include <combo/entrance.h>

u16 EnTest7_TriggerWarp(Actor* actor, GameState_Play* play)
{
    u32 entrance;

    if (actor->variable & 0x8000)
    {
        entrance = gComboData.entrancesSong[actor->variable & 0xff] ^ MASK_FOREIGN_ENTRANCE;
        if (entrance & MASK_FOREIGN_ENTRANCE)
        {
            comboTransition(play, entrance);
            return 0;
        }

        return entrance;
    }

    switch (play->sceneId)
    {
    case SCE_MM_TEMPLE_WOODFALL:
    case SCE_MM_TEMPLE_SNOWHEAD:
    case SCE_MM_TEMPLE_GREAT_BAY:
    case SCE_MM_TEMPLE_STONE_TOWER:
    case SCE_MM_TEMPLE_STONE_TOWER_INVERTED:
    case SCE_MM_SAKON_HIDEOUT:
    case SCE_MM_LAIR_ODOLWA:
    case SCE_MM_LAIR_GOHT:
    case SCE_MM_LAIR_GYORG:
    case SCE_MM_LAIR_TWINMOLD:
        break;
    default:
        gIsEntranceOverride = 1;
    }

    if (play->sceneId == 0x4f) {
        return 0x2060;
    }
    return 0;
}

#include <combo.h>
#include <combo/entrance.h>

void EnTest7_TriggerWarpWrapper(Actor* actor, GameState_Play* play)
{
    ActorFunc EnTest7_TriggerWarp;
    u32 entrance;

    if (actor->variable & 0x8000)
    {
        entrance = gComboData.entrancesSong[actor->variable & 0xff] ^ MASK_FOREIGN_ENTRANCE;
    }
    else
    {
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
            /* TODO: Call the base func */
            EnTest7_TriggerWarp = actorAddr(AC_EN_TEST7, 0x80af2350);
            EnTest7_TriggerWarp(actor, play);
            return;
        }

        entrance = gComboData.entrancesOwl[(actor->variable & 0xff) - 0x1c];
    }
    comboTransition(play, entrance);
    ActorDestroy(actor);
}

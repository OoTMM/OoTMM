#include <combo.h>
#include <combo/entrance.h>
#include <combo/dungeon.h>

static const u32 kDungeonEntrances[] =
{
    ENTR_OOT_DEKU_TREE | MASK_FOREIGN_ENTRANCE,
    ENTR_OOT_DODONGO_CAVERN | MASK_FOREIGN_ENTRANCE,
    ENTR_OOT_JABU_JABU | MASK_FOREIGN_ENTRANCE,
    ENTR_OOT_TEMPLE_FOREST | MASK_FOREIGN_ENTRANCE,
    ENTR_OOT_TEMPLE_FIRE | MASK_FOREIGN_ENTRANCE,
    ENTR_OOT_TEMPLE_WATER | MASK_FOREIGN_ENTRANCE,
    ENTR_OOT_TEMPLE_SHADOW | MASK_FOREIGN_ENTRANCE,
    ENTR_OOT_TEMPLE_SPIRIT | MASK_FOREIGN_ENTRANCE,
    ENTR_MM_TEMPLE_WOODFALL,
    ENTR_MM_TEMPLE_SNOWHEAD,
    ENTR_MM_TEMPLE_GREAT_BAY,
    ENTR_MM_TEMPLE_STONE_TOWER_INVERTED,
};

void EnTest7_TriggerWarpWrapper(Actor* actor, GameState_Play* play)
{
    ActorFunc EnTest7_TriggerWarp;
    u32 entrance;
    s8 bossId;
    s8 dungeonId;

    if (actor->variable & 0x8000)
    {
        entrance = gComboData.entrancesSong[actor->variable & 0xff] ^ MASK_FOREIGN_ENTRANCE;
    }
    else
    {
        bossId = -1;
        switch (play->sceneId)
        {
        case SCE_MM_TEMPLE_WOODFALL:
        case SCE_MM_TEMPLE_SNOWHEAD:
        case SCE_MM_TEMPLE_GREAT_BAY:
        case SCE_MM_TEMPLE_STONE_TOWER:
        case SCE_MM_TEMPLE_STONE_TOWER_INVERTED:
        case SCE_MM_SAKON_HIDEOUT:
            /* TODO: Call the base func */
            EnTest7_TriggerWarp = actorAddr(AC_EN_TEST7, 0x80af2350);
            EnTest7_TriggerWarp(actor, play);
            return;
        case SCE_MM_LAIR_ODOLWA:
            bossId = BOSSID_ODOLWA;
            break;
        case SCE_MM_LAIR_GOHT:
            bossId = BOSSID_GOHT;
            break;
        case SCE_MM_LAIR_GYORG:
            bossId = BOSSID_GYORG;
            break;
        case SCE_MM_LAIR_TWINMOLD:
            bossId = BOSSID_TWINMOLD;
            break;
        }

        if (bossId != -1)
        {
            dungeonId = gComboData.boss[bossId];
            entrance = kDungeonEntrances[dungeonId];
        }
        else
            entrance = gComboData.entrancesOwl[(actor->variable & 0xff) - 0x1c];
    }

    comboTransition(play, entrance);
    ActorDestroy(actor);
}

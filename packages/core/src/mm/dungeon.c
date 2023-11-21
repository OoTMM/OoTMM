#include <combo.h>
#include <combo/dungeon.h>

int comboCurrentDungeon(GameState_Play* play)
{
    switch (play->sceneId)
    {
    /* Dungeons */
    case SCE_MM_TEMPLE_WOODFALL:
        return DUNGEONID_TEMPLE_WOODFALL;
    case SCE_MM_TEMPLE_SNOWHEAD:
        return DUNGEONID_TEMPLE_SNOWHEAD;
    case SCE_MM_TEMPLE_GREAT_BAY:
        return DUNGEONID_TEMPLE_GREAT_BAY;
    case SCE_MM_TEMPLE_STONE_TOWER:
        return DUNGEONID_TEMPLE_STONE_TOWER;
    case SCE_MM_TEMPLE_STONE_TOWER_INVERTED:
        return DUNGEONID_TEMPLE_STONE_TOWER_INVERTED;
    case SCE_MM_SPIDER_HOUSE_SWAMP:
        return DUNGEONID_SPIDER_HOUSE_SWAMP;
    case SCE_MM_SPIDER_HOUSE_OCEAN:
        return DUNGEONID_SPIDER_HOUSE_OCEAN;
    case SCE_MM_BENEATH_THE_WELL:
        return DUNGEONID_BENEATH_THE_WELL;
    case SCE_MM_PIRATE_FORTRESS_ENTRANCE:
    case SCE_MM_PIRATE_FORTRESS_EXTERIOR:
    case SCE_MM_PIRATE_FORTRESS_INTERIOR:
        return DUNGEONID_PIRATE_FORTRESS;
    case SCE_MM_CASTLE_IKANA:
    case SCE_MM_LAIR_IKANA:
        return DUNGEONID_IKANA_CASTLE;
    case SCE_MM_SECRET_SHRINE:
        return DUNGEONID_SECRET_SHRINE;
    case SCE_MM_CLOCK_TOWER_ROOFTOP:
    case SCE_MM_MOON:
    case SCE_MM_MOON_DEKU:
    case SCE_MM_MOON_GORON:
    case SCE_MM_MOON_ZORA:
    case SCE_MM_MOON_LINK:
    case SCE_MM_LAIR_MAJORA:
        return DUNGEONID_MOON;

    /* Boss lairs */
    case SCE_MM_LAIR_ODOLWA:
        return gComboData.boss[BOSSID_ODOLWA];
    case SCE_MM_LAIR_GOHT:
        return gComboData.boss[BOSSID_GOHT];
    case SCE_MM_LAIR_GYORG:
        return gComboData.boss[BOSSID_GYORG];
    case SCE_MM_LAIR_TWINMOLD:
        return gComboData.boss[BOSSID_TWINMOLD];

    default:
        return -1;
    }
}

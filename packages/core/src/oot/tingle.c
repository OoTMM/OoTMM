#include <combo.h>

static const u8 kTingleMapScenes[][12] =
{
    {
        SCE_MM_TERMINA_FIELD,
        SCE_MM_IKANA_GRAVEYARD,
        SCE_MM_PATH_MOUNTAIN_VILLAGE,
        SCE_MM_ROAD_SOUTHERN_SWAMP,
        SCE_MM_ROAD_IKANA,
        SCE_MM_CLOCK_TOWN_EAST,
        SCE_MM_CLOCK_TOWN_WEST,
        SCE_MM_CLOCK_TOWN_NORTH,
        SCE_MM_CLOCK_TOWN_SOUTH,
        SCE_MM_LAUNDRY_POOL,
        0xff,
    },
    {
        SCE_MM_SOUTHERN_SWAMP,
        SCE_MM_SOUTHERN_SWAMP_CLEAR,
        SCE_MM_WOODFALL,
        SCE_MM_DEKU_PALACE,
        SCE_MM_DEKU_KING_CHAMBER,
        SCE_MM_SPIDER_HOUSE_SWAMP,
        0xff,
    },
    {
        SCE_MM_MOUNTAIN_VILLAGE_WINTER,
        SCE_MM_MOUNTAIN_VILLAGE_SPRING,
        SCE_MM_GORON_VILLAGE_WINTER,
        SCE_MM_GORON_VILLAGE_SPRING,
        SCE_MM_GORON_SHRINE,
        SCE_MM_SNOWHEAD,
        SCE_MM_PATH_SNOWHEAD,
        SCE_MM_GORON_RACETRACK,
        SCE_MM_TWIN_ISLANDS_WINTER,
        SCE_MM_TWIN_ISLANDS_SPRING,
        0xff,
    },
    {
        SCE_MM_ROMANI_RANCH,
        SCE_MM_MILK_ROAD,
        SCE_MM_RANCH_HOUSE_BARN,
        SCE_MM_CUCCO_SHACK,
        SCE_MM_DOG_RACETRACK,
        SCE_MM_GORMAN_TRACK,
        0xff,
    },
    {
        SCE_MM_GREAT_BAY_COAST,
        SCE_MM_PINNACLE_ROCK,
        SCE_MM_ZORA_CAPE,
        SCE_MM_PIRATE_FORTRESS_ENTRANCE,
        SCE_MM_PIRATE_FORTRESS_EXTERIOR,
        SCE_MM_ZORA_HALL,
        SCE_MM_WATERFALL_RAPIDS,
        0xff,
    },
    {
        SCE_MM_STONE_TOWER,
        SCE_MM_STONE_TOWER_INVERTED,
        SCE_MM_CASTLE_IKANA,
        SCE_MM_IKANA_CANYON,
        SCE_MM_BENEATH_THE_WELL,
        0xff,
    },
};

static const u32 kTingleMapCloudMasks[] = {
    0x0003,
    0x001c,
    0x00e0,
    0x0100,
    0x1e00,
    0x6000,
};

/* Port from MM */
void Inventory_SetWorldMapCloudVisibility(s16 tingleId)
{
    const u8* tingleMapSceneIds = kTingleMapScenes[tingleId];
    u8 sceneId;

    for (;;)
    {
        sceneId = *tingleMapSceneIds;
        if (sceneId == 0xff)
            break;
        BITMAP32_SET(gMmSave.scenesVisible, sceneId);
        tingleMapSceneIds++;
    }

    gMmSave.worldMapCloudVisibility |= kTingleMapCloudMasks[tingleId];
}

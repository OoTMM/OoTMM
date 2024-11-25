#include <combo.h>

static int Horse_IsValidEntrance(u32 entranceId)
{
    switch (entranceId)
    {
    /* Field */
    case ENTR_MM_TERMINA_FIELD_FROM_ROAD_TO_SWAMP:
    case ENTR_MM_TERMINA_FIELD_FROM_PATH_TO_MOUNTAIN_VILLAGE:
    case ENTR_MM_TERMINA_FIELD_FROM_GREAT_BAY_COAST:
    case ENTR_MM_TERMINA_FIELD_FROM_ROAD_TO_IKANA:
    case ENTR_MM_TERMINA_FIELD_FROM_MILK_ROAD:

    /* Road to Swamp */
    case ENTR_MM_SWAMP_ROAD_FROM_FIELD:
    case ENTR_MM_SWAMP_ROAD_FROM_SWAMP:

    /* Swamp */
    case ENTR_MM_SWAMP_FROM_ROAD:

    /* Road to Mountain Village */
    case ENTR_MM_MOUNTAIN_VILLAGE_PATH_FROM_TERMINA_FIELD:

    /* Great Bay Coast */
    case ENTR_MM_GREAT_BAY_COAST_FROM_FIELD:
    case ENTR_MM_GREAT_BAY_COAST_FROM_ZORA_CAPE:

    /* Zora Cape */
    case ENTR_MM_ZORA_CAPE_FROM_GREAT_BAY_COAST:

    /* Road to Ikana */
    case ENTR_MM_IKANA_ROAD_FROM_FIELD:

    /* Milk Road */
    case ENTR_MM_MILK_ROAD_FROM_FIELD:
    case ENTR_MM_MILK_ROAD_FROM_ROMANI_RANCH:
    case ENTR_MM_MILK_ROAD_FROM_GORMAN_BACK:
    case ENTR_MM_MILK_ROAD_FROM_GORMAN_TRACK:
    case ENTR_MM_MILK_ROAD_FROM_RANCH2:
    case ENTR_MM_MILK_ROAD_FROM_GORMAN_TRACK2:

    /* Romani Ranch */
    case ENTR_MM_ROMANI_RANCH_FROM_MILK_ROAD:

    /* Gorman track */
    case ENTR_MM_GORMAN_TRACK_FROM_MILK_ROAD:
    case ENTR_MM_GORMAN_TRACK_UNK2:
    case ENTR_MM_GORMAN_TRACK_UNK3:
    case ENTR_MM_GORMAN_BACK_FROM_MILK_ROAD:
    case ENTR_MM_GORMAN_TRACK_UNK5:
    case ENTR_MM_GORMAN_TRACK_UNK6:
        return 1;
    default:
        return 0;
    }
}

void Horse_SpawnOverworldWrapper(PlayState* play, Player* link)
{
    if (!Horse_IsValidEntrance(gSave.entrance))
    {
        gHorseIsMounted = 0;
        if(link->transformation != MM_PLAYER_FORM_FIERCE_DEITY)
        {
            if(gSave.info.itemEquips.sword)
                gSave.info.itemEquips.buttonItems[0][0] = ITEM_MM_SWORD_KOKIRI + gSave.info.itemEquips.sword - 1;
            else
                gSave.info.itemEquips.buttonItems[0][0] = ITEM_NONE;
        }
    }
    Horse_SpawnOverworld(play, link);
}

PATCH_CALL(0x800f413c, Horse_SpawnOverworldWrapper);

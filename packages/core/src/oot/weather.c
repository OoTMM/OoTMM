#include <combo.h>

static void PatchWeather(GameState_Play* play)
{
    gFogState = 0;
    if (gSave.age != AGE_ADULT || gSaveContext.sceneSetupId >= 4)
        return;

    if (gSave.entrance == ENTR_OOT_FIELD_FROM_MARKET_ENTRANCE)
    {
        gFogState = 1;
        return;
    }

    if (!GetEventChk(EV_OOT_CHK_EPONA))
    {
        switch (gSave.entrance)
        {
        case ENTR_OOT_LON_LON_RANCH_FROM_FIELD:
        case ENTR_OOT_FIELD_FROM_LON_LON_RANCH:
            gFogState = 2;
            return;
        }
    }

    if (!GetEventChk(EV_OOT_CHK_MEDALLION_WATER))
    {
        switch (gSave.entrance)
        {
        case ENTR_OOT_ZORA_RIVER_FROM_LOST_WOODS:
        case ENTR_OOT_LOST_WOODS_FROM_ZORA_RIVER:
        case 0x019D: /* Zora River from Zora's Domain? */
            gFogState = 3;
            return;
        }

        switch (play->sceneId)
        {
        case SCE_OOT_ZORA_DOMAIN:
        case SCE_OOT_ZORA_FOUNTAIN:
            gFogState = 3;
            return;
        }
    }

    if (gSave.inventory.quest.medallionForest && gSave.inventory.quest.medallionFire && gSave.inventory.quest.medallionWater && !GetEventChk(EV_OOT_CHK_MEDALLION_SHADOW))
    {
        switch (play->sceneId)
        {
        case SCE_OOT_KAKARIKO_VILLAGE:
        case SCE_OOT_GRAVEYARD:
            switch (gSave.entrance)
            {
            case ENTR_OOT_KAKARIKO_FROM_FIELD:
            case ENTR_OOT_KAKARIKO_FROM_DEATH_MOUNTAIN:
            case ENTR_OOT_GRAVEYARD_FROM_TEMPLE_SHADOW:
                break;
            default:
                gFogState = 5;
                return;
            }
        }
    }

    if (!GetEventChk(EV_OOT_CHK_MEDALLION_FIRE))
    {
        if (gSave.entrance == ENTR_OOT_LOST_WOODS_FROM_GORON_CITY)
        {
            gFogState = 2;
            return;
        }

        switch (play->sceneId)
        {
        case SCE_OOT_KAKARIKO_VILLAGE:
        case SCE_OOT_GRAVEYARD:
        case SCE_OOT_DEATH_MOUNTAIN_TRAIL:
        case SCE_OOT_DEATH_MOUNTAIN_CRATER:
            switch (gSave.entrance)
            {
            case ENTR_OOT_KAKARIKO_FROM_FIELD:
            case 0x0195: /* Kakariko from Graveyard */
                break;
            default:
                gFogState = 2;
                return;
            }
        }
    }
}

static void Play_InitEnvironmentWrapper(GameState_Play* play, u16 skyboxId)
{
    /* Patch the weather system */
    PatchWeather(play);

    /* Forward */
    Play_InitEnvironment(play, skyboxId);
}

PATCH_CALL(0x8009ced0, Play_InitEnvironmentWrapper);

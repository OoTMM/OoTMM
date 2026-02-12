#include "multi.h"

#if defined(GAME_OOT)
u16 Multi_SceneKey(PlayState* play)
{
    u16 sceneKey;
    int instanceId;

    instanceId = Instance_Get(play);
    if (instanceId)
        return 0x100 + ((u16)instanceId - 1);

    sceneKey = play->sceneId;
    switch (sceneKey)
    {
    case SCE_OOT_MARKET_CHILD_DAY:
    case SCE_OOT_MARKET_CHILD_NIGHT:
        sceneKey = SCE_OOT_MARKET_ADULT;
        break;
    case SCE_OOT_MARKET_ENTRANCE_CHILD_DAY:
    case SCE_OOT_MARKET_ENTRANCE_CHILD_NIGHT:
        sceneKey = SCE_OOT_MARKET_ENTRANCE_ADULT;
        break;
    case SCE_OOT_BACK_ALLEY_NIGHT:
        sceneKey = SCE_OOT_BACK_ALLEY_DAY;
        break;
    case SCE_OOT_HYRULE_CASTLE:
        if (gSave.age == AGE_ADULT)
            sceneKey = 0x80;
        break;
    }
    return play->sceneId;
}
#endif

#if defined(GAME_MM)
u16 Multi_SceneKey(PlayState* play)
{
    u16 sceneKey;
    int instanceId;

    instanceId = Instance_Get(play);
    if (instanceId)
        return (0x100 + ((u16)instanceId - 1)) | 0x8000;

    sceneKey = play->sceneId;
    switch (sceneKey)
    {
    case SCE_MM_TEMPLE_STONE_TOWER_INVERTED:
        sceneKey = SCE_MM_TEMPLE_STONE_TOWER;
        break;
    case SCE_MM_SOUTHERN_SWAMP_CLEAR:
        sceneKey = SCE_MM_SOUTHERN_SWAMP;
        break;
    case SCE_MM_MOUNTAIN_VILLAGE_SPRING:
        sceneKey = SCE_MM_MOUNTAIN_VILLAGE_WINTER;
        break;
    case SCE_MM_GORON_VILLAGE_SPRING:
        sceneKey = SCE_MM_GORON_VILLAGE_WINTER;
        break;
    case SCE_MM_TWIN_ISLANDS_SPRING:
        sceneKey = SCE_MM_TWIN_ISLANDS_WINTER;
        break;
    }

    return sceneKey | 0x8000;
}
#endif


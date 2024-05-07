#include <combo.h>
#include <combo/util.h>
#include <combo/item.h>
#include <combo/config.h>

u32 popcount(u32 x)
{
    x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x & 0x0f0f0f0f) + ((x >> 4) & 0x0f0f0f0f);
    x = (x & 0x00ff00ff) + ((x >> 8) & 0x00ff00ff);
    x = (x & 0x0000ffff) + ((x >> 16) & 0x0000ffff);
    return x;
}

void* actorAddr(u16 actorId, u32 addr)
{
    ActorOvl* ovl;
    u32 offset;

    ovl = gActorOvl + actorId;
    offset = addr - ovl->vramStart;
    return (char*)ovl->data + offset;
}

int comboStrayFairyIndex(void)
{
#if defined(GAME_MM)
    if (gPlay->sceneId == SCE_MM_LAUNDRY_POOL || gPlay->sceneId == SCE_MM_CLOCK_TOWN_EAST)
        return 4;
    else
        return gSaveContext.dungeonId;
#endif

    return -1;
}

int comboOotDungeonScene(GameState_Play* play, int isBossKey)
{
#if defined(GAME_OOT)
    u16 mapIndex;

    /* Desert colossus hands */
    if (play->sceneId == SCE_OOT_DESERT_COLOSSUS)
        return SCE_OOT_TEMPLE_SPIRIT;

    mapIndex = gSaveContext.mapIndex;
    if (mapIndex == SCE_OOT_GANON_TOWER || mapIndex == SCE_OOT_INSIDE_GANON_CASTLE)
        return isBossKey ? SCE_OOT_GANON_TOWER : SCE_OOT_INSIDE_GANON_CASTLE;
    return mapIndex;
#endif

    return -1;
}

int comboMmDungeonIndex(void)
{
#if defined(GAME_MM)
    return gSaveContext.dungeonId;
#endif

    return -1;
}

int comboIsChateauActive(void)
{
#if defined(GAME_OOT)
    if (!Config_Flag(CFG_SHARED_MAGIC))
        return 0;
#endif

    return !!MM_GET_EVENT_WEEK(EV_MM_WEEK_DRANK_CHATEAU_ROMANI);
}

int comboIsLinkAdult(void)
{
#if defined(GAME_MM)
    if (!Config_Flag(CFG_MM_CROSS_AGE))
        return 0;
#endif
    return gOotSave.age == 0;
}

#include <combo.h>
#include <combo/item.h>
#include <combo/sr.h>

static void refillMagic(int level)
{
    gSave.playerData.magicLevel = 0;
    gSave.playerData.magicAmount = level * 0x30;
    gSaveContext.magicFillTarget = level * 0x30;
}

int comboAddItemEffect(GameState_Play* play, s16 gi)
{
    int count;

    count = 0;
    switch (gi)
    {
    case GI_MM_MAGIC_UPGRADE:
        refillMagic(1);
        break;
    case GI_MM_MAGIC_UPGRADE2:
        refillMagic(2);
        break;
    case GI_MM_STRAY_FAIRY:
        if (play->sceneId == SCE_MM_LAUNDRY_POOL || play->sceneId == SCE_MM_CLOCK_TOWN_EAST)
            count = comboAddStrayFairyMm(4);
        else
            count = comboAddStrayFairyMm(gSaveContext.dungeonId);
        break;
    }

    return count;
}

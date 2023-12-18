#include <combo.h>
#include <combo/item.h>
#include <combo/sr.h>

int comboAddItemEffect(GameState_Play* play, s16 gi)
{
    int count;

    count = 0;
    switch (gi)
    {
    case GI_MM_STRAY_FAIRY:
        if (play->sceneId == SCE_MM_LAUNDRY_POOL || play->sceneId == SCE_MM_CLOCK_TOWN_EAST)
            count = comboAddStrayFairyMm(4);
        else
            count = comboAddStrayFairyMm(gSaveContext.dungeonId);
        break;
    }

    return count;
}

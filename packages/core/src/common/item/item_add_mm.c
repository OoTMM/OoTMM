#include <combo.h>
#include <combo/item.h>
#include <combo/souls.h>

int comboAddItemMm(s16 gi, int noEffect)
{
    int count;

    count = 0;
    switch (gi)
    {
    case GI_MM_OWL_GREAT_BAY:
        gMmOwlFlags |= (1 << 0);
        break;
    case GI_MM_OWL_ZORA_CAPE:
        gMmOwlFlags |= (1 << 1);
        break;
    case GI_MM_OWL_SNOWHEAD:
        gMmOwlFlags |= (1 << 2);
        break;
    case GI_MM_OWL_MOUNTAIN_VILLAGE:
        gMmOwlFlags |= (1 << 3);
        break;
    case GI_MM_OWL_CLOCK_TOWN:
        gMmOwlFlags |= (1 << 4);
        break;
    case GI_MM_OWL_MILK_ROAD:
        gMmOwlFlags |= (1 << 5);
        break;
    case GI_MM_OWL_WOODFALL:
        gMmOwlFlags |= (1 << 6);
        break;
    case GI_MM_OWL_SOUTHERN_SWAMP:
        gMmOwlFlags |= (1 << 7);
        break;
    case GI_MM_OWL_IKANA_CANYON:
        gMmOwlFlags |= (1 << 8);
        break;
    case GI_MM_OWL_STONE_TOWER:
        gMmOwlFlags |= (1 << 9);
        break;
    }

    return count;
}

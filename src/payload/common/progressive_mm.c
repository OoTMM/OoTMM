#include <combo.h>

static s32 progressiveSword(void)
{
    switch (gMmSave.itemEquips.sword)
    {
    case 0:
        return GI_MM_KOKIRI_SWORD;
    case 1:
        return GI_MM_RAZOR_SWORD;
    default:
        return GI_MM_GILDED_SWORD;
    }
}

s32 comboProgressiveMm(s32 gi)
{
    switch (gi)
    {
    case GI_MM_KOKIRI_SWORD:
    case GI_MM_RAZOR_SWORD:
    case GI_MM_GILDED_SWORD:
        gi = progressiveSword();
        break;
    }
    return gi;
}

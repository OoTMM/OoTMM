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

static s32 progressiveShield(void)
{
    /* TODO: Use extra field like OoT */
    switch (gMmSave.itemEquips.shield)
    {
    case 0:
        return GI_MM_HYLIAN_SHIELD;
    default:
        return GI_MM_MIRROR_SHIELD;
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
    case GI_MM_HYLIAN_SHIELD:
    case GI_MM_MIRROR_SHIELD:
        gi = progressiveShield();
        break;
    }
    return gi;
}

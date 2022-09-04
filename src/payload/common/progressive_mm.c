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

static s32 progressiveBow(void)
{
    switch (gMmSave.inventory.upgrades.quiver)
    {
    case 0:
        return GI_MM_BOW;
    case 1:
        return GI_MM_QUIVER2;
    default:
        return GI_MM_QUIVER3;
    }
}

static s32 progressiveBombBag(void)
{
    switch (gMmSave.inventory.upgrades.bombBag)
    {
    case 0:
        return GI_MM_BOMB_BAG;
    case 1:
        return GI_MM_BOMB_BAG2;
    default:
        return GI_MM_BOMB_BAG3;
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
    case GI_MM_BOMB_BAG:
    case GI_MM_BOMB_BAG2:
    case GI_MM_BOMB_BAG3:
        gi = progressiveBombBag();
        break;
    case GI_MM_BOW:
    case GI_MM_QUIVER2:
    case GI_MM_QUIVER3:
        gi = progressiveBow();
        break;
    }
    return gi;
}

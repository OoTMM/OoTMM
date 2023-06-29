#include <combo.h>

static s32 progressiveGoldDustBottle(void)
{
    if (gMmExtraItems.goldDust)
        return GI_MM_BOTTLE_EMPTY;
    return GI_MM_BOTTLED_GOLD_DUST;
}

static s32 progressiveSword(void)
{
    switch (gMmSave.itemEquips.sword)
    {
    case 0:
        return GI_MM_SWORD_KOKIRI;
    case 1:
        return GI_MM_SWORD_RAZOR;
    default:
        return GI_MM_SWORD_GILDED;
    }
}

static s32 progressiveShield(void)
{
    if (gMmExtraFlags2.progressiveShield)
        return GI_MM_SHIELD_MIRROR;
    return GI_MM_PROGRESSIVE_SHIELD_HERO;
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

static s32 progressiveWallet(void)
{
    if (!gMmExtraFlags2.childWallet)
        return GI_MM_WALLET;
    switch (gMmSave.inventory.upgrades.wallet)
    {
    case 0: return GI_MM_WALLET2;
    case 1: return GI_MM_WALLET3;
    case 2: return comboConfig(CFG_COLOSSAL_WALLET) ? GI_MM_WALLET4 : GI_MM_WALLET3;
    case 3: return comboConfig(CFG_BOTTOMLESS_WALLET) ? GI_MM_WALLET5 : GI_MM_WALLET4;
    }
}

static s32 progressiveMagic(void)
{
    switch (gMmSave.playerData.magicAcquired)
    {
    case 0:
        return GI_MM_MAGIC_UPGRADE;
    default:
        return GI_MM_MAGIC_UPGRADE2;
    }
}

static s32 progressiveSongLullaby(void)
{
    switch (gMmSave.inventory.quest.songLullabyIntro)
    {
    case 0:
        return GI_MM_SONG_GORON_HALF;
    default:
        return GI_MM_SONG_GORON;
    }
}

// This is only called when hookshots are shared
static s32 progressiveHookshot(s32 gi)
{
    /* Short hookshot enabled - ignore shared items */
    if (comboConfig(CFG_MM_HOOKSHOT_SHORT))
    {
        if (!gMmExtraItems.hookshot)
            return GI_MM_HOOKSHOT_SHORT;
        return GI_MM_HOOKSHOT;
    }

    /* No short hook but shared hookshots */
    if (comboConfig(CFG_SHARED_HOOKSHOT))
    {
        if (!gOotExtraItems.hookshot)
            return GI_MM_HOOKSHOT;
        return GI_OOT_LONGSHOT | MASK_FOREIGN_GI;
    }

    /* Not progressive */
    return gi;
}

static s32 progressiveOcarina(void)
{
    if (!gMmExtraItems.ocarina)
        return GI_MM_OCARINA_FAIRY;
    return GI_MM_OCARINA_OF_TIME;
}

s32 comboProgressiveMm(s32 gi)
{
    switch (gi)
    {
    case GI_MM_BOTTLED_GOLD_DUST:
        gi = progressiveGoldDustBottle();
        break;
    case GI_MM_OCARINA_FAIRY:
    case GI_MM_OCARINA_OF_TIME:
        if (comboConfig(CFG_MM_OCARINA_FAIRY))
            gi = progressiveOcarina();
        break;
    case GI_MM_SWORD_KOKIRI:
    case GI_MM_SWORD_RAZOR:
    case GI_MM_SWORD_GILDED:
        gi = progressiveSword();
        break;
    case GI_MM_PROGRESSIVE_SHIELD_HERO:
    case GI_MM_SHIELD_MIRROR:
        if (comboConfig(CFG_MM_PROGRESSIVE_SHIELDS))
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
    case GI_MM_WALLET:
    case GI_MM_WALLET2:
    case GI_MM_WALLET3:
    case GI_MM_WALLET4:
    case GI_MM_WALLET5:
        gi = progressiveWallet();
        break;
    case GI_MM_MAGIC_UPGRADE:
    case GI_MM_MAGIC_UPGRADE2:
        gi = progressiveMagic();
        break;
    case GI_MM_SONG_GORON:
    case GI_MM_SONG_GORON_HALF:
        if (comboConfig(CFG_MM_PROGRESSIVE_LULLABY))
            gi = progressiveSongLullaby();
        break;
    case GI_MM_HOOKSHOT_SHORT:
    case GI_MM_HOOKSHOT:
        gi = progressiveHookshot(gi);
        break;
    }

    return gi;
}

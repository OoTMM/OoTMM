#include <combo.h>

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
    switch (gMmSave.inventory.upgrades.wallet)
    {
    case 0:
        return GI_MM_WALLET2;
    default:
        return GI_MM_WALLET3;
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

static s32 progressiveHookshot(void)
{
    if (!gOotExtraItems.hookshot)
        return GI_MM_HOOKSHOT;
    return GI_OOT_LONGSHOT | MASK_FOREIGN_GI;
}

s32 comboProgressiveMm(s32 gi)
{
    switch (gi)
    {
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
    case GI_MM_WALLET2:
    case GI_MM_WALLET3:
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
    case GI_MM_HOOKSHOT:
        if (comboConfig(CFG_SHARED_HOOKSHOT))
            gi = progressiveHookshot();
        break;
    }

    return gi;
}

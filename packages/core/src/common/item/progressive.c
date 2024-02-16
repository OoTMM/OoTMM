#include <combo.h>
#include <combo/item.h>

static s16 progressiveRutoLetter(void)
{
    if (gOotExtraItems.rutoLetter)
        return GI_OOT_BOTTLE_EMPTY;
    return GI_OOT_RUTO_LETTER;
}

static s16 progressiveGoldDustBottle(void)
{
    if (gMmExtraItems.goldDust)
        return GI_MM_BOTTLE_EMPTY;
    return GI_MM_BOTTLED_GOLD_DUST;
}

static s16 progressiveBombBagOot(void)
{
    if (gOotSave.inventory.items[ITS_OOT_BOMBS] == ITEM_NONE)
        return GI_OOT_BOMB_BAG;
    switch (gOotSave.inventory.upgrades.bombBag)
    {
    case 0:
        return GI_OOT_BOMB_BAG; /* Redundent */
    case 1:
        return GI_OOT_BOMB_BAG2;
    default:
        return GI_OOT_BOMB_BAG3;
    }
}

static s16 progressiveBombBagMm(void)
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

static s16 progressiveBowOot(void)
{
    if (gOotSave.inventory.items[ITS_OOT_BOW] == ITEM_NONE)
        return GI_OOT_BOW;
    switch (gOotSave.inventory.upgrades.quiver)
    {
    case 0:
    case 1:
        return GI_OOT_QUIVER2;
    default:
        return GI_OOT_QUIVER3;
    }
}

static s16 progressiveBowMm(void)
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

static s16 progressiveSlingshot(void)
{
    if (gOotSave.inventory.items[ITS_OOT_SLINGSHOT] == ITEM_NONE)
        return GI_OOT_SLINGSHOT;
    switch (gOotSave.inventory.upgrades.bulletBag)
    {
    case 0:
        return GI_OOT_BULLET_BAG; /* Not used */
    case 1:
        return GI_OOT_BULLET_BAG2;
    default:
        return GI_OOT_BULLET_BAG3;
    }
}

static s16 progressiveOcarinaOot(void)
{
    if (gOotExtraItems.ocarina == 0)
        return GI_OOT_OCARINA_FAIRY;
    return GI_OOT_OCARINA_TIME;
}

static s16 progressiveOcarinaMm(void)
{
    if (!gMmExtraItems.ocarina)
        return GI_MM_OCARINA_FAIRY;
    return GI_MM_OCARINA_OF_TIME;
}

static s16 progressiveHookshotOot(void)
{
    if (gOotExtraItems.hookshot == 0)
        return GI_OOT_HOOKSHOT;
    return GI_OOT_LONGSHOT;
}

/* This is only called when hookshots are shared */
static s16 progressiveHookshotMm(s16 gi)
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
        return GI_OOT_LONGSHOT;
    }

    /* Not progressive */
    return gi;
}

static s16 progressiveSwordGoron(void)
{
    if (!(gOotSave.inventory.equipment.swords & (EQ_OOT_SWORD_KNIFE | EQ_OOT_SWORD_KNIFE_BROKEN)))
        return GI_OOT_SWORD_KNIFE;
    return GI_OOT_SWORD_BIGGORON;
}

static s16 progressiveSwordOot(void)
{
    if (!(gOotSave.inventory.equipment.swords & EQ_OOT_SWORD_KOKIRI))
        return GI_OOT_SWORD_KOKIRI;
    if (!(gOotSave.inventory.equipment.swords & EQ_OOT_SWORD_MASTER))
        return GI_OOT_SWORD_MASTER;
    return progressiveSwordGoron();
}

static s16 progressiveSwordMm(void)
{
    switch (gMmSave.itemEquips.sword)
    {
    case 0:
        return GI_MM_SWORD_KOKIRI;
    case 1:
        return GI_MM_SWORD_RAZOR;
    case 2:
        return GI_MM_SWORD_GILDED;
    default:
        return comboConfig(CFG_MM_PROGRESSIVE_GFS) ? GI_MM_GREAT_FAIRY_SWORD : GI_MM_SWORD_GILDED;
    }
}

/* We use an extra field to know which shields we got from shops */
static s16 progressiveShieldOot(void)
{
    if (!(gOotExtraItems.shield & EQ_OOT_SHIELD_DEKU))
        return GI_OOT_PROGRESSIVE_SHIELD_DEKU;
    if (!(gOotExtraItems.shield & EQ_OOT_SHIELD_HYLIAN))
    {
#if defined(GAME_MM)
        if (comboConfig(CFG_SHARED_SHIELDS))
            return GI_MM_PROGRESSIVE_SHIELD_HERO;
#endif
        return GI_OOT_PROGRESSIVE_SHIELD_HYLIAN;
    }

#if defined(GAME_MM)
    if (comboConfig(CFG_SHARED_SHIELDS))
        return GI_MM_SHIELD_MIRROR;
#endif

    return GI_OOT_SHIELD_MIRROR;
}

static s16 progressiveShieldMm(void)
{
    if (gMmExtraFlags2.progressiveShield)
        return GI_MM_SHIELD_MIRROR;
    return GI_MM_PROGRESSIVE_SHIELD_HERO;
}

static s16 progressiveStrengthOot(void)
{
    switch (gOotSave.inventory.upgrades.strength)
    {
    case 0:
        return GI_OOT_GORON_BRACELET;
    case 1:
        return GI_OOT_SILVER_GAUNTLETS;
    default:
        return GI_OOT_GOLDEN_GAUNTLETS;
    }
}

static s16 progressiveStrengthMm(void)
{
    switch (gMmSave.inventory.upgrades.strength)
    {
    case 0:
        return GI_MM_GORON_BRACELET;
    case 1:
        return GI_MM_SILVER_GAUNTLETS;
    default:
        return GI_MM_GOLDEN_GAUNTLETS;
    }
}

static s16 progressiveDiveOot(void)
{
    if (gOotSave.inventory.upgrades.dive)
        return GI_OOT_SCALE_GOLDEN;
    return GI_OOT_SCALE_SILVER;
}

static s16 progressiveDiveMm(void)
{
    if (gMmSave.inventory.upgrades.scale)
        return GI_MM_SCALE_GOLDEN;
    return GI_MM_SCALE_SILVER;
}

static s16 progressiveWalletOot(void)
{
    if (!gOotExtraFlags.childWallet)
        return GI_OOT_WALLET;
    switch (gOotSave.inventory.upgrades.wallet)
    {
    case 0: return GI_OOT_WALLET2;
    case 1: return GI_OOT_WALLET3;
    case 2: return comboConfig(CFG_COLOSSAL_WALLET) ? GI_OOT_WALLET4 : GI_OOT_WALLET3;
    case 3: return comboConfig(CFG_BOTTOMLESS_WALLET) ? GI_OOT_WALLET5 : GI_OOT_WALLET4;
    }
}

static s16 progressiveWalletMm(void)
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

static s16 progressiveMagicOot(void)
{
    if (gOotSave.playerData.magicUpgrade)
        return GI_OOT_MAGIC_UPGRADE2;
    return GI_OOT_MAGIC_UPGRADE;
}

static s16 progressiveMagicMm(void)
{
    switch (gMmSave.playerData.magicAcquired)
    {
    case 0:
        return GI_MM_MAGIC_UPGRADE;
    default:
        return GI_MM_MAGIC_UPGRADE2;
    }
}

static s16 progressiveUpgradeNut(void)
{
    if (gOotSave.inventory.upgrades.dekuNut < 2)
        return GI_OOT_NUT_UPGRADE;
    return GI_OOT_NUT_UPGRADE2;
}

static s16 progressiveUpgradeStick(void)
{
    if (gOotSave.inventory.upgrades.dekuStick < 2)
        return GI_OOT_STICK_UPGRADE;
    return GI_OOT_STICK_UPGRADE2;
}

static s16 progressiveSongLullaby(void)
{
    switch (gMmSave.inventory.quest.songLullabyIntro)
    {
    case 0:
        return GI_MM_SONG_GORON_HALF;
    default:
        return GI_MM_SONG_GORON;
    }
}

static s16 progressiveBombchuBagOot(s16 gi, int ovflags)
{
    if (!comboConfig(CFG_OOT_BOMBCHU_BAG) || (gOotSave.inventory.items[ITS_OOT_BOMBCHU] == ITEM_OOT_BOMBCHU_10) || (ovflags & OVF_PRECOND))
        return gi;

    switch (gi)
    {
    case GI_OOT_BOMBCHU_5:  return GI_OOT_BOMBCHU_BAG_5;
    case GI_OOT_BOMBCHU_10: return GI_OOT_BOMBCHU_BAG_10;
    case GI_OOT_BOMBCHU_20: return GI_OOT_BOMBCHU_BAG_20;
    default: return gi;
    }
}

static s16 progressiveBombchuBagMm(s16 gi, int ovflags)
{
    if (!comboConfig(CFG_MM_BOMBCHU_BAG) || (gMmSave.inventory.items[ITS_MM_BOMBCHU] == ITEM_MM_BOMBCHU) || (ovflags & OVF_PRECOND))
        return gi;

    switch (gi)
    {
    case GI_MM_BOMBCHU:    return GI_MM_BOMBCHU_BAG;
    case GI_MM_BOMBCHU_5:  return GI_MM_BOMBCHU_BAG_5;
    case GI_MM_BOMBCHU_10: return GI_MM_BOMBCHU_BAG_10;
    case GI_MM_BOMBCHU_20: return GI_MM_BOMBCHU_BAG_20;
    default: return gi;
    }
}

static s16 progressiveClock(void)
{
    static s16 kClocks[] = {
        GI_MM_CLOCK1,
        GI_MM_CLOCK2,
        GI_MM_CLOCK3,
        GI_MM_CLOCK4,
        GI_MM_CLOCK5,
        GI_MM_CLOCK6,
    };

    int index;

    index = popcount(gSharedCustomSave.mm.halfDays);
    if (comboConfig(CFG_MM_CLOCKS_PROGRESSIVE_REVERSE))
        index = ARRAY_SIZE(kClocks) - index - 1;
    if (index < 0)
        index = 0;
    if (index >= ARRAY_SIZE(kClocks))
        index = ARRAY_SIZE(kClocks) - 1;

    return kClocks[index];
}

s16 comboProgressive(s16 gi, int ovflags)
{
    switch (gi)
    {
    /* Items */
    case GI_OOT_BOMBCHU_5:
    case GI_OOT_BOMBCHU_10:
    case GI_OOT_BOMBCHU_20:
        gi = progressiveBombchuBagOot(gi, ovflags);
        break;
    case GI_OOT_BOMB_BAG:
    case GI_OOT_BOMB_BAG2:
    case GI_OOT_BOMB_BAG3:
        gi = progressiveBombBagOot();
        break;
    case GI_OOT_BOW:
    case GI_OOT_QUIVER2:
    case GI_OOT_QUIVER3:
        gi = progressiveBowOot();
        break;
    case GI_OOT_SLINGSHOT:
    case GI_OOT_BULLET_BAG:
    case GI_OOT_BULLET_BAG2:
    case GI_OOT_BULLET_BAG3:
        gi = progressiveSlingshot();
        break;
    case GI_OOT_OCARINA_FAIRY:
    case GI_OOT_OCARINA_TIME:
        gi = progressiveOcarinaOot();
        break;
    case GI_OOT_HOOKSHOT:
    case GI_OOT_LONGSHOT:
        gi = progressiveHookshotOot();
        break;
    /* Equipment */
    case GI_OOT_SWORD_KOKIRI:
    case GI_OOT_SWORD_MASTER:
        if (comboConfig(CFG_OOT_PROGRESSIVE_SWORDS))
            gi = progressiveSwordOot();
        break;
    case GI_OOT_SWORD_BIGGORON:
    case GI_OOT_SWORD_KNIFE:
        if (comboConfig(CFG_OOT_PROGRESSIVE_SWORDS) || comboConfig(CFG_OOT_PROGRESSIVE_SWORDS_GORON))
            gi = progressiveSwordGoron();
        break;
    case GI_OOT_PROGRESSIVE_SHIELD_DEKU:
    case GI_OOT_PROGRESSIVE_SHIELD_HYLIAN:
    case GI_OOT_SHIELD_MIRROR:
        if (comboConfig(CFG_OOT_PROGRESSIVE_SHIELDS))
            gi = progressiveShieldOot();
        break;
    /* Upgrades */
    case GI_OOT_GORON_BRACELET:
    case GI_OOT_SILVER_GAUNTLETS:
    case GI_OOT_GOLDEN_GAUNTLETS:
        gi = progressiveStrengthOot();
        break;
    case GI_OOT_SCALE_SILVER:
    case GI_OOT_SCALE_GOLDEN:
        gi = progressiveDiveOot();
        break;
    case GI_OOT_WALLET:
    case GI_OOT_WALLET2:
    case GI_OOT_WALLET3:
    case GI_OOT_WALLET4:
    case GI_OOT_WALLET5:
        gi = progressiveWalletOot();
        break;
    case GI_OOT_MAGIC_UPGRADE:
    case GI_OOT_MAGIC_UPGRADE2:
        gi = progressiveMagicOot();
        break;
    case GI_OOT_NUT_UPGRADE:
    case GI_OOT_NUT_UPGRADE2:
        gi = progressiveUpgradeNut();
        break;
    case GI_OOT_STICK_UPGRADE:
    case GI_OOT_STICK_UPGRADE2:
        gi = progressiveUpgradeStick();
        break;
    case GI_OOT_RUTO_LETTER:
        gi = progressiveRutoLetter();
        break;
    case GI_MM_BOMBCHU:
    case GI_MM_BOMBCHU_5:
    case GI_MM_BOMBCHU_10:
    case GI_MM_BOMBCHU_20:
        gi = progressiveBombchuBagMm(gi, ovflags);
        break;
    case GI_MM_BOTTLED_GOLD_DUST:
        gi = progressiveGoldDustBottle();
        break;
    case GI_MM_OCARINA_FAIRY:
    case GI_MM_OCARINA_OF_TIME:
        if (comboConfig(CFG_MM_OCARINA_FAIRY))
            gi = progressiveOcarinaMm();
        break;
    case GI_MM_SWORD_KOKIRI:
    case GI_MM_SWORD_RAZOR:
    case GI_MM_SWORD_GILDED:
        gi = progressiveSwordMm();
        break;
    case GI_MM_GREAT_FAIRY_SWORD:
        if (comboConfig(CFG_MM_PROGRESSIVE_GFS))
            gi = progressiveSwordMm();
        break;
    case GI_MM_PROGRESSIVE_SHIELD_HERO:
    case GI_MM_SHIELD_MIRROR:
        if (comboConfig(CFG_MM_PROGRESSIVE_SHIELDS))
            gi = progressiveShieldMm();
        break;
    case GI_MM_BOMB_BAG:
    case GI_MM_BOMB_BAG2:
    case GI_MM_BOMB_BAG3:
        gi = progressiveBombBagMm();
        break;
    case GI_MM_BOW:
    case GI_MM_QUIVER2:
    case GI_MM_QUIVER3:
        gi = progressiveBowMm();
        break;
    case GI_MM_GORON_BRACELET:
    case GI_MM_SILVER_GAUNTLETS:
    case GI_MM_GOLDEN_GAUNTLETS:
        gi = progressiveStrengthMm();
        break;
    case GI_MM_SCALE_SILVER:
    case GI_MM_SCALE_GOLDEN:
        gi = progressiveDiveMm();
        break;
    case GI_MM_WALLET:
    case GI_MM_WALLET2:
    case GI_MM_WALLET3:
    case GI_MM_WALLET4:
    case GI_MM_WALLET5:
        gi = progressiveWalletMm();
        break;
    case GI_MM_MAGIC_UPGRADE:
    case GI_MM_MAGIC_UPGRADE2:
        gi = progressiveMagicMm();
        break;
    case GI_MM_SONG_GORON:
    case GI_MM_SONG_GORON_HALF:
        if (comboConfig(CFG_MM_PROGRESSIVE_LULLABY))
            gi = progressiveSongLullaby();
        break;
    case GI_MM_HOOKSHOT_SHORT:
    case GI_MM_HOOKSHOT:
        gi = progressiveHookshotMm(gi);
        break;
    case GI_MM_CLOCK1:
    case GI_MM_CLOCK2:
    case GI_MM_CLOCK3:
    case GI_MM_CLOCK4:
    case GI_MM_CLOCK5:
    case GI_MM_CLOCK6:
        if (comboConfig(CFG_MM_CLOCKS_PROGRESSIVE))
            gi = progressiveClock();
        break;
    default:
        break;
    }

    return gi;
}

s16 comboRenewable(s16 gi, s16 def)
{
    switch (gi)
    {
    case GI_OOT_RECOVERY_HEART:
    case GI_OOT_STICK:
    case GI_OOT_STICKS_5:
    case GI_OOT_STICKS_10:
    case GI_OOT_NUTS_5:
    case GI_OOT_NUTS_5_ALT:
    case GI_OOT_NUTS_10:
    case GI_OOT_BOMB:
    case GI_OOT_BOMBS_5:
    case GI_OOT_BOMBS_10:
    case GI_OOT_BOMBS_20:
    case GI_OOT_BOMBS_30:
    case GI_OOT_ARROWS_5:
    case GI_OOT_ARROWS_10:
    case GI_OOT_ARROWS_30:
    case GI_OOT_DEKU_SEEDS_5:
    case GI_OOT_DEKU_SEEDS_30:
    case GI_OOT_BOMBCHU_5:
    case GI_OOT_BOMBCHU_10:
    case GI_OOT_BOMBCHU_20:
    case GI_OOT_SHIELD_DEKU:
    case GI_OOT_SHIELD_HYLIAN:
    case GI_OOT_MILK:
    case GI_OOT_POTION_RED:
    case GI_OOT_POTION_GREEN:
    case GI_OOT_POTION_BLUE:
    case GI_OOT_FAIRY:
    case GI_OOT_FISH:
    case GI_OOT_BUG:
    case GI_OOT_BLUE_FIRE:
    case GI_OOT_POE:
    case GI_OOT_MAGIC_JAR_SMALL:
    case GI_OOT_MAGIC_JAR_LARGE:
    case GI_OOT_FAIRY_BIG:
    case GI_MM_FAIRY_BIG:
    case GI_MM_RECOVERY_HEART:
    case GI_MM_STICK:
    case GI_MM_NUT:
    case GI_MM_NUTS_5:
    case GI_MM_NUTS_10:
    case GI_MM_BOMB:
    case GI_MM_BOMBS_5:
    case GI_MM_BOMBS_10:
    case GI_MM_BOMBS_20:
    case GI_MM_BOMBS_30:
    case GI_MM_BOMBCHU:
    case GI_MM_BOMBCHU_5:
    case GI_MM_BOMBCHU_10:
    case GI_MM_BOMBCHU_20:
    case GI_MM_ARROWS_10:
    case GI_MM_ARROWS_30:
    case GI_MM_ARROWS_40:
    case GI_MM_SHIELD_HERO:
    case GI_MM_MILK:
    case GI_MM_CHATEAU:
    case GI_MM_FISH:
    case GI_MM_BUGS:
    case GI_MM_FAIRY:
    case GI_MM_POTION_RED:
    case GI_MM_POTION_GREEN:
    case GI_MM_POTION_BLUE:
    case GI_MM_MAGIC_BEAN:
    case GI_MM_POE:
    case GI_MM_MAGIC_JAR_SMALL:
    case GI_MM_MAGIC_JAR_LARGE:
        break;
    case GI_MM_BOTTLE_POTION_RED:
        gi = GI_MM_POTION_RED;
        break;
    case GI_MM_BOTTLE_POTION_GREEN:
        gi = GI_MM_POTION_GREEN;
        break;
    case GI_MM_BOTTLE_POTION_BLUE:
        gi = GI_MM_POTION_BLUE;
        break;
    case GI_MM_BOTTLE_CHATEAU:
        gi = GI_MM_CHATEAU;
        break;
    case GI_MM_BOTTLE_MILK:
        gi = GI_MM_MILK;
        break;
    case GI_OOT_BOTTLE_MILK:
        gi = GI_OOT_MILK;
        break;
    case GI_OOT_BOTTLE_POTION_RED:
        gi = GI_OOT_POTION_RED;
        break;
    case GI_OOT_BOTTLE_POTION_GREEN:
        gi = GI_OOT_POTION_GREEN;
        break;
    case GI_OOT_BOTTLE_POTION_BLUE:
        gi = GI_OOT_POTION_BLUE;
        break;
    case GI_OOT_BOTTLE_FAIRY:
        gi = GI_OOT_FAIRY;
        break;
    case GI_MM_BOTTLE_FAIRY:
        gi = GI_MM_FAIRY;
        break;
    case GI_OOT_BOTTLE_POE:
        gi = GI_OOT_POE;
        break;
    case GI_MM_BOTTLE_POE:
        gi = GI_MM_POE;
        break;
    case GI_OOT_BOTTLE_BIG_POE:
        gi = GI_OOT_BIG_POE;
        break;
    case GI_MM_BOTTLE_BIG_POE:
        gi = GI_MM_BIG_POE;
        break;
    case GI_OOT_BOTTLE_BLUE_FIRE:
        gi = GI_OOT_BLUE_FIRE;
        break;
    default:
        return def;
    }

    return gi;
}

#include <combo.h>

#define SPF_STONES                  (1 <<  0)
#define SPF_MEDALLIONS              (1 <<  1)
#define SPF_REMAINS                 (1 <<  2)
#define SPF_SKULLS_GOLD             (1 <<  3)
#define SPF_SKULLS_SWAMP            (1 <<  4)
#define SPF_SKULLS_OCEAN            (1 <<  5)
#define SPF_FAIRIES_WF              (1 <<  6)
#define SPF_FAIRIES_SH              (1 <<  7)
#define SPF_FAIRIES_GB              (1 <<  8)
#define SPF_FAIRIES_ST              (1 <<  9)
#define SPF_FAIRY_TOWN              (1 << 10)
#define SPF_MASKS_REGULAR           (1 << 11)
#define SPF_MASKS_TRANSFORM         (1 << 12)
#define SPF_MASKS_OOT               (1 << 13)
#define SPF_TRIFORCE                (1 << 14)
#define SPF_COIN_RED                (1 << 15)
#define SPF_COIN_GREEN              (1 << 16)
#define SPF_COIN_BLUE               (1 << 17)
#define SPF_COIN_YELLOW             (1 << 18)

int comboGoalCond(void)
{
    if (comboConfig(CFG_GOAL_GANON) && !gOotExtraFlags.ganon)
        return 0;
    if (comboConfig(CFG_GOAL_MAJORA) && !gMmExtraFlags2.majora)
        return 0;
    return 1;
}

int comboSpecialCond(int special)
{
    SpecialCond* cond;
    s16 count;

    /* Shared masks */
    u8 hasMaskGoron;
    u8 hasMaskZora;
    u8 hasMaskKeaton;
    u8 hasMaskBunny;
    u8 hasMaskTruth;
    u8 hasMaskBlast;
    u8 hasMaskStone;

    cond = &gComboData.special[special];
    count = 0;
    hasMaskGoron = 0;
    hasMaskZora = 0;
    hasMaskKeaton = 0;
    hasMaskBunny = 0;
    hasMaskTruth = 0;
    hasMaskBlast = 0;
    hasMaskStone = 0;

    if (cond->flags & SPF_STONES)
    {
        count += gOotSave.inventory.quest.stoneEmerald;
        count += gOotSave.inventory.quest.stoneRuby;
        count += gOotSave.inventory.quest.stoneSapphire;
    }

    if (cond->flags & SPF_MEDALLIONS)
    {
        count += gOotSave.inventory.quest.medallionForest;
        count += gOotSave.inventory.quest.medallionFire;
        count += gOotSave.inventory.quest.medallionWater;
        count += gOotSave.inventory.quest.medallionSpirit;
        count += gOotSave.inventory.quest.medallionShadow;
        count += gOotSave.inventory.quest.medallionLight;
    }

    if (cond->flags & SPF_REMAINS)
    {
        count += gMmSave.inventory.quest.remainsOdolwa;
        count += gMmSave.inventory.quest.remainsGoht;
        count += gMmSave.inventory.quest.remainsGyorg;
        count += gMmSave.inventory.quest.remainsTwinmold;
    }

    if (cond->flags & SPF_SKULLS_GOLD)
        count += gOotSave.inventory.goldTokens;

    if (cond->flags & SPF_SKULLS_SWAMP)
        count += gMmSave.skullCountSwamp;

    if (cond->flags & SPF_SKULLS_OCEAN)
        count += gMmSave.skullCountOcean;

    if (cond->flags & SPF_FAIRIES_WF)
        count += gMmSave.inventory.strayFairies[0];

    if (cond->flags & SPF_FAIRIES_SH)
        count += gMmSave.inventory.strayFairies[1];

    if (cond->flags & SPF_FAIRIES_GB)
        count += gMmSave.inventory.strayFairies[2];

    if (cond->flags & SPF_FAIRIES_ST)
        count += gMmSave.inventory.strayFairies[3];

    if (cond->flags & SPF_FAIRY_TOWN)
        count += !!MM_GET_EVENT_WEEK(EV_MM_WEEK_TOWN_FAIRY);

    if (cond->flags & SPF_MASKS_REGULAR)
    {
        if (comboConfig(CFG_SHARED_MASK_KEATON))
        {
            if (gMmSave.inventory.items[ITS_MM_MASK_KEATON] == ITEM_MM_MASK_KEATON) hasMaskKeaton = 1;
        }
        else
        {
            if (gMmSave.inventory.items[ITS_MM_MASK_KEATON] == ITEM_MM_MASK_KEATON) count++;
        }

        if (comboConfig(CFG_SHARED_MASK_BUNNY))
        {
            if (gMmSave.inventory.items[ITS_MM_MASK_BUNNY] == ITEM_MM_MASK_BUNNY) hasMaskBunny = 1;
        }
        else
        {
            if (gMmSave.inventory.items[ITS_MM_MASK_BUNNY] == ITEM_MM_MASK_BUNNY) count++;
        }

        if (comboConfig(CFG_SHARED_MASK_TRUTH))
        {
            if (gMmSave.inventory.items[ITS_MM_MASK_TRUTH] == ITEM_MM_MASK_TRUTH) hasMaskTruth = 1;
        }
        else
        {
            if (gMmSave.inventory.items[ITS_MM_MASK_TRUTH] == ITEM_MM_MASK_TRUTH) count++;
        }

        if (comboConfig(CFG_SHARED_MASK_BLAST))
        {
            if (gMmSave.inventory.items[ITS_MM_MASK_BLAST] == ITEM_MM_MASK_BLAST) hasMaskBlast = 1;
        }
        else
        {
            if (gMmSave.inventory.items[ITS_MM_MASK_BLAST] == ITEM_MM_MASK_BLAST) count++;
        }

        if (comboConfig(CFG_SHARED_MASK_STONE))
        {
            if (gMmSave.inventory.items[ITS_MM_MASK_STONE] == ITEM_MM_MASK_STONE) hasMaskStone = 1;
        }
        else
        {
            if (gMmSave.inventory.items[ITS_MM_MASK_STONE] == ITEM_MM_MASK_STONE) count++;
        }

        if (gMmSave.inventory.items[ITS_MM_MASK_POSTMAN] == ITEM_MM_MASK_POSTMAN) count++;
        if (gMmSave.inventory.items[ITS_MM_MASK_ALL_NIGHT] == ITEM_MM_MASK_ALL_NIGHT) count++;
        if (gMmSave.inventory.items[ITS_MM_MASK_GREAT_FAIRY] == ITEM_MM_MASK_GREAT_FAIRY) count++;
        if (gMmSave.inventory.items[ITS_MM_MASK_BREMEN] == ITEM_MM_MASK_BREMEN) count++;
        if (gMmSave.inventory.items[ITS_MM_MASK_DON_GERO] == ITEM_MM_MASK_DON_GERO) count++;
        if (gMmSave.inventory.items[ITS_MM_MASK_SCENTS] == ITEM_MM_MASK_SCENTS) count++;
        if (gMmSave.inventory.items[ITS_MM_MASK_ROMANI] == ITEM_MM_MASK_ROMANI) count++;
        if (gMmSave.inventory.items[ITS_MM_MASK_TROUPE_LEADER] == ITEM_MM_MASK_TROUPE_LEADER) count++;
        if (gMmSave.inventory.items[ITS_MM_MASK_KAFEI] == ITEM_MM_MASK_KAFEI) count++;
        if (gMmSave.inventory.items[ITS_MM_MASK_COUPLE] == ITEM_MM_MASK_COUPLE) count++;
        if (gMmSave.inventory.items[ITS_MM_MASK_KAMARO] == ITEM_MM_MASK_KAMARO) count++;
        if (gMmSave.inventory.items[ITS_MM_MASK_GIBDO] == ITEM_MM_MASK_GIBDO) count++;
        if (gMmSave.inventory.items[ITS_MM_MASK_GARO] == ITEM_MM_MASK_GARO) count++;
        if (gMmSave.inventory.items[ITS_MM_MASK_CAPTAIN] == ITEM_MM_MASK_CAPTAIN) count++;
        if (gMmSave.inventory.items[ITS_MM_MASK_GIANT] == ITEM_MM_MASK_GIANT) count++;
    }

    if (cond->flags & SPF_MASKS_TRANSFORM)
    {
        if (comboConfig(CFG_SHARED_MASK_ZORA))
        {
            if (gMmSave.inventory.items[ITS_MM_MASK_ZORA] == ITEM_MM_MASK_ZORA) hasMaskZora = 1;
        }
        else
        {
            if (gMmSave.inventory.items[ITS_MM_MASK_ZORA] == ITEM_MM_MASK_ZORA) count++;
        }

        if (comboConfig(CFG_SHARED_MASK_GORON))
        {
            if (gMmSave.inventory.items[ITS_MM_MASK_GORON] == ITEM_MM_MASK_GORON) hasMaskGoron = 1;
        }
        else
        {
            if (gMmSave.inventory.items[ITS_MM_MASK_GORON] == ITEM_MM_MASK_GORON) count++;
        }

        if (gMmSave.inventory.items[ITS_MM_MASK_DEKU] == ITEM_MM_MASK_DEKU) count++;
        if (gMmSave.inventory.items[ITS_MM_MASK_FIERCE_DEITY] == ITEM_MM_MASK_FIERCE_DEITY) count++;
    }

    if (cond->flags & SPF_MASKS_OOT)
    {
        if (comboConfig(CFG_SHARED_MASK_KEATON))
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_KEATON_MASK)) hasMaskKeaton = 1;
        }
        else
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_KEATON_MASK)) count++;
        }

        if (comboConfig(CFG_SHARED_MASK_BUNNY))
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_BUNNY_HOOD)) hasMaskBunny = 1;
        }
        else
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_BUNNY_HOOD)) count++;
        }

        if (comboConfig(CFG_SHARED_MASK_TRUTH))
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_MASK_OF_TRUTH)) hasMaskTruth = 1;
        }
        else
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_MASK_OF_TRUTH)) count++;
        }

        if (comboConfig(CFG_SHARED_MASK_ZORA))
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_ZORA_MASK)) hasMaskZora = 1;
        }
        else
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_ZORA_MASK)) count++;
        }

        if (comboConfig(CFG_SHARED_MASK_GORON))
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_GORON_MASK)) hasMaskGoron = 1;
        }
        else
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_GORON_MASK)) count++;
        }

        if (comboConfig(CFG_SHARED_MASK_BLAST))
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_MASK_BLAST)) hasMaskBlast = 1;
        }
        else
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_MASK_BLAST)) count++;
        }

        if (comboConfig(CFG_SHARED_MASK_STONE))
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_MASK_STONE)) hasMaskStone = 1;
        }
        else
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_MASK_STONE)) count++;
        }

        if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_GERUDO_MASK)) count++;
        if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_SPOOKY_MASK)) count++;
        if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_SKULL_MASK)) count++;
    }

    if (cond->flags & SPF_TRIFORCE)
    {
        count += gTriforceCount;
    }

    if (cond->flags & SPF_COIN_RED)
        count += gSharedCustomSave.coins[0];

    if (cond->flags & SPF_COIN_GREEN)
        count += gSharedCustomSave.coins[1];

    if (cond->flags & SPF_COIN_BLUE)
        count += gSharedCustomSave.coins[2];

    if (cond->flags & SPF_COIN_YELLOW)
        count += gSharedCustomSave.coins[3];

    count += hasMaskKeaton;
    count += hasMaskBunny;
    count += hasMaskTruth;
    count += hasMaskZora;
    count += hasMaskGoron;
    count += hasMaskBlast;
    count += hasMaskStone;

    return count >= cond->count;
}

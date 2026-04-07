#include <combo.h>
#include <combo/special_conds.h>

#define SPFT_STONES                  (1 <<  0)
#define SPFT_MEDALLIONS              (1 <<  1)
#define SPFT_REMAINS                 (1 <<  2)
#define SPFT_DUNGEON_REWARDS         (1 <<  3)
#define SPFT_SKULLS_GOLD             (1 <<  4)
#define SPFT_SKULLS_SWAMP            (1 <<  5)
#define SPFT_SKULLS_OCEAN            (1 <<  6)
#define SPFT_SKULLS_HOUSE            (1 <<  7)
#define SPFT_SKULLS_ALL              (1 <<  8)
#define SPFT_FAIRIES_WF              (1 <<  9)
#define SPFT_FAIRIES_SH              (1 << 10)
#define SPFT_FAIRIES_GB              (1 << 11)
#define SPFT_FAIRIES_ST              (1 << 12)
#define SPFT_FAIRIES_DUNGEON         (1 << 13)
#define SPFT_FAIRY_TOWN              (1 << 14)
#define SPFT_FAIRIES_ALL             (1 << 15)
#define SPFT_MASKS_OOT               (1 << 16)
#define SPFT_MASKS_REGULAR           (1 << 17)
#define SPFT_MASKS_TRANSFORM         (1 << 18)
#define SPFT_MASKS_MM                (1 << 19)
#define SPFT_MASKS_ALL               (1 << 20)
#define SPFT_TRIFORCE                (1 << 21)
#define SPFT_COIN_RED                (1 << 22)
#define SPFT_COIN_GREEN              (1 << 23)
#define SPFT_COIN_BLUE               (1 << 24)
#define SPFT_COIN_YELLOW             (1 << 25)
#define SPFT_COINS_ALL               (1 << 26)

static const char* kSpecialCondNames[] = {
    "open the" TEXT_COLOR_RED "Bridge",
    "reach the " TEXT_COLOR_RED "Moon",
    NULL,
    NULL,
    NULL,
};

static const char* kSpecialCondFieldsStr[] = {
    TEXT_COLOR_BLUE "Stones",
    TEXT_COLOR_BLUE "Medallions",
    TEXT_COLOR_BLUE "Remains",
    TEXT_COLOR_BLUE "Dungeon Rewards",
    TEXT_COLOR_YELLOW "Gold Tokens",
    TEXT_COLOR_YELLOW "Swamp Tokens",
    TEXT_COLOR_YELLOW "Ocean Tokens",
    TEXT_COLOR_YELLOW "Spider House Tokens",
    TEXT_COLOR_YELLOW "Skulltula Tokens",
    TEXT_COLOR_PINK "Woodfall Fairies",
    TEXT_COLOR_PINK "Snowhead Fairies",
    TEXT_COLOR_PINK "Great Bay Fairies",
    TEXT_COLOR_PINK "Stone Tower Fairies",
    TEXT_COLOR_PINK "Dungeon Fairies",
    TEXT_COLOR_PINK "Town Fairy",
    TEXT_COLOR_PINK "Stray Fairies",
    TEXT_COLOR_RED "Masks (OoT)",
    TEXT_COLOR_RED "Regular Masks",
    TEXT_COLOR_RED "Transformation Masks",
    TEXT_COLOR_RED "Masks (MM)",
    TEXT_COLOR_RED "Masks",
    TEXT_COLOR_YELLOW "Triforce Pieces",
    TEXT_COLOR_GREEN "Red Coins",
    TEXT_COLOR_GREEN "Green Coins",
    TEXT_COLOR_GREEN "Blue Coins",
    TEXT_COLOR_GREEN "Yellow Coins",
    TEXT_COLOR_GREEN "Coins"
};

static void SpecialConds_TextBitsRule(u32* bits, u32 mask, u32 maskRaw, u32 flag)
{
    if (mask && ((*bits & mask) == mask))
    {
        *bits &= ~maskRaw;
        *bits |= flag;
    }
}

static void SpecialConds_TextBitsRuleOotMm(u32* bits, u32 mmFlags, u32 ootFlags, u32 flag)
{
    u32 mask;
    u32 maskRaw;

    maskRaw = ootFlags | mmFlags;
    mask = maskRaw;

    if (Config_Flag(CFG_ONLY_OOT))
        mask &= ~mmFlags;
    if (Config_Flag(CFG_ONLY_MM))
        mask &= ~ootFlags;

    SpecialConds_TextBitsRule(bits, mask, maskRaw, flag);
}

static void SpecialConds_GetTextBits(int special, u32* out)
{
    u32 condBits;
    u32 textBits;
    u32 coinsMask;

    condBits = gComboConfig.special[special].flags;
    textBits = 0;

    /* Normal cond bits */
    if (condBits & SPF_STONES) textBits |= SPFT_STONES;
    if (condBits & SPF_MEDALLIONS) textBits |= SPFT_MEDALLIONS;
    if (condBits & SPF_REMAINS) textBits |= SPFT_REMAINS;
    if (condBits & SPF_SKULLS_GOLD) textBits |= SPFT_SKULLS_GOLD;
    if (condBits & SPF_SKULLS_SWAMP) textBits |= SPFT_SKULLS_SWAMP;
    if (condBits & SPF_SKULLS_OCEAN) textBits |= SPFT_SKULLS_OCEAN;
    if (condBits & SPF_FAIRIES_WF) textBits |= SPFT_FAIRIES_WF;
    if (condBits & SPF_FAIRIES_SH) textBits |= SPFT_FAIRIES_SH;
    if (condBits & SPF_FAIRIES_GB) textBits |= SPFT_FAIRIES_GB;
    if (condBits & SPF_FAIRIES_ST) textBits |= SPFT_FAIRIES_ST;
    if (condBits & SPF_FAIRY_TOWN) textBits |= SPFT_FAIRY_TOWN;
    if (condBits & SPF_MASKS_REGULAR) textBits |= SPFT_MASKS_REGULAR;
    if (condBits & SPF_MASKS_TRANSFORM) textBits |= SPFT_MASKS_TRANSFORM;
    if (condBits & SPF_MASKS_OOT) textBits |= SPFT_MASKS_OOT;
    if (condBits & SPF_TRIFORCE) textBits |= SPFT_TRIFORCE;
    if (condBits & SPF_COIN_RED) textBits |= SPFT_COIN_RED;
    if (condBits & SPF_COIN_GREEN) textBits |= SPFT_COIN_GREEN;
    if (condBits & SPF_COIN_BLUE) textBits |= SPFT_COIN_BLUE;
    if (condBits & SPF_COIN_YELLOW) textBits |= SPFT_COIN_YELLOW;

    SpecialConds_TextBitsRuleOotMm(&textBits, SPFT_STONES | SPFT_MEDALLIONS, SPFT_REMAINS, SPFT_DUNGEON_REWARDS);
    SpecialConds_TextBitsRuleOotMm(&textBits, SPFT_SKULLS_GOLD, SPFT_SKULLS_SWAMP | SPFT_SKULLS_OCEAN, SPFT_SKULLS_ALL);
    SpecialConds_TextBitsRuleOotMm(&textBits, 0, SPFT_SKULLS_SWAMP | SPFT_SKULLS_OCEAN, SPFT_SKULLS_HOUSE);
    SpecialConds_TextBitsRuleOotMm(&textBits, 0, SPFT_FAIRIES_WF | SPFT_FAIRIES_SH | SPFT_FAIRIES_GB | SPFT_FAIRIES_ST | SPFT_FAIRY_TOWN, SPFT_FAIRIES_ALL);
    SpecialConds_TextBitsRuleOotMm(&textBits, 0, SPFT_FAIRIES_WF | SPFT_FAIRIES_SH | SPFT_FAIRIES_GB | SPFT_FAIRIES_ST, SPFT_FAIRIES_DUNGEON);
    SpecialConds_TextBitsRuleOotMm(&textBits, SPFT_MASKS_OOT, SPFT_MASKS_TRANSFORM | SPFT_MASKS_REGULAR, SPFT_MASKS_ALL);
    SpecialConds_TextBitsRuleOotMm(&textBits, 0, SPFT_MASKS_TRANSFORM | SPFT_MASKS_REGULAR, SPFT_MASKS_MM);

    coinsMask = 0;
    if (gComboConfig.maxCoins[0])
        coinsMask |= SPFT_COIN_RED;
    if (gComboConfig.maxCoins[1])
        coinsMask |= SPFT_COIN_GREEN;
    if (gComboConfig.maxCoins[2])
        coinsMask |= SPFT_COIN_BLUE;
    if (gComboConfig.maxCoins[3])
        coinsMask |= SPFT_COIN_YELLOW;
    SpecialConds_TextBitsRule(&textBits, coinsMask, SPFT_COIN_RED | SPFT_COIN_GREEN | SPFT_COIN_BLUE | SPFT_COIN_YELLOW, SPFT_COINS_ALL);

    *out = textBits;
}

void SpecialConds_HijackTextRequirements(PlayState* play, int special)
{
    u32 textBits;
    u32 textBitsCount;
    u32 count;
    char* b;
    char* start;

    SpecialConds_GetTextBits(special, &textBits);
    textBitsCount = popcount(textBits);

#if defined(GAME_OOT)
    b = play->msgCtx.font.msgBuf;
#else
    b = play->msgCtx.font.textBuffer.schar;
#endif

    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "To ");
    comboTextAppendStr(&b, kSpecialCondNames[special]);
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, ", you must obtain " TEXT_COLOR_RED);
    comboTextAppendNum(&b, gComboConfig.special[special].count);
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, " of the following: ");

    count = 0;
    for (u32 i = 0; i < 32; ++i)
    {
        if (!(textBits & (1 << i)))
            continue;
        comboTextAppendStr(&b, kSpecialCondFieldsStr[i]);
        comboTextAppendClearColor(&b);
        if (++count < textBitsCount)        {
            if (count == textBitsCount - 1)
                comboTextAppendStr(&b, " and ");
            else
                comboTextAppendStr(&b, ", ");
        }
    }
    comboTextAppendStr(&b, "." TEXT_END);
    comboTextAutoLineBreaks(start);
}

int SpecialConds_Eval(int special)
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

    cond = &gComboConfig.special[special];
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
        count += gOotSave.info.inventory.quest.stoneEmerald;
        count += gOotSave.info.inventory.quest.stoneRuby;
        count += gOotSave.info.inventory.quest.stoneSapphire;
    }

    if (cond->flags & SPF_MEDALLIONS)
    {
        count += gOotSave.info.inventory.quest.medallionForest;
        count += gOotSave.info.inventory.quest.medallionFire;
        count += gOotSave.info.inventory.quest.medallionWater;
        count += gOotSave.info.inventory.quest.medallionSpirit;
        count += gOotSave.info.inventory.quest.medallionShadow;
        count += gOotSave.info.inventory.quest.medallionLight;
    }

    if (cond->flags & SPF_REMAINS)
    {
        count += gMmSave.info.inventory.quest.remainsOdolwa;
        count += gMmSave.info.inventory.quest.remainsGoht;
        count += gMmSave.info.inventory.quest.remainsGyorg;
        count += gMmSave.info.inventory.quest.remainsTwinmold;
    }

    if (cond->flags & SPF_SKULLS_GOLD)
        count += gOotSave.info.inventory.goldTokens;

    if (cond->flags & SPF_SKULLS_SWAMP)
        count += gMmSave.info.skullCountSwamp;

    if (cond->flags & SPF_SKULLS_OCEAN)
        count += gMmSave.info.skullCountOcean;

    if (cond->flags & SPF_FAIRIES_WF)
        count += gMmSave.info.inventory.strayFairies[0];

    if (cond->flags & SPF_FAIRIES_SH)
        count += gMmSave.info.inventory.strayFairies[1];

    if (cond->flags & SPF_FAIRIES_GB)
        count += gMmSave.info.inventory.strayFairies[2];

    if (cond->flags & SPF_FAIRIES_ST)
        count += gMmSave.info.inventory.strayFairies[3];

    if (cond->flags & SPF_FAIRY_TOWN)
        count += !!MM_GET_EVENT_WEEK(EV_MM_WEEK_TOWN_FAIRY);

    if (cond->flags & SPF_MASKS_REGULAR)
    {
        if (Config_Flag(CFG_SHARED_MASK_KEATON))
        {
            if (gMmSave.info.inventory.items[ITS_MM_MASK_KEATON] == ITEM_MM_MASK_KEATON) hasMaskKeaton = 1;
        }
        else
        {
            if (gMmSave.info.inventory.items[ITS_MM_MASK_KEATON] == ITEM_MM_MASK_KEATON) count++;
        }

        if (Config_Flag(CFG_SHARED_MASK_BUNNY))
        {
            if (gMmSave.info.inventory.items[ITS_MM_MASK_BUNNY] == ITEM_MM_MASK_BUNNY) hasMaskBunny = 1;
        }
        else
        {
            if (gMmSave.info.inventory.items[ITS_MM_MASK_BUNNY] == ITEM_MM_MASK_BUNNY) count++;
        }

        if (Config_Flag(CFG_SHARED_MASK_TRUTH))
        {
            if (gMmSave.info.inventory.items[ITS_MM_MASK_TRUTH] == ITEM_MM_MASK_TRUTH) hasMaskTruth = 1;
        }
        else
        {
            if (gMmSave.info.inventory.items[ITS_MM_MASK_TRUTH] == ITEM_MM_MASK_TRUTH) count++;
        }

        if (Config_Flag(CFG_SHARED_MASK_BLAST))
        {
            if (gMmSave.info.inventory.items[ITS_MM_MASK_BLAST] == ITEM_MM_MASK_BLAST) hasMaskBlast = 1;
        }
        else
        {
            if (gMmSave.info.inventory.items[ITS_MM_MASK_BLAST] == ITEM_MM_MASK_BLAST) count++;
        }

        if (Config_Flag(CFG_SHARED_MASK_STONE))
        {
            if (gMmSave.info.inventory.items[ITS_MM_MASK_STONE] == ITEM_MM_MASK_STONE) hasMaskStone = 1;
        }
        else
        {
            if (gMmSave.info.inventory.items[ITS_MM_MASK_STONE] == ITEM_MM_MASK_STONE) count++;
        }

        if (gMmSave.info.inventory.items[ITS_MM_MASK_POSTMAN] == ITEM_MM_MASK_POSTMAN) count++;
        if (gMmSave.info.inventory.items[ITS_MM_MASK_ALL_NIGHT] == ITEM_MM_MASK_ALL_NIGHT) count++;
        if (gMmSave.info.inventory.items[ITS_MM_MASK_GREAT_FAIRY] == ITEM_MM_MASK_GREAT_FAIRY) count++;
        if (gMmSave.info.inventory.items[ITS_MM_MASK_BREMEN] == ITEM_MM_MASK_BREMEN) count++;
        if (gMmSave.info.inventory.items[ITS_MM_MASK_DON_GERO] == ITEM_MM_MASK_DON_GERO) count++;
        if (gMmSave.info.inventory.items[ITS_MM_MASK_SCENTS] == ITEM_MM_MASK_SCENTS) count++;
        if (gMmSave.info.inventory.items[ITS_MM_MASK_ROMANI] == ITEM_MM_MASK_ROMANI) count++;
        if (gMmSave.info.inventory.items[ITS_MM_MASK_TROUPE_LEADER] == ITEM_MM_MASK_TROUPE_LEADER) count++;
        if (gMmSave.info.inventory.items[ITS_MM_MASK_KAFEI] == ITEM_MM_MASK_KAFEI) count++;
        if (gMmSave.info.inventory.items[ITS_MM_MASK_COUPLE] == ITEM_MM_MASK_COUPLE) count++;
        if (gMmSave.info.inventory.items[ITS_MM_MASK_KAMARO] == ITEM_MM_MASK_KAMARO) count++;
        if (gMmSave.info.inventory.items[ITS_MM_MASK_GIBDO] == ITEM_MM_MASK_GIBDO) count++;
        if (gMmSave.info.inventory.items[ITS_MM_MASK_GARO] == ITEM_MM_MASK_GARO) count++;
        if (gMmSave.info.inventory.items[ITS_MM_MASK_CAPTAIN] == ITEM_MM_MASK_CAPTAIN) count++;
        if (gMmSave.info.inventory.items[ITS_MM_MASK_GIANT] == ITEM_MM_MASK_GIANT) count++;
    }

    if (cond->flags & SPF_MASKS_TRANSFORM)
    {
        if (Config_Flag(CFG_SHARED_MASK_ZORA))
        {
            if (gMmSave.info.inventory.items[ITS_MM_MASK_ZORA] == ITEM_MM_MASK_ZORA) hasMaskZora = 1;
        }
        else
        {
            if (gMmSave.info.inventory.items[ITS_MM_MASK_ZORA] == ITEM_MM_MASK_ZORA) count++;
        }

        if (Config_Flag(CFG_SHARED_MASK_GORON))
        {
            if (gMmSave.info.inventory.items[ITS_MM_MASK_GORON] == ITEM_MM_MASK_GORON) hasMaskGoron = 1;
        }
        else
        {
            if (gMmSave.info.inventory.items[ITS_MM_MASK_GORON] == ITEM_MM_MASK_GORON) count++;
        }

        if (gMmSave.info.inventory.items[ITS_MM_MASK_DEKU] == ITEM_MM_MASK_DEKU) count++;
        if (gMmSave.info.inventory.items[ITS_MM_MASK_FIERCE_DEITY] == ITEM_MM_MASK_FIERCE_DEITY) count++;
    }

    if (cond->flags & SPF_MASKS_OOT)
    {
        if (Config_Flag(CFG_SHARED_MASK_KEATON))
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_KEATON_MASK)) hasMaskKeaton = 1;
        }
        else
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_KEATON_MASK)) count++;
        }

        if (Config_Flag(CFG_SHARED_MASK_BUNNY))
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_BUNNY_HOOD)) hasMaskBunny = 1;
        }
        else
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_BUNNY_HOOD)) count++;
        }

        if (Config_Flag(CFG_SHARED_MASK_TRUTH))
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_MASK_OF_TRUTH)) hasMaskTruth = 1;
        }
        else
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_MASK_OF_TRUTH)) count++;
        }

        if (Config_Flag(CFG_SHARED_MASK_ZORA))
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_ZORA_MASK)) hasMaskZora = 1;
        }
        else
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_ZORA_MASK)) count++;
        }

        if (Config_Flag(CFG_SHARED_MASK_GORON))
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_GORON_MASK)) hasMaskGoron = 1;
        }
        else
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_GORON_MASK)) count++;
        }

        if (Config_Flag(CFG_SHARED_MASK_BLAST))
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_MASK_BLAST)) hasMaskBlast = 1;
        }
        else
        {
            if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_MASK_BLAST)) count++;
        }

        if (Config_Flag(CFG_SHARED_MASK_STONE))
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

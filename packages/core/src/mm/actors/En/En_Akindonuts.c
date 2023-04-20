#include <combo.h>

#define SCRUB_SWAMP     1
#define SCRUB_MOUNTAIN  2
#define SCRUB_OCEAN     3
#define SCRUB_VALLEY    4
#define SCRUB_BOMB_BAG  5

static void EnAkindonuts_AlterMessageMountainIntro(GameState_Play* play)
{
    s16 gi;
    char* b;
    char* start;

    gi = comboOverrideEx(OV_NPC, 0, NPC_MM_SCRUB_BOMB_BAG, GI_MM_BOMB_BAG3, 0);
    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "I sell ");
    comboTextAppendItemName(&b, gi, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b,
        " but I'm focusing my marketing efforts on Gorons..."
        TEXT_BB
        "What I'd really like to do is go back home and do business where I'm surrounded by trees and grass."
        TEXT_SIGNAL TEXT_END
    );
    comboTextAutoLineBreaks(start);
}

static void EnAkindonuts_AlterMessageMountainBuy(GameState_Play* play)
{
    s16 gi;
    char* b;
    char* start;

    gi = comboOverrideEx(OV_NPC, 0, NPC_MM_SCRUB_BOMB_BAG, GI_MM_BOMB_BAG3, 0);
    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "I'll give you ");
    comboTextAppendItemName(&b, gi, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, " for 200 Rupees!" TEXT_SIGNAL TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void EnAkindonuts_DisplayTextBox(GameState_Play* play, s16 messageId, Actor* this)
{
    PlayerDisplayTextBox(play, messageId, this);

    if (messageId == 0x15f5)
        EnAkindonuts_AlterMessageMountainIntro(play);
    else if (messageId == 0x1600)
        EnAkindonuts_AlterMessageMountainBuy(play);
}

PATCH_CALL(0x80bedb70, EnAkindonuts_DisplayTextBox);

static u8 sScrubFlag;

int EnAkindonuts_HasGivenItem(Actor* this)
{
    int ret;

    ret = Actor_HasParent(this);
    if (ret)
    {
        switch (sScrubFlag)
        {
        case SCRUB_SWAMP:
            gMmExtraFlags.scrubSwamp = 1;
            break;
        case SCRUB_MOUNTAIN:
            gMmExtraFlags.scrubMountain = 1;
            break;
        case SCRUB_OCEAN:
            gMmExtraFlags.scrubOcean = 1;
            break;
        case SCRUB_VALLEY:
            gMmExtraFlags.scrubValley = 1;
            break;
        case SCRUB_BOMB_BAG:
            gMmExtraFlags.scrubBombBag = 1;
            break;
        }
        sScrubFlag = 0;
    }
    return ret;
}

PATCH_CALL(0x80bef3f4, EnAkindonuts_HasGivenItem);
PATCH_CALL(0x80bef38c, EnAkindonuts_HasGivenItem);

int EnAkindonuts_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_MM_DEED_SWAMP:
        comboRemoveTradeItem1(XITEM_MM_TRADE1_DEED_LAND);
        if (gMmExtraFlags.scrubSwamp)
            gi = GI_MM_RECOVERY_HEART;
        else
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_SCRUB_SWAMP, gi);
            sScrubFlag = SCRUB_SWAMP;
        }
        break;
    case GI_MM_DEED_MOUNTAIN:
        comboRemoveTradeItem1(XITEM_MM_TRADE1_DEED_SWAMP);
        if (gMmExtraFlags.scrubMountain)
            gi = GI_MM_RECOVERY_HEART;
        else
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_SCRUB_MOUNTAIN, gi);
            sScrubFlag = SCRUB_MOUNTAIN;
        }
        break;
    case GI_MM_DEED_OCEAN:
        comboRemoveTradeItem1(XITEM_MM_TRADE1_DEED_MOUNTAIN);
        if (gMmExtraFlags.scrubOcean)
            gi = GI_MM_RECOVERY_HEART;
        else
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_SCRUB_OCEAN, gi);
            sScrubFlag = SCRUB_OCEAN;
        }
        break;
    case GI_MM_RUPEE_GOLD:
        comboRemoveTradeItem1(XITEM_MM_TRADE1_DEED_OCEAN);
        if (gMmExtraFlags.scrubValley)
            gi = GI_MM_RECOVERY_HEART;
        else
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_SCRUB_VALLEY, gi);
            sScrubFlag = SCRUB_VALLEY;
        }
        break;
    case GI_MM_BOMB_BAG3:
        if (gMmExtraFlags.scrubBombBag)
            gi = GI_MM_RECOVERY_HEART;
        else
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_SCRUB_BOMB_BAG, gi);
            sScrubFlag = SCRUB_BOMB_BAG;
        }
        break;
    }

    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80bef3e4, EnAkindonuts_GiveItem);
PATCH_CALL(0x80bef434, EnAkindonuts_GiveItem);

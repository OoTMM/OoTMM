#include <combo.h>

#define SCRUB_SWAMP     1
#define SCRUB_MOUNTAIN  2
#define SCRUB_OCEAN     3
#define SCRUB_VALLEY    4
#define SCRUB_BOMB_BAG  5

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
        if (gMmExtraFlags.scrubSwamp)
            gi = GI_MM_RECOVERY_HEART;
        else
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_SCRUB_SWAMP, gi);
            sScrubFlag = SCRUB_SWAMP;
        }
        break;
    case GI_MM_DEED_MOUNTAIN:
        if (gMmExtraFlags.scrubMountain)
            gi = GI_MM_RECOVERY_HEART;
        else
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_SCRUB_MOUNTAIN, gi);
            sScrubFlag = SCRUB_MOUNTAIN;
        }
        break;
    case GI_MM_DEED_OCEAN:
        if (gMmExtraFlags.scrubOcean)
            gi = GI_MM_RECOVERY_HEART;
        else
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_SCRUB_OCEAN, gi);
            sScrubFlag = SCRUB_OCEAN;
        }
        break;
    case GI_MM_RUPEE_GOLD:
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

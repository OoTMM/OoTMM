#include <combo.h>
#include <combo/item.h>
#include <combo/sr.h>
#include <combo/souls.h>

int comboAddItemOot(s16 gi, int noEffect)
{
    int count;
    u16 dungeonId;

    count = 0;
    (void)dungeonId;
    switch (gi)
    {
    case GI_OOT_FISHING_POND_CHILD_FISH_2LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_3LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_4LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_5LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_6LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_7LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_8LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_9LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_10LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_11LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_12LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_13LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_14LBS:
    {
        u8 caughtListLength = gSharedCustomSave.caughtChildFishWeight[0];
        if (caughtListLength < ARRAY_SIZE(gSharedCustomSave.caughtChildFishWeight) - 1) {
            s16 pounds = gi - GI_OOT_FISHING_POND_CHILD_FISH_2LBS + 2;
            gSharedCustomSave.caughtChildFishWeight[++gSharedCustomSave.caughtChildFishWeight[0]] = pounds & 0x7F;
        }
    }
        break;
    case GI_OOT_FISHING_POND_CHILD_LOACH_14LBS:
    case GI_OOT_FISHING_POND_CHILD_LOACH_15LBS:
    case GI_OOT_FISHING_POND_CHILD_LOACH_16LBS:
    case GI_OOT_FISHING_POND_CHILD_LOACH_17LBS:
    case GI_OOT_FISHING_POND_CHILD_LOACH_18LBS:
    case GI_OOT_FISHING_POND_CHILD_LOACH_19LBS:
    {
        u8 caughtListLength = gSharedCustomSave.caughtChildFishWeight[0];
        if (caughtListLength < ARRAY_SIZE(gSharedCustomSave.caughtChildFishWeight) - 1) {
            s16 pounds = gi - GI_OOT_FISHING_POND_CHILD_LOACH_14LBS + 14;
            gSharedCustomSave.caughtChildFishWeight[++gSharedCustomSave.caughtChildFishWeight[0]] = (pounds & 0x7F) | 0x80;
        }
    }
        break;
    case GI_OOT_FISHING_POND_ADULT_FISH_4LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_5LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_6LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_7LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_8LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_9LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_10LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_11LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_12LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_13LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_14LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_15LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_16LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_17LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_18LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_19LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_20LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_21LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_22LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_23LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_24LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_25LBS:
    {
        u8 caughtListLength = gSharedCustomSave.caughtAdultFishWeight[0];
        if (caughtListLength < ARRAY_SIZE(gSharedCustomSave.caughtAdultFishWeight) - 1) {
            s16 pounds = gi - GI_OOT_FISHING_POND_ADULT_FISH_4LBS + 4;
            gSharedCustomSave.caughtAdultFishWeight[++gSharedCustomSave.caughtAdultFishWeight[0]] = pounds & 0x7F;
        }
    }
        break;
    case GI_OOT_FISHING_POND_ADULT_LOACH_29LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_30LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_31LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_32LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_33LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_34LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_35LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_36LBS:
    {
        u8 caughtListLength = gSharedCustomSave.caughtAdultFishWeight[0];
        if (caughtListLength < ARRAY_SIZE(gSharedCustomSave.caughtAdultFishWeight) - 1) {
            s16 pounds = gi - GI_OOT_FISHING_POND_ADULT_LOACH_29LBS + 29;
            gSharedCustomSave.caughtAdultFishWeight[++gSharedCustomSave.caughtAdultFishWeight[0]] = (pounds & 0x7F) | 0x80;
        }
    }
        break;
    }

    return count;
}

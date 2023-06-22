#include <combo.h>
#include <combo/item.h>

#define FLAG_ROOM_KEY           1
#define FLAG_LETTER_TO_KAFEI    2

static u8 sAnjuFlag;

void EnAn_AfterGivingItem(Actor* this)
{
    switch (sAnjuFlag)
    {
    case FLAG_ROOM_KEY:
        gMmExtraFlags2.roomKey = 1;
        break;
    case FLAG_LETTER_TO_KAFEI:
        gMmExtraFlags2.letterKafei = 1;
        break;
    }
    sAnjuFlag = 0;
}

void EnAn_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    switch (gi)
    {
    case GI_MM_ROOM_KEY:
        if (!gMmExtraFlags2.roomKey)
        {
            sAnjuFlag = FLAG_ROOM_KEY;
            npc = NPC_MM_ROOM_KEY;
        }
        else
        {
            gi = GI_MM_RECOVERY_HEART;
        }
        break;
    case GI_MM_LETTER_TO_KAFEI:
        if (!gMmExtraFlags2.letterKafei)
        {
            sAnjuFlag = FLAG_LETTER_TO_KAFEI;
            npc = NPC_MM_LETTER_TO_KAFEI;
        }
        else
        {
            gi = GI_MM_RECOVERY_HEART;
        }
        break;
    }
    comboGiveItemNpc(this, play, gi, npc, a, b);
}

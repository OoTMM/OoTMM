#include <combo.h>
#include <combo/item.h>

static void hintBeanSeller(GameState_Play* play)
{
    char* b;
    char* start;

    b = play->msgCtx.textBuffer;
    start = b;

    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, TEXT_FAST "Chomp chomp chomp..." TEXT_NL "How about buying ");
    comboTextAppendNpcReward(&b, NPC_OOT_BEAN_SELLER, GI_OOT_MAGIC_BEAN);
    comboTextAppendStr(&b, "?" TEXT_BB TEXT_FAST "It's only 60 rupees..." TEXT_NL TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "Buy" TEXT_NL "Don't buy" TEXT_END);
    comboTextAutoLineBreaks(start);
}

void EnMs_TalkedTo(Actor* this, GameState_Play* play)
{
    if (this->messageId != 0x405e)
        return;
    hintBeanSeller(play);
}

void EnMs_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gSave.inventory.beans = 1;
    comboGiveItemNpc(actor, play, gi, NPC_OOT_BEAN_SELLER, a, b);
}

PATCH_CALL(0x80aa08dc, EnMs_GiveItem);
PATCH_CALL(0x80aa0998, EnMs_GiveItem);

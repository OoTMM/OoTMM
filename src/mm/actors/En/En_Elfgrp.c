#include <combo.h>

static const s16 kGreatFairyRewards[] = {
    GI_MM_MASK_GREAT_FAIRY,
    GI_MM_MAGIC_UPGRADE,
    GI_MM_SPIN_UPGRADE,
    GI_MM_MAGIC_UPGRADE2,
    GI_MM_DEFENSE_UPGRADE,
    GI_MM_GREAT_FAIRY_SWORD,
};

static const s16 kGreatFairyNPCs[] = {
    NPC_MM_GREAT_FAIRY_TOWN,
    NPC_MM_GREAT_FAIRY_TOWN_ALT,
    NPC_MM_GREAT_FAIRY_SWAMP,
    NPC_MM_GREAT_FAIRY_MOUNTAIN,
    NPC_MM_GREAT_FAIRY_OCEAN,
    NPC_MM_GREAT_FAIRY_VALLEY,
};

static u8 EnElfgrp_GetFairyIndex(Actor* this)
{
    u8 index;

    index = (this->variable & 0xf) + 1;
    if (index == 1 && gSave.playerForm == 0x04)
        index = 0;
    return index;
}

static void EnElfgrp_GiveReward(Actor* actor, GameState_Play* play)
{
    u8      fairyIndex;
    u8      mask;
    u16     gi;

    fairyIndex = EnElfgrp_GetFairyIndex(actor);
    mask = (1 << fairyIndex);

    if (gMmExtraFlags.greatFairies & mask)
        return;

    if (Actor_HasParent(actor))
    {
        gMmExtraFlags.greatFairies |= mask;
        actor->attachedA = NULL;
        /* TODO: Probably need an event flag? */
        return;
    }

    gi = comboOverride(OV_NPC, 0, kGreatFairyNPCs[fairyIndex], kGreatFairyRewards[fairyIndex]);
    GiveItem(actor, play, gi, 200.f, 200.f);
}

PATCH_FUNC(0x80a3a398, EnElfgrp_GiveReward);

static int EnElfgrp_GetFairyCount(GameState_Play* play, int type)
{
    if (type == 0 || type > 4)
        return 0;
    if (gSave.inventory.strayFairies[type - 1] >= 15)
        return 25;
    return 0;
}

PATCH_FUNC(0x80a39bd0, EnElfgrp_GetFairyCount);

static void fairyHint(GameState_Play* play, s16 itemId)
{
    char* b;
    char* start;

    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "Young one, please help us! If you bring the " TEXT_COLOR_PINK "15 Stray Fairies");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, " here, we will give you ");
    comboTextAppendItemName(&b, itemId, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, "!" TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void fairyHintTown(GameState_Play* play, s16 itemId, s16 itemId2)
{
    char* b;
    char* start;

    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "Young one, please help us! If you bring the missing " TEXT_COLOR_ORANGE "Stray Fairy");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, " here, we will give you ");
    comboTextAppendItemName(&b, itemId, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, " and ");
    comboTextAppendItemName(&b, itemId2, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, "!" TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void EnElfgrp_DisplayTextBox(GameState_Play* play, s16 messageId, Actor* this)
{
    s16 gi;
    s16 itemId;
    s16 gi2;
    s16 itemId2;
    u8 fairyIndex;

    fairyIndex = EnElfgrp_GetFairyIndex(this);
    PlayerDisplayTextBox(play, messageId, this);

    if (fairyIndex <= 1)
    {
        gi = comboOverrideEx(OV_NPC, 0, kGreatFairyNPCs[0], kGreatFairyRewards[0], OVF_NO_PROGRESSIVE);
        gi2 = comboOverrideEx(OV_NPC, 0, kGreatFairyNPCs[1], kGreatFairyRewards[1], OVF_NO_PROGRESSIVE);
        itemId = comboItemFromGI(gi);
        itemId2 = comboItemFromGI(gi2);
        fairyHintTown(play, itemId, itemId2);
        return;
    }

    /* Hint */
    gi = comboOverrideEx(OV_NPC, 0, kGreatFairyNPCs[fairyIndex], kGreatFairyRewards[fairyIndex], OVF_NO_PROGRESSIVE);
    itemId = comboItemFromGI(gi);
    fairyHint(play, itemId);
}

PATCH_CALL(0x80a3a9f0, EnElfgrp_DisplayTextBox);

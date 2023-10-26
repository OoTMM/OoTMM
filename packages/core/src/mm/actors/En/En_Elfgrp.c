#include <combo.h>
#include <combo/item.h>

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

static void EnElfgrp_ItemQuery(ComboItemQuery* q, int index)
{
    bzero(q, sizeof(*q));

    q->ovType = OV_NPC;
    q->gi = kGreatFairyRewards[index];
    q->id = kGreatFairyNPCs[index];
}

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
    ComboItemQuery q;
    u8 fairyIndex;
    u8 mask;

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

    EnElfgrp_ItemQuery(&q, fairyIndex);
    comboGiveItem(actor, play, &q, 200.f, 200.f);
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

static void fairyHint(GameState_Play* play, int index)
{
    ComboItemQuery q;
    char* b;
    char* start;

    EnElfgrp_ItemQuery(&q, index);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "Young one, please help us! If you bring the " TEXT_COLOR_PINK "15 Stray Fairies");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, " here, we will give you ");
    comboTextAppendItemNameQueryEx(&b, &q, TF_PREPOS | TF_PROGRESSIVE, gComboData.staticHintsImportance[9 + (index - 2)]);
    comboTextAppendStr(&b, "!" TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void fairyHintTown(GameState_Play* play)
{
    ComboItemQuery q1;
    ComboItemQuery q2;
    char* b;
    char* start;

    EnElfgrp_ItemQuery(&q1, 0);
    EnElfgrp_ItemQuery(&q2, 1);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "Young one, please help us! If you bring the missing " TEXT_COLOR_ORANGE "Stray Fairy");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, " here, we will give you ");
    comboTextAppendItemNameQueryEx(&b, &q1, TF_PREPOS | TF_PROGRESSIVE, gComboData.staticHintsImportance[18]);
    comboTextAppendStr(&b, " and ");
    comboTextAppendItemNameQueryEx(&b, &q2, TF_PREPOS | TF_PROGRESSIVE, gComboData.staticHintsImportance[19]);
    comboTextAppendStr(&b, "!" TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void EnElfgrp_DisplayTextBox(GameState_Play* play, s16 messageId, Actor* this)
{
    u8 fairyIndex;

    fairyIndex = EnElfgrp_GetFairyIndex(this);
    PlayerDisplayTextBox(play, messageId, this);

    if (fairyIndex <= 1)
        fairyHintTown(play);
    else
        fairyHint(play, fairyIndex);
}

PATCH_CALL(0x80a3a9f0, EnElfgrp_DisplayTextBox);

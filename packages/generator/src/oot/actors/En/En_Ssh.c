#include <combo.h>
#include <combo/item.h>
#include <combo/config.h>

static const s16 kNPCs[] = {
    NPC_OOT_GS_100,
    NPC_OOT_GS_10,
    NPC_OOT_GS_20,
    NPC_OOT_GS_30,
    NPC_OOT_GS_40,
    NPC_OOT_GS_50,
};

static void EnSsh_ItemQuery(ComboItemQuery* q, int index, int flags)
{
    bzero(q, sizeof(*q));

    q->ovType = OV_NPC;
    q->gi = GI_OOT_RUPEE_BLUE; /* Dummy */
    q->id = kNPCs[index];
    q->ovFlags = flags;
}

static void EnSsh_Hint(PlayState* play, int index)
{
    ComboItemQuery q;
    char* b;
    int flags;

    flags = 0;
    EnSsh_ItemQuery(&q, index, flags);
    b = play->msgCtx.font.msgBuf;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b,
        "Yeaaarrgh! I'm cursed!! Please save me by destroying " TEXT_COLOR_RED
    );
    comboTextAppendNum(&b, index == 0 ? 100 : index * 10);
    comboTextAppendStr(&b,
        " Spiders of the Curse" TEXT_CZ " and I will give you "
    );
    comboTextAppendItemNameQueryEx(&b, &q, TF_PREPOS | TF_PROGRESSIVE, gComboConfig.staticHintsImportance[index]);
    comboTextAppendStr(&b, TEXT_CZ "." TEXT_END);
    comboTextAutoLineBreaks(play->msgCtx.font.msgBuf);
}

void EnSsh_TalkedTo(Actor* this, PlayState* play)
{
    EnSsh_Hint(play, this->params);
}

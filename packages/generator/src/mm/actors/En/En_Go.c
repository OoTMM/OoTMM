#include <combo.h>
#include <combo/item.h>
#include <combo/config.h>
#include "combo/inventory.h"

void EnGo_AfterGivingItem(Actor* actor)
{
    gMmExtraFlags.powderKeg = 1;
}

static Actor* sEnGoRepairActor;

#define ENGO_SCRIPT_U16(x) (((u16)(x) >> 8) & 0xff), ((u16)(x) & 0xff)

enum
{
    ENGO_MSG_CHECK_CHOICE   = 0x05,
    ENGO_MSG_CHECK_RUPEES   = 0x08,
    ENGO_MSG_AWAIT_TEXT     = 0x0c,
    ENGO_MSG_BEGIN_TEXT     = 0x0e,
    ENGO_MSG_DONE           = 0x10,
    ENGO_MSG_CHANGE_RUPEES  = 0x14,
    ENGO_MSG_PLAY_DECIDE    = 0x30,
    ENGO_MSG_PLAY_CANCEL    = 0x31,
    ENGO_MSG_PLAY_ERROR     = 0x32,
};

enum
{
    ENGO_REPAIR_AFTER_CHOICE = 0x0b,
    ENGO_REPAIR_AFTER_RUPEES = 0x10,
    ENGO_REPAIR_SUCCESS      = 0x16,
    ENGO_REPAIR_POWDER_KEGS  = 0x1f,
};

#define ENGO_KEGS (ENGO_REPAIR_POWDER_KEGS - ENGO_REPAIR_AFTER_CHOICE)
#define ENGO_REPAIR_PURCHASE (ENGO_REPAIR_SUCCESS - ENGO_REPAIR_AFTER_RUPEES)

static const u8 sEnGoRepairPrefix[] = {
    ENGO_MSG_BEGIN_TEXT, ENGO_SCRIPT_U16(0x0e52),
    ENGO_MSG_AWAIT_TEXT,
    ENGO_MSG_CHECK_CHOICE,
        ENGO_SCRIPT_U16(0),
        ENGO_SCRIPT_U16(ENGO_KEGS),
        ENGO_SCRIPT_U16(ENGO_KEGS),
    ENGO_MSG_CHECK_RUPEES,
        ENGO_SCRIPT_U16(200),
        ENGO_SCRIPT_U16(ENGO_REPAIR_PURCHASE),
    ENGO_MSG_PLAY_ERROR,
    ENGO_MSG_BEGIN_TEXT, ENGO_SCRIPT_U16(0x0e51),
    ENGO_MSG_AWAIT_TEXT,
    ENGO_MSG_DONE,
    ENGO_MSG_PLAY_DECIDE,
    ENGO_MSG_CHANGE_RUPEES, ENGO_SCRIPT_U16(-200),
    ENGO_MSG_BEGIN_TEXT, ENGO_SCRIPT_U16(0x0e50),
    ENGO_MSG_AWAIT_TEXT,
    ENGO_MSG_DONE,
    ENGO_MSG_PLAY_CANCEL,
};

static u8 sEnGoRepairScript[
    sizeof(sEnGoRepairPrefix) + 0xCE
];

static int EnGo_HasBrokenGiantsKnife(void)
{
    return
        gSharedCustomSave.mm.swords.giantSword == 1 &&
        MmSword_GetGiantsKnifeHealth() == 0;
}

void* EnGo_SelectMedigoronScript(Actor* actor, void* originalScript, s32* scriptPos)
{
    void* medigoronScript;

    medigoronScript = actorAddr(ACTOR_EN_GO, 0x80a16280);
    if (originalScript != medigoronScript)
    {
        if (sEnGoRepairActor == actor)
            sEnGoRepairActor = NULL;

        return originalScript;
    }
    if (*scriptPos == 0)
    {
        sEnGoRepairActor = NULL;
        if (EnGo_HasBrokenGiantsKnife())
        {
            memcpy(sEnGoRepairScript, sEnGoRepairPrefix, sizeof(sEnGoRepairPrefix));
            memcpy(sEnGoRepairScript + sizeof(sEnGoRepairPrefix), originalScript, 0xCE);

            sEnGoRepairActor = actor;
        }
    }
    if (sEnGoRepairActor == actor)
        return sEnGoRepairScript;

    return originalScript;
}

static void EnGo_KnifeRepairOffer(PlayState* play)
{
    char* b;

    b = play->msgCtx.font.textBuffer.schar;

    comboTextAppendHeader(&b);

    comboTextAppendStr(&b,
        "Oh! Your " TEXT_COLOR_RED "Giant's Knife");

    comboTextAppendClearColor(&b);

    comboTextAppendStr(&b,
        " is broken!" TEXT_NL
        "I can repair it for " TEXT_COLOR_RED);

    comboTextAppendNum(&b, 200);
    comboTextAppendStr(&b, " Rupees");

    comboTextAppendClearColor(&b);

    comboTextAppendStr(&b,
        "." TEXT_BOX_BREAK_2
        "Or are you interested in my" TEXT_NL
        TEXT_COLOR_RED "Powder Kegs");

    comboTextAppendClearColor(&b);

    comboTextAppendStr(&b,
        "?" TEXT_NL
        TEXT_CHOICE2 TEXT_COLOR_GREEN
        "Repair knife" TEXT_NL
        "Powder Kegs" TEXT_END);
}

static void EnGo_KnifeRepairNoMoney(PlayState* play)
{
    char* b;

    b = play->msgCtx.font.textBuffer.schar;

    comboTextAppendHeader(&b);

    comboTextAppendStr(&b,
        "Sorry, but you don't have" TEXT_NL
        "enough " TEXT_COLOR_RED "Rupees");

    comboTextAppendClearColor(&b);

    comboTextAppendStr(&b,
        "!" TEXT_NL
        "Come back with " TEXT_COLOR_RED);

    comboTextAppendNum(&b, 200);
    comboTextAppendStr(&b, " Rupees");

    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, "." TEXT_END);
}

static void EnGo_KnifeRepairSuccess(PlayState* play)
{
    char* b;
    char* start;

    b = play->msgCtx.font.textBuffer.schar;

    comboTextAppendHeader(&b);
    start = b;

    comboTextAppendStr(&b,
        "There you go!" TEXT_NL
        "Your " TEXT_COLOR_RED "Giant's Knife");

    comboTextAppendClearColor(&b);

    comboTextAppendStr(&b,
        " is as good as " TEXT_NL
        "new! Try not to break it again!"
        TEXT_END);

    comboTextAutoLineBreaks(start);
}

static void EnGo_RepairGiantsKnife(PlayState* play)
{
    if (Config_Flag(CFG_SHARED_GORON_SWORDS))
    {
        gOotSave.info.playerData.swordHealth = gComboConfig.giantsKnifeDurabilityOot;
        gOotSave.info.inventory.equipment.swords &= ~EQ_OOT_SWORD_KNIFE_BROKEN;
        if (gOotSave.info.equips.buttonItems[0] == ITEM_OOT_SWORD_KNIFE_BROKEN)
        {
            gOotSave.info.equips.buttonItems[0] = ITEM_OOT_SWORD_KNIFE_BIGGORON;
        }
        if (gOotSave.info.childEquips.buttonItems[0] == ITEM_OOT_SWORD_KNIFE_BROKEN)
        {
            gOotSave.info.childEquips.buttonItems[0] = ITEM_OOT_SWORD_KNIFE_BIGGORON;
        }
        if (gOotSave.info.adultEquips.buttonItems[0] == ITEM_OOT_SWORD_KNIFE_BROKEN)
        {
            gOotSave.info.adultEquips.buttonItems[0] = ITEM_OOT_SWORD_KNIFE_BIGGORON;
        }
    }
    else
    {
        MmSword_SetGiantsKnifeHealth(gComboConfig.giantsKnifeDurabilityMm);
    }
    Interface_LoadItemIconImpl(play, EQUIP_SLOT_B);
}

static void EnGo_ItemQuery(ComboItemQuery* q)
{
    bzero(q, sizeof(*q));
    if (!gMmExtraFlags.powderKeg)
    {
        q->ovType = OV_NPC;
        q->gi = GI_MM_POWDER_KEG;
        q->id = NPC_MM_GORON_KEG;
    }
    else
    {
        q->ovType = OV_NONE;
        q->gi = (gMmSave.info.inventory.items[ITS_MM_KEG] == ITEM_MM_POWDER_KEG) ? GI_MM_POWDER_KEG : GI_MM_RECOVERY_HEART;
    }
}

void EnGo_GiveItem(Actor* this, PlayState* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    EnGo_ItemQuery(&q);
    comboGiveItem(this, play, &q, a, b);
}

static void powderKegHint(PlayState* play)
{
    ComboItemQuery q;
    char* b;
    char* start;

    EnGo_ItemQuery(&q);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "If you pass my test, I will give you ");
    comboTextAppendItemNameQueryEx(&b, &q, TF_PREPOS | TF_PROGRESSIVE, gComboConfig.staticHintsImportance[14]);
    comboTextAppendStr(&b, "!" TEXT_SIGNAL TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void strengthText(PlayState* play)
{
    if (!Config_Flag(CFG_MM_KEG_STRENGTH_3)) return;

    char* b;

    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "I'm the Goron who sells " TEXT_COLOR_RED "Powder" TEXT_NL "Kegs");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, ", the most famous product of" TEXT_NL);
    comboTextAppendStr(&b, "the Gorons." TEXT_NL TEXT_BOX_BREAK_2);
    comboTextAppendStr(&b, "But the rules say I can't sell" TEXT_NL);
    comboTextAppendStr(&b, TEXT_COLOR_RED "Powder Kegs");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, " to anyone who isn't" TEXT_NL);
    comboTextAppendStr(&b, TEXT_COLOR_RED "strong enough");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, " to carry them. " TEXT_SFX("\x39\x1c") "Sorry." TEXT_END);
}

static void strengthTextRepeat(PlayState* play)
{
    if (!Config_Flag(CFG_MM_KEG_STRENGTH_3)) return;

    char* b;

    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "The rules say I can't sell " TEXT_COLOR_RED "Powder" TEXT_NL);
    comboTextAppendStr(&b, "Kegs");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, " to anyone who isn't " TEXT_COLOR_RED "strong" TEXT_NL);
    comboTextAppendStr(&b, "enough");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, " to carry them. " TEXT_SFX("\x39\x1c") "Sorry." TEXT_END);
}

void EnGo_AfterTextBox(Actor* this, PlayState* play, s16 messageId)
{
    if (sEnGoRepairActor == this)
    {
        switch (messageId)
        {
        case 0x0e52:
            EnGo_KnifeRepairOffer(play);
            return;

        case 0x0e51:
            EnGo_KnifeRepairNoMoney(play);
            return;

        case 0x0e50:
            EnGo_RepairGiantsKnife(play);
            EnGo_KnifeRepairSuccess(play);
            return;
        }
    }
    switch (messageId)
    {
    case 0x0c81:
        powderKegHint(play);
        break;

    case 0x0c8e:
        strengthText(play);
        break;

    case 0x0c8f:
        strengthTextRepeat(play);
        break;
    }
}

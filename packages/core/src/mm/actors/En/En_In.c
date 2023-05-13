#include <combo.h>
#include <combo/item.h>

static u8 sIsMask;

static s16 EnIn_MilkGI(int flags)
{
    s16 gi;

    gi = comboOverrideEx(OV_NPC, 0, NPC_MM_GORMAN_MILK, GI_MM_MILK, flags);
    if (gMmExtraFlags2.gormanMilk)
        gi = comboRenewable(gi, GI_MM_RECOVERY_HEART);
    return gi;
}

static int EnIn_CanBuy(void)
{
    int ret;
    s16 gi;

    gi = EnIn_MilkGI(0);
    ret = comboItemPrecond(gi, 0);
    if (ret == SC_OK || ret == SC_OK_NOCUTSCENE)
        return 1;
    return 0;
}

PATCH_CALL(0x808f47cc, EnIn_CanBuy);
PATCH_CALL(0x808f4cb0, EnIn_CanBuy);
PATCH_CALL(0x808f5030, EnIn_CanBuy);
PATCH_CALL(0x808f53f0, EnIn_CanBuy);

static void EnIn_DisplayBuy(GameState_Play* play)
{
    char* b;
    char* start;
    s16 gi;

    gi = EnIn_MilkGI(0);

    b = play->textBuffer;
    comboTextAppendShopHeader(&b, 50);
    start = b;

    comboTextAppendStr(&b, "50 Rupees will do ya for ");
    comboTextAppendItemName(&b, gi, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, "!" TEXT_NL TEXT_CHOICE2 "I'll buy it" TEXT_NL "No thanks" TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void EnIn_DisplayTextBox3(GameState_Play* play, Actor* this, s16 textId)
{
    /* Display the original textbox */
    DisplayTextBox3(play, this, textId);

    /* Overrides */
    switch (textId)
    {
    case 0x3466:
    case 0x3490:
        EnIn_DisplayBuy(play);
        break;
    case 0x3469:
    case 0x347f:
        comboTextMessageCantBuy(play, TF_SIGNAL);
        break;
    }
}

PATCH_CALL(0x808f4210, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4230, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4250, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4318, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f434c, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4394, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f43c4, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4470, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4544, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f456c, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f45f0, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f460c, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4658, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4674, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4690, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f46c4, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f46e0, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4744, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4760, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f477c, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f483c, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f485c, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f487c, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f48b0, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f48cc, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4948, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4964, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4bd4, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4c74, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4d00, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4d1c, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4e50, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4e7c, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4ed8, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4ef4, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4f94, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4fc0, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f4fdc, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f50a0, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f50c0, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f50e0, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f5270, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f5338, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f5440, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f545c, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f5498, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f54d0, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f5528, EnIn_DisplayTextBox3);
PATCH_CALL(0x808f55d4, EnIn_DisplayTextBox3);

int EnIn_HasActorGivenItem(Actor* actor)
{
    int ret;

    ret = Actor_HasParent(actor);
    if (ret)
    {
        if (sIsMask)
            gMmExtraFlags.maskGaro = 1;
        else
            gMmExtraFlags2.gormanMilk = 1;
    }
    return ret;
}

PATCH_CALL(0x808f3b50, EnIn_HasActorGivenItem);
PATCH_CALL(0x808f3c50, EnIn_HasActorGivenItem);
PATCH_CALL(0x808f3d50, EnIn_HasActorGivenItem);

static void EnIn_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_MM_MASK_GARO:
        sIsMask = 1;
        if (gMmExtraFlags.maskGaro)
            gi = GI_MM_RECOVERY_HEART;
        else
            gi = comboOverride(OV_NPC, 0, NPC_MM_MASK_GARO, GI_MM_MASK_GARO);
        break;
    case GI_MM_MILK:
        sIsMask = 0;
        gi = EnIn_MilkGI(OVF_PROGRESSIVE);
        break;
    }

    GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x808f3bbc, EnIn_GiveItem);
PATCH_CALL(0x808f3cbc, EnIn_GiveItem);
PATCH_CALL(0x808f3dbc, EnIn_GiveItem);
PATCH_CALL(0x808f4808, EnIn_GiveItem);
PATCH_CALL(0x808f4ce8, EnIn_GiveItem);
PATCH_CALL(0x808f4da0, EnIn_GiveItem);
PATCH_CALL(0x808f506c, EnIn_GiveItem);
PATCH_CALL(0x808f5390, EnIn_GiveItem);
PATCH_CALL(0x808f5428, EnIn_GiveItem);

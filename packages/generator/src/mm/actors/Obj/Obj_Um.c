#include <combo.h>
#include <combo/player.h>

static u8 sObjUmWrongBowAgeText;

static int ObjUm_CheckBowAgeReq(void)
{
    if (Config_Flag(CFG_MM_AGE_REQ_ADULT_BOW))
        return comboIsLinkAdult();

    if (Config_Flag(CFG_MM_AGE_REQ_CHILD_BOW))
        return !comboIsLinkAdult();

    return 1;
}

static void ObjUm_ShowWrongBowAge(PlayState* play)
{
    char* b;
    char* start;

    Audio_PlaySfx(NA_SE_SY_ERROR);
    Message_ContinueTextbox(play, 0x33B5);

    b = play->msgCtx.font.textBuffer.schar;

    comboTextAppendHeader(&b);
    start = b;

    comboTextAppendStr(
        &b,
        "The milk road can be dangerous! Sorry, but I can't give you a ride unless you can use a bow."
        TEXT_SIGNAL
        TEXT_END
    );
    comboTextAutoLineBreaks(start);
}

static int ObjUm_MessageShouldAdvance_Age(PlayState* play)
{
    if (!Message_ShouldAdvance(play))
        return 0;
    if (sObjUmWrongBowAgeText)
    {
        Message_CloseTextbox(play);
        sObjUmWrongBowAgeText = 0;
        return 0;
    }
    if ((play->msgCtx.currentTextId == 0x33B4 || play->msgCtx.currentTextId == 0x33CF) && play->msgCtx.choiceIndex == 0 && !ObjUm_CheckBowAgeReq())
    {
        ObjUm_ShowWrongBowAge(play);
        sObjUmWrongBowAgeText = 1;
        return 0;
    }
    return 1;
}

PATCH_CALL(0x80b7979c, ObjUm_MessageShouldAdvance_Age);
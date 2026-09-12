#include <combo.h>
#include <combo/player.h>
#include <combo/item.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x33c) = (h); } while (0)

static void EnMa4_HandleLearnSongEpona(Actor* this, PlayState* play)
{
    Player* link;
    void* handler;

    link = GET_PLAYER(play);
    if (link->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM)
        return;

    if (Actor_HasParentZ(this))
    {
        this->parent = NULL;
        gMmExtraFlags.songEpona = 1;
    }

    if (gMmExtraFlags.songEpona)
    {
        handler = actorAddr(ACTOR_EN_MA4, 0x80abe560);
        SET_HANDLER(this, handler);
        return;
    }

    comboGiveItemNpc(this, play, GI_MM_SONG_EPONA, NPC_MM_SONG_EPONA, 10000.f, 5000.f);
}

PATCH_FUNC(0x80abf4a8, EnMa4_HandleLearnSongEpona);

static int EnMa4_CheckBowAgeReq(void)
{
    if (Config_Flag(CFG_MM_AGE_REQ_ADULT_BOW))
        return comboIsLinkAdult();
    if (Config_Flag(CFG_MM_AGE_REQ_CHILD_BOW))
        return !comboIsLinkAdult();
    return 1;
}

static void EnMa4_ShowWrongBowAge(PlayState* play)
{
    char* b;
    char* start;

    Audio_PlaySfx(NA_SE_SY_ERROR);
    Message_ContinueTextbox(play, 0x3357);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(
        &b,
        "Grasshopper! You can't practice if you can't use a bow!"
        TEXT_END
    );
    comboTextAutoLineBreaks(start);
}

static int EnMa4_MessageShouldAdvance_Age(PlayState* play)
{
    if (!Message_ShouldAdvance(play))
        return 0;

    if (!EnMa4_CheckBowAgeReq())
    {
        if (play->msgCtx.currentTextId == 0x3356 && play->msgCtx.choiceIndex == 0)
        {
            EnMa4_ShowWrongBowAge(play);
            return 0;
        }
        if (play->msgCtx.currentTextId == 0x334A || play->msgCtx.currentTextId == 0x2390)
        {
            EnMa4_ShowWrongBowAge(play);
            return 0;
        }
    }

    return 1;
}

PATCH_CALL(0x80abe6e0, EnMa4_MessageShouldAdvance_Age);
PATCH_CALL(0x80abeb8c, EnMa4_MessageShouldAdvance_Age);
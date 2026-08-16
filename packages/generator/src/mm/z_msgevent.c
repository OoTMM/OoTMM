#include <combo.h>

s32 MsgEvent_CheckStrength3(Actor* actor, PlayState* play, u8** script, MsgScriptCallback callback, s32* endScript)
{
    MsgScriptCmdCheckStrength3* cmd = (MsgScriptCmdCheckStrength3*)*script;
    s16 skip = SCRIPT_PACK_16(cmd->offsetH, cmd->offsetL);

    if (Player_HasStrength(3))
    {
        *script += skip;
    }

    return false;
}

typedef struct
{
    MsgScriptCmdBase base;
    u8 rupeesH;
    u8 rupeesL;
    u8 offsetH;
    u8 offsetL;
} MsgScriptCmdKoumeBowAgeRupees;

static int Koume_CheckBowAgeReq(void)
{
    if (Config_Flag(CFG_MM_AGE_REQ_ADULT_BOW))
        return comboIsLinkAdult();
    if (Config_Flag(CFG_MM_AGE_REQ_CHILD_BOW))
        return !comboIsLinkAdult();
    return 1;
}

static void Koume_ShowWrongBowAge(PlayState* play)
{
    char* b;
    char* start;
    Audio_PlaySfx(NA_SE_SY_ERROR);
    Message_ContinueTextbox(play, 0x0875);
    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(
        &b,
        "Sorry, but you can't play if you can't use a bow!"
        TEXT_END
    );
    comboTextAutoLineBreaks(start);
}

static s32 Koume_HandleWrongBowAge(
    PlayState* play,
    s32* endScript)
{
    switch (Message_GetState(&play->msgCtx))
    {
        case TEXT_STATE_CHOICE:
            Koume_ShowWrongBowAge(play);
            return true;
        case TEXT_STATE_DONE:
            if (Message_ShouldAdvance(play))
            {
                Message_CloseTextbox(play);
                *endScript = true;
            }
            return true;
        case TEXT_STATE_CLOSING:
            *endScript = true;
            return true;

        default:
            return true;
    }
}

static s32 MsgEvent_KoumeBowAgeFree(
    Actor* actor,
    PlayState* play,
    u8** script,
    MsgScriptCallback callback,
    s32* endScript)
{
    (void)actor;
    (void)script;
    (void)callback;
    if (!Koume_CheckBowAgeReq())
        return Koume_HandleWrongBowAge(play, endScript);
    Audio_PlaySfx_MessageDecide();
    return false;
}

static s32 MsgEvent_KoumeBowAgeRupees(
    Actor* actor,
    PlayState* play,
    u8** script,
    MsgScriptCallback callback,
    s32* endScript)
{
    MsgScriptCmdKoumeBowAgeRupees* cmd;
    s16 rupees;
    s16 skip;
    (void)actor;
    (void)callback;
    if (!Koume_CheckBowAgeReq())
        return Koume_HandleWrongBowAge(play, endScript);

    cmd = (MsgScriptCmdKoumeBowAgeRupees*)*script;

    rupees = SCRIPT_PACK_16(
        cmd->rupeesH,
        cmd->rupeesL
    );

    skip = SCRIPT_PACK_16(
        cmd->offsetH,
        cmd->offsetL
    );
    if (gMmSave.info.playerData.rupees >= rupees)
        *script += skip;

    return false;
}

MsgScriptCmdHandler sCustomMsgScriptCmdHandlers[] =
{
    MsgEvent_CheckStrength3,
    MsgEvent_KoumeBowAgeFree,
    MsgEvent_KoumeBowAgeRupees,
};

u8 sCustomMsgScriptCmdSizes[] =
{
    sizeof(MsgScriptCmdCheckStrength3),
    sizeof(MsgScriptCmdBase),
    sizeof(MsgScriptCmdKoumeBowAgeRupees),
};

s32 MsgEvent_RunScript(Actor* actor, PlayState* play, MsgScript* script, MsgScriptCallback callback, s32* pos)
{
    u8* start;
    u8* cur;
    s32 scriptDone = false;
    s32 cmdLen;
    u8 cmdId;
    u8 customCmdId;
    s32 stop;

    start = script;
    script += *pos;

    cmdLen = 0;
    do
    {
        /* Skip data from previous command */
        script += cmdLen;

        /* Get command id */
        cmdId = *script;

        /* Get command length */
        if (cmdId < ARRAY_COUNTU(sMsgScriptCmdSizes))
        {
            cmdLen = sMsgScriptCmdSizes[cmdId];
            stop = sMsgScriptCmdHandlers[cmdId](actor, play, &script, callback, &scriptDone);
        }
        else
        {
            customCmdId = cmdId - ARRAY_COUNTU(sMsgScriptCmdSizes);
            if (customCmdId < ARRAY_COUNTU(sCustomMsgScriptCmdSizes))
            {
                cmdLen = sCustomMsgScriptCmdSizes[customCmdId];
                stop = sCustomMsgScriptCmdHandlers[customCmdId](actor, play, &script, callback, &scriptDone);
            }
            else
            {
                cmdLen = -1;
            }
        }
    } while (!stop);

    cur = script;
    if (!scriptDone)
    {
        *pos = cur - start;
    }
    else
    {
        *pos = 0;
    }
    return scriptDone;
}

PATCH_FUNC(0x8010BF58, MsgEvent_RunScript)

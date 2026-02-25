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

MsgScriptCmdHandler sCustomMsgScriptCmdHandlers[] =
{
    MsgEvent_CheckStrength3, /* MSCRIPT_CUSTOM_CMD_ID_CHECK_STRENGTH_3 */
};

u8 sCustomMsgScriptCmdSizes[] =
{
    sizeof(MsgScriptCmdCheckStrength3), /* MSCRIPT_CUSTOM_CMD_ID_CHECK_STRENGTH_3 */
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

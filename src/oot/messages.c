#include <combo.h>

static u8 sInCustomSong;

static void soaringMessage(GameState_Play* play)
{
    char* b;

    b = play->msgCtx.textBuffer;
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "Warp to " TEXT_COLOR_PINK "Clock Town" TEXT_CZ "?" TEXT_NL TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "OK" TEXT_NL "No" TEXT_END);
}

static void SetupSoaring(GameState_Play* play)
{
    if (play->interfaceCtx.restrictions.warpSongs)
    {
        PlayerDisplayTextBox(play, 0x88c, NULL);
    }
    else
    {
        PlayerDisplayTextBox(play, 0x88d, NULL);
        soaringMessage(play);
    }
}

static void HandleSoaring(GameState_Play* play)
{
    int msgState;

    msgState = Message_GetState(&play->msgCtx);
    if (msgState == 2)
    {
        /* Stop ocarina */
        play->msgCtx.ocarinaMode = 4;
        sInCustomSong = 0;

        /* Check the selected option */
        if (play->msgCtx.choice == 0 && !play->interfaceCtx.restrictions.warpSongs)
        {
            comboGameSwitch(play, -1);
        }
    }
}

void HandleMessagesWrapper(GameState_Play* play, void* unk)
{
    if (gCustomOcarinaSong)
    {
        PlaySound(0x4807);
        sInCustomSong = gCustomOcarinaSong;
        gCustomOcarinaSong = 0;
        gPlayedOcarinaSong = 0;

        switch (sInCustomSong)
        {
        case 0x01:
            SetupSoaring(play);
            break;
        }
    }
    else if (sInCustomSong)
    {
        switch (sInCustomSong)
        {
        case 0x01:
            HandleSoaring(play);
            break;
        }
    }

    HandleMessages(play, unk);
}

PATCH_CALL(0x800e0a60, HandleMessagesWrapper);

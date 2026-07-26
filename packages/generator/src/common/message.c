#include <combo.h>
#include <combo/message.h>

#if defined(GAME_OOT)
# define DUMMY_MSG 0xb4
#else
# define DUMMY_MSG 0x52
#endif

void Message_StartBlocking(PlayState* play)
{
    g.blockingMessage = 1;
    PlayerDisplayTextBox(play, DUMMY_MSG, NULL);
    Player_Freeze(play);
}

void Message_UpdateBlocking(PlayState* play)
{
    if (!g.blockingMessage)
        return;

    if (Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING)
    {
        g.blockingMessage = 0;
        Player_Unfreeze(play);
    }
    else
    {
        Player_Freeze(play);
    }
}

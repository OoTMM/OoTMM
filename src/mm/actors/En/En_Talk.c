#include <combo.h>

int EnTalk_TalkedTo(Actor* this, GameState_Play* play)
{
    static int sHintNum;
    static int sInMessage;
    int messageState;

    if ((this->variable & 0x3f) != 0x18)
        return ActorTalkedTo(this);

    if (!ActorTalkedTo(this) && !sInMessage)
        return 0;

    if (!sInMessage)
    {
        sHintNum = 0;
        sInMessage = 1;
        comboTextHijackDungeonRewardHints(play, 0);
        return 0;
    }

    messageState = Message_GetState((char*)play + 0x4908);
    if ((messageState == 5 || sHintNum == 3) && Message_ShouldAdvance(play))
    {
        sHintNum++;
        if (sHintNum == 4)
        {
            sInMessage = 0;
            return 1;
        }
        PlayerDisplayTextBox(play, 0x1c00, 0);
        comboTextHijackDungeonRewardHints(play, sHintNum);
    }
    return 0;
}

PATCH_CALL(0x80bde0a0, EnTalk_TalkedTo);

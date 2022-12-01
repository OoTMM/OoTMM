#include <combo.h>

int EnWonderTalk_TalkedTo(Actor* this, GameState_Play* play)
{
    if (ActorTalkedTo(this))
    {
        comboTextHijackDungeonRewardHints(play, 0, 3);
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80aab0ec, EnWonderTalk_TalkedTo);

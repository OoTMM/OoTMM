#include <combo.h>

int EnWonderTalk_TalkedTo(Actor* this, GameState_Play* play)
{
    if (ActorTalkedTo(this))
    {
        if ((this->variable & 0xf800) == 0x0800)
        {
            if (gSave.age == AGE_CHILD)
                comboTextHijackDungeonRewardHints(play, 0, 3);
            else
                comboTextHijackDungeonRewardHints(play, 3, 6);
        }
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80aab0ec, EnWonderTalk_TalkedTo);

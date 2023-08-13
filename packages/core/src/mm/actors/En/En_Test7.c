#include <combo.h>
#include <combo/entrance.h>

u16 EnTest7_TriggerWarp(Actor* actor, GameState_Play* play)
{
    u32 entrance;

    if (actor->variable & 0x8000)
    {
        entrance = gComboData.entrancesSong[actor->variable & 0xff] ^ MASK_FOREIGN_ENTRANCE;
        if (entrance & MASK_FOREIGN_ENTRANCE)
        {
            comboTransition(play, entrance);
            return 0;
        }

        return entrance;
    }

    gIsEntranceOverride = 1;
    if (play->sceneId == 0x4f) {
        return 0x2060;
    }
    return 0;
}

#include <combo.h>
#include <combo/entrance.h>

static void EnGe2_ThrowPlayerOut(Actor* this, GameState_Play* play)
{
    EntranceDescr entrance;
    u8* timer;

    /* Handle timer */
    timer = ((u8*)this) + 0x300;
    if (*timer)
    {
        *timer -= 1;
        return;
    }

    /* Transition */
    comboGetDungeonExit(&entrance, gComboData.dungeons[DUNGEONID_PIRATE_FORTRESS]);
    comboTransition(play, &entrance);
}

PATCH_FUNC(0x80b8bc78, EnGe2_ThrowPlayerOut);

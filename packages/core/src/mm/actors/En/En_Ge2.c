#include <combo.h>
#include <combo/entrance.h>
#include <combo/dungeon.h>

static void EnGe2_ThrowPlayerOut(Actor* this, GameState_Play* play)
{
    s32 override;
    u32 entrance;
    u8* timer;

    /* Handle timer */
    timer = ((u8*)this) + 0x300;
    if (*timer)
    {
        *timer -= 1;
        return;
    }

    /* Transition */
    if (play->sceneId == SCE_MM_PIRATE_FORTRESS_ENTRANCE)
        entrance = ENTR_MM_GREAT_BAY_FROM_PIRATE_FORTRESS;
    else
        entrance = ENTR_MM_PIRATE_FORTRESS_EXTERIOR_FROM_INTERIOR;
    override = comboEntranceOverride(entrance);

    if (override == -1 || (u32)override == entrance)
    {
        play->nextEntrance = play->setupExitList[this->variable & 0x1f];
        play->transitionTrigger = TRANS_TRIGGER_NORMAL;
        play->transitionType = 0x26;
    }
    else
    {
        comboTransition(play, (u32)override);
    }
}

PATCH_FUNC(0x80b8bc78, EnGe2_ThrowPlayerOut);

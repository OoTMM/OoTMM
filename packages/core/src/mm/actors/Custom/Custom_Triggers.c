#include <combo.h>

#define TRIGGER_SONG_HEALING 0x100

void CustomTriggers_HandleTriggerGame(Actor_CustomTriggers* this, GameState_Play* play)
{
    switch (gComboTriggersData.trigger)
    {
    case TRIGGER_SONG_HEALING:
        if (CustomTriggers_GiveItemNpc(this, play, GI_MM_SONG_HEALING, NPC_MM_SONG_HEALING))
        {
            gMmExtraFlags2.songHealing = 1;
            gComboTriggersData.trigger = 0;
        }
        break;
    default:
        break;
    }
}

void CustomTriggers_CheckTriggerGame(Actor_CustomTriggers* this, GameState_Play* play)
{
    if (!gMmExtraFlags2.songHealing && gSave.entrance == ENTR_MM_CLOCK_TOWN)
    {
        gComboTriggersData.trigger = TRIGGER_SONG_HEALING;
        return;
    }
}

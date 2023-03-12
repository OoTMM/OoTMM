#include <combo.h>

u16 EnTest7_TriggerWarp(Actor* actor, GameState_Play* play)
{
    u8 warpId;

    if (actor->variable & 0x8000)
    {
        warpId = actor->variable & 0xff;
        switch (warpId) {
            case 0: // Minuet
                comboGameSwitch(play, 0x600);
            case 1: // Bolero
                comboGameSwitch(play, 0x4f6);
            case 2: // Serenade
                comboGameSwitch(play, 0x604);
            case 3: // Requiem
                comboGameSwitch(play, 0x1f1);
            case 4: // Nocturne
                comboGameSwitch(play, 0x568);
            case 5: // Prelude
                comboGameSwitch(play, 0x5f4);
            default:
                return 0;
        }
    }

    // Displaced code
    if (play->sceneId == 0x4f) {
        return 0x2060;
    }
    // End displaced code

    return 0;
}

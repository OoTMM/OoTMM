#include <combo.h>

u16 EnTest7_TriggerWarp(Actor* actor, GameState_Play* play) {
    if (actor->variable & 0x8000) {
        u8 warpId = actor->variable & 0xFF;
        switch (warpId) {
            case 0: // Minuet
                comboGameSwitch(play, 0x600);
                return 0x8640; // Soar to Woodfall if combo switch fails, which it shouldn't
            case 1: // Bolero
                comboGameSwitch(play, 0x4F6);
                return 0xB230; // Soar to Snowhead if combo switch fails, which it shouldn't
            case 2: // Serenade
                comboGameSwitch(play, 0x604);
                return 0x6A60; // Soar to Zora Cape if combo switch fails, which it shouldn't
            case 3: // Requiem
                comboGameSwitch(play, 0x1F1);
                return 0xAA30; // Soar to Stone Tower if combo switch fails, which it shouldn't
            case 4: // Nocturne
                comboGameSwitch(play, 0x568);
                return 0x2040; // Soar to Ikana Canyon if combo switch fails, which it shouldn't
            case 5: // Prelude
                comboGameSwitch(play, 0x5F4);
                return 0xD890; // Soar to South Clock Town if combo switch fails, which it shouldn't
            default:
                return 0;
        }
    }

    // Displaced code
    if (play->sceneId == 0x4F) {
        return 0x2060;
    }
    // End displaced code

    return 0;
}

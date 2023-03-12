#include <combo.h>

static void SpawnRoomActors_Wrapper(GameState_Play* play, int id)
{
    /* Spawn the normal room actors */
    SpawnRoomActors(play, id);

    /* Spawn our custom triggers actor */
    SpawnActor(
        (char*)play + 0x1c24,
        play,
        AC_CUSTOM_TRIGGERS,
        0, 0, 0,
        0, 0, 0,
        0
    );
}

PATCH_CALL(0x80080c18, SpawnRoomActors_Wrapper);
PATCH_CALL(0x80080cd0, SpawnRoomActors_Wrapper);

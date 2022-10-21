#include <combo.h>

Actor* hookSpawnActor(void* const_1, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable)
{
    switch (actorId)
    {
    case AC_ITEM00:
#if 0
        if ((variable & 0xff) == 0x11) /* Small Key */
        {
            actorId = AC_ITEM_CUSTOM;
            rz = 0x1000 | ((variable >> 8) & 0x3f);
            variable = GI_OOT_SMALL_KEY;
        }
#endif
        break;
    }

    return SpawnActor(const_1, play, actorId, x, y, z, rx, ry, rz, variable);
}

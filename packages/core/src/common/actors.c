#include <combo.h>

void SetRoomClearHook(GameState_Play* play, u32 flag)
{
    if (!g.roomEnemyLackSoul)
        SetRoomClear(play, flag);
}

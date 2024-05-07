#include <combo.h>
#include <combo/global.h>

void SetRoomClearHook(GameState_Play* play, u32 flag)
{
    if (!g.roomEnemyLackSoul)
        SetRoomClear(play, flag);
}

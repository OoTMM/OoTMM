#include <combo.h>
#include <combo/global.h>

void SetRoomClearHook(PlayState* play, u32 flag)
{
    if (!g.roomEnemyLackSoul)
        SetRoomClear(play, flag);
}

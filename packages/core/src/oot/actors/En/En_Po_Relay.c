#include <combo.h>

static int EnPoRelay_GetHeartPieceFlag(GameState_Play* play, int flag)
{
    /* Also spawn chest */
    SetRoomClear(play, 4);
    return GetCollectibleFlag(play, flag);
}

PATCH_CALL(0x80a7d008, EnPoRelay_GetHeartPieceFlag);

#include <combo.h>
#include <combo/item.h>

int EnRd_ShouldNotDance(PlayState* play)
{
    int mask = Player_GetMask(play);
    if (mask == PLAYER_MASK_GIBDO || mask == PLAYER_MASK_CAPTAIN || mask == PLAYER_MASK_GARO)
    {
        return 0;
    }

    if (Audio_GetActiveSeqId(SEQ_PLAYER_BGM_SUB) == NA_BGM_SARIAS_SONG)
    {
        return 0;
    }

    return 1;
}

PATCH_FUNC(0x808d41fc, EnRd_ShouldNotDance)

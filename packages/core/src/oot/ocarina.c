#include <combo.h>

static OcarinaSongButtons sSongSoaring = {
    6,
    {
        OCARINA_BTN_C_DOWN,
        OCARINA_BTN_C_LEFT,
        OCARINA_BTN_C_UP,
        OCARINA_BTN_C_DOWN,
        OCARINA_BTN_C_LEFT,
        OCARINA_BTN_C_UP,
    }
};

void Ocarina_CheckCustomSongs(void)
{
    if (!gMmSave.inventory.quest.songSoaring)
        return;

    if (!comboConfig(CFG_MM_CROSS_WARP))
        return;

    if (!comboConfig(CFG_MM_CROSS_WARP_ADULT) && gSave.age == AGE_ADULT)
        return;

    comboCheckSong(&sSongSoaring, 0);
}

#include <combo.h>

s32 comboProgressiveChestItem(s32 gi)
{
#if defined(GAME_MM)
    gi ^= 0x100;
#endif

    switch (gi)
    {
    case 0xc:
    case 0x3b:
        /* Ocarina */
        if (gOotSave.inventory[7] == ITEM_OOT_NONE)
            gi = 0x3b;
        else
            gi = 0xc;
        break;
    default:
        break;
    }

#if defined(GAME_MM)
    gi ^= 0x100;
#endif

    return gi;
}

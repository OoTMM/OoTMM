#include <combo.h>

static u32 kRupeeIconColor[] = {
    0x00000000, // invisible - no wallet
    0xc8ff64ff, // green     - child wallet
    0x8282ffff, // blue      - adult wallet
    0xff6464ff, // red       - giant wallet
    0xff5affff, // purple    - colossal wallet
    0xe0e0e0ff, // silver    - bottomless wallet
};

u32 Hud_RupeeIconColor(void)
{
    if (!gOotExtraFlags.childWallet)
        return kRupeeIconColor[0];
    if(gOotExtraFlags.bottomlessWallet)
        return kRupeeIconColor[5];
    return kRupeeIconColor[gOotSave.inventory.upgrades.wallet + 1];
}


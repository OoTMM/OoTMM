#include <combo.h>

static u32 kRupeeIconColor[] = {
    0x00000000, // black     - no wallet
    0xc8ff6400, // green     - child wallet
    0x8282ff00, // blue      - adult wallet
    0xff646400, // red       - giant wallet
    0xff5aff00, // purple    - colossal wallet
    0x181a6400, // dark blue - bottomless wallet
};

u32 Hud_RupeeIconColor() {
    if (!gOotExtraFlags.childWallet)
        return kRupeeIconColor[0];
    if(gOotExtraFlags.bottomlessWallet)
        return kRupeeIconColor[5];
    return kRupeeIconColor[gOotSave.inventory.upgrades.wallet+1];
}


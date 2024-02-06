#include <combo.h>
#include <combo/item.h>

u32 gInterfaceRupeeColor;

#if defined(GAME_OOT)
# define HAS_WALLET_CHILD()         (!!(gOotExtraFlags.childWallet))
# define HAS_WALLET_BOTTOMLESS()    (!!(gOotExtraFlags.bottomlessWallet))
#endif

#if defined(GAME_MM)
# define HAS_WALLET_CHILD()         (!!(gMmExtraFlags2.childWallet))
# define HAS_WALLET_BOTTOMLESS()    (!!(gMmExtraFlags3.bottomlessWallet))
#endif

static const u32 kRupeeColor[] = {
    0x00000000, /* invisible - no wallet */
    0xc8ff6400, /* green     - child wallet */
    0x8282ff00, /* blue      - adult wallet */
    0xff646400, /* red       - giant wallet */
    0xff5aff00, /* purple    - colossal wallet */
    0xe0e0e000, /* silver    - bottomless wallet */
};

static void updateRupeeColor(void)
{
    int index;

    if (!HAS_WALLET_CHILD())
        index = 0;
    else if (HAS_WALLET_BOTTOMLESS())
        index = 5;
    else
        index = gSave.inventory.upgrades.wallet + 1;
    gInterfaceRupeeColor = kRupeeColor[index];
}

void comboWalletRefresh(void)
{
    gOotMaxRupees[0] = gOotExtraFlags.childWallet ? 99 : 0;
    gMmMaxRupees[0] = gMmExtraFlags2.childWallet ? 99 : 0;
    gOotMaxRupees[3] = gOotExtraFlags.bottomlessWallet ? 9999 : 999;
    gMmMaxRupees[3] = gMmExtraFlags3.bottomlessWallet ? 9999 : 999;

    gWalletDigits[0] = 0;
    gWalletDigits[3] = 3;

    if (HAS_WALLET_CHILD())
        gWalletDigits[0] = 2;
    if (HAS_WALLET_BOTTOMLESS())
        gWalletDigits[3] = 4;

    updateRupeeColor();
}

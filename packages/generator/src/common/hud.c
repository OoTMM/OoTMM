#include <combo.h>
#include <combo/item.h>

char gHudRupeesBuffer[4];
static const int kDivisors[] = { 1000, 100, 10, 1 };

static void rupeesText(void)
{
    char tmp[4];
    int divisor;
    int digits;
    u16 rupees;

    bzero(tmp, sizeof(tmp));
    rupees = gSave.info.playerData.rupees;
    for (int i = 0; i < 4; ++i)
    {
        divisor = kDivisors[i];
        while (rupees >= divisor)
        {
            rupees -= divisor;
            tmp[i]++;
        }
    }

    digits = gWalletDigits[gSave.info.inventory.upgrades.wallet];
    memcpy(gHudRupeesBuffer, tmp + (4 - digits), digits);
}

#if defined(GAME_MM)
extern int Picto_IsCapturing(void);
#endif

void DrawHUDWrapper(PlayState* play)
{
    //needed because picto is directly grabbing frame buffer no to fix banding so we need to hide ui during the snap
#if defined(GAME_MM)
    if (Picto_IsCapturing())
        return;
#endif
    rupeesText();
    DrawHUD(play);
}
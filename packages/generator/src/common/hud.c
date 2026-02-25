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

void DrawHUDWrapper(PlayState* play)
{
    rupeesText();
    DrawHUD(play);
}

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
    rupees = gSave.playerData.rupees;
    for (int i = 0; i < 4; ++i)
    {
        divisor = kDivisors[i];
        while (rupees >= divisor)
        {
            rupees -= divisor;
            tmp[i]++;
        }
    }

    digits = gWalletDigits[gSave.inventory.upgrades.wallet];
    memcpy(gHudRupeesBuffer, tmp + (4 - digits), digits);
}

void DrawHUDWrapper(GameState_Play* play)
{
    rupeesText();
    DrawHUD(play);
}

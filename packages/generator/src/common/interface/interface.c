#include <stdlib.h>
#include <combo.h>
#include <combo/item.h>

#if defined(GAME_OOT)
# define gRupeesDelta gSaveContext.rupeesDelta
#endif

#if defined(GAME_MM)
# define gRupeesDelta gSave.rupeesDelta
#endif

/* Original func */
void _Interface_Update(PlayState* play);

static void Interface_UpdateRupees(void)
{
    int tmp;
    int d;

    if (!gRupeesDelta)
        return;

    /* Faster if the delta is huge */
    tmp = abs(gRupeesDelta);
    if (tmp >= 5000)
        d = 1000;
    else if (tmp >= 500)
        d = 100;
    else if (tmp >= 50)
        d = 10;
    else
        d = 1;

    if (gRupeesDelta < 0)
        d = -d;

    /* Transfer rupees */
    gSave.info.playerData.rupees += d;
    gRupeesDelta -= d;

    if (gSave.info.playerData.rupees < 0)
    {
        gSave.info.playerData.rupees = 0;
        gRupeesDelta = 0;
    }

    if (gSave.info.playerData.rupees > gMaxRupees[gSave.info.inventory.upgrades.wallet])
    {
        gSave.info.playerData.rupees = gMaxRupees[gSave.info.inventory.upgrades.wallet];
        gRupeesDelta = 0;
    }

    /* Play the sound effect */
    PlaySound(0x482f);
}

void Interface_Update(PlayState* play)
{
    Interface_UpdateRupees();
    _Interface_Update(play);
}

void Interface_AfterDraw(void)
{
    PlayState* play;

    play = gPlay;
    Interface_AgonyIconDraw(play);
}

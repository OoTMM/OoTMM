#include <combo.h>

#define SPF_STONES                  (1 <<  0)
#define SPF_MEDALLIONS              (1 <<  1)
#define SPF_REMAINS                 (1 <<  2)
#define SPF_SKULLS_GOLD             (1 <<  3)
#define SPF_SKULLS_SWAMP            (1 <<  4)
#define SPF_SKULLS_OCEAN            (1 <<  5)
#define SPF_FAIRIES_WF              (1 <<  6)
#define SPF_FAIRIES_SH              (1 <<  7)
#define SPF_FAIRIES_GB              (1 <<  8)
#define SPF_FAIRIES_ST              (1 <<  9)
#define SPF_FAIRY_TOWN              (1 << 10)

int comboSpecialCond(int special)
{
    SpecialCond* cond;
    s16 count;

    cond = &gComboData.special[special];
    count = 0;

    if (cond->flags & SPF_STONES)
    {
        count += gOotSave.inventory.quest.stoneEmerald;
        count += gOotSave.inventory.quest.stoneRuby;
        count += gOotSave.inventory.quest.stoneSapphire;
    }

    if (cond->flags & SPF_MEDALLIONS)
    {
        count += gOotSave.inventory.quest.medallionForest;
        count += gOotSave.inventory.quest.medallionFire;
        count += gOotSave.inventory.quest.medallionWater;
        count += gOotSave.inventory.quest.medallionSpirit;
        count += gOotSave.inventory.quest.medallionShadow;
        count += gOotSave.inventory.quest.medallionLight;
    }

    if (cond->flags & SPF_REMAINS)
    {
        count += gMmSave.inventory.quest.remainsOdolwa;
        count += gMmSave.inventory.quest.remainsGoht;
        count += gMmSave.inventory.quest.remainsGyorg;
        count += gMmSave.inventory.quest.remainsTwinmold;
    }

    if (cond->flags & SPF_SKULLS_GOLD)
        count += gOotSave.inventory.goldTokens;

    if (cond->flags & SPF_SKULLS_SWAMP)
        count += gMmSave.skullCountSwamp;

    if (cond->flags & SPF_SKULLS_OCEAN)
        count += gMmSave.skullCountOcean;

    if (cond->flags & SPF_FAIRIES_WF)
        count += gMmSave.inventory.strayFairies[0];

    if (cond->flags & SPF_FAIRIES_SH)
        count += gMmSave.inventory.strayFairies[1];

    if (cond->flags & SPF_FAIRIES_GB)
        count += gMmSave.inventory.strayFairies[2];

    if (cond->flags & SPF_FAIRIES_ST)
        count += gMmSave.inventory.strayFairies[3];

    if (cond->flags & SPF_FAIRY_TOWN)
        count += !!MM_GET_EVENT_WEEK(EV_MM_WEEK_TOWN_FAIRY);

    return count >= cond->count;
}

#include <combo.h>
#include <combo/item.h>
#include <combo/sr.h>

static void refillMagic(int level)
{
    gOotSave.playerData.magicSize = 0;
    gSaveContext.magicFillTarget = 0x30 * level;
}

int comboAddItemEffect(GameState_Play* play, s16 gi)
{
    int count;

    count = 0;
    switch (gi)
    {
    case GI_OOT_MAGIC_UPGRADE:
        refillMagic(1);
        break;
    case GI_OOT_MAGIC_UPGRADE2:
        refillMagic(2);
        break;
    }

    return count;
}

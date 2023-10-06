#include <combo.h>
#include <combo/dungeon.h>
#include <combo/dma.h>

ALIGNED(16) ComboData gComboData;

void comboInitData(void)
{
    /* Load the data chunk */
    comboDmaLoadFile(&gComboData, COMBO_VROM_DATA);

    /* Normal keys OoT */
    g.maxKeysOot[SCE_OOT_INSIDE_DEKU_TREE] = 0;
    g.maxKeysOot[SCE_OOT_DODONGO_CAVERN] = 0;
    g.maxKeysOot[SCE_OOT_INSIDE_JABU_JABU] = 0;
    g.maxKeysOot[SCE_OOT_TEMPLE_FOREST] = 5;
    g.maxKeysOot[SCE_OOT_TEMPLE_FIRE] = 7;
    g.maxKeysOot[SCE_OOT_TEMPLE_WATER] = 5;
    g.maxKeysOot[SCE_OOT_TEMPLE_SHADOW] = 5;
    g.maxKeysOot[SCE_OOT_TEMPLE_SPIRIT] = 5;
    g.maxKeysOot[SCE_OOT_BOTTOM_OF_THE_WELL] = 3;
    g.maxKeysOot[SCE_OOT_ICE_CAVERN] = 0;
    g.maxKeysOot[SCE_OOT_THIEVES_HIDEOUT] = 4;
    g.maxKeysOot[SCE_OOT_GERUDO_TRAINING_GROUND] = 9;
    g.maxKeysOot[SCE_OOT_INSIDE_GANON_CASTLE] = 2;
    g.maxKeysOot[SCE_OOT_TREASURE_SHOP] = 0;

    /* Normal keys MM */
    g.maxKeysMm[0] = 1;
    g.maxKeysMm[1] = 3;
    g.maxKeysMm[2] = 1;
    g.maxKeysMm[3] = 4;

    /* MQ Forest */
    if (gComboData.mq & (1 << MQ_TEMPLE_FOREST))
    {
        g.maxKeysOot[SCE_OOT_TEMPLE_FOREST] = 6;
    }

    /* MQ Fire / keysanity */
    if (gComboData.mq & (1 << MQ_TEMPLE_FIRE))
    {
        g.maxKeysOot[SCE_OOT_TEMPLE_FIRE] = 5;
    }
    else if (comboConfig(CFG_SMALL_KEY_SHUFFLE))
    {
        g.maxKeysOot[SCE_OOT_TEMPLE_FIRE] = 8;
    }

    /* MQ Water */
    if (gComboData.mq & (1 << MQ_TEMPLE_WATER))
    {
        g.maxKeysOot[SCE_OOT_TEMPLE_WATER] = 2;
    }

    /* MQ Shadow */
    if (gComboData.mq & (1 << MQ_TEMPLE_SHADOW))
    {
        g.maxKeysOot[SCE_OOT_TEMPLE_SHADOW] = 6;
    }

    /* MQ Spirit */
    if (gComboData.mq & (1 << MQ_TEMPLE_SPIRIT))
    {
        g.maxKeysOot[SCE_OOT_TEMPLE_SPIRIT] = 7;
    }

    /* MQ Well */
    if (gComboData.mq & (1 << MQ_BOTTOM_OF_THE_WELL))
    {
        g.maxKeysOot[SCE_OOT_BOTTOM_OF_THE_WELL] = 2;
    }

    /* MQ GTG */
    if (gComboData.mq & (1 << MQ_GERUDO_TRAINING_GROUNDS))
    {
        g.maxKeysOot[SCE_OOT_GERUDO_TRAINING_GROUND] = 3;
    }

    /* MQ Ganon */
    if (gComboData.mq & (1 << MQ_GANON_CASTLE))
    {
        g.maxKeysOot[SCE_OOT_INSIDE_GANON_CASTLE] = 3;
    }

    if (comboConfig(CFG_OOT_CARPENTERS_NONE))
    {
        g.maxKeysOot[SCE_OOT_THIEVES_HIDEOUT] = 0;
    }
    else if (comboConfig(CFG_OOT_CARPENTERS_ONE))
    {
        g.maxKeysOot[SCE_OOT_THIEVES_HIDEOUT] = 1;
    }

    /* OoT keysy */
    if (comboConfig(CFG_OOT_NO_SMALL_KEY))
    {
        g.maxKeysOot[SCE_OOT_TEMPLE_FOREST] = 0;
        g.maxKeysOot[SCE_OOT_TEMPLE_FIRE] = 0;
        g.maxKeysOot[SCE_OOT_TEMPLE_WATER] = 0;
        g.maxKeysOot[SCE_OOT_TEMPLE_SHADOW] = 0;
        g.maxKeysOot[SCE_OOT_TEMPLE_SPIRIT] = 0;
        g.maxKeysOot[SCE_OOT_BOTTOM_OF_THE_WELL] = 0;
        g.maxKeysOot[SCE_OOT_GERUDO_TRAINING_GROUND] = 0;
    }

    /* MM keysy */
    if (comboConfig(CFG_MM_NO_SMALL_KEY))
    {
        g.maxKeysMm[0] = 0;
        g.maxKeysMm[1] = 0;
        g.maxKeysMm[2] = 0;
        g.maxKeysMm[3] = 0;
    }

    /* Chest game */
    if (comboConfig(CFG_OOT_CHEST_GAME_SHUFFLE))
        g.maxKeysOot[SCE_OOT_TREASURE_SHOP] = 6;
}

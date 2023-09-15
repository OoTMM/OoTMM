#include <combo.h>
#include <combo/csmc.h>

int csmcEnabled(void)
{
    return (comboConfig(CFG_CSMC) || (comboConfig(CFG_CSMC_AGONY) && gOotSave.inventory.quest.agonyStone));
}

int csmcEnabledExtra(void)
{
    return csmcEnabled() && comboConfig(CFG_CSMC_EXTRA);
}

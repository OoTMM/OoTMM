#include <combo.h>
#include <combo/net.h>
#include <combo/sr.h>
#include <combo/dma.h>
#include <combo/menu.h>

ComboGlobal g;

void initHeap(void);
void comboInitObjects(void);

void comboInit(void)
{
    g.delayedSwitchFlag = 0xff;

    initHeap();
    comboLoadContext();
    comboInitData();
    comboInitObjects();
    comboInitOverride();
    comboInitHints();
    comboInitEntrances();
    comboSilverRupeesInit();
    comboMenuInit();
    netInit();
}

ALIGNED(16) ComboData gComboData;

void comboInitData(void)
{
    comboDmaLoadFile(&gComboData, COMBO_VROM_DATA);
}

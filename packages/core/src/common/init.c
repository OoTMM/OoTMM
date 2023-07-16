#include <combo.h>
#include <combo/net.h>
#include <combo/sr.h>

ComboGlobal g;

void initHeap(void);
void comboInitObjects(void);

void comboInit(void)
{
    g.delayedSwitchFlag = 0xff;

    initHeap();
    comboLoadContext();
    comboInitDma();
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
    DMARomToRam(0x03fe0000 | PI_DOM1_ADDR2, &gComboData, sizeof(gComboData));
}

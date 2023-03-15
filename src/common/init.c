#include <combo.h>

void comboInitObjects(void);

void comboInit(void)
{
    comboLoadContext();
    comboInitDma();
    comboInitData();
    comboInitObjects();
    comboInitOverride();
    comboInitHints();
    comboInitEntrances();
    comboMenuInit();
}

ALIGNED(16) ComboData gComboData;

void comboInitData(void)
{
    DMARomToRam(0x03fe0000 | PI_DOM1_ADDR2, &gComboData, sizeof(gComboData));
}

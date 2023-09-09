#include <combo.h>
#include <combo/net.h>
#include <combo/sr.h>
#include <combo/dma.h>
#include <combo/menu.h>

ComboGlobal g;

void initHeap(void);
void comboInitObjects(void);

void menuInit();

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
    netInit();
    menuInit();
}


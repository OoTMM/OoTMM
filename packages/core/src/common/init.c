#include <combo.h>
#include <combo/net.h>
#include <combo/sr.h>
#include <combo/dma.h>
#include <combo/menu.h>
#include <combo/debug.h>
#include <combo/config.h>
#include <combo/global.h>
#include <combo/context.h>
#include <combo/hint.h>

ComboGlobal g;

void initHeap(void);
void AudioCustom_Init(void);
void comboInitObjects(void);

void menuInit();

void comboInit(void)
{
    g.delayedSwitchFlag = 0xff;

    initHeap();
    AudioCustom_Init();
    Context_Init();
    Config_Init();
    comboInitData();
    comboInitObjects();
    comboInitOverride();
    Hint_Init();
    comboInitEntrances();
    comboSilverRupeesInit();
    netInit();
    menuInit();
    Debug_Init();
}

#include <combo.h>
#include <combo/sr.h>
#include <combo/dma.h>
#include <combo/menu.h>
#include <combo/debug.h>
#include <combo/config.h>
#include <combo/global.h>
#include <combo/context.h>
#include <combo/hint.h>
#include <combo/ipc.h>

ComboGlobal g;

void initHeap(void);
void comboAnimationDmaInit(void);
#if defined(GAME_OOT)
void comboCollisionCapacityInit(void);
#endif
void AudioCustom_Init(void);
void comboInitObjects(void);
void Xflag_InitSystem(void);

void menuInit();

void comboInit(void)
{
    initHeap();
    comboAnimationDmaInit();
#if defined(GAME_OOT)
    comboCollisionCapacityInit();
#endif
    AudioCustom_Init();
    Context_Init();
    Config_Init();
    comboInitData();
    comboInitObjects();
    comboInitOverride();
    Xflag_InitSystem();
    Hint_Init();
    comboInitEntrances();
    SilverRupees_Init();
    IPC_Init();
    menuInit();
    Debug_Init();
}

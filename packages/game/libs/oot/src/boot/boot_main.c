#include "boot.h"
#include "stack.h"
#include "stackcheck.h"
#if PLATFORM_N64
#include "cic6105.h"
#endif
#include "z_locale.h"
#include "z64thread.h"

#include "global.h"

#pragma increment_block_number "gc-eu:0 gc-eu-mq:0 gc-jp:0 gc-jp-ce:0 gc-jp-mq:0 gc-us:0 gc-us-mq:0 ntsc-1.2:128"

OSThread sIdleThread;
STACK(sIdleThreadStack, 0x400);
StackEntry sIdleThreadInfo;

extern char _bootSegmentBssStart;
extern char _bootSegmentBssEnd;

void bootclear(void) {
    bzero(&_bootSegmentBssStart, &_bootSegmentBssEnd - &_bootSegmentBssStart);
    bzero(_bootSegmentEnd, osMemSize - OS_K0_TO_PHYSICAL(_bootSegmentEnd));
}

void bootproc(void) {
    osMemSize = osGetMemSize();
#if PLATFORM_N64
    func_80001720();
#endif
    bootclear();
    osInitialize();

    gCartHandle = osCartRomInit();
    osDriveRomInit();
#if DEBUG_FEATURES
    isPrintfInit();
#endif
    Locale_Init();

    StackCheck_Init(&sIdleThreadInfo, sIdleThreadStack, STACK_TOP(sIdleThreadStack), 0, 256, "idle");
    osCreateThread(&sIdleThread, THREAD_ID_IDLE, Idle_ThreadEntry, NULL, STACK_TOP(sIdleThreadStack),
                   THREAD_PRI_IDLE_INIT);
    osStartThread(&sIdleThread);
}

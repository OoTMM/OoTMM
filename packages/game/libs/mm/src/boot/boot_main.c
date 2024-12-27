#include "carthandle.h"
#include "CIC6105.h"
#include "idle.h"
#include "stack.h"
#include "libu64/stackcheck.h"
#include "z64thread.h"

OSThread sIdleThread;
STACK(sIdleStack, 0x400);
StackEntry sIdleStackInfo;

extern char _bootSegmentBssStart;
extern char _bootSegmentBssEnd;

void bootproc(void) {
    osMemSize = osGetMemSize();
    bzero(&_bootSegmentBssStart, &_bootSegmentBssEnd - &_bootSegmentBssStart);
    CIC6105_Init();
    osInitialize();
    osUnmapTLBAll();
    gCartHandle = osCartRomInit();
    StackCheck_Init(&sIdleStackInfo, sIdleStack, STACK_TOP(sIdleStack), 0, 0x100, "idle");
    osCreateThread(&sIdleThread, Z_THREAD_ID_IDLE, Idle_ThreadEntry, NULL, STACK_TOP(sIdleStack), Z_PRIORITY_IDLE);
    osStartThread(&sIdleThread);
}

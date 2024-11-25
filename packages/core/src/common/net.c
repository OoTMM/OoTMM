#include <combo.h>
#include <combo/net.h>
#include <combo/config.h>

#define gNetGlobal (*((NetGlobal*)(0x800001a0)))

NetContext gNetCtx;

volatile unsigned int gWaitCycles;

static void wait(void)
{
    /* Small wait */
    for (int i = 0; i < 100000; ++i)
    {
        gWaitCycles++;
        if (gWaitCycles == -1000000000)
            gWaitCycles = 0;
    }
}

ALIGNED(16) static const u8 kMultiId[16];
EXPORT_SYMBOL(MULTI_ID, kMultiId);

void netInit(void)
{
    /* Init the context */
    gNetCtx.uuid = kMultiId;
    gNetCtx.ledgerBase = 0xffffffff;

    /* Init the global struct */
    gNetGlobal.ctx = &gNetCtx;
    gNetGlobal.mutexSystem = 0;
    gNetGlobal.mutexScript = 0;
    gNetGlobal.magic = NET_MAGIC;
}

NetContext* netMutexLock(void)
{
    for (;;)
    {
        /* Preemptive lock */
        gNetGlobal.mutexSystem = 1;

        /* Check for concurrent lock */
        if (!gNetGlobal.mutexScript)
            return &gNetCtx;

        /* The mutex is alrady locked */
        gNetGlobal.mutexSystem = 0;

        /* Sleep */
        Sleep_Usec(10);
    }
}

void netMutexUnlock(void)
{
    gNetGlobal.mutexSystem = 0;
}

void netClose(void)
{
    netMutexLock();
    gNetGlobal.magic = 0;
    netMutexUnlock();
}

void netWaitCmdClear(void)
{
    for (;;)
    {
        if (gNetCtx.cmdOut.op == NET_OP_NOP)
            return;

        /* Command is not clear, need to loop */
        netMutexUnlock();
        wait();
        netMutexLock();
    }
}

void netWaitSave(void)
{
    netMutexLock();
    netWaitCmdClear();
    netMutexUnlock();
}

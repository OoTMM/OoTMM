#include "ipc.h"

static u8 sSystem;
static u8 sInitialized;

void IPC_Init(void)
{
    if (sInitialized)
        return;

    if (IPC_Init_Project64())
        sSystem = IPC_SYSTEM_PROJECT64;
    else
        sSystem = IPC_SYSTEM_NONE;

    sInitialized = 1;
}

int IPC_GetSystem(void)
{
    return sSystem;
}

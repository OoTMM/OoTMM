#include "ipc.h"

IPCState gIPC;

void IPC_Init(void)
{
    if (gIPC.isInitialized)
        return;

    if (IPC_Init_Project64())
        gIPC.systemType = IPC_SYSTEM_PROJECT64;
    else
    {
        IPC_Init_Null();
        gIPC.systemType = IPC_SYSTEM_NONE;
    }

    gIPC.isInitialized = 1;
}

int IPC_GetSystem(void)
{
    return gIPC.systemType;
}

void IPC_Refresh(void)
{
    gIPC.cbRefresh();
}

int IPC_Read(void* buf, u32 len)
{
    return gIPC.cbRead(buf, len);
}

int IPC_Write(const void* buf, u32 len)
{
    return gIPC.cbWrite(buf, len);
}

int IPC_IsConnected(void)
{
    return gIPC.isConnected;
}

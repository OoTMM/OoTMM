#include "ipc.h"

static void IPC_Null_Refresh(void)
{
}

static int IPC_Null_Read(void* buf, u32 len)
{
    return 0;
}

static int IPC_Null_Write(const void* buf, u32 len)
{
    return 0;
}

int IPC_Init_Null(void)
{
    gIPC.cbRefresh = IPC_Null_Refresh;
    gIPC.cbRead = IPC_Null_Read;
    gIPC.cbWrite = IPC_Null_Write;
    return 1;
}


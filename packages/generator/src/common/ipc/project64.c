#include "ipc.h"

#define IPC_REG_BASE        0x1fe00000
#define IPC_REG_KEY         (IPC_REG_BASE + 0x00)
#define IPC_REG_STATUS      (IPC_REG_BASE + 0x04)
#define IPC_REG_RAM_ADDR    (IPC_REG_BASE + 0x08)
#define IPC_REG_WRITE_LEN   (IPC_REG_BASE + 0x0C)
#define IPC_REG_READ_LEN    (IPC_REG_BASE + 0x10)

#define IPC_MAGIC_IN    0xae67e45b
#define IPC_MAGIC_OUT   0x64738358

#define IPC_STATUS_CONNECTED    0x01
#define IPC_STATUS_READ_READY   0x02
#define IPC_STATUS_ERROR        0x04

extern OSPiHandle* gCartHandle;

static u32 IPC_PJ64_ReadReg(u32 reg)
{
    u32 data;

    osEPiReadIo(gCartHandle, reg, &data);
    return data;
}

static u32 IPC_PJ64_WriteReg(u32 reg, u32 data)
{
    osEPiWriteIo(gCartHandle, reg, data);
    return data;
}

static u32 IPC_PJ64_ReadStatus(void)
{
    return IPC_PJ64_ReadReg(IPC_REG_STATUS);
}

static void IPC_PJ64_Refresh(void)
{
    u32 status;

    status = IPC_PJ64_ReadStatus();
    gIPC.isConnected = !!(status & IPC_STATUS_CONNECTED);
}

static int IPC_PJ64_Read(void* buf, u32 len)
{
    u32 status;

    IPC_PJ64_WriteReg(IPC_REG_RAM_ADDR, (u32)buf & 0x1fffffff);
    IPC_PJ64_WriteReg(IPC_REG_READ_LEN, len);
    len = IPC_PJ64_ReadReg(IPC_REG_READ_LEN);
    status = IPC_PJ64_ReadStatus();
    gIPC.isConnected = !!(status & IPC_STATUS_CONNECTED);
    return (int)len;
}

static int IPC_PJ64_Write(const void* buf, u32 len)
{
    u32 status;

    IPC_PJ64_WriteReg(IPC_REG_RAM_ADDR, (u32)buf & 0x1fffffff);
    IPC_PJ64_WriteReg(IPC_REG_WRITE_LEN, len);
    status = IPC_PJ64_ReadStatus();
    gIPC.isConnected = !!(status & IPC_STATUS_CONNECTED);
    if (status & IPC_STATUS_ERROR)
        return 0;
    return (int)len;
}


int IPC_Init_Project64(void)
{
    u32 tmp;

    IPC_PJ64_WriteReg(IPC_REG_KEY, IPC_MAGIC_IN);
    tmp = IPC_PJ64_ReadReg(IPC_REG_KEY);

    if (tmp != IPC_MAGIC_OUT)
        return 0;

    gIPC.cbRefresh = IPC_PJ64_Refresh;
    gIPC.cbRead = IPC_PJ64_Read;
    gIPC.cbWrite = IPC_PJ64_Write;

    tmp = IPC_PJ64_ReadStatus();
    if (tmp & IPC_STATUS_CONNECTED)
        gIPC.isConnected = 1;

    return 1;
}


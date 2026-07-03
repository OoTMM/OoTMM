#include "ipc.h"

#define IPC_REG_BASE        0x1fe00000
#define IPC_REG_KEY         (IPC_REG_BASE + 0x00)
#define IPC_REG_STATUS      (IPC_REG_BASE + 0x04)
#define IPC_REG_RAM_ADDR    (IPC_REG_BASE + 0x08)
#define IPC_REG_WRITE_LEN   (IPC_REG_BASE + 0x0C)
#define IPC_REG_READ_LEN    (IPC_REG_BASE + 0x10)

#define IPC_MAGIC_IN    0xae67e45b
#define IPC_MAGIC_OUT   0x64738358

extern OSPiHandle* gCartHandle;

static u32 IPC_ReadReg(u32 reg)
{
    u32 data;

    osEPiReadIo(gCartHandle, reg, &data);
    return data;
}

static u32 IPC_WriteReg(u32 reg, u32 data)
{
    osEPiWriteIo(gCartHandle, reg, data);
    return data;
}

int IPC_Init_Project64(void)
{
    u32 tmp;

    IPC_WriteReg(IPC_REG_KEY, IPC_MAGIC_IN);
    tmp = IPC_ReadReg(IPC_REG_KEY);

    if (tmp != IPC_MAGIC_OUT)
        return 0;

    return 1;
}

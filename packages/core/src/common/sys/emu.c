#include <sys/emu.h>

static int sSysCallCount = -1;

void _emuSysCall(u32* args, int bufSize)
{
    osWritebackDCache(args, bufSize);
    osInvalDCache(args, bufSize);
    IO_WRITE(K0_TO_K1(0x049da154), args);
}

static int _emuSysCheck(int index)
{
    if (sSysCallCount < 0)
        sSysCallCount = emuSysCount();

    return index < sSysCallCount;
}

int emuSysCount(void)
{
    ALIGNED(16) u32 buffer[1];

    buffer[0] = 0;
    _emuSysCall(buffer, sizeof(buffer));

    return buffer[0];
}

int emuSysValidIPC(void)
{
    ALIGNED(16) u32 buffer[1];

    if (!_emuSysCheck(1))
        return -1;

    buffer[0] = 1;
    _emuSysCall(buffer, sizeof(buffer));
    return buffer[0];
}

int emuSysOpenIPC(void)
{
    ALIGNED(16) u32 buffer[1];

    if (!_emuSysCheck(2))
        return -1;

    buffer[0] = 2;
    _emuSysCall(buffer, sizeof(buffer));
    return buffer[0];
}

int emuSysCloseIPC(void)
{
    ALIGNED(16) u32 buffer[1];

    if (!_emuSysCheck(3))
        return -1;

    buffer[0] = 3;
    _emuSysCall(buffer, sizeof(buffer));
    return buffer[0];
}

int emuSysSendIPC(const void* data, u32 size)
{
    ALIGNED(16) u32 buffer[3];

    if (!_emuSysCheck(4))
        return -1;

    buffer[0] = 4;
    buffer[1] = (u32)data;
    buffer[2] = size;
    osWritebackDCache((void*)data, size);
    _emuSysCall(buffer, sizeof(buffer));
    return buffer[0];
}

int emuSysRecvIPC(void* data, u32 bufSize)
{
    ALIGNED(16) u32 buffer[3];

    if (!_emuSysCheck(5))
        return -1;

    buffer[0] = 5;
    buffer[1] = (u32)data;
    buffer[2] = bufSize;
    osInvalDCache(data, bufSize);
    _emuSysCall(buffer, sizeof(buffer));
    return buffer[0];
}

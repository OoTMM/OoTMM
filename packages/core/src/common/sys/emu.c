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

int emuSysSocketOpen(int slot)
{
    ALIGNED(16) u32 buffer[2];

    if (!_emuSysCheck(1))
        return -1;

    buffer[0] = 1;
    buffer[1] = (u32)slot;
    _emuSysCall(buffer, sizeof(buffer));

    return (int)buffer[0];
}

int emuSysSocketClose(int slot)
{
    ALIGNED(16) u32 buffer[2];

    if (!_emuSysCheck(2))
        return -1;

    buffer[0] = 2;
    buffer[1] = (u32)slot;
    _emuSysCall(buffer, sizeof(buffer));

    return (int)buffer[0];
}

int emuSysSocketSend(int slot, const void* data, u32 size)
{
    ALIGNED(16) u32 buffer[4];

    if (!_emuSysCheck(3))
        return -1;

    buffer[0] = 3;
    buffer[1] = (u32)slot;
    buffer[2] = (u32)data;
    buffer[3] = size;
    _emuSysCall(buffer, sizeof(buffer));

    return (int)buffer[0];
}

int emuSysSocketRecv(int slot, void* data, u32 size)
{
    ALIGNED(16) u32 buffer[4];

    if (!_emuSysCheck(4))
        return -1;

    buffer[0] = 4;
    buffer[1] = (u32)slot;
    buffer[2] = (u32)data;
    buffer[3] = size;
    _emuSysCall(buffer, sizeof(buffer));

    return (int)buffer[0];
}

int emuSysSocketIsValid(int slot)
{
    ALIGNED(16) u32 buffer[2];

    if (!_emuSysCheck(5))
        return -1;

    buffer[0] = 5;
    buffer[1] = (u32)slot;
    _emuSysCall(buffer, sizeof(buffer));

    return (int)buffer[0];
}

int emuSysSocketOpenAsync(int slot)
{
    ALIGNED(16) u32 buffer[2];

    if (!_emuSysCheck(6))
        return -1;

    /* Not implemented yet */
    return -1;
}

#include <ultra64.h>
#include <combo/io.h>

extern OSPiHandle* gCartHandle;

u8 IO_ReadPhysU8(u32 paddr)
{
    u32 paddrAligned;
    u32 value;

    paddrAligned = paddr & ~3;
    value = IO_ReadPhysU32(paddrAligned);
    value >>= ((3 - (paddr & 3)) * 8);
    value &= 0xff;
    return (u8)value;
}

s8 IO_ReadPhysI8(u32 paddr)
{
    return (s8)IO_ReadPhysU8(paddr);
}

u16 IO_ReadPhysU16(u32 paddr)
{
    u32 paddrAligned;
    u32 value;

    paddrAligned = paddr & ~3;
    value = IO_ReadPhysU32(paddrAligned);
    value >>= ((2 - (paddr & 2)) * 8);
    value &= 0xffff;
    return (u16)value;
}

s16 IO_ReadPhysI16(u32 paddr)
{
    return (s16)IO_ReadPhysU16(paddr);
}

u32 IO_ReadPhysU32(u32 paddr)
{
    u32 tmp;

    osEPiReadIo(gCartHandle, paddr | PI_DOM1_ADDR2, &tmp);
    return tmp;
}

s32 IO_ReadPhysI32(u32 paddr)
{
    return (s32)IO_ReadPhysU32(paddr);
}

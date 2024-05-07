#include <ultra64.h>
#include <combo/io.h>

extern OSPiHandle* gCartHandle;

u8 comboReadPhysU8(u32 paddr)
{
    u32 paddrAligned;
    u32 value;

    paddrAligned = paddr & ~3;
    value = comboReadPhysU32(paddrAligned);
    value >>= ((3 - (paddr & 3)) * 8);
    value &= 0xff;
    return (u8)value;
}

s8 comboReadPhysI8(u32 paddr)
{
    return (s8)comboReadPhysU8(paddr);
}

u16 comboReadPhysU16(u32 paddr)
{
    u32 paddrAligned;
    u32 value;

    paddrAligned = paddr & ~3;
    value = comboReadPhysU32(paddrAligned);
    value >>= ((2 - (paddr & 2)) * 8);
    value &= 0xffff;
    return (u16)value;
}

s16 comboReadPhysI16(u32 paddr)
{
    return (s16)comboReadPhysU16(paddr);
}

u32 comboReadPhysU32(u32 paddr)
{
    u32 tmp;

    osEPiReadIo(gCartHandle, paddr | PI_DOM1_ADDR2, &tmp);
    return tmp;
}

s32 comboReadPhysI32(u32 paddr)
{
    return (s32)comboReadPhysU32(paddr);
}

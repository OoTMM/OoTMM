#include <combo.h>
#include <combo/util.h>

u32 popcount(u32 x)
{
    x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x & 0x0f0f0f0f) + ((x >> 4) & 0x0f0f0f0f);
    x = (x & 0x00ff00ff) + ((x >> 8) & 0x00ff00ff);
    x = (x & 0x0000ffff) + ((x >> 16) & 0x0000ffff);
    return x;
}

void* actorAddr(u16 actorId, u32 addr)
{
    ActorOvl* ovl;
    u32 offset;

    ovl = gActorOvl + actorId;
    offset = addr - ovl->vramStart;
    return (char*)ovl->data + offset;
}

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

    osEPiReadIo(__osPiHandle, paddr | PI_DOM1_ADDR2, &tmp);
    return tmp;
}

s32 comboReadPhysI32(u32 paddr)
{
    return (s32)comboReadPhysU32(paddr);
}

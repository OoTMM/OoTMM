#include <combo.h>

#define CACHELINE 0x20

void comboInvalICache(void* addr, u32 size)
{
    u32 iaddr;
    u32 iend;
    u32 count;

    iaddr = (u32)addr & ~(CACHELINE - 1);
    iend = (u32)addr + size;
    count = (iend - iaddr + (CACHELINE - 1)) / CACHELINE;

    for (u32 i = 0; i < count; ++i)
    {
        __asm__ __volatile__("cache 0x10, 0(%0)" :: "r"(iaddr));
        iaddr += CACHELINE;
    }
}

void comboInvalDCache(void* addr, u32 size)
{
    u32 daddr;
    u32 dend;
    u32 count;

    daddr = (u32)addr & ~(CACHELINE - 1);
    dend = (u32)addr + size;
    count = (dend - daddr + (CACHELINE - 1)) / CACHELINE;

    for (u32 i = 0; i < count; ++i)
    {
        __asm__ __volatile__("cache 0x15, 0(%0)" :: "r"(daddr));
        __asm__ __volatile__("cache 0x11, 0(%0)" :: "r"(daddr));
        daddr += CACHELINE;
    }
}

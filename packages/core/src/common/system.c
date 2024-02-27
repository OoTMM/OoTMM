#include <combo.h>

#define ICACHELINE 0x20
#define DCACHELINE 0x10

void comboInvalICache(void* addr, u32 size)
{
    u32 iaddr;
    u32 iend;
    u32 count;

    iaddr = (u32)addr & ~(ICACHELINE - 1);
    iend = (u32)addr + size;
    count = (iend - iaddr + (ICACHELINE - 1)) / ICACHELINE;

    for (u32 i = 0; i < count; ++i)
    {
        __asm__ __volatile__("cache 0x10, 0(%0)" :: "r"(iaddr));
        iaddr += ICACHELINE;
    }
}

void comboInvalDCache(void* addr, u32 size)
{
    u32 daddr;
    u32 dend;
    u32 count;

    daddr = (u32)addr & ~(DCACHELINE - 1);
    dend = (u32)addr + size;
    count = (dend - daddr + (DCACHELINE - 1)) / DCACHELINE;

    for (u32 i = 0; i < count; ++i)
    {
        /*__asm__ __volatile__("cache 0x15, 0(%0)" :: "r"(daddr)); */
        __asm__ __volatile__("cache 0x11, 0(%0)" :: "r"(daddr));
        daddr += DCACHELINE;
    }
}

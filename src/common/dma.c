#include <combo.h>
#include <stdint.h>

#define MM_VROM_OFFSET  0x04000000

typedef struct
{
    uint32_t vstart;
    uint32_t vend;
    uint32_t pstart;
    uint32_t pend;
}
DmaEntry;

DmaEntry kComboDmaData[0x5e6 + 0x610];

void comboDma(void* addr, uint32_t cartAddr, uint32_t size);

void comboInitDma(void)
{
    DmaEntry* e;
    comboDma(kComboDmaData, 0x7430, 0x5e6 * sizeof(DmaEntry));
    comboDma(kComboDmaData + 0x5e6, 0x1a500 | MM_BASE, 0x610 * sizeof(DmaEntry));
    for (int i = 0x5e6; i < 0x5e6 + 0x610; ++i)
    {
        e = kComboDmaData + i;

        if (e->pend == 0xffffffff)
            continue;

        e->vstart |= MM_VROM_OFFSET;
        e->vend |= MM_VROM_OFFSET;
    }
}

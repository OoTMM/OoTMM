#include <combo.h>

extern void Play_Init(void*);
extern void comboSwitchToMM(void);

void hookPlay_Init(void* a)
{
    if ((gSaveContext.entrance & 0xfffc) == 0x0530)
    {
        comboSwitchToMM();
        return;
    }

    Play_Init(a);
}

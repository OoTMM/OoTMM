#include <combo.h>

extern void Play_Init(void*);
extern void comboSwitchToOoT(void);

void hookPlay_Init(void* a)
{
    if (gSave.entranceIndex == 0xc010)
    {
        comboGameSwitch();
        return;
    }

    Play_Init(a);
}

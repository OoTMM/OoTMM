#include <combo.h>

extern void Play_Init(void*);

void hookPlay_Init(void* a)
{
    comboObjectsReset();

    Play_Init(a);

    if (gSave.entranceIndex == 0xc010)
    {
        comboGameSwitch();
        return;
    }
}

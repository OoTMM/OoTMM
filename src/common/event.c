#include <combo.h>

void comboOotSetEventChk(u16 flag)
{
    u16 index;
    u8 bits;

    index = flag / 16;
    bits = flag % 16;
    gOotSave.eventsChk[index] |= (1 << bits);
}

void comboMmSetEventWeek(u16 flag)
{
    gMmSave.weekEventReg[flag / 8] |= (1 << (flag % 8));
}

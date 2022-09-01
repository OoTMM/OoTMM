#include <combo.h>

void comboOotSetEventChk(u16 flag)
{
    u16 index;
    u8 bits;

    index = flag / 16;
    bits = flag % 16;
    gOotSave.eventsChk[index] |= (1 << bits);
}

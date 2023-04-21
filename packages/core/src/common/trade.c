#include <combo.h>

void comboToggleTrade(u8* slot, u32 flags, const u8* table, u32 tableSize)
{
    int bitPos;
    /* We need to get the bit index of the current item */

    bitPos = -1;
    for (u32 i = 0; i < tableSize; ++i)
    {
        if (*slot == table[i])
        {
            bitPos = i;
            break;
        }
    }
    if (bitPos == -1)
    {
        *slot = ITEM_NONE;
        return;
    }
    for (;;)
    {
        bitPos++;
        if (bitPos >= tableSize)
            bitPos = 0;
        if (flags & (1 << bitPos))
        {
            *slot = table[bitPos];
            break;
        }
    }
}

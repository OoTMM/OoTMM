#include <combo.h>

u32 ArmsHook_GetPlayerDisplayList()
{
    if (comboIsLinkAdult())
    {
        return 0x06019d60;
    }
    else
    {
        return 0x0601d960;
    }
}

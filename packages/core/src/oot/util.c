#include <combo.h>

void swapFarore(void)
{
    OotFaroreWind* current;
    OotFaroreWind* prev;
    OotFaroreWind tmp;

    current = &gOotSave.fw;
    prev = current - 1;

    memcpy(&tmp, current, sizeof(tmp));
    memcpy(current, prev, sizeof(tmp));
    memcpy(prev, &tmp, sizeof(tmp));
}

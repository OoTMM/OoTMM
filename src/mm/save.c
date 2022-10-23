#include <combo.h>

void Sram_AfterOpenSave(void)
{
    gSave.entranceIndex = 0xd800;
}

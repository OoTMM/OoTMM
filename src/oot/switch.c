#include <types.h>
#include <combo.h>

void comboSwitchToMM(void)
{
    comboDisableInterrupts();
    comboExportContext();
    comboDma((void*)0x80000, 0x14001000, 0x100000);
    ((EntryPoint)0x80080000)();
}

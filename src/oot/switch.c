#include <types.h>
#include <combo.h>

void comboSwitchToMM(void)
{
    gComboCtx.saveIndex = gSaveContext.fileIndex;

    comboDisableInterrupts();
    comboDma((void*)0x80000, 0x10001000 | MM_BASE, 0x100000);
    comboExportContext();
    ((EntryPoint)0x80080000)();
}

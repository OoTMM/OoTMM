#include <types.h>
#include <combo.h>

void comboSwitchToOoT(void)
{
    //gComboCtx.saveIndex = gSaveContext.fileIndex;

    comboDisableInterrupts();
    comboDma((void*)0x400, 0x10001000, 0x100000);
    comboExportContext();
    ((EntryPoint)0x80000400)();
}

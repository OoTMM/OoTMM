#include <combo.h>

typedef void (*EntryPoint)(void)  __attribute__ ((noreturn));

#if defined(GAME_OOT)
# define FOREIGN_OFF    0x80000
# define FOREIGN_CART   (0x10001000 | MM_BASE)
#else
# define FOREIGN_OFF    0x400
# define FOREIGN_CART   0x10001000
#endif
#define FOREIGN_DRAM   (0x80000000 | FOREIGN_OFF)

void comboGameSwitch(void)
{
#if defined(GAME_OOT)
    gComboCtx.saveIndex = gSaveContext.fileIndex;
#endif

    comboDisableInterrupts();
    comboDma((void*)FOREIGN_OFF, FOREIGN_CART, 0x100000);
    comboExportContext();
    ((EntryPoint)FOREIGN_DRAM)();
}

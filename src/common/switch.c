#include <combo.h>

void comboGameSwitch2(void);
void comboGameSwitch(void)
{
#if defined(GAME_OOT)
    gComboCtx.saveIndex = gSaveContext.fileIndex;
#endif

    comboGameSwitch2();
}

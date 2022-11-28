#include <combo.h>

#define ENTRANCE_MARKET       0x1d1

void Sram_AfterOpenSave(void)
{
#if defined(DEBUG) && defined(DEBUG_OOT_ENTRANCE)
    gSave.entrance = DEBUG_OOT_ENTRANCE;
#endif

    if (gComboCtx.valid)
    {
        gSave.entrance = ENTRANCE_MARKET;
        gComboCtx.valid = 0;
    }
}

void Sram_CopySaveWrapper(void* fileSelect, void* sramCtx)
{
    Sram_CopySave(fileSelect, sramCtx);
    comboCopyMmSave(*(short*)((char*)fileSelect + 0x1ca50), *(short*)((char*)fileSelect + 0x1ca38));
}

PATCH_CALL(0x808048d8, Sram_CopySaveWrapper);

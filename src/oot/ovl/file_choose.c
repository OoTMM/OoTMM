#include <combo.h>

/* DEBUG */
void hook_FileChoose_Update(void* fc)
{
    if (gComboCtx.valid)
    {
        /* Extremely ugly hack */
        *(u16*)((char*)fc + 0x1ca2e) = 2;
        *(u16*)((char*)fc + 0x1ca36) = 7;

        /* File slot */
        *(u16*)((char*)fc + 0x1ca2a) = gComboCtx.saveIndex;
    }
}

void hook_FileChoose_Init(void)
{
    if (!gComboCtx.valid)
    {
        PlayMusic(0, 0x57, 0, 7, 1);
    }
}

void hookAfter_FileChoose_OpenSave(void)
{
    /* Read the other save */
    comboReadForeignSave();

    /* Skip early game (Mido out of the way, deku tree open) */
    gSave.eventsChk[0] |= 0x103c;
    gSave.eventsMisc[0] |= 0x000b;
}

#include <combo.h>

void hook_FileChoose_Update(void* fc)
{
    /* Save file index */
    *(u16*)((char*)fc + 0x24480) = gComboCtx.saveIndex;

    *(u16*)((char*)fc + 0x2448c) = 7;
    *(u16*)((char*)fc + 0x24484) = 2;
}

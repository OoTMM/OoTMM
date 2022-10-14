#include <combo.h>

void dummyDraw(void)
{
    /* Hide the screen - horrible hack */
    void* gfx = *(void**)0x801C84A0;
    u64** dlist = (u64**)((char*)gfx + 0x02B0);

    **dlist = 0xf700000000000000;
    *dlist = *dlist + 1;

    **dlist = 0xf6ffffff00000000ull;
    *dlist = *dlist + 1;
}

void hook_En_Mag_Main(void* mag, void* ctx)
{
    if (gComboCtx.valid)
    {
        /* Terrible hacks */
        *(void**)((char*)mag + 0x134) = &dummyDraw;
        *(u32*)(((char*)&gSaveContext) + 4956) = 2;
        *(u8*)((char*)ctx + 0x11e15) = 0x14;
        *(u8*)((char*)ctx + 0x11e5e) = 2;
    }
}

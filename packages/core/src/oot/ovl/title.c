#include <combo.h>
#include <combo/context.h>

void hook_Title_Main(u8* data)
{
    if (gComboCtx.valid)
        data[0x1e1] = 1;
}

#include <combo.h>
#include <combo/context.h>

#define MAGIC "OoT+MM<3"

extern ComboContext gComboCtxRead;
extern ComboContext gComboCtxWrite;

ComboContext gComboCtx;

void Context_Init(void)
{
    memcpy(&gComboCtx, &gComboCtxRead, sizeof(gComboCtx));
    if (memcmp(gComboCtx.magic, MAGIC, sizeof(gComboCtx.magic)) != 0)
    {
        bzero(&gComboCtx, sizeof(gComboCtx));
        memcpy(gComboCtx.magic, MAGIC, sizeof(gComboCtx.magic));
        gComboCtx.entrance = -1;
    }
    else
    {
        gComboCtx.valid = 1;
    }
    bzero(&gComboCtxRead, sizeof(gComboCtx));
}

void Context_Export(void)
{
    void* src;
    void* dst;

    src = (void*)((u32)&gComboCtx | 0xa0000000);
    dst = (void*)((u32)&gComboCtxWrite | 0xa0000000);
    memcpy(dst, src, sizeof(gComboCtx));
}

#include <combo.h>
#include <combo/context.h>
#include <combo/system.h>

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
    memcpy(&gComboCtxWrite, &gComboCtx, sizeof(gComboCtx));
}

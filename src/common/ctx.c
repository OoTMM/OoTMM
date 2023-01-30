#include <combo.h>

#define MAGIC "OoT+MM<3"

extern ComboContext gComboCtxRead;
extern ComboContext gComboCtxWrite;

ComboContext gComboCtx;

void comboLoadContext(void)
{
    memcpy(&gComboCtx, &gComboCtxRead, sizeof(gComboCtx));
    if (memcmp(gComboCtx.magic, MAGIC, sizeof(gComboCtx.magic)) != 0)
    {
        bzero(&gComboCtx, sizeof(gComboCtx));
        memcpy(gComboCtx.magic, MAGIC, sizeof(gComboCtx.magic));
    }
    else
    {
        gComboCtx.valid = 1;
    }
    bzero(&gComboCtxRead, sizeof(gComboCtx));
}

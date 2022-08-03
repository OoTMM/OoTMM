#include <combo.h>

ComboContext gComboContext;

void comboLoadContext(void)
{
    memcpy(&gComboContext, (void*)COMBO_CONTEXT_ADDR, sizeof(gComboContext));
    if (memcmp(gComboContext.magic, COMBO_CONTEXT_MAGIC, sizeof(gComboContext.magic)) != 0)
    {
        bzero(&gComboContext, sizeof(gComboContext));
        memcpy(gComboContext.magic, COMBO_CONTEXT_MAGIC, sizeof(gComboContext.magic));
    }
    else
    {
        gComboContext.valid = 1;
    }
    bzero((void*)COMBO_CONTEXT_ADDR, sizeof(gComboContext));
}

void comboExportContext(void)
{
    memcpy((void*)COMBO_CONTEXT_ADDR, &gComboContext, sizeof(gComboContext));
}

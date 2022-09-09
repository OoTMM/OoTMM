#include <combo.h>

void comboInitObjects(void);

void comboInit(void)
{
    comboLoadContext();
    comboInitDma();
    comboInitObjects();
    comboInitOverride();
}

#include <combo.h>

void comboInit(void)
{
    comboLoadContext();
    comboInitDma();
    comboInitOverride();
}

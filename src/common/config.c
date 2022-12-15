#include <combo.h>

int comboConfig(int flag)
{
    return BITMAP8_GET(gComboData.config, flag);
}

#include <combo/util.h>
#include <combo/config.h>

int Config_Flag(int flag)
{
    return BITMAP8_GET(gComboConfig.config, flag);
}

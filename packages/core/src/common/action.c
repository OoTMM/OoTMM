#include <combo.h>
#include <combo/action.h>
#include <combo/config.h>

int ActionCustom_EnabledOot(int action)
{
    if (!Config_Flag(CFG_OOT_ACTIONS))
        return 1;
    return !!(gSharedCustomSave.actionsOot & (1 << action));
}

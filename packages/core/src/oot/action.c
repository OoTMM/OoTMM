#include <combo.h>
#include <combo/action.h>

u16 gActionMaskRoll;
u16 gActionMaskOpen;
u16 gActionMaskGrab;

static int hasAction(int action)
{
    return 0;

    return !!(gSharedCustomSave.actionsOot & (1 << action));
}

void ActionCustom_Refresh(void)
{
    gActionMaskRoll = hasAction(ACTION_CUSTOM_ROLL) ? A_BUTTON : 0xffff;
    gActionMaskOpen = hasAction(ACTION_CUSTOM_OPEN) ? A_BUTTON : 0xffff;
    gActionMaskGrab = hasAction(ACTION_CUSTOM_GRAB) ? A_BUTTON : 0xffff;
}

#include <combo.h>
#include <combo/action.h>
#include <combo/config.h>

u16 gActionMaskRoll;
u16 gActionMaskOpen;
u16 gActionMaskGrab;
u16 gActionMaskCrawl;

void ActionCustom_Refresh(void)
{
    if (!Config_Flag(CFG_OOT_ACTIONS))
        return;

    gActionMaskRoll = ActionCustom_EnabledOot(ACTION_CUSTOM_ROLL) ? A_BUTTON : 0xffff;
    gActionMaskOpen = ActionCustom_EnabledOot(ACTION_CUSTOM_OPEN) ? A_BUTTON : 0xffff;
    gActionMaskGrab = ActionCustom_EnabledOot(ACTION_CUSTOM_GRAB) ? A_BUTTON : 0xffff;
    gActionMaskCrawl = ActionCustom_EnabledOot(ACTION_CUSTOM_CRAWL) ? A_BUTTON : 0xffff;
}

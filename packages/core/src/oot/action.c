#include <combo.h>
#include <combo/action.h>
#include <combo/config.h>

u16 gActionMaskRoll;
u16 gActionMaskOpen;
u16 gActionMaskGrab;
u16 gActionMaskCrawl;
u16 gActionMaskDive;
u16 gActionMaskPush;
u16 gActionMaskTalk;

void ActionCustom_Refresh(void)
{
    if (!Config_Flag(CFG_OOT_ACTIONS))
        return;

    gActionMaskRoll = ActionCustom_EnabledOot(ACTION_CUSTOM_ROLL) ? A_BUTTON : 0xffff;
    gActionMaskOpen = ActionCustom_EnabledOot(ACTION_CUSTOM_OPEN) ? A_BUTTON : 0xffff;
    gActionMaskGrab = ActionCustom_EnabledOot(ACTION_CUSTOM_GRAB) ? A_BUTTON : 0xffff;
    gActionMaskCrawl = ActionCustom_EnabledOot(ACTION_CUSTOM_CRAWL) ? A_BUTTON : 0xffff;
    gActionMaskDive = ActionCustom_EnabledOot(ACTION_CUSTOM_DIVE) ? A_BUTTON : 0xffff;
    gActionMaskPush = ActionCustom_EnabledOot(ACTION_CUSTOM_PUSH) ? A_BUTTON : 0xffff;
    gActionMaskTalk = ActionCustom_EnabledOot(ACTION_CUSTOM_TALK) ? A_BUTTON : 0xffff;
}

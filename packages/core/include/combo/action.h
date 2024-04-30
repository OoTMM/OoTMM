#ifndef COMBO_ACTION_H
#define COMBO_ACTION_H

#include <combo/game_state.h>

#define ACTION_CUSTOM_ROLL              0x00
#define ACTION_CUSTOM_TALK              0x01
#define ACTION_CUSTOM_OPEN              0x02
#define ACTION_CUSTOM_GRAB              0x03
#define ACTION_CUSTOM_CLIMB             0x04
#define ACTION_CUSTOM_CRAWL             0x05
#define ACTION_CUSTOM_DIVE              0x06
#define ACTION_CUSTOM_PUSH              0x07
#define ACTION_CUSTOM_MAX               0x08

extern u16 gActionMaskRoll;
extern u16 gActionMaskOpen;
extern u16 gActionMaskGrab;
extern u16 gActionMaskCrawl;
extern u16 gActionMaskDive;

int ActionCustom_EnabledOot(int action);
void ActionCustom_Refresh(void);

#endif

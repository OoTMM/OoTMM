#ifndef COMBO_INTERFACE_H
#define COMBO_INTERFACE_H

typedef struct PlayState PlayState;

void Interface_Update(PlayState* play);
void Interface_AgonyIconTick(void);
void Interface_AgonyIconDraw(PlayState* play);
void Interface_AfterDraw(void);

void Interface_SetSubTimerToFinalSecond(PlayState* play);

#if defined(GAME_MM)
void Interface_NewDay(PlayState* play, int day);
void Interface_SetHudVisibility(u16 hudVisibility);
void Interface_SkipMoonCrash(PlayState* play);
#endif

typedef enum {
    /* -1 */ EQUIP_SLOT_NONE = -1,
    /*  0 */ EQUIP_SLOT_B,
    /*  1 */ EQUIP_SLOT_C_LEFT,
    /*  2 */ EQUIP_SLOT_C_DOWN,
    /*  3 */ EQUIP_SLOT_C_RIGHT,
    /*  4 */ EQUIP_SLOT_A
} EquipSlot;

#endif

#ifndef COMBO_MM_PAUSE_MENU_H
#define COMBO_MM_PAUSE_MENU_H

typedef enum DebugEditor
{
    /* 0 */ DEBUG_EDITOR_NONE,
    /* 1 */ DEBUG_EDITOR_INVENTORY_INIT,
    /* 2 */ DEBUG_EDITOR_INVENTORY,
    /* 3 */ DEBUG_EDITOR_EVENTS
} DebugEditor;

typedef enum PauseState {
    /* 0x00 */ PAUSE_STATE_OFF,
    /* 0x01 */ PAUSE_STATE_OPENING_0,
    /* 0x02 */ PAUSE_STATE_OPENING_1,
    /* 0x03 */ PAUSE_STATE_OPENING_2,
    /* 0x04 */ PAUSE_STATE_OPENING_3,
    /* 0x05 */ PAUSE_STATE_OPENING_4,
    /* 0x06 */ PAUSE_STATE_MAIN, // Pause menu ready for player inputs.
    /* 0x07 */ PAUSE_STATE_SAVEPROMPT,
    /* 0x08 */ PAUSE_STATE_GAMEOVER_0,
    /* 0x09 */ PAUSE_STATE_GAMEOVER_1,
    /* 0x0A */ PAUSE_STATE_GAMEOVER_2,
    /* 0x0B */ PAUSE_STATE_GAMEOVER_3,
    /* 0x0C */ PAUSE_STATE_GAMEOVER_4,
    /* 0x0D */ PAUSE_STATE_GAMEOVER_5,
    /* 0x0E */ PAUSE_STATE_GAMEOVER_SAVE_PROMPT,
    /* 0x0F */ PAUSE_STATE_GAMEOVER_7,
    /* 0x10 */ PAUSE_STATE_GAMEOVER_8,
    /* 0x11 */ PAUSE_STATE_GAMEOVER_CONTINUE_PROMPT,
    /* 0x12 */ PAUSE_STATE_GAMEOVER_10,
    /* 0x13 */ PAUSE_STATE_OWL_WARP_0,
    /* 0x14 */ PAUSE_STATE_OWL_WARP_1,
    /* 0x15 */ PAUSE_STATE_OWL_WARP_2,
    /* 0x16 */ PAUSE_STATE_OWL_WARP_3,
    /* 0x17 */ PAUSE_STATE_OWL_WARP_SELECT, // Selecting the destination
    /* 0x18 */ PAUSE_STATE_OWL_WARP_CONFIRM, // Confirming the choice given
    /* 0x19 */ PAUSE_STATE_OWL_WARP_6,
    /* 0x1A */ PAUSE_STATE_UNPAUSE_SETUP, // Unpause
    /* 0x1B */ PAUSE_STATE_UNPAUSE_CLOSE
} PauseState;

#define IS_PAUSED(pauseCtx) \
    (((pauseCtx)->state != PAUSE_STATE_OFF) || ((pauseCtx)->debugEditor != DEBUG_EDITOR_NONE))

#endif // COMBO_MM_PAUSE_MENU_H

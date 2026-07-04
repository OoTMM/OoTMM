#ifndef COMBO_COMMON_KALEIDO_SCOPE_H
#define COMBO_COMMON_KALEIDO_SCOPE_H

int KaleidoScope_CanSave(PlayState* play);

typedef enum PauseMenuPage {
    /* 0 */ PAUSE_ITEM,
    /* 1 */ PAUSE_MAP,
    /* 2 */ PAUSE_QUEST,
    /* 3 */ PAUSE_MASK,
    /* 4 */ PAUSE_WORLD_MAP,
    /* 5 */ PAUSE_PAGE_MAX
} PauseMenuPage;

#define ITEM_GRID_ROWS 4
#define ITEM_GRID_COLS 6
#define MASK_GRID_ROWS 4
#define MASK_GRID_COLS 6

#define ITEM_NUM_SLOTS (ITEM_GRID_ROWS * ITEM_GRID_COLS)
#define MASK_NUM_SLOTS (MASK_GRID_ROWS * MASK_GRID_COLS)

s32 Player_GetCurMaskItemId(PlayState* play);

#endif

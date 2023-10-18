#ifndef COMBO_MENU_H
#define COMBO_MENU_H

#define MENU_NONE               0
#define MENU_INFO               1
#define MENU_SOULS_OOT_ENEMY    2
#define MENU_SOULS_OOT_BOSS     3
#define MENU_SOULS_MM_ENEMY     4
#define MENU_SOULS_MM_BOSS      5
#define MENU_MAX                6

typedef struct GameState_Play GameState_Play;

/* Menu */
void comboMenuUpdate(GameState_Play* play);
void comboMenuDraw(GameState_Play* play);
void comboMenuNext(void);

#endif

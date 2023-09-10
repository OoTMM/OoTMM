#ifndef COMBO_MENU_H
#define COMBO_MENU_H

#define MENU_NONE       0
#define MENU_INFO       1
#define MENU_SOULS_OOT  2
#define MENU_SOULS_MM   3
#define MENU_MAX        4

typedef struct GameState_Play GameState_Play;

/* Menu */
void comboMenuUpdate(GameState_Play* play);
void comboMenuDraw(GameState_Play* play);
void comboMenuNext(void);

#endif

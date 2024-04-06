#ifndef COMBO_MENU_H
#define COMBO_MENU_H

#include <PR/gbi.h>

#define MENU_NONE               0
#define MENU_INFO               1
#define MENU_SOULS_OOT_ENEMY    2
#define MENU_SOULS_OOT_BOSS     3
#define MENU_SOULS_OOT_NPC      4
#define MENU_SOULS_OOT_MISC     5
#define MENU_SOULS_MM_ENEMY     6
#define MENU_SOULS_MM_BOSS      7
#define MENU_SOULS_MM_MISC      8
#define MENU_MAX                9

typedef struct GameState_Play GameState_Play;

/* Menu */
void comboMenuTick(void);
void comboMenuUpdate(GameState_Play* play);
void comboMenuDraw(GameState_Play* play);
void comboMenuNext(void);

#endif

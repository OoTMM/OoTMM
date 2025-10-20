#ifndef COMBO_MENU_H
#define COMBO_MENU_H

#include <PR/gbi.h>

#define MENU_NONE               0
#define MENU_INFO               1
#define MENU_SONG_NOTES_OOT     2
#define MENU_SONG_NOTES_MM      3
#define MENU_SOULS_OOT_ENEMY    4
#define MENU_SOULS_OOT_BOSS     5
#define MENU_SOULS_OOT_NPC      6
#define MENU_SOULS_OOT_MISC     7
#define MENU_SOULS_MM_ENEMY     8
#define MENU_SOULS_MM_BOSS      9
#define MENU_SOULS_MM_NPC       10
#define MENU_SOULS_MM_MISC      11
#define MENU_MAX                12

typedef struct PlayState PlayState;

/* Menu */
void comboMenuTick(void);
void comboMenuUpdate(PlayState* play);
void comboMenuDraw(PlayState* play);
void comboMenuNext(void);

#endif

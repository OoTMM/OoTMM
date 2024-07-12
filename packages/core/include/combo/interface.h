#ifndef COMBO_INTERFACE_H
#define COMBO_INTERFACE_H

typedef struct GameState_Play GameState_Play;

void Interface_Update(GameState_Play* play);

#if defined(GAME_MM)
void Interface_NewDay(GameState_Play* play, int day);
void Interface_SetHudVisibility(u16 hudVisibility);
#endif

#endif

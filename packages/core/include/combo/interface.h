#ifndef COMBO_INTERFACE_H
#define COMBO_INTERFACE_H

typedef struct GameState_Play GameState_Play;

#if defined(GAME_MM)
void Interface_NewDay(GameState_Play* play, int day);
#endif

#endif

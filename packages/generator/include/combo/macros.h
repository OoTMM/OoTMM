#ifndef COMBO_MACROS_H
#define COMBO_MACROS_H

#define GAME_STATE_ALLOC(gameState, size, ...) THA_AllocTailAlign16(&(gameState)->tha, size)

#endif

#ifndef COMBO_MARK_H
#define COMBO_MARK_H

#include <combo/types.h>

extern u32 gMarkChests;
extern u32 gMarkCollectibles;
extern u32 gMarkSwitch0;
extern u32 gMarkSwitch1;

#if defined(GAME_OOT)
# define Mark_Get Mark_GetOot
# define Mark_Set Mark_SetOot
#endif

#if defined(GAME_MM)
# define Mark_Get Mark_GetMm
# define Mark_Set Mark_SetMm
#endif

typedef struct PlayState PlayState;

void    Mark_SetOot(PlayState* play, u8 ovType, u8 sceneId, u8 roomId, u8 id);
void    Mark_SetMm(PlayState* play, u8 ovType, u8 sceneId, u8 roomId, u8 id);
int     Mark_GetOot(PlayState* play, u8 ovType, u8 sceneId, u8 roomId, u8 id);
int     Mark_GetMm(PlayState* play, u8 ovType, u8 sceneId, u8 roomId, u8 id);

#endif

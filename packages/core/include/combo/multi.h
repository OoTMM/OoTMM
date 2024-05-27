#ifndef COMBO_MULTI_H
#define COMBO_MULTI_H

#include <combo/types.h>

extern u32 gMultiMarkChests;
extern u32 gMultiMarkCollectibles;
extern u32 gMultiMarkSwitch0;
extern u32 gMultiMarkSwitch1;

#if defined(GAME_OOT)
# define Multi_IsMarked Multi_IsMarkedOot
#endif

#if defined(GAME_MM)
# define Multi_IsMarked Multi_IsMarkedMm
#endif

typedef struct GameState_Play GameState_Play;

void    Multi_DrawWisps(GameState_Play* play);
void    Multi_ResetWisps(void);
void    Multi_SetMarkedOot(GameState_Play* play, u8 ovType, u8 sceneId, u8 roomId, u8 id);
void    Multi_SetMarkedMm(GameState_Play* play, u8 ovType, u8 sceneId, u8 roomId, u8 id);
int     Multi_IsMarkedOot(GameState_Play* play, u8 ovType, u8 sceneId, u8 roomId, u8 id);
int     Multi_IsMarkedMm(GameState_Play* play, u8 ovType, u8 sceneId, u8 roomId, u8 id);
void    Multi_Update(GameState_Play* play);

#endif

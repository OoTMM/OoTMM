#ifndef COMBO_MULTI_H
#define COMBO_MULTI_H

#include <combo/types.h>
#define MULTI_FLAGS_ITEM_NONCE 0x01

extern u32 gMultiMarkChests;
extern u32 gMultiMarkCollectibles;
extern u32 gMultiMarkSwitch0;
extern u32 gMultiMarkSwitch1;

#define GAME_ID_OOT 0
#define GAME_ID_MM  1

#if defined(GAME_OOT)
# define GAME_ID GAME_ID_OOT
# define Multi_SetMarked    Multi_SetMarkedOot
# define Multi_IsMarked     Multi_IsMarkedOot
#endif

#if defined(GAME_MM)
# define GAME_ID GAME_ID_MM
# define Multi_SetMarked    Multi_SetMarkedMm
# define Multi_IsMarked     Multi_IsMarkedMm
#endif

typedef struct PlayState PlayState;

inline void    Multi_DrawWisps(PlayState* play) {}
inline void    Multi_ResetWisps(void) {}

void    Multi_SetMarkedOot(PlayState* play, u8 ovType, u8 sceneId, u8 roomId, u8 id);
void    Multi_SetMarkedMm(PlayState* play, u8 ovType, u8 sceneId, u8 roomId, u8 id);
int     Multi_IsMarkedOot(PlayState* play, u8 ovType, u8 sceneId, u8 roomId, u8 id);
int     Multi_IsMarkedMm(PlayState* play, u8 ovType, u8 sceneId, u8 roomId, u8 id);

typedef struct PACKED
{
    u8  playerFrom;
    u8  playerTo;
    u8  game;
    u8  flags;
    u32 key;
    s16 gi;
}
MultiEntryItem;

int MultiEx_IsMultiplayer(void);
int MultiEx_IsSupported(void);
int MultiEx_Open(void);
void MultiEx_Close(void);
void MultiEx_Update(PlayState* play);

void MultiEx_SendEntryItem(const MultiEntryItem* entry);

#endif

#ifndef COMBO_MESSAGE_H
#define COMBO_MESSAGE_H

#include <combo.h>

#if defined(GAME_OOT)
# include <combo/oot/message.h>
#endif

#if defined(GAME_MM)
# include <combo/mm/message.h>
#endif

typedef struct PlayState PlayState;
void Message_StartBlocking(PlayState* play);
void Message_UpdateBlocking(PlayState* play);
void Message_DisplayOcarinaStaff(PlayState* play, u16 ocarinaAction);

#endif

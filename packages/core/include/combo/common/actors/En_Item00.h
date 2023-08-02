#ifndef COMBO_ACTORS_EN_ITEM00_H
#define COMBO_ACTORS_EN_ITEM00_H

#include <combo/common/actor.h>

typedef struct
{
    Actor   base;
    char    unk_13c[0x4];
    u16     collectibleFlag;
    char    unk_142[0x5a];
#if defined(GAME_MM)
    char    unk_mm[0x4];
#endif

    /* Extra flags */
    u8      isExtended;
    u8      extendedRoomId;
    u8      extendedId;
    u8      pad0;
    s16     extendedGi;
    s16     pad1;
}
Actor_EnItem00;

#if defined(GAME_OOT)
_Static_assert(sizeof(Actor_EnItem00) == 0x1a4, "OoT Actor_EnItem00 size is wrong");
#else
_Static_assert(sizeof(Actor_EnItem00) == 0x1b0, "MM Actor_EnItem00 size is wrong");
#endif

typedef struct GameState_Play GameState_Play;
void EnItem00_Init(Actor_EnItem00* this, GameState_Play* play);

#endif

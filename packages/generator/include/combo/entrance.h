#ifndef COMBO_ENTRANCE_H
#define COMBO_ENTRANCE_H

#include <combo/types.h>
#include <combo/defs.h>

#define ENTR_EXTENDED      0xfffe
#define ENTR_FW_CROSS      0xfffd
#define ENTR_CROSS_RESPAWN 0xfffc
#define ENTR_INVALID       0xffffffff

typedef struct PlayState PlayState;

int  comboIsEntranceValidEpona(u32 entranceId);
void comboTransitionOverride(PlayState* play, u32 entranceId);
void comboTransition(PlayState* play, u32 entranceId);

static inline u32 Entrance_ToRelative(u32 entranceId)
{
#if defined(GAME_MM)
    entranceId ^= MASK_FOREIGN_ENTRANCE;
#endif
    return entranceId;
}

static inline u32 Entrance_ToAbsolute(u32 entranceId)
{
    /* Symmetric function */
    return Entrance_ToRelative(entranceId);
}

#endif

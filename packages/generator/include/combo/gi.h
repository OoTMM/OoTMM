#ifndef COMBO_GI_H
#define COMBO_GI_H

#include <combo/gi_data.h>

#if defined(GAME_OOT)
# define GI_RECOVERY_HEART         GI_OOT_RECOVERY_HEART
#endif

#if defined(GAME_MM)
# define GI_RECOVERY_HEART         GI_MM_RECOVERY_HEART
#endif

#if !defined(__ASSEMBLER__)
# include <combo/types.h>

typedef struct PACKED ALIGNED(2)
{
    u8  itemId;
    u8  flags;
    u8  drawGiId;
    u8  type; /* textId in vanilla */
    u16 objectId;
}
GetItem;

extern const GetItem kExtendedGetItems[];
extern const u8 kGetItemDrawGiParam[];
extern const char* const kItemNames[];

#endif

#endif /* COMBO_COMMON_GI_H */

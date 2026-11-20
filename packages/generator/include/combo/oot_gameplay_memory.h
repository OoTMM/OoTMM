#ifndef COMBO_OOT_GAMEPLAY_MEMORY_H
#define COMBO_OOT_GAMEPLAY_MEMORY_H

#include <combo/types.h>

#define OOT_GAMEPLAY_ARENA_BYTES 0x100000u
#define OOT_GAMEPLAY_NATIVE_RESERVE 0x78000u
#define OOT_GAMEPLAY_FRAGMENT_RESERVE 0x10000u
#define OOT_GAMEPLAY_CONTIGUOUS_RESERVE 0x8000u
#define OOT_GAMEPLAY_CUSTOM_BASE 0x80580000u
#define OOT_GAMEPLAY_CUSTOM_RESERVE 0x80000u
#define OOT_GAMEPLAY_ACTOR_SLOTS 96u
int comboEnemyOotHeadroomEnabled(void);
u32 comboEnemyOotGameplayReserve(void);
static inline int comboOotGameplayObject(u16 id)
{
    return (id >= 0x2000 && id <= 0x201d) || id == 0x2041 || id == 0x2020 || id == 0x2021 || id == 0x2022 || id == 0x2023 || id == 0x2042 || id == 0x11e7 || id == 0x2026;
}

#endif

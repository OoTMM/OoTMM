#ifndef COMBO_DEBUG_H
#define COMBO_DEBUG_H

#include <combo/types.h>

#if defined(DEBUG_ALLOC)
void malloc_check(void);
#else
# define malloc_check() do {} while (0)
#endif

#define CHEAT_ALL_ITEMS     0
#define CHEAT_HEALTH        1
#define CHEAT_MAGIC         2

#if defined(DEBUG)
typedef struct
{
    const char* name;
    u32         data;
}
DebugMenuEntry;

void Debug_Init(void);
void Debug_Input(void);
void Debug_Update(void);

extern const DebugMenuEntry kDebugMenuWarp[];

# define CHEAT_ON(x) (BITMAP8_GET(gSharedCustomSave.cheats, x))

#else
# define Debug_Init() do {} while (0)
# define Debug_Input() do {} while (0)
# define Debug_Update() do {} while (0)
# define CHEAT_ON(x) (0)
#endif

#endif

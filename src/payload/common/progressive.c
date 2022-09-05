#include <combo.h>

s32 comboProgressive(s32 gi)
{
#if defined(GAME_MM)
    gi ^= MASK_FOREIGN_GI;
#endif

    if (gi & MASK_FOREIGN_GI)
    {
        gi = MASK_FOREIGN_GI | comboProgressiveMm(gi & (MASK_FOREIGN_GI - 1));
    }
    else
    {
        gi = comboProgressiveOot(gi);
    }

#if defined(GAME_MM)
    gi ^= MASK_FOREIGN_GI;
#endif

    return gi;
}

#include <combo.h>

s16 comboItemResolve(GameState_Play* play, s16 gi)
{
    int isNeg;

    isNeg = 0;
    if (gi == GI_NONE)
        return 0;
    if (gi < 0)
    {
        gi = -gi;
        isNeg = 1;
    }

    /* Resolve scene-specific items into their global counterparts */
    switch (gi)
    {
    case GI_MM_STRAY_FAIRY:
        switch (comboStrayFairyIndex())
        {
        case 0: gi = GI_MM_STRAY_FAIRY_WF; break;
        case 1: gi = GI_MM_STRAY_FAIRY_SH; break;
        case 2: gi = GI_MM_STRAY_FAIRY_GB; break;
        case 3: gi = GI_MM_STRAY_FAIRY_ST; break;
        case 4: gi = GI_MM_STRAY_FAIRY_TOWN; break;
        }
        break;
    }

    if (isNeg)
        gi = -gi;
    return gi;
}

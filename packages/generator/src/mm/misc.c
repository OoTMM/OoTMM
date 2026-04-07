#include <combo.h>

int Misc_IsMoonAccessible(void)
{
    if (Config_Flag(CFG_MM_MOON_COND_OPEN))
        return 1;

    if (Config_Flag(CFG_MM_MOON_COND_VANILLA) && gSave.info.inventory.quest.remainsOdolwa && gSave.info.inventory.quest.remainsGoht && gSave.info.inventory.quest.remainsGyorg && gSave.info.inventory.quest.remainsTwinmold)
        return 1;

    if (Config_Flag(CFG_MM_MOON_COND_CUSTOM) && SpecialConds_Eval(SPECIAL_MOON))
        return 1;

    return 0;
}

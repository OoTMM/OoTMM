#include <combo.h>

int comboCanAccessMoon(void)
{
    return (gSave.inventory.quest.remainsOdolwa
        && gSave.inventory.quest.remainsGoht
        && gSave.inventory.quest.remainsGyorg
        && gSave.inventory.quest.remainsTwinmold);
}

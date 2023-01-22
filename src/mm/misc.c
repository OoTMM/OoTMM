#include <combo.h>

int comboCanAccessMoon(void)
{
    return (gSave.inventory.questItems.remainsOdolwa
        && gSave.inventory.questItems.remainsGoht
        && gSave.inventory.questItems.remainsGyorg
        && gSave.inventory.questItems.remainsTwinmold);
}

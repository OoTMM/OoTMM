#include <combo.h>
#include <combo/csmc.h>
#include <combo/item.h>

void EnSi_ItemOverride(ComboItemOverride* o, Actor* this, GameState_Play* play);

static void EnSw_GoldPreDraw1(Actor* this, GameState_Play* play, int unk)
{
    ComboItemOverride o;

    /* CSMC */
    EnSi_ItemOverride(&o, this, play);

    /* Init */
    PreDraw1(this, play, unk);

    /* Inject */
    csmcGsPreDraw(play, o.gi);
}

PATCH_CALL(0x808db928, EnSw_GoldPreDraw1);

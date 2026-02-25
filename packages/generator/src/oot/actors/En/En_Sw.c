#include <combo.h>
#include <combo/csmc.h>
#include <combo/item.h>

void EnSi_ItemOverride(ComboItemOverride* o, Actor* this, PlayState* play);

static void EnSw_GoldPreDraw1(Actor* this, PlayState* play, int unk)
{
    ComboItemOverride o;

    /* CSMC */
    EnSi_ItemOverride(&o, this, play);

    /* Init */
    PreDraw1(this, play, unk);

    /* Inject */
    csmcGsPreDraw(play, o.gi, o.cloakGi);
}

PATCH_CALL(0x809488c0, EnSw_GoldPreDraw1);

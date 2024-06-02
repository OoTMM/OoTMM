#include <combo.h>
#include <combo/global.h>
#include <combo/item.h>
#include <combo/draw.h>
#include <combo/mm/actors/En_Invisible_Ruppe.h>

static void EnInvisibleRupee_DrawGlitter(Actor_EnInvisibleRuppe* this, GameState_Play* play)
{
    ComboItemOverride o;

    /* Check xflag */
    if (comboXflagsGet(&this->xflag))
    {
        this->base.draw = NULL;
        return;
    }

    comboXflagItemOverride(&o, &this->xflag, 0);
    Draw_GlitterGi(play, &this->base, o.gi);
}

void EnInvisibleRupee_InitWrapper(Actor_EnInvisibleRuppe* this, GameState_Play* play)
{
    int switchFlag;
    ComboItemOverride o;
    EnInvisibleRuppeFunc init;

    /* Setup the xflag */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = g.sceneSetupId;
    this->xflag.roomId = this->base.room;
    this->xflag.sliceId = 0;
    this->xflag.id = g.actorIndex;

    comboXflagItemOverride(&o, &this->xflag, 0);
    this->isExtended = !!(o.gi && !comboXflagsGet(&this->xflag));

    switchFlag = (this->base.variable & 0x1fc) >> 2;
    if (this->isExtended && switchFlag != 0x7f && GetSwitchFlag(play, switchFlag))
    {
        /* Already collected but no xflag */
        this->base.variable |= 0x1fc;
    }

    /* Forward */
    init = actorAddr(AC_EN_INVISIBLE_RUPPE, 0x80c25a0c);
    init(this, play);

    if (this->isExtended)
        this->base.draw = EnInvisibleRupee_DrawGlitter;
}

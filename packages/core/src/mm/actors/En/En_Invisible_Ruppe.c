#include <combo.h>
#include <combo/global.h>
#include <combo/item.h>
#include <combo/draw.h>
#include <combo/mm/actors/En_Invisible_Ruppe.h>

static void EnInvisibleRupee_DrawGlitter(Actor_EnInvisibleRuppe* this, PlayState* play)
{
    ComboItemOverride o;

    /* Check xflag */
    if (comboXflagsGet(&this->xflag))
    {
        this->base.draw = NULL;
        return;
    }

    comboXflagItemOverride(&o, &this->xflag, 0);
    Draw_GlitterGi(play, &this->base, o.gi, o.cloakGi);
}

void EnInvisibleRupee_HandleExtended(Actor_EnInvisibleRuppe* this, PlayState* play)
{
    ComboItemQuery q;
    EnInvisibleRuppeFunc handleNormal;

    if (comboXflagsGet(&this->xflag) || !this->isExtended)
    {
        this->base.draw = NULL;
        handleNormal = actorAddr(ACTOR_EN_INVISIBLE_RUPPE, 0x80c2590c);
        this->handler = handleNormal;
        handleNormal(this, play);
        return;
    }

    if (this->collider.base.ocFlags1 & 0x02)
    {
        comboXflagItemQuery(&q, &this->xflag, 0);
        Item_AddWithDecoy(play, &q);
        comboXflagsSet(&this->xflag);

        if (this->switchFlag > 0)
            Flags_SetSwitch(play, this->switchFlag);

        this->base.draw = NULL;
        this->handler = actorAddr(ACTOR_EN_INVISIBLE_RUPPE, 0x80c259e8);
    }
}

void EnInvisibleRupee_InitWrapper(Actor_EnInvisibleRuppe* this, PlayState* play)
{
    int switchFlag;
    EnInvisibleRuppeFunc init;

    /* Setup the xflag */
    comboXflagInit(&this->xflag, &this->base, play);
    this->isExtended = Xflag_IsShuffled(&this->xflag);

    switchFlag = (this->base.params & 0x1fc) >> 2;
    if (this->isExtended && switchFlag != 0x7f && Flags_GetSwitch(play, switchFlag))
    {
        /* Already collected but no xflag */
        this->base.params |= 0x1fc;
    }

    /* Forward */
    init = actorAddr(ACTOR_EN_INVISIBLE_RUPPE, 0x80c25a0c);
    init(this, play);

    if (this->isExtended)
    {
        this->base.draw = EnInvisibleRupee_DrawGlitter;
        this->handler = EnInvisibleRupee_HandleExtended;
    }
}

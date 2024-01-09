#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>

#if defined(GAME_OOT)
# define DUMMY_MSG 0xb4
#else
# define DUMMY_MSG 0x52
#endif

void EnItem00_AliasFreestandingRupee(Xflag* xflag);
void EnItem00_AliasFreestandingHeart(Xflag* xflag);

void EnItem00_InitWrapper(Actor_EnItem00* this, GameState_Play* play)
{
    /* Forward */
    EnItem00_Init(this, play);

    /* Set extended fields */
    bzero(&this->xflag, sizeof(this->xflag));
    this->xflagGi = 0;
    this->isExtended = 0;
    this->isExtendedCollected = 0;
    this->isExtendedMajor = 0;
}

void EnItem00_DrawXflag(Actor_EnItem00* this, GameState_Play* play)
{
    ComboItemOverride o;
    s16 gi;

    if (this->isExtendedCollected)
        gi = this->xflagGi;
    else
    {
        comboXflagItemOverride(&o, &this->xflag, 0);
        gi = o.gi;
        this->xflagGi = gi;
    }

    ModelViewTranslate(this->base.position.x, this->base.position.y + 20.f, this->base.position.z, MAT_SET);
    ModelViewScale(0.35f, 0.35f, 0.35f, MAT_MUL);
    ModelViewRotateY(this->base.rot2.y * ((M_PI * 2.f) / 32767.f), MAT_MUL);
    comboDrawGI(play, &this->base, gi, 0);
}

void EnItem00_AddXflag(Actor_EnItem00* this)
{
    ComboItemQuery q;
    ComboItemOverride o;

    if (!this->isExtended)
    {
        AddItem(gPlay, ITEM_RUPEE_GREEN);
        return;
    }

    /* Collect the item */
    comboXflagItemQuery(&q, &this->xflag, 0);
    comboItemOverride(&o, &q);
    if (!isItemFastBuy(o.gi))
    {
        PlayerDisplayTextBox(gPlay, DUMMY_MSG, NULL);
        FreezePlayer(gPlay);
        this->isExtendedMajor = 1;
    }
    comboAddItemEx(gPlay, &q, this->isExtendedMajor);
    comboXflagsSet(&this->xflag);
    this->isExtendedCollected = 1;
}

void EnItem00_XflagInitFreestanding(Actor_EnItem00* this, GameState_Play* play, u8 actorIndex, u8 slice)
{
    ComboItemOverride o;
    Xflag xflag;

    /* Setup the xflag */
    bzero(&xflag, sizeof(xflag));
    xflag.sceneId = play->sceneId;
    xflag.setupId = g.sceneSetupId;
    xflag.roomId = this->base.room;
    xflag.sliceId = slice;
    xflag.id = actorIndex;

    /* Alias as required */
    switch (this->base.variable & 0xff)
    {
    case ITEM00_RUPEE_GREEN:
    case ITEM00_RUPEE_BLUE:
    case ITEM00_RUPEE_RED:
        EnItem00_AliasFreestandingRupee(&xflag);
        break;
    case ITEM00_RECOVERY_HEART:
        EnItem00_AliasFreestandingHeart(&xflag);
        break;
    }

    /* Query */
    comboXflagItemOverride(&o, &xflag, 0);

    /* Check if there is an override */
    if (o.gi == 0)
        return;
    if (comboXflagsGet(&xflag))
        return;

    /* It's an actual item - mark it as such */
    memcpy(&this->xflag, &xflag, sizeof(xflag));
    this->xflagGi = o.gi;
    this->isExtended = 1;

    /* Use our draw func */
    this->base.draw = EnItem00_DrawXflag;
    this->base.variable = 0;
}

static void EnItem00_XflagCollectedHandler(Actor_EnItem00* this, GameState_Play* play)
{
    this->timer = 1;
    EnItem00_CollectedHandler(this, play);
    if (Message_IsClosed(&this->base, play))
    {
        UnfreezePlayer(play);
        this->handler = EnItem00_CollectedHandler;
    }
    else
        FreezePlayer(play);
}

void EnItem00_SetXflagCollectedHandler(Actor_EnItem00* this)
{
    if (!this->isExtendedMajor)
    {
        this->handler = EnItem00_CollectedHandler;
        return;
    }

#if defined(GAME_MM)
    this->base.flags |= 0x100000;
#endif

    this->handler = EnItem00_XflagCollectedHandler;
}

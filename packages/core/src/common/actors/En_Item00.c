#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/global.h>
#include <combo/draw.h>
#include <combo/actor.h>

#if defined(GAME_OOT)
# define DUMMY_MSG 0xb4
#else
# define DUMMY_MSG 0x52
#endif

void EnItem00_AliasFreestandingRupee(Xflag* xflag);
void EnItem00_AliasFreestandingHeart(Xflag* xflag);

static void EnItem00_DrawXflag(Actor_EnItem00* this, PlayState* play)
{
    ComboItemOverride o;
    s16 gi;
    s16 cloakGi;

    if (this->isExtendedCollected)
    {
        gi = this->xflagGi;
        cloakGi = GI_NONE;
    }
    else
    {
        comboXflagItemOverride(&o, &this->xflag, 0);
        gi = o.gi;
        cloakGi = o.cloakGi;
        this->xflagGi = gi;
    }

    Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y + 20.f, this->actor.world.pos.z, MTXMODE_NEW);
    Matrix_Scale(0.35f, 0.35f, 0.35f, MTXMODE_APPLY);
    Matrix_RotateY(this->actor.shape.rot.y * ((M_PI * 2.f) / 32767.f), MTXMODE_APPLY);
    Draw_GiCloaked(play, &this->actor, gi, cloakGi, 0);
}

void EnItem00_InitWrapper(Actor_EnItem00* this, PlayState* play)
{
    ComboItemOverride o;

    /* Forward */
    EnItem00_Init(this, play);

    /* Zero the extended flags */
    this->xflagGi = 0;
    this->isExtended = 0;
    this->isExtendedCollected = 0;
    this->isExtendedMajor = 0;

    /* Init the xflag */
    if (comboXflagInit(&this->xflag, &this->actor, play))
    {
        switch (this->actor.params & 0xff)
        {
        case ITEM00_RUPEE_GREEN:
        case ITEM00_RUPEE_BLUE:
        case ITEM00_RUPEE_RED:
            EnItem00_AliasFreestandingRupee(&this->xflag);
            break;
        case ITEM00_RECOVERY_HEART:
            EnItem00_AliasFreestandingHeart(&this->xflag);
            break;
        }
    }

    if (Xflag_IsShuffled(&this->xflag))
    {
        comboXflagItemOverride(&o, &this->xflag, 0);
        this->isExtended = 1;
        this->xflagGi = o.gi;

        this->actor.params = 0;
        this->actor.draw = EnItem00_DrawXflag;
    }
}

static int EnItem00_XflagCanCollect(Actor_EnItem00* this, PlayState* play)
{
    Player* link;

    link = GET_PLAYER(play);
    if (link->stateFlags1 & (PLAYER_ACTOR_STATE_FROZEN | PLAYER_ACTOR_STATE_EPONA))
        return 0;

    /* Check for textbox */
    if (Message_GetState(&play->msgCtx) != 0)
        return 0;

    return 1;
}

static void EnItem00_UpdateXflagDrop(Actor_EnItem00* this, PlayState* play)
{
    /* Artifically disable collisions if the items shouldn't be collected */
    if (!EnItem00_XflagCanCollect(this, play))
        this->actor.xzDistToPlayer = 100.f;

    /* Item permanence */
    if (!this->isExtendedCollected)
        this->timer++;

    /* Update */
    EnItem00_Update(this, play);
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

    comboXflagItemQuery(&q, &this->xflag, 0);
    comboItemOverride(&o, &q);
    if (!isItemFastBuy(o.gi))
    {
        PlayerDisplayTextBox(gPlay, DUMMY_MSG, NULL);
        Player_Freeze(gPlay);
        this->isExtendedMajor = 1;
    }
    comboAddItemEx(gPlay, &q, this->isExtendedMajor);
    comboXflagsSet(&this->xflag);

    comboPlayItemFanfare(o.gi, 1);
    this->isExtendedCollected = 1;
}

void EnItem00_PlaySoundXflag(Actor_EnItem00* this)
{
    if (this->isExtended)
        return;
    PlaySound(0x4803);
}

static void EnItem00_XflagCollectedHandler(Actor_EnItem00* this, PlayState* play)
{
    this->timer = 1;
    EnItem00_CollectedHandler(this, play);
    if (Message_IsClosed(&this->actor, play))
    {
        Player_Unfreeze(play);
        this->handler = EnItem00_CollectedHandler;
    }
    else
        Player_Freeze(play);
}

void EnItem00_SetXflagCollectedHandler(Actor_EnItem00* this)
{
    if (!this->isExtendedMajor)
    {
        this->handler = EnItem00_CollectedHandler;
        return;
    }

#if defined(GAME_MM)
    this->actor.flags |= 0x100000;
#endif

    this->handler = EnItem00_XflagCollectedHandler;
}

Actor_EnItem00* EnItem00_DropCustom(PlayState* play, const Vec3f* pos, const Xflag* xflag)
{
    Actor* actor;
    Actor_EnItem00* item;
    ComboItemOverride o;

    /* Check if the xflag item is already spawned */
    for (actor = play->actorCtx.actors[0x08].first; actor != NULL; actor = actor->next)
    {
        if (actor->id != ACTOR_EN_ITEM00)
            continue;
        item = (Actor_EnItem00*)actor;
        if (memcmp(&item->xflag, xflag, sizeof(Xflag)) == 0)
            return NULL;
    }

    /* Check if the item to be spawned is literaly Nothing */
    comboXflagItemOverride(&o, xflag, 0);
    if (o.gi == GI_NOTHING)
    {
        comboXflagsSet(xflag);
        return NULL;
    }

    /* Spawn the item */
    memcpy(&g.xflag, xflag, sizeof(Xflag));
    g.xflagOverride = TRUE;
    item = (Actor_EnItem00*)Item_DropCollectible(play, pos, 0x0000);
    g.xflagOverride = FALSE;
    if (!item)
        return NULL;

    /* Persist the drop */
    item->actor.update = EnItem00_UpdateXflagDrop;

    return item;
}

Actor_EnItem00* EnItem00_DropCustomNoInertia(PlayState* play, const Vec3f* pos, const Xflag* xflag)
{
    Actor_EnItem00* item;

    item = EnItem00_DropCustom(play, pos, xflag);
    if (!item)
        return NULL;
    item->actor.speed = 0.f;
    item->actor.velocity.x = 0.f;
    item->actor.velocity.y = 0.f;
    item->actor.velocity.z = 0.f;
    return item;
}

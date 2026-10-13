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

static void EnItem00_DrawXflag(Actor_EnItem00* this, PlayState* play)
{
    ComboItemOverride o;
    s16 gi;
    s16 cloakGi;
    s16 angle;

    if (this->isExtendedCollected)
    {
        gi = this->xflagGi;
        cloakGi = GI_NONE;
    }
    else
    {
        Xflag_ItemOverride(&o, this->xflag, GI_NONE);
        gi = o.gi;
        cloakGi = o.cloakGi;
        this->xflagGi = gi;
    }

    angle = this->actor.shape.rot.y;
    if (cloakGi)
    {
        gi = cloakGi;
        angle = -angle;
    }
    Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y + 20.f, this->actor.world.pos.z, MTXMODE_NEW);
    Matrix_Scale(0.35f, 0.35f, 0.35f, MTXMODE_APPLY);
    Matrix_RotateYS(angle, MTXMODE_APPLY);
    Draw_Gi(play, &this->actor, gi, 0);
}

static s32 EnItem00_IsBoomerangHeld(Actor* item, PlayState* play)
{
#if defined(GAME_OOT)
    Actor* boom = play->actorCtx.actors[ACTORCAT_MISC].first;
#else
    Actor* boom = play->actorCtx.actors[ACTORCAT_ITEMACTION].first;
#endif

    for (; boom; boom = boom->next)
    {
        if (!boom->update)
            continue;

#if defined(GAME_OOT)
        if (boom->id == ACTOR_EN_BOOM &&
            *(Actor**)((u8*)boom + 0x1d0) == item)
            return 1;
#else
        if ((boom->id == ACTOR_EN_BOOM &&
             *(Actor**)((u8*)boom + 0x1c8) == item) ||
            (boom->id == ACTOR_CUSTOM_BOOMERANG &&
             *(Actor**)((u8*)boom + 0x1d0) == item))
            return 1;
#endif
    }

    return 0;
}

static void EnItem00_ReturnHome(Actor_EnItem00* this)
{
    Actor* a = &this->actor;
    a->world.pos = a->prevPos = a->home.pos;
    a->velocity = (Vec3f){0};
    a->colChkInfo.displacement = a->velocity;
    a->speed = 0.0f;
    a->bgCheckFlags &= ~(BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH | BGCHECKFLAG_GROUND_LEAVE);
    a->home.rot.x = 0;
}


void EnItem00_UpdateGuard(Actor_EnItem00* this, PlayState* play)
{
    Actor* a = &this->actor;
    ActorFunc update = a->update;
    ActorFunc draw = a->draw;
    u32 attention = a->flags & ACTOR_FLAG_ATTENTION_ENABLED;
    s32 canRecover = a->gravity != 0.0f && !a->parent && !this->isExtendedCollected;

    if (EnItem00_IsBoomerangHeld(a, play))
    {
        f32 gravity = a->gravity;
        a->gravity = 0.0f;
        EnItem00_Update(this, play);
        a->gravity = gravity;
        return;
    }

    EnItem00_Update(this, play);

    if (!canRecover || a->parent || this->isExtendedCollected)
        return;

    if (!a->update)
    {
        if (a->floorHeight > -32000.0f)
            return;

        a->update = update;
        a->draw = draw;
        a->flags |= attention;
    }
    else if (a->world.pos.y >= a->home.pos.y - 1000.0f)
        return;

    EnItem00_ReturnHome(this);
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
    this->xflag = Xflag_InitEx(&this->actor, play);
    if (Xflag_IsShuffledEx(this->xflag))
    {
        Xflag_ItemOverride(&o, this->xflag, 0);
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
    if (link->stateFlags1 & (PLAYER_ACTOR_STATE_FROZEN | PLAYER_ACTOR_STATE_EPONA | PLAYER_ACTOR_STATE_GROTTO))
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
    EnItem00_UpdateGuard(this, play);
}

void EnItem00_AddXflag(Actor_EnItem00* this)
{
    ComboItemQuery q;
    ComboItemOverride o;

    if (!this->isExtended)
    {
        Item_Give(gPlay, ITEM_RUPEE_GREEN);
        return;
    }

    Xflag_ItemQuery(&q, this->xflag, 0);
    comboItemOverride(&o, &q);
    if (!isItemFastBuy(o.gi))
    {
        PlayerDisplayTextBox(gPlay, DUMMY_MSG, NULL);
        Player_Freeze(gPlay);
        this->isExtendedMajor = 1;
    }
    comboAddItemEx(gPlay, &q, this->isExtendedMajor);
    Xflag_Set(this->xflag);

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
    XflagID id;

    id = Xflag_Lookup(xflag);
    return EnItem00_DropCustomEx(play, pos, id);
}


Actor_EnItem00* EnItem00_DropCustomEx(PlayState* play, const Vec3f* pos, XflagID id)
{
    Actor* actor;
    Actor_EnItem00* item;
    ComboItemOverride o;

    if (id == XFLAGID_NONE)
        return (Actor_EnItem00*)Item_DropCollectible(play, pos, 0x0000);

    /* Check if the xflag item is already spawned */
    for (actor = play->actorCtx.actors[0x08].first; actor != NULL; actor = actor->next)
    {
        if (actor->id != ACTOR_EN_ITEM00)
            continue;
        item = (Actor_EnItem00*)actor;
        if (item->xflag == id)
            return NULL;
    }

    /* Check if the item to be spawned is literaly Nothing */
    Xflag_ItemOverride(&o, id, GI_NONE);
    if (o.gi == GI_NOTHING)
    {
        Xflag_Set(id);
        return NULL;
    }

    /* Spawn the item */
    g.xflagOverride = TRUE;
    g.xflagId = id;
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

Actor_EnItem00* EnItem00_DropCustomNoInertiaEx(PlayState* play, const Vec3f* pos, XflagID xflag)
{
    Actor_EnItem00* item;

    item = EnItem00_DropCustomEx(play, pos, xflag);
    if (!item)
        return NULL;
    item->actor.speed = 0.f;
    item->actor.velocity.x = 0.f;
    item->actor.velocity.y = 0.f;
    item->actor.velocity.z = 0.f;
    return item;
}

#include <combo.h>
#include <combo/player.h>
#include <combo/draw.h>
#include <combo/item.h>
#include <combo/global.h>
#include <combo/common/actors/Item_Decoy.h>

#if defined(GAME_OOT)
# define DUMMY_MSG 0xb4
#else
# define DUMMY_MSG 0x52
#endif

static u16 sMsgTimer;

static void ItemDecoy_SetHandler(Actor_ItemDecoy* this, GameState_Play* play, Actor_ItemDecoyFunc handler)
{
    this->handler = handler;
    handler(this, play);
}

static void ItemDecoy_Draw(Actor_ItemDecoy* this, GameState_Play* play)
{
    Actor_Player* link;
    Vec3f pos;

    if (this->gi == GI_NONE)
        return;

    link = GET_PLAYER(play);
    pos.x = link->actor.world.pos.x;
    pos.y = link->actor.world.pos.y + Player_GetHeight(link) + 20.f;
    pos.z = link->actor.world.pos.z;
    Matrix_Translate(pos.x, pos.y, pos.z, MAT_SET);
    Matrix_Scale(0.35f, 0.35f, 0.35f, MAT_MUL);
    Matrix_RotateY(this->base.shape.rot.y * ((M_PI * 2.f) / 32767.f), MAT_MUL);
    Draw_Gi(play, &this->base, this->gi, 0);
}

static void ItemDecoy_HandlerTimer(Actor_ItemDecoy* this, GameState_Play* play)
{
    if (!this->timer)
        Actor_Kill(&this->base);
}

static int ItemDecoy_CanCollect(Actor_ItemDecoy* this, GameState_Play* play)
{
    Actor_Player* link;

    if (Player_InCsMode(play))
        return 0;
    if (gSaveContext.gameMode || (gSaveContext.minigameState == 1))
        return 0;
    link = GET_PLAYER(play);
    if (link->state & (PLAYER_ACTOR_STATE_FROZEN | PLAYER_ACTOR_STATE_GET_ITEM | PLAYER_ACTOR_STATE_CUTSCENE_FROZEN))
        return 0;

    /* Check for textbox */
    if (Message_GetState(&play->msgCtx) != 0)
        return 0;

    return 1;
}

static void ItemDecoy_HandlerMessageTimer(Actor_ItemDecoy* this, GameState_Play* play)
{
    int end;

    end = 0;
    if (Message_IsClosed(&this->base, play))
        end = 1;
    else if (sMsgTimer)
        sMsgTimer--;
    else
    {
        Message_Close(play);
        end = 1;
    }

    if (end)
    {
        g.decoysCount--;
        this->handler = ItemDecoy_HandlerTimer;
    }
}

static void ItemDecoy_HandlerImportantItemConfirm(Actor_ItemDecoy* this, GameState_Play* play)
{
    if (Message_IsClosed(&this->base, play))
    {
        sMsgTimer = 0;
        Player_Unfreeze(play);
        g.decoysCount--;
        this->handler = ItemDecoy_HandlerTimer;
    }
    else
        Player_Freeze(play);
}

static void ItemDecoy_HandlerImportantItem(Actor_ItemDecoy* this, GameState_Play* play)
{
    ComboItemOverride o;
    Actor_Player* link;

    if (!ItemDecoy_CanCollect(this, play))
        return;

    memset(&o, 0, sizeof(o));
    o.gi = this->gi;
    o.giRaw = this->gi;
    o.player = this->player;
    o.playerFrom = this->playerFrom;

    comboPlayItemFanfare(this->gi, 1);
    this->base.draw = ItemDecoy_Draw;
    PlayerDisplayTextBox(play, DUMMY_MSG, NULL);
    comboTextHijackItemEx(play, &o, this->count);

    link = GET_PLAYER(play);
    if (link->state & PLAYER_ACTOR_STATE_EPONA)
    {
        sMsgTimer = 60;
        this->handler = ItemDecoy_HandlerMessageTimer;
    }
    else
    {
        Player_Freeze(play);
        this->handler = ItemDecoy_HandlerImportantItemConfirm;
    }
}

static void ItemDecoy_HandlerInit(Actor_ItemDecoy* this, GameState_Play* play)
{
    if (!isItemFastBuy(this->gi))
        ItemDecoy_SetHandler(this, play, ItemDecoy_HandlerImportantItem);
    else
    {
        comboPlayItemFanfare(this->gi, 1);
        this->base.draw = ItemDecoy_Draw;
        g.decoysCount--;
        this->handler = ItemDecoy_HandlerTimer;
    }
}

static void ItemDecoy_Init(Actor_ItemDecoy* this, GameState_Play* play)
{
    this->base.room = 0xff;
    this->timer = 20;
    this->handler = ItemDecoy_HandlerInit;

#if defined(GAME_MM)
    this->base.flags |= 0x100000;
#endif
}

static void ItemDecoy_Update(Actor_ItemDecoy* this, GameState_Play* play)
{
    Actor_Player* link;

    link = GET_PLAYER(play);
    this->base.world.pos.x = link->actor.world.pos.x;
    this->base.world.pos.y = link->actor.world.pos.y;
    this->base.world.pos.z = link->actor.world.pos.z;
    this->base.xzDistToPlayer = 0.f;

    /* Rotation */
    if (this->timer)
        this->timer--;
    this->base.shape.rot.y += 0x200;
    this->handler(this, play);
}

ActorProfile ItemDecoy_gActorProfile = {
    AC_ITEM_DECOY,
    0x8,
    0x10,
    0x1,
    sizeof(Actor_ItemDecoy),
    (ActorFunc)ItemDecoy_Init,
    NULL,
    (ActorFunc)ItemDecoy_Update,
    NULL,
};

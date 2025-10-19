#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/draw.h>
#include <combo/actor.h>

void EnExRuppy_ItemQuery(ComboItemQuery* q, Actor_EnExRuppy* this, PlayState* play)
{
    memset(q, 0, sizeof(*q));

    q->ovType = OV_COLLECTIBLE;
    q->id = this->colorIdx + 1;
    q->sceneId = play->sceneId;

    switch (this->colorIdx)
    {
    case 0x00:
        q->gi = GI_OOT_RUPEE_GREEN;
        break;
    case 0x01:
        q->gi = GI_OOT_RUPEE_BLUE;
        break;
    case 0x02:
        q->gi = GI_OOT_RUPEE_RED;
        break;
    case 0x03:
        q->gi = GI_OOT_RUPEE_HUGE; /* TODO GI_OOT_RUPEE_500? */
        break;
    case 0x04:
        q->gi = GI_OOT_RUPEE_PURPLE;
        break;
    default:
        break;
    }
    q->giRenew = q->gi;
    if (Flags_GetCollectible(play, q->id)) {
        q->ovFlags |= OVF_RENEW;
    }
}

static int EnExRupy_IsShuffled(Actor_EnExRuppy* this, PlayState* play)
{
    ComboItemQuery q;
    ComboItemOverride o;

    EnExRuppy_ItemQuery(&q, this, play);
    q.gi = GI_NONE;
    comboItemOverride(&o, &q);

    return o.gi != GI_NONE;
}

void EnExRuppy_Draw(Actor_EnExRuppy* this, PlayState* play)
{
    ComboItemQuery q;
    ComboItemOverride o;
    s16 gi;

    EnExRuppy_ItemQuery(&q, this, play);
    comboItemOverride(&o, &q);
    gi = o.gi;
    if (o.cloakGi)
    {
        gi = o.cloakGi;
        Matrix_RotateYS(-this->actor.shape.rot.y * 2, MTXMODE_APPLY);
    }
    Matrix_Scale(25.0f, 25.0f, 25.0f, MTXMODE_APPLY);
    Draw_Gi(play, &this->actor, gi, 0);
}

void EnExRuppy_HandlerCollected(Actor_EnExRuppy* this, PlayState* play)
{
    Actor_EnDivingGame* divingGame;
    if (Message_IsClosed(&this->actor, play))
    {
        divingGame = (Actor_EnDivingGame*)this->actor.parent;
        divingGame->grabbedRupeesCounter++;
        Player_Unfreeze(play);
        Actor_Kill(&this->actor);
    }
    else
    {
        Player_Freeze(play);
    }
}

void EnExRuppy_GiveItem(Actor_EnExRuppy* this, PlayState* play, Actor_EnDivingGame* divingGame)
{
    Player* link;
    ComboItemQuery q;
    ComboItemOverride o;
    int major;

    link = GET_PLAYER(play);
    if (link->stateFlags1 & (PLAYER_ACTOR_STATE_FROZEN | PLAYER_ACTOR_STATE_EPONA))
        return;

    /* Check for textbox */
    if (Message_GetState(&play->msgCtx) != 0)
        return;

    /* Query the item */
    EnExRuppy_ItemQuery(&q, this, play);
    comboItemOverride(&o, &q);
    major = !isItemFastBuy(o.gi);
    if (major)
    {
        PlayerDisplayTextBox(play, 0xb4, NULL);
        Player_Freeze(play);

        /* Set the collected handler */
        this->actionFunc = EnExRuppy_HandlerCollected;
        this->actionFunc(this, play);
        this->actor.draw = NULL;
    }
    else
    {
        divingGame->grabbedRupeesCounter++;
        Actor_Kill(&this->actor);
    }

    comboAddItemEx(play, &q, major);
    Flags_SetCollectible(play, this->colorIdx + 1);

    /* Play the sound */
    comboPlayItemFanfare(o.gi, 1);
}

void EnExRuppy_InitWrapper(Actor_EnExRuppy* this, PlayState* play)
{
    ActorCallback init;

    init = actorAddr(ACTOR_EN_EX_RUPPY, 0x80a8ab70);
    init(&this->actor, play);

    if (this->type == 0 && EnExRupy_IsShuffled(this, play))
    {
        this->actor.draw = EnExRuppy_Draw;
    }
}

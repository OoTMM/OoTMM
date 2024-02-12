#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>

void EnExRuppy_ItemQuery(ComboItemQuery* q, Actor_EnExRuppy* this, GameState_Play* play)
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
        q->gi = GI_OOT_RUPEE_HUGE; // TODO GI_OOT_RUPEE_500?
        break;
    case 0x04:
        q->gi = GI_OOT_RUPEE_PURPLE;
        break;
    default:
        break;
    }
    q->giRenew = q->gi;
    if (GetCollectibleFlag(play, q->id)) {
        q->ovFlags |= OVF_RENEW;
    }
}

void EnExRuppy_Draw(Actor_EnExRuppy* this, GameState_Play* play)
{
    ComboItemQuery q;
    ComboItemOverride o;

    EnExRuppy_ItemQuery(&q, this, play);
    comboItemOverride(&o, &q);
    ModelViewScale(25.0f, 25.0f, 25.0f, MAT_MUL);
    comboDrawGI(play, &this->actor, o.gi, 0);
}

void EnExRuppy_HandlerCollected(Actor_EnExRuppy* this, GameState_Play* play)
{
    Actor_EnDivingGame* divingGame;
    if (Message_IsClosed(&this->actor, play))
    {
        divingGame = (Actor_EnDivingGame*)this->actor.attachedA;
        divingGame->grabbedRupeesCounter++;
        UnfreezePlayer(play);
        ActorDestroy(&this->actor);
    }
    else
    {
        FreezePlayer(play);
    }
}

void EnExRuppy_GiveItem(Actor_EnExRuppy* this, GameState_Play* play, Actor_EnDivingGame* divingGame)
{
    Actor_Player* link;
    ComboItemQuery q;
    ComboItemOverride o;
    int major;

    link = GET_LINK(play);
    if (link->state & (PLAYER_ACTOR_STATE_FROZEN | PLAYER_ACTOR_STATE_EPONA))
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
        FreezePlayer(play);

        /* Set the collected handler */
        this->actionFunc = EnExRuppy_HandlerCollected;
        this->actionFunc(this, play);
        this->actor.draw = NULL;
    }
    else
    {
        divingGame->grabbedRupeesCounter++;
        ActorDestroy(&this->actor);
    }

    comboAddItemEx(play, &q, major);
    SetCollectibleFlag(play, this->colorIdx + 1);

    /* Play the sound */
    comboPlayItemFanfare(o.gi, 1);
}

void EnExRuppy_InitWrapper(Actor_EnExRuppy* this, GameState_Play* play)
{
    ActorCallback init = actorAddr(AC_EN_EX_RUPPY, 0x80a8ab70);
    init(&this->actor, play);

    if (this->type == 0) {
        this->actor.draw = EnExRuppy_Draw;
    }
}

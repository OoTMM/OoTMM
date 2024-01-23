#include <combo.h>
#include <combo/item.h>

static void ItemOcarina_ItemQuery(ComboItemQuery* q, int index)
{
    bzero(q, sizeof(*q));

    q->ovType = OV_NPC;
    q->gi = index == 0 ? GI_OOT_OCARINA_TIME : GI_OOT_SONG_TIME;
    q->id = index == 0 ? NPC_OOT_OCARINA_TIME_ITEM : NPC_OOT_OCARINA_TIME_SONG;
}

static void ItemOcarina_ItemOverride(ComboItemOverride* o, int index)
{
    ComboItemQuery q;

    ItemOcarina_ItemQuery(&q, index);
    comboItemOverride(o, &q);
}

void ItemOcarina_HandlerItem2(Actor* this, GameState_Play* play)
{
    if (Message_IsClosed(this, play))
    {
        play->transitionTrigger = TRANS_TYPE_NORMAL;
        play->transitionType = TRANS_GFX_SHORTCUT;
        play->nextEntranceIndex = 0x050f;
        ActorDestroy(this);
    }
}

void ItemOcarina_HandlerItem(Actor* this, GameState_Play* play)
{
    ComboItemQuery q;

    if (Actor_HasParent(this))
    {
        SetEventChk(EV_OOT_CHK_SONG_TIME);
        SetSwitchFlag(play, 3);
        this->draw = NULL;
        this->update = ItemOcarina_HandlerItem2;
        ItemOcarina_HandlerItem2(this, play);
        return;
    }

    ItemOcarina_ItemQuery(&q, 0);
    comboGiveItem(this, play, &q, 30.f, 50.f);
}

void ItemOcarina_HandlerSong(Actor* this, GameState_Play* play)
{
    ComboItemQuery q;

    if (Actor_HasParent(this))
    {
        SetEventChk(EV_OOT_CHK_OCARINA_OF_TIME);
        ActorDestroy(this);
        return;
    }

    ItemOcarina_ItemQuery(&q, 1);
    comboGiveItem(this, play, &q, 10000.f, 1000.f);
}

void ItemOcarina_Handler(Actor* this, GameState_Play* play)
{
    /* We invert the two events or else the game will not respawn the ocarina */
    if (GetEventChk(EV_OOT_CHK_SONG_TIME))
    {
        this->update = ItemOcarina_HandlerSong;
        this->draw = NULL;
    }
    else
    {
        this->update = ItemOcarina_HandlerItem;
    }
}

void ItemOcarina_Draw(Actor* this, GameState_Play* play)
{
    ComboItemOverride o;

    ItemOcarina_ItemOverride(&o, 0);
    comboDrawGI(play, this, o.gi, 0);
}

PATCH_FUNC(0x80a2b7c0, ItemOcarina_Handler);
PATCH_FUNC(0x80a2b8b8, ItemOcarina_Draw);

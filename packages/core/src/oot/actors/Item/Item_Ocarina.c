#include <combo.h>
#include <combo/item.h>
#include <combo/draw.h>

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

void ItemOcarina_HandlerItem2(Actor* this, PlayState* play)
{
    if (Message_IsClosed(this, play))
    {
        play->transitionTrigger = TRANS_TRIGGER_START;
        play->transitionType = TRANS_GFX_SHORTCUT;
        play->nextEntranceIndex = 0x050f;
        Actor_Kill(this);
    }
}

void ItemOcarina_HandlerItem(Actor* this, PlayState* play)
{
    ComboItemQuery q;

    if (Actor_HasParentZ(this))
    {
        SetEventChk(EV_OOT_CHK_SONG_TIME);
        Flags_SetSwitch(play, 3);
        this->draw = NULL;
        this->update = ItemOcarina_HandlerItem2;
        ItemOcarina_HandlerItem2(this, play);
        return;
    }

    ItemOcarina_ItemQuery(&q, 0);
    comboGiveItem(this, play, &q, 30.f, 50.f);
}

void ItemOcarina_HandlerSong(Actor* this, PlayState* play)
{
    ComboItemQuery q;

    if (Actor_HasParentZ(this))
    {
        SetEventChk(EV_OOT_CHK_OCARINA_OF_TIME);
        Actor_Kill(this);
        return;
    }

    ItemOcarina_ItemQuery(&q, 1);
    comboGiveItem(this, play, &q, 10000.f, 1000.f);
}

void ItemOcarina_Handler(Actor* this, PlayState* play)
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

void ItemOcarina_Draw(Actor* this, PlayState* play)
{
    ComboItemOverride o;

    ItemOcarina_ItemOverride(&o, 0);
    Draw_GiCloaked(play, this, o.gi, o.cloakGi, 0);
}

PATCH_FUNC(0x80a2b7c0, ItemOcarina_Handler);
PATCH_FUNC(0x80a2b8b8, ItemOcarina_Draw);

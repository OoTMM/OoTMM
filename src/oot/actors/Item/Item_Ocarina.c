#include <combo.h>

void ItemOcarina_HandlerItem2(Actor* this, GameState_Play* play)
{
    TransitionContext* t;

    if (Message_IsClosed(this, play))
    {
        t = &play->transition;
        t->type = TRANS_TYPE_NORMAL;
        t->gfx = TRANS_GFX_SHORTCUT;
        t->entrance = 0x050f;
        ActorDestroy(this);
    }
}

void ItemOcarina_HandlerItem(Actor* this, GameState_Play* play)
{
    s16 gi;

    if (Actor_HasParent(this))
    {
        SetEventChk(EV_OOT_CHK_SONG_TIME);
        SetSwitchFlag(play, 3);
        this->draw = NULL;
        this->update = ItemOcarina_HandlerItem2;
        ItemOcarina_HandlerItem2(this, play);
    }
    else
    {
        gi = comboOverride(OV_NPC, 0, NPC_OOT_OCARINA_TIME_ITEM, GI_OOT_OCARINA_TIME);
        GiveItem(this, play, gi, 30.f, 50.f);
    }
}

void ItemOcarina_HandlerSong(Actor* this, GameState_Play* play)
{
    s16 gi;

    if (Actor_HasParent(this))
    {
        SetEventChk(EV_OOT_CHK_OCARINA_OF_TIME);
        ActorDestroy(this);
    }
    else
    {
        gi = comboOverride(OV_NPC, 0, NPC_OOT_OCARINA_TIME_SONG, GI_OOT_SONG_TIME);
        GiveItem(this, play, gi, 10000.f, 1000.f);
    }
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
    s16 gi;

    gi = comboOverride(OV_NPC, 0, NPC_OOT_OCARINA_TIME_ITEM, GI_OOT_OCARINA_TIME);
    comboDrawGI(play, this, gi, 0);
}

PATCH_FUNC(0x80a2b7c0, ItemOcarina_Handler);
PATCH_FUNC(0x80a2b8b8, ItemOcarina_Draw);

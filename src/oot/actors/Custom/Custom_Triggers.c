#include <combo.h>

#define TRIGGER_NONE            0
#define TRIGGER_SHEIK_COLOSSUS  1


Actor_CustomTriggers* gActorCustomTriggers;

static int CustomTriggers_GiveItem(Actor_CustomTriggers* this, GameState_Play* play, s16 gi)
{
    Actor_Player* link;

    link = GET_LINK(play);
    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return 0;
    if (Actor_HasParent(this))
    {
        this->base.attachedA = NULL;
        return 1;
    }
    GiveItem(&this->base, play, gi, 10000.f, 10000.f);
    return 0;
}

static int CustomTriggers_GiveItemNpc(Actor_CustomTriggers* this, GameState_Play* play, s16 gi, s16 npcId)
{
    gi = comboOverride(OV_NPC, 0, npcId, gi);
    return CustomTriggers_GiveItem(this, play, gi);
}

static void CustomTriggers_Init(Actor_CustomTriggers* this, GameState_Play* play)
{
    this->trigger = TRIGGER_NONE;
    gActorCustomTriggers = this;
}

static void CustomTriggers_HandleTrigger(Actor_CustomTriggers* this, GameState_Play* play)
{
    switch (this->trigger)
    {
    case TRIGGER_SHEIK_COLOSSUS:
        if (CustomTriggers_GiveItemNpc(this, play, GI_OOT_SONG_TP_SPIRIT, NPC_OOT_SHEIK_SPIRIT))
        {
            SetEventChk(EV_OOT_CHK_SONG_TP_SPIRIT);
            this->trigger = TRIGGER_NONE;
        }
        break;
    }
}

static void CustomTriggers_CheckTrigger(Actor_CustomTriggers* this, GameState_Play* play)
{
    /* Sheik in colossus */
    if (gSave.entrance == 0x1e1 && !GetEventChk(EV_OOT_CHK_SONG_TP_SPIRIT))
    {
        this->trigger = TRIGGER_SHEIK_COLOSSUS;
        return;
    }
}

static void CustomTriggers_Update(Actor_CustomTriggers* this, GameState_Play* play)
{
    if (this->trigger == TRIGGER_NONE)
        CustomTriggers_CheckTrigger(this, play);
    if (this->trigger != TRIGGER_NONE)
        CustomTriggers_HandleTrigger(this, play);
}

ActorInit CustomTriggers_gActorInit = {
    AC_CUSTOM_TRIGGERS,
    0x6,
    0x10,
    0x1,
    sizeof(Actor_CustomTriggers),
    (ActorFunc)CustomTriggers_Init,
    NULL,
    (ActorFunc)CustomTriggers_Update,
    NULL,
};

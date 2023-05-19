#include <combo.h>

#define TRIGGER_NONE            0x00
#define TRIGGER_GANON_BK        0x01
#define TRIGGER_TRIFORCE        0x02

#if defined(GAME_OOT)
# define GI_OOT 0
# define GI_MM  MASK_FOREIGN_GI
#else
# define GI_OOT MASK_FOREIGN_GI
# define GI_MM  0
#endif

void CustomTriggers_HandleTriggerGame(Actor_CustomTriggers* this, GameState_Play* play);
void CustomTriggers_CheckTriggerGame(Actor_CustomTriggers* this, GameState_Play* play);

Actor_CustomTriggers* gActorCustomTriggers;

int CustomTriggers_GiveItem(Actor_CustomTriggers* this, GameState_Play* play, s16 gi)
{
    Actor_Player* link;

    link = GET_LINK(play);
    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return 0;
    if (Actor_HasParent(&this->base))
    {
        this->base.attachedA = NULL;
        return 1;
    }
    GiveItem(&this->base, play, gi, 10000.f, 10000.f);
    return 0;
}

int CustomTriggers_GiveItemNpc(Actor_CustomTriggers* this, GameState_Play* play, s16 gi, s16 npcId)
{
    gi = comboOverride(OV_NPC, 0, npcId, gi);
    return CustomTriggers_GiveItem(this, play, gi);
}

int CustomTrigger_ItemSafe(Actor_CustomTriggers* this, GameState_Play* play)
{
    Actor_Player* link;

    link = GET_LINK(play);
    if (link->state & (PLAYER_ACTOR_STATE_GET_ITEM | PLAYER_ACTOR_STATE_CUTSCENE_FROZEN))
    {
        this->acc = 0;
        return 0;
    }
    this->acc++;
    if (this->acc > 3)
        return 1;
    return 0;
}

static void CustomTriggers_HandleTrigger(Actor_CustomTriggers* this, GameState_Play* play)
{
    switch (this->trigger)
    {
    case TRIGGER_GANON_BK:
        if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItem(this, play, GI_OOT | GI_OOT_BOSS_KEY_GANON))
        {
            gOotExtraFlags.ganonBossKey = 1;
            this->trigger = TRIGGER_NONE;
        }
        break;
    case TRIGGER_TRIFORCE:
        if (CustomTrigger_ItemSafe(this, play))
        {
            gOotExtraFlags.triforceWin = 1;
            this->trigger = TRIGGER_NONE;
            comboCreditWarp(play);
        }
        break;
    }

    CustomTriggers_HandleTriggerGame(this, play);
}

static void CustomTriggers_CheckTrigger(Actor_CustomTriggers* this, GameState_Play* play)
{
    /* Ganon BK */
    if (comboConfig(CFG_OOT_GANON_BK_CUSTOM) && !gOotExtraFlags.ganonBossKey && comboSpecialCond(SPECIAL_GANON_BK))
    {
        this->acc = 0;
        this->trigger = TRIGGER_GANON_BK;
        return;
    }

    /* Triforce */
    if (comboConfig(CFG_GOAL_TRIFORCE) && !gOotExtraFlags.triforceWin && gOotExtraFlags.triforceCount >= 20)
    {
        this->acc = 0;
        this->trigger = TRIGGER_TRIFORCE;
        return;
    }

    CustomTriggers_CheckTriggerGame(this, play);
}

static void CustomTriggers_Init(Actor_CustomTriggers* this, GameState_Play* play)
{
    this->trigger = TRIGGER_NONE;
    gActorCustomTriggers = this;
}

static void CustomTriggers_Update(Actor_CustomTriggers* this, GameState_Play* play)
{
    /* Always be near link */
    Actor_Player* link;
    link = GET_LINK(play);
    if (link)
    {
        this->base.position.x = link->base.position.x;
        this->base.position.y = link->base.position.y;
        this->base.position.z = link->base.position.z;
    }

    if (this->trigger == TRIGGER_NONE)
        CustomTriggers_CheckTrigger(this, play);
    if (this->trigger != TRIGGER_NONE)
        CustomTriggers_HandleTrigger(this, play);
}

ActorInit CustomTriggers_gActorInit = {
    AC_CUSTOM_TRIGGERS,
    0x8,
    0x10,
    0x1,
    sizeof(Actor_CustomTriggers),
    (ActorFunc)CustomTriggers_Init,
    NULL,
    (ActorFunc)CustomTriggers_Update,
    NULL,
};

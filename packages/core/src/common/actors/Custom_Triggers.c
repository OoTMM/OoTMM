#include <combo.h>
#include <combo/item.h>
#include <combo/net.h>
#include <combo/player.h>
#include <combo/config.h>
#include <combo/multi.h>
#include <combo/actor.h>

#define TRIGGER_NONE            0x00
#define TRIGGER_GANON_BK        0x01
#define TRIGGER_TRIFORCE        0x02

#if defined(GAME_OOT)
# define RECOVERY_HEART GI_OOT_RECOVERY_HEART
#else
# define RECOVERY_HEART GI_MM_RECOVERY_HEART
#endif

void CustomTriggers_HandleTriggerGame(Actor_CustomTriggers* this, PlayState* play);
void CustomTriggers_CheckTriggerGame(Actor_CustomTriggers* this, PlayState* play);

Actor_CustomTriggers* gActorCustomTriggers;
ComboTriggersData gComboTriggersData;

int CustomTriggers_GiveItem(Actor_CustomTriggers* this, PlayState* play, const ComboItemQuery* q)
{
    Player* link;

    link = GET_PLAYER(play);
    if (link->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM)
        return 0;

    if (Actor_HasParentZ(&this->base))
    {
        this->base.parent = NULL;
        return 1;
    }

    comboGiveItem(&this->base, play, q, 10000.f, 10000.f);
    return 0;
}

int CustomTriggers_GiveItemNpc(Actor_CustomTriggers* this, PlayState* play, s16 gi, int npc)
{
    ComboItemQuery q = ITEM_QUERY_INIT;

    q.ovType = OV_NPC;
    q.gi = gi;
    q.id = npc;

    return CustomTriggers_GiveItem(this, play, &q);
}

int CustomTriggers_GiveItemDirect(Actor_CustomTriggers* this, PlayState* play, s16 gi)
{
    ComboItemQuery q = ITEM_QUERY_INIT;
    q.gi = gi;
    return CustomTriggers_GiveItem(this, play, &q);
}

int CustomTrigger_ItemSafe(Actor_CustomTriggers* this, PlayState* play)
{
    Player* link;

    link = GET_PLAYER(play);
    if (link->stateFlags1 & (PLAYER_ACTOR_STATE_GET_ITEM | PLAYER_ACTOR_STATE_CUTSCENE_FROZEN))
    {
        gComboTriggersData.acc = 0;
        return 0;
    }

#if defined(GAME_OOT)
    if (play->sceneId == SCE_OOT_BOMBCHU_BOWLING_ALLEY)
        return 0;
#endif

    gComboTriggersData.acc++;
    if (gComboTriggersData.acc > 3)
        return 1;
    return 0;
}

static void CustomTriggers_HandleTrigger(Actor_CustomTriggers* this, PlayState* play)
{
    switch (gComboTriggersData.trigger)
    {
    case TRIGGER_GANON_BK:
        if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_BOSS_KEY_GANON))
        {
            gOotExtraFlags.ganonBossKey = 1;
            gComboTriggersData.trigger = TRIGGER_NONE;
        }
        break;
    case TRIGGER_TRIFORCE:
        if (CustomTrigger_ItemSafe(this, play))
        {
            gOotExtraFlags.triforceWin = 1;
            gComboTriggersData.trigger = TRIGGER_NONE;
            comboCreditWarp(play);
        }
        break;
    }

    CustomTriggers_HandleTriggerGame(this, play);
}

static void CustomTriggers_CheckTrigger(Actor_CustomTriggers* this, PlayState* play)
{
    /* Ganon BK */
    if (Config_Flag(CFG_OOT_GANON_BK_CUSTOM) && !gOotExtraFlags.ganonBossKey && Config_SpecialCond(SPECIAL_GANON_BK))
    {
        gComboTriggersData.acc = 0;
        gComboTriggersData.trigger = TRIGGER_GANON_BK;
        return;
    }

    /* Triforce (Hunt) */
    if (Config_Flag(CFG_GOAL_TRIFORCE) && !gOotExtraFlags.triforceWin && gTriforceCount >= gComboConfig.triforceGoal)
    {
        gComboTriggersData.acc = 0;
        gComboTriggersData.trigger = TRIGGER_TRIFORCE;
        return;
    }

    /* Triforce (Quest) */
    if (Config_Flag(CFG_GOAL_TRIFORCE3) && !gOotExtraFlags.triforceWin && gTriforceCount >= 3)
    {
        gComboTriggersData.acc = 0;
        gComboTriggersData.trigger = TRIGGER_TRIFORCE;
        return;
    }

    /* Endgame Item Win */
    if (gOotExtraFlags.endgameItemIsWin && !gOotExtraFlags.triforceWin)
    {
        gComboTriggersData.acc = 0;
        gComboTriggersData.trigger = TRIGGER_TRIFORCE;
        return;
    }

    CustomTriggers_CheckTriggerGame(this, play);
}

static void CustomTriggers_Init(Actor_CustomTriggers* this, PlayState* play)
{
    this->base.room = 0xff;
}

static void CustomTriggers_Fini(Actor_CustomTriggers* this, PlayState* play)
{
    gActorCustomTriggers = NULL;
}

static void CustomTriggers_Update(Actor_CustomTriggers* this, PlayState* play)
{
    /* Always be near link */
    Player* link;
    link = GET_PLAYER(play);
    if (link)
    {
        this->base.world.pos.x = link->actor.world.pos.x;
        this->base.world.pos.y = link->actor.world.pos.y;
        this->base.world.pos.z = link->actor.world.pos.z;
    }

    if (gComboTriggersData.trigger == TRIGGER_NONE)
        CustomTriggers_CheckTrigger(this, play);
    if (gComboTriggersData.trigger != TRIGGER_NONE)
        CustomTriggers_HandleTrigger(this, play);
}

void CustomTriggers_Spawn(PlayState* play)
{
    if (gActorCustomTriggers)
        return;

    bzero(&gComboTriggersData, sizeof(gComboTriggersData));

    gActorCustomTriggers = (Actor_CustomTriggers*)Actor_Spawn(
        &play->actorCtx,
        play,
        ACTOR_CUSTOM_TRIGGERS,
        0, 0, 0,
        0, 0, 0,
        0
    );
}

void CustomTriggers_Draw(Actor* this, PlayState* play)
{
}

ActorProfile CustomTriggers_gActorProfile = {
    ACTOR_CUSTOM_TRIGGERS,
    0x8,
    0x10,
    0x1,
    sizeof(Actor_CustomTriggers),
    (ActorFunc)CustomTriggers_Init,
    (ActorFunc)CustomTriggers_Fini,
    (ActorFunc)CustomTriggers_Update,
    (ActorFunc)CustomTriggers_Draw,
};

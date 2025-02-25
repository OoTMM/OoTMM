#include <stdlib.h>
#include <string.h>
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
#define TRIGGER_SONG_STORMS     0x03
#define TRIGGER_SONG_FOREST     0x04

#if defined(GAME_OOT)
# define RECOVERY_HEART GI_OOT_RECOVERY_HEART
#else
# define RECOVERY_HEART GI_MM_RECOVERY_HEART
#endif

void CustomTriggers_HandleTriggerGame(Actor_CustomTriggers* this, PlayState* play);
void CustomTriggers_CheckTriggerGame(Actor_CustomTriggers* this, PlayState* play);

Actor_CustomTriggers* gActorCustomTriggers;
ComboTriggersData gComboTriggersData;
TriggerArray gCustomTriggers;

void CustomTriggers_AddTrigger(TriggerArray* triggers, int value) {
    if (triggers->size >= triggers->capacity) {
        size_t new_capacity = triggers->capacity * 2;
        int* new_array = malloc(new_capacity * sizeof(int));
        // Copy old data to the new memory
        memcpy(new_array, triggers->array, triggers->size * sizeof(int));

        // Free old memory and update pointer
        free(triggers->array);
        triggers->array = new_array;
        triggers->capacity = new_capacity;
    }

    // Add the new trigger value
    triggers->array[triggers->size++] = value;
}

void CustomTriggers_RemoveTrigger(TriggerArray* triggers, int value) {
    size_t i;
    for (i = 0; i < triggers->size; i++) {
        if (triggers->array[i] == value) {
            // Shift elements left
            for (size_t j = i; j < triggers->size - 1; j++) {
                triggers->array[j] = triggers->array[j + 1];
            }
            triggers->size--;
            return;
        }
    }
}

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
    case TRIGGER_SONG_STORMS:
        if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_STORMS))
        {
            gOotSave.info.inventory.quest.songStorms = 1;
            gComboTriggersData.trigger = TRIGGER_NONE;
        }
        break;

    case TRIGGER_SONG_FOREST:
        if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT_SONG_TP_FOREST))
        {
            gOotSave.info.inventory.quest.songTpForest = 1;
            gComboTriggersData.trigger = TRIGGER_NONE;
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

static void CustomTriggers_HandleBulkTrigger(Actor_CustomTriggers* this, PlayState* play, int trigger)
{
    gComboTriggersData.trigger = trigger;
    CustomTriggers_HandleTrigger(this, play);
    if (gComboTriggersData.trigger == TRIGGER_NONE) {
        CustomTriggers_RemoveTrigger(&gCustomTriggers, trigger);
    }
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

    if (gCustomTriggers.size > 0) {
        int trigger = gCustomTriggers.array[0];
        CustomTriggers_HandleBulkTrigger(this, play, trigger);
    }
}

static void CustomTriggers_InitTriggerArray(TriggerArray* triggers, size_t initialCapacity) {

    triggers->array = malloc(initialCapacity * sizeof(int));
    triggers->size = 0;
    triggers->capacity = initialCapacity;
}

void CustomTriggers_Spawn(PlayState* play)
{
    if (gActorCustomTriggers)
        return;

    CustomTriggers_InitTriggerArray(&gCustomTriggers, 1);

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

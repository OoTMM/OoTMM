#include <combo.h>

#define TRIGGER_NONE                    0
#define TRIGGER_SHEIK_COLOSSUS          1
#define TRIGGER_SHEIK_KAKARIKO          2
#define TRIGGER_SARIA_OCARINA           3
#define TRIGGER_ZELDA_LIGHT_ARROW       4
#define TRIGGER_WEIRD_EGG               5
#define TRIGGER_POCKET_EGG              6
#define TRIGGER_GANON_BK                7

Actor_CustomTriggers* gActorCustomTriggers;

static int CustomTriggers_GiveItem(Actor_CustomTriggers* this, GameState_Play* play, s16 gi)
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

static int CustomTrigger_ItemSafe(Actor_CustomTriggers* this, GameState_Play* play)
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
    case TRIGGER_SHEIK_COLOSSUS:
        if (CustomTriggers_GiveItemNpc(this, play, GI_OOT_SONG_TP_SPIRIT, NPC_OOT_SHEIK_SPIRIT))
        {
            SetEventChk(EV_OOT_CHK_SONG_TP_SPIRIT);
            this->trigger = TRIGGER_NONE;
        }
        break;
    case TRIGGER_SHEIK_KAKARIKO:
        if (CustomTriggers_GiveItemNpc(this, play, GI_OOT_SONG_TP_SHADOW, NPC_OOT_SHEIK_SHADOW))
        {
            SetEventChk(EV_OOT_CHK_SONG_TP_SHADOW);
            SetEventChk(EV_OOT_CHK_BONGO_ESCAPE);
            this->trigger = TRIGGER_NONE;
        }
        break;
    case TRIGGER_SARIA_OCARINA:
        if (CustomTriggers_GiveItemNpc(this, play, GI_OOT_OCARINA_FAIRY, NPC_OOT_SARIA_OCARINA))
        {
            SetEventChk(EV_OOT_CHK_SARIA_OCARINA);
            this->trigger = TRIGGER_NONE;
        }
        break;
    case TRIGGER_ZELDA_LIGHT_ARROW:
        if (CustomTriggers_GiveItemNpc(this, play, GI_OOT_ARROW_LIGHT, NPC_OOT_ZELDA_LIGHT_ARROW))
        {
            SetEventChk(EV_OOT_CHK_LIGHT_ARROW);
            this->trigger = TRIGGER_NONE;
        }
        break;
    case TRIGGER_WEIRD_EGG:
        if (CustomTriggers_GiveItemNpc(this, play, GI_OOT_CHICKEN, NPC_OOT_WEIRD_EGG))
        {
            comboRemoveTradeItemChild(XITEM_OOT_CHILD_WEIRD_EGG);
            this->trigger = TRIGGER_NONE;
            this->events.weirdEgg = 0;
        }
        break;
    case TRIGGER_POCKET_EGG:
        if (CustomTriggers_GiveItemNpc(this, play, GI_OOT_POCKET_CUCCO, NPC_OOT_POCKET_EGG))
        {
            comboRemoveTradeItemAdult(XITEM_OOT_ADULT_POCKET_EGG);
            this->trigger = TRIGGER_NONE;
            this->events.pocketEgg = 0;
        }
        break;
    case TRIGGER_GANON_BK:
        if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItem(this, play, GI_OOT_BOSS_KEY_GANON))
        {
            gOotExtraFlags.ganonBossKey = 1;
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

    /* Sheik in Kakariko */
    if (play->sceneId == SCE_OOT_KAKARIKO_VILLAGE && gSave.inventory.quest.medallionForest && gSave.inventory.quest.medallionFire && gSave.inventory.quest.medallionWater && gSave.age == AGE_ADULT && !GetEventChk(EV_OOT_CHK_SONG_TP_SHADOW))
    {
        this->trigger = TRIGGER_SHEIK_KAKARIKO;
        return;
    }

    /* Saria's Ocarina */
    if (gSave.entrance == 0x05e0 && !GetEventChk(EV_OOT_CHK_SARIA_OCARINA))
    {
        this->trigger = TRIGGER_SARIA_OCARINA;
        return;
    }

    /* Zelda Light Arrows */
    if (play->sceneId == SCE_OOT_TEMPLE_OF_TIME && gSave.age == AGE_ADULT && !GetEventChk(EV_OOT_CHK_LIGHT_ARROW))
    {
        int shouldTrigger;
        if (comboConfig(CFG_OOT_LACS_CUSTOM))
            shouldTrigger = comboSpecialCond(SPECIAL_LACS);
        else
            shouldTrigger = (gSave.inventory.quest.medallionSpirit && gSave.inventory.quest.medallionShadow);

        if (shouldTrigger)
            this->trigger = TRIGGER_ZELDA_LIGHT_ARROW;
        return;
    }

    /* Weird egg */
    if (this->events.weirdEgg)
    {
        this->trigger = TRIGGER_WEIRD_EGG;
        this->events.weirdEgg = 0;
        return;
    }

    /* Pocket egg */
    if (this->events.pocketEgg)
    {
        this->trigger = TRIGGER_POCKET_EGG;
        this->events.pocketEgg = 0;
        return;
    }

    /* Ganon BK */
    if (comboConfig(CFG_OOT_GANON_BK_CUSTOM) && !gOotExtraFlags.ganonBossKey && comboSpecialCond(SPECIAL_GANON_BK))
    {
        this->acc = 0;
        this->trigger = TRIGGER_GANON_BK;
        return;
    }
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

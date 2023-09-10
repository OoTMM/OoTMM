#include <combo.h>
#include <combo/item.h>
#include <combo/net.h>

#define TRIGGER_NONE            0x00
#define TRIGGER_GANON_BK        0x01
#define TRIGGER_TRIFORCE        0x02
#define TRIGGER_NET             0x03

#if defined(GAME_OOT)
# define GI_OOT 0
# define GI_MM  MASK_FOREIGN_GI
# define RECOVERY_HEART GI_OOT_RECOVERY_HEART
#else
# define GI_OOT MASK_FOREIGN_GI
# define GI_MM  0
# define RECOVERY_HEART GI_MM_RECOVERY_HEART
#endif

void CustomTriggers_HandleTriggerGame(Actor_CustomTriggers* this, GameState_Play* play);
void CustomTriggers_CheckTriggerGame(Actor_CustomTriggers* this, GameState_Play* play);

Actor_CustomTriggers* gActorCustomTriggers;
ComboTriggersData gComboTriggersData;

int CustomTriggers_GiveItem(Actor_CustomTriggers* this, GameState_Play* play, const ComboItemQuery* q)
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

    comboGiveItem(&this->base, play, q, 10000.f, 10000.f);
    return 0;
}

int CustomTriggers_GiveItemNpc(Actor_CustomTriggers* this, GameState_Play* play, s16 gi, int npc)
{
    ComboItemQuery q = ITEM_QUERY_INIT;

    q.ovType = OV_NPC;
    q.gi = gi;
    q.id = npc;

    return CustomTriggers_GiveItem(this, play, &q);
}

int CustomTriggers_GiveItemDirect(Actor_CustomTriggers* this, GameState_Play* play, s16 gi)
{
    ComboItemQuery q = ITEM_QUERY_INIT;
    q.gi = gi;
    return CustomTriggers_GiveItem(this, play, &q);
}

int CustomTriggers_GiveItemNet(Actor_CustomTriggers* this, GameState_Play* play, s16 gi, u8 from, int flags)
{
    ComboItemQuery q = ITEM_QUERY_INIT;

    q.gi = gi;
    q.from = from;

    if ((flags & OVF_PRECOND) && (!isItemLicensed(gi)))
    {
        bzero(&q, sizeof(q));
        q.ovType = OV_NONE;
        q.gi = RECOVERY_HEART;
    }

    return CustomTriggers_GiveItem(this, play, &q);
}

int CustomTrigger_ItemSafe(Actor_CustomTriggers* this, GameState_Play* play)
{
    Actor_Player* link;

    link = GET_LINK(play);
    if (link->state & (PLAYER_ACTOR_STATE_GET_ITEM | PLAYER_ACTOR_STATE_CUTSCENE_FROZEN))
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

int CustomTrigger_ItemSafeNet(Actor_CustomTriggers* this, GameState_Play* play)
{
    Actor_Player* link;

    link = GET_LINK(play);
    if (link->state & (PLAYER_ACTOR_STATE_GET_ITEM | PLAYER_ACTOR_STATE_CUTSCENE_FROZEN))
    {
        gComboTriggersData.acc = 0;
        return 0;
    }

    switch (play->sceneId)
    {
#if defined(GAME_OOT)
    case SCE_OOT_BOMBCHU_BOWLING_ALLEY:
    case SCE_OOT_TREASURE_SHOP:
    case SCE_OOT_SHOOTING_GALLERY:
    case SCE_OOT_KOKIRI_SHOP:
    case SCE_OOT_ZORA_SHOP:
    case SCE_OOT_GORON_SHOP:
    case SCE_OOT_BOMBCHU_SHOP:
    case SCE_OOT_MARKET_POTION_SHOP:
    case SCE_OOT_KAKARIKO_POTION_SHOP:
    case SCE_OOT_BAZAAR:
#endif
#if defined(GAME_MM)
    case SCE_MM_BOMB_SHOP:
    case SCE_MM_CURIOSITY_SHOP:
    case SCE_MM_POTION_SHOP:
    case SCE_MM_TRADING_POST:
    case SCE_MM_GORON_SHOP:
    case SCE_MM_ZORA_HALL_ROOMS:
    case SCE_MM_SHOOTING_GALLERY:
    case SCE_MM_SHOOTING_GALLERY_SWAMP:
    case SCE_MM_HONEY_DARLING:
    case SCE_MM_TREASURE_SHOP:
    case SCE_MM_WATERFALL_RAPIDS:
#endif
        return 0;

    default:
        break;
    }

    gComboTriggersData.acc++;
    if (gComboTriggersData.acc > 3)
        return 1;
    return 0;
}

static void CustomTriggers_HandleTrigger(Actor_CustomTriggers* this, GameState_Play* play)
{
    NetContext* net;
    s16 gi;

    switch (gComboTriggersData.trigger)
    {
    case TRIGGER_GANON_BK:
        if (CustomTrigger_ItemSafe(this, play) && CustomTriggers_GiveItemDirect(this, play, GI_OOT | GI_OOT_BOSS_KEY_GANON))
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
    case TRIGGER_NET:
        net = netMutexLock();
        gi = net->cmdIn.itemRecv.gi;
#if defined(GAME_MM)
        gi ^= MASK_FOREIGN_GI;
#endif
        if (CustomTrigger_ItemSafeNet(this, play) && CustomTriggers_GiveItemNet(this, play, gi, net->cmdIn.itemRecv.playerFrom, net->cmdIn.itemRecv.flags))
        {
            bzero(&net->cmdIn, sizeof(net->cmdIn));
            gComboTriggersData.trigger = TRIGGER_NONE;
            gSaveLedgerBase++;
            net->ledgerBase = gSaveLedgerBase;
        }
        netMutexUnlock();
        break;
    }

    CustomTriggers_HandleTriggerGame(this, play);
}

static void CustomTriggers_CheckTrigger(Actor_CustomTriggers* this, GameState_Play* play)
{
    NetContext* net;

    /* Ganon BK */
    if (comboConfig(CFG_OOT_GANON_BK_CUSTOM) && !gOotExtraFlags.ganonBossKey && comboSpecialCond(SPECIAL_GANON_BK))
    {
        gComboTriggersData.acc = 0;
        gComboTriggersData.trigger = TRIGGER_GANON_BK;
        return;
    }

    /* Triforce (Hunt) */
    if (comboConfig(CFG_GOAL_TRIFORCE) && !gOotExtraFlags.triforceWin && gTriforceCount >= gComboData.triforceGoal)
    {
        gComboTriggersData.acc = 0;
        gComboTriggersData.trigger = TRIGGER_TRIFORCE;
        return;
    }

    /* Triforce (Quest) */
    if (comboConfig(CFG_GOAL_TRIFORCE3) && !gOotExtraFlags.triforceWin && gTriforceCount >= 3)
    {
        gComboTriggersData.acc = 0;
        gComboTriggersData.trigger = TRIGGER_TRIFORCE;
        return;
    }

    /* Net */
    net = netMutexLock();
    if (net->cmdIn.op == NET_OP_ITEM_RECV)
    {
        if (net->cmdIn.itemRecv.playerTo != gComboData.playerId)
        {
            bzero(&net->cmdIn, sizeof(net->cmdIn));
            gSaveLedgerBase++;
            net->ledgerBase = gSaveLedgerBase;
        }
        else
        {
            gComboTriggersData.acc = 0;
            gComboTriggersData.trigger = TRIGGER_NET;
        }
    }
    netMutexUnlock();

    CustomTriggers_CheckTriggerGame(this, play);
}

static void CustomTriggers_Init(Actor_CustomTriggers* this, GameState_Play* play)
{
}

static void CustomTriggers_Fini(Actor_CustomTriggers* this, GameState_Play* play)
{
    gActorCustomTriggers = NULL;
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

    if (gComboTriggersData.trigger == TRIGGER_NONE)
        CustomTriggers_CheckTrigger(this, play);
    if (gComboTriggersData.trigger != TRIGGER_NONE)
        CustomTriggers_HandleTrigger(this, play);
}

void CustomTriggers_Spawn(GameState_Play* play)
{
    if (gActorCustomTriggers)
        return;

    gActorCustomTriggers = (Actor_CustomTriggers*)SpawnActor(
#if defined(GAME_OOT)
        (char*)play + 0x1c24,
#else
        (char*)play + 0x1ca0,
#endif
        play,
        AC_CUSTOM_TRIGGERS,
        0, 0, 0,
        0, 0, 0,
        0
    );
}

ActorInit CustomTriggers_gActorInit = {
    AC_CUSTOM_TRIGGERS,
    0x8,
    0x10,
    0x1,
    sizeof(Actor_CustomTriggers),
    (ActorFunc)CustomTriggers_Init,
    (ActorFunc)CustomTriggers_Fini,
    (ActorFunc)CustomTriggers_Update,
    NULL,
};

#include <combo.h>
#include <combo/actor.h>
#include <combo/config.h>
#include <combo/dma.h>
#include <combo/dungeon.h>
#include <combo/entrance.h>
#include <combo/global.h>
#include <combo/inventory.h>
#include <combo/io.h>
#include <combo/item.h>
#include <combo/mark.h>
#include <combo/multi.h>
#include <combo/play.h>
#include <combo/shop.h>
#include <combo/checks.h>
#include <combo/xflags.h>
#include <combo/age.h>

#if defined(GAME_OOT)
u16 gMmMaxRupees[] = { 0, 200, 500, 999 };
#else
u16 gOotMaxRupees[] = { 0, 200, 500, 999 };
#endif

const u8 kMaxSticks[] = { 0, 10, 20, 30 };
const u8 kMaxNuts[] = { 0, 20, 30, 40 };
const u8 kMaxBombs[] = { 0, 20, 30, 40 };
const u8 kMaxArrows[] = { 0, 30, 40, 50 };
const u8 kMaxSeeds[] = { 0, 30, 40, 50 };

int Item_SafeToReceive(PlayState* play)
{
    Player* link;

#if defined(GAME_OOT)
    /* Used to track shop interactions, interferes with the message system */
    if (YREG(31) != 0)
        return 0;
    if (play->shootingGalleryStatus)
        return 0;
#endif

    if (play->transitionTrigger || gSaveContext.gameMode || (gSaveContext.minigameState == 1) || Player_InCsMode(play) || Message_GetState(&play->msgCtx))
        return 0;

    link = GET_PLAYER(play);
    if (link->stateFlags1 & (PLAYER_ACTOR_STATE_GET_ITEM | PLAYER_ACTOR_STATE_FROZEN | PLAYER_ACTOR_STATE_CUTSCENE_FROZEN | PLAYER_ACTOR_STATE_EPONA | PLAYER_ACTOR_STATE_GROTTO))
        return 0;

#if defined(GAME_OOT)
    if (link->stateFlags2 & PLAYER_STATE2_CRAWLING)
        return 0;
#endif

    return 1;
}

int Item_IsPlayerSelf(u8 playerId)
{
    if (playerId == PLAYER_SELF || playerId == PLAYER_ALL || playerId == Multi_WorldID())
        return 1;
    return 0;
}

static u8 getForeignBottle(u8 itemId)
{
    switch (itemId)
    {
#if defined(GAME_OOT)
    case ITEM_OOT_BOTTLE_EMPTY:
        return ITEM_MM_BOTTLE_EMPTY;
    case ITEM_OOT_POTION_RED:
        return ITEM_MM_POTION_RED;
    case ITEM_OOT_POTION_GREEN:
        return ITEM_MM_POTION_GREEN;
    case ITEM_OOT_POTION_BLUE:
        return ITEM_MM_POTION_BLUE;
    case ITEM_OOT_FAIRY:
        return ITEM_MM_FAIRY;
    case ITEM_OOT_FISH:
        return ITEM_MM_FISH;
    case ITEM_OOT_MILK:
        return ITEM_MM_MILK;
    case ITEM_OOT_RUTO_LETTER:
        return ITEM_MM_RUTO_LETTER;
    case ITEM_OOT_BLUE_FIRE:
        return ITEM_MM_BLUE_FIRE;
    case ITEM_OOT_BUGS:
        return ITEM_MM_BUGS;
    case ITEM_OOT_BIG_POE:
        return ITEM_MM_BIG_POE;
    case ITEM_OOT_MILK_HALF:
        return ITEM_MM_MILK_HALF;
    case ITEM_OOT_POE:
        return ITEM_MM_POE;
    case ITEM_OOT_MAGIC_MUSHROOM:
        return ITEM_MM_MAGIC_MUSHROOM;
    case ITEM_OOT_CHATEAU:
        return ITEM_MM_CHATEAU;
    case ITEM_OOT_GOLD_DUST:
        return ITEM_MM_GOLD_DUST;
    case ITEM_OOT_SEAHORSE:
        return ITEM_MM_SEAHORSE;
    case ITEM_OOT_DEKU_PRINCESS:
        return ITEM_MM_DEKU_PRINCESS;
    case ITEM_OOT_SPRING_WATER:
        return ITEM_MM_SPRING_WATER;
    case ITEM_OOT_SPRING_WATER_HOT:
        return ITEM_MM_SPRING_WATER_HOT;
    case ITEM_OOT_ZORA_EGG:
        return ITEM_MM_ZORA_EGG;
#else
    case ITEM_MM_BOTTLE_EMPTY:
        return ITEM_OOT_BOTTLE_EMPTY;
    case ITEM_MM_POTION_RED:
        return ITEM_OOT_POTION_RED;
    case ITEM_MM_POTION_GREEN:
        return ITEM_OOT_POTION_GREEN;
    case ITEM_MM_POTION_BLUE:
        return ITEM_OOT_POTION_BLUE;
    case ITEM_MM_FAIRY:
        return ITEM_OOT_FAIRY;
    case ITEM_MM_DEKU_PRINCESS:
        return ITEM_OOT_DEKU_PRINCESS;
    case ITEM_MM_MILK:
        return ITEM_OOT_MILK;
    case ITEM_MM_MILK_HALF:
        return ITEM_OOT_MILK_HALF;
    case ITEM_MM_FISH:
        return ITEM_OOT_FISH;
    case ITEM_MM_BUGS:
        return ITEM_OOT_BUGS;
    case ITEM_MM_BLUE_FIRE:
        return ITEM_OOT_BLUE_FIRE;
    case ITEM_MM_POE:
        return ITEM_OOT_POE;
    case ITEM_MM_BIG_POE:
        return ITEM_OOT_BIG_POE;
    case ITEM_MM_SPRING_WATER:
        return ITEM_OOT_SPRING_WATER;
    case ITEM_MM_SPRING_WATER_HOT:
        return ITEM_OOT_SPRING_WATER_HOT;
    case ITEM_MM_ZORA_EGG:
        return ITEM_OOT_ZORA_EGG;
    case ITEM_MM_GOLD_DUST:
        return ITEM_OOT_GOLD_DUST;
    case ITEM_MM_MAGIC_MUSHROOM:
        return ITEM_OOT_MAGIC_MUSHROOM;
    case ITEM_MM_SEAHORSE:
        return ITEM_OOT_SEAHORSE;
    case ITEM_MM_CHATEAU:
        return ITEM_OOT_CHATEAU;
    case ITEM_MM_RUTO_LETTER:
        return ITEM_OOT_RUTO_LETTER;
#endif
    default:
        return ITEM_NONE;
    }
}

static void syncBottleSlotEquips(u8* buttonItems, u8* cButtonSlots, int slot, u8 oldItem, u8 newItem)
{
    for (int i = 0; i < 3; ++i)
    {
        if (cButtonSlots[i] == slot && buttonItems[i] == oldItem)
            buttonItems[i] = newItem;
    }
}

static void syncBottleSlot(int slot, u8 newItem)
{
    u8 oldItem;

    oldItem = gForeignSave.info.inventory.items[slot];
    if (oldItem == newItem)
        return;

    gForeignSave.info.inventory.items[slot] = newItem;

#if defined(GAME_OOT)
    syncBottleSlotEquips(&gMmSave.info.itemEquips.buttonItems[0][1], &gMmSave.info.itemEquips.cButtonSlots[0][1], slot, oldItem, newItem);
#else
    syncBottleSlotEquips(&gOotSave.info.equips.buttonItems[1], &gOotSave.info.equips.cButtonSlots[0], slot, oldItem, newItem);
    syncBottleSlotEquips(&gOotSave.info.childEquips.buttonItems[1], &gOotSave.info.childEquips.cButtonSlots[0], slot, oldItem, newItem);
    syncBottleSlotEquips(&gOotSave.info.adultEquips.buttonItems[1], &gOotSave.info.adultEquips.cButtonSlots[0], slot, oldItem, newItem);
#endif
}

void comboSyncItems(void)
{
    if (Config_Flag(CFG_SHARED_BOWS))
        gForeignSave.info.inventory.ammo[ITS_FOREIGN_BOW] = gSave.info.inventory.ammo[ITS_NATIVE_BOW];

    if (Config_Flag(CFG_SHARED_SLINGSHOT))
    {
#if defined(GAME_OOT)
        gMmExtraAmmo.slingshotSeeds = gSave.info.inventory.ammo[ITS_NATIVE_SLINGSHOT];
#else
        gForeignSave.info.inventory.ammo[ITS_FOREIGN_SLINGSHOT] = gMmExtraAmmo.slingshotSeeds;
#endif
    }

    if (Config_Flag(CFG_SHARED_BOMB_BAGS))
        gForeignSave.info.inventory.ammo[ITS_FOREIGN_BOMBS] = gSave.info.inventory.ammo[ITS_NATIVE_BOMBS];

    if (Config_Flag(CFG_SHARED_BOMBCHU))
        gForeignSave.info.inventory.ammo[ITS_FOREIGN_BOMBCHU] = gSave.info.inventory.ammo[ITS_NATIVE_BOMBCHU];

    if (Config_Flag(CFG_SHARED_POWDER_KEG))
    {
#if defined(GAME_OOT)
        gForeignSave.info.inventory.ammo[ITS_MM_KEG] = gOotExtraAmmo.kegAmmo;
#else
        gOotExtraAmmo.kegAmmo = gSave.info.inventory.ammo[ITS_MM_KEG];
#endif
    }

    if (Config_Flag(CFG_SHARED_MAGIC))
    {
        gForeignSave.info.playerData.magic = gSave.info.playerData.magic;
#if defined(GAME_OOT)
        if (gCustomSave.chateauActive)
        {
            MM_SET_EVENT_WEEK(EV_MM_WEEK_DRANK_CHATEAU_ROMANI);
        }
        else
        {
            MM_CLEAR_EVENT_WEEK(EV_MM_WEEK_DRANK_CHATEAU_ROMANI);
        }
#else
        if (MM_GET_EVENT_WEEK(EV_MM_WEEK_DRANK_CHATEAU_ROMANI))
        {
            gSharedCustomSave.oot.chateauActive = 1;
        }
        else
        {
            gSharedCustomSave.oot.chateauActive = 0;
        }
#endif
    }

    if (Config_Flag(CFG_SHARED_NUTS_STICKS))
    {
        gForeignSave.info.inventory.ammo[ITS_FOREIGN_NUTS] = gSave.info.inventory.ammo[ITS_NATIVE_NUTS];
        gForeignSave.info.inventory.ammo[ITS_FOREIGN_STICKS] = gSave.info.inventory.ammo[ITS_NATIVE_STICKS];
    }

    if (Config_Flag(CFG_SHARED_WALLETS))
        gForeignSave.info.playerData.rupees = gSave.info.playerData.rupees;

    if (Config_Flag(CFG_SHARED_HEALTH))
    {
        gForeignSave.info.playerData.healthCapacity = gSave.info.playerData.healthCapacity;
        gForeignSave.info.playerData.health = gSave.info.playerData.health;
        gForeignSave.info.inventory.quest.heartPieces = gSave.info.inventory.quest.heartPieces;
    }

    if (Config_Flag(CFG_SHARED_BOTTLES))
    {
        syncBottleSlot(ITS_FOREIGN_BOTTLE, getForeignBottle(gSave.info.inventory.items[ITS_NATIVE_BOTTLE]));
        syncBottleSlot(ITS_FOREIGN_BOTTLE2, getForeignBottle(gSave.info.inventory.items[ITS_NATIVE_BOTTLE2]));
        syncBottleSlot(ITS_FOREIGN_BOTTLE3, getForeignBottle(gSave.info.inventory.items[ITS_NATIVE_BOTTLE3]));
        syncBottleSlot(ITS_FOREIGN_BOTTLE4, getForeignBottle(gSave.info.inventory.items[ITS_NATIVE_BOTTLE4]));
#if defined (GAME_OOT)
        if (gOotExtraTrade.adult & (1 << XITEM_OOT_ADULT_BOTTLE))
        {
            if (comboIsTradeBottleOot(gSave.info.inventory.items[ITS_OOT_TRADE_ADULT]))
            {
                gOotExtraItems.bottleAdultSlot = gSave.info.inventory.items[ITS_OOT_TRADE_ADULT];
            }
            syncBottleSlot(ITS_MM_BOTTLE5, getForeignBottle(gOotExtraItems.bottleAdultSlot));
        }
        if (gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_BOTTLE))
        {
            if (comboIsTradeBottleOot(gSave.info.inventory.items[ITS_OOT_TRADE_CHILD]))
            {
                gOotExtraItems.bottleChildSlot = gSave.info.inventory.items[ITS_OOT_TRADE_CHILD];
            }
            syncBottleSlot(ITS_MM_BOTTLE6, getForeignBottle(gOotExtraItems.bottleChildSlot));
        }
#else
        gOotExtraItems.bottleAdultSlot = getForeignBottle(gSave.info.inventory.items[ITS_MM_BOTTLE5]);
        gOotExtraItems.bottleChildSlot = getForeignBottle(gSave.info.inventory.items[ITS_MM_BOTTLE6]);
        if (comboIsTradeBottleOot(gOotSave.info.inventory.items[ITS_OOT_TRADE_ADULT]))
        {
            syncBottleSlot(ITS_OOT_TRADE_ADULT, gOotExtraItems.bottleAdultSlot);
        }
        if (comboIsTradeBottleOot(gOotSave.info.inventory.items[ITS_OOT_TRADE_CHILD]))
        {
            syncBottleSlot(ITS_OOT_TRADE_CHILD, gOotExtraItems.bottleChildSlot);
        }
#endif
    }

    if (Config_Flag(CFG_CROSS_GAME_FW))
    {
#if defined(GAME_MM)
        u8 fwAge = comboMmFwAge();
        RespawnData* fw = &gCustomSave.fw[fwAge];
        OotFaroreWind* foreignFw = Age_GetFaroreOot(fwAge);

        if (fw->data <= 0 || fw->entrance != ENTR_FW_CROSS)
        {
            foreignFw->set = 0;
        }

        if (fw->data > 0 && foreignFw->set <= 0)
        {
            foreignFw->set = 1;
            foreignFw->entrance = ENTR_FW_CROSS;
        }
#else
        u8 fwAge = gOotSave.age;
        RespawnData* foreignFw = &gSharedCustomSave.mm.fw[fwAge];
        OotFaroreWind* fw = Age_GetFaroreOot(fwAge);

        if (fw->set <= 0 || fw->entrance != ENTR_FW_CROSS)
        {
            foreignFw->data = 0;
        }

        if (fw->set > 0 && foreignFw->data <= 0)
        {
            foreignFw->data = 1;
            foreignFw->entrance = ENTR_FW_CROSS;
        }
#endif
    }
}

int comboItemPrecond(const ComboItemQuery* q, s16 price)
{
    ComboItemOverride o;

    comboSyncItems();
    comboItemOverride(&o, q);

    /* Nothing can never be bought */
    if (o.gi == GI_NOTHING)
        return SC_ERR_CANNOTBUY;

    if (Item_IsPlayerSelf(o.player) && ((!Config_Flag(CFG_MULTIPLAYER) && !isItemBuyable(o.gi)) || !isItemLicensed(o.gi)))
        return SC_ERR_CANNOTBUY;

    if (gSave.info.playerData.rupees < price)
        return SC_ERR_NORUPEES;

    if (isItemFastBuy(o.gi))
        return SC_OK_NOCUTSCENE;

    return SC_OK;
}

static void comboGiveItemRaw(Actor* actor, PlayState* play, const ComboItemQuery* q, float a, float b)
{
    static ComboItemQuery sItemQ;
    static ComboItemQuery sItemQBox;
    ComboItemOverride o;

    comboItemOverride(&o, q);
    if (Actor_OfferGetItem(actor, play, o.gi, a, b))
    {
        if (q->gi < 0)
        {
            memcpy(&sItemQBox, q, sizeof(sItemQBox));
            g.itemQueryBox = &sItemQBox;
        }
        else
        {
            memcpy(&sItemQ, q, sizeof(sItemQ));
            g.itemQuery = &sItemQ;
        }
    }
}

void comboGiveItem(Actor* actor, PlayState* play, const ComboItemQuery* q, float a, float b)
{
    ComboItemQuery qNothing = ITEM_QUERY_INIT;
    const ComboItemQuery* qPtr;

    if (Mark_Get(play, q->ovType, q->sceneId, q->roomId, q->id) && !(q->ovFlags & OVF_RENEW))
    {
        qNothing.gi = GI_NOTHING;
        qPtr = &qNothing;
    }
    else
        qPtr = q;
    comboGiveItemRaw(actor, play, qPtr, a, b);
}

void comboGiveItemNpc(Actor* actor, PlayState* play, s16 gi, int npc, float a, float b)
{
    comboGiveItemNpcEx(actor, play, gi, npc, 0, a, b);
}

void comboGiveItemNpcEx(Actor* actor, PlayState* play, s16 gi, int npc, int flags, float a, float b)
{
    ComboItemQuery q = ITEM_QUERY_INIT;

    q.gi = gi;
    q.ovFlags = flags;
    if (npc != -1)
    {
        q.ovType = OV_NPC;
        q.id = npc;
    }

    comboGiveItem(actor, play, &q, a, b);
}

typedef struct ComboOverrideData
{
    u32  key;
    s16  player;
    u16  value;
    s16  giCloak;
    s16  unused[3];
}
ComboOverrideData;

static ComboOverrideData sComboOverridesCache[64];
static int sComboOverridesCacheCursor;
static u32 sComboOverridesDevAddr;
static u32 sComboOverridesCount;

void comboInitOverride(void)
{
    DmaEntry e;

    comboDmaLookup(&e, COMBO_VROM_CHECKS);
    sComboOverridesCount = comboDmaLoadFile(NULL, COMBO_VROM_CHECKS) / sizeof(ComboOverrideData);
    sComboOverridesDevAddr = e.pstart | PI_DOM1_ADDR2;
    memset(sComboOverridesCache, 0xff, sizeof(sComboOverridesCache));
    sComboOverridesCacheCursor = 0;
}

u8 Play_SceneKey(u8 sceneId)
{
#if defined(GAME_OOT)
    sceneId = Play_ExpandMQ(gPlay, sceneId);
#endif

#if defined(GAME_MM)
    switch (sceneId)
    {
    case SCE_MM_SOUTHERN_SWAMP_CLEAR:
        sceneId = SCE_MM_SOUTHERN_SWAMP;
        break;
    case SCE_MM_TWIN_ISLANDS_SPRING:
        sceneId = SCE_MM_TWIN_ISLANDS_WINTER;
        break;
    case SCE_MM_GORON_VILLAGE_SPRING:
        sceneId = SCE_MM_GORON_VILLAGE_WINTER;
        break;
    case SCE_MM_MOUNTAIN_VILLAGE_SPRING:
        sceneId = SCE_MM_MOUNTAIN_VILLAGE_WINTER;
        break;
    case SCE_MM_TEMPLE_STONE_TOWER_INVERTED:
        sceneId = SCE_MM_TEMPLE_STONE_TOWER;
        break;
    }
#endif

    return sceneId;
}

static int overrideData(ComboOverrideData* data, u32 key)
{
    u32 min;
    u32 max;
    u32 cursor;
    u32 ovData[4];

    /* Check the cache */
    for (int i = 0; i < ARRAY_COUNT(sComboOverridesCache); ++i)
    {
        if (sComboOverridesCache[i].key == key)
        {
            memcpy(data, &sComboOverridesCache[i], sizeof(*data));
            return 1;
        }
    }

    /* Cache miss, have to lookup manually */
    min = 0;
    max = sComboOverridesCount;
    for (;;)
    {
        if (min >= max)
            return 0;
        cursor = (min + max) / 2;

        /* Read from cart */
        /* TODO: Have a generic helper for this */
        ovData[0] = IO_ReadPhysU32(sComboOverridesDevAddr + cursor * sizeof(ComboOverrideData) + 0x00);
        if (ovData[0] == key)
        {
            ovData[1] = IO_ReadPhysU32(sComboOverridesDevAddr + cursor * sizeof(ComboOverrideData) + 0x04);
            ovData[2] = IO_ReadPhysU32(sComboOverridesDevAddr + cursor * sizeof(ComboOverrideData) + 0x08);
            ovData[3] = IO_ReadPhysU32(sComboOverridesDevAddr + cursor * sizeof(ComboOverrideData) + 0x0c);

            /* Copy and add to cache */
            memcpy(data, ovData, sizeof(*data));
            memcpy(&sComboOverridesCache[sComboOverridesCacheCursor], data, sizeof(*data));
            sComboOverridesCacheCursor = (sComboOverridesCacheCursor + 1) % ARRAY_COUNT(sComboOverridesCache);
            return 1;
        }
        if (key > ovData[0])
            min = cursor + 1;
        else
            max = cursor;
    }
}

void comboItemOverride(ComboItemOverride* dst, const ComboItemQuery* q)
{
    ComboOverrideData data;
    int isOverride;
    s16 gi;
    int neg;

    memset(dst, 0, sizeof(*dst));

    if (q->gi < 0)
    {
        gi = -q->gi;
        neg = 1;
    }
    else
    {
        gi = q->gi;
        neg = 0;
    }

    if (q->ovType == OV_NONE)
        isOverride = 0;
    else
    {
        isOverride = overrideData(&data, Checks_MakeOverrideKey(q));
    }

    if (isOverride)
    {
        dst->player = data.player;
        gi = (s16)data.value;
        dst->cloakGi = data.giCloak;
    }

    if (q->ovFlags & OVF_RENEW)
        gi = comboRenewable(gi, q->giRenew);

    dst->giRaw = gi;

    if (Item_IsPlayerSelf(dst->player))
    {
        gi = Item_Progressive(gi, q->ovFlags);
        dst->cloakGi = Item_Progressive(dst->cloakGi, q->ovFlags);
    }

    if (neg)
        gi = -gi;

    dst->gi = gi;
    dst->playerFrom = q->from;
}


int comboAddItemRawEx(PlayState* play, const ComboItemQuery* q, int updateText)
{
    ComboItemOverride o;
    int count;

    comboItemOverride(&o, q);
    count = 0;

    /* Add the item if it's for us */
    if (Item_IsPlayerSelf(o.player))
        count = comboAddItemRaw(play, o.gi);

    /* Update text */
    if (updateText)
        comboTextHijackItemEx(play, &o, count);

    /* Mark the item, and add to GI skips in multi */
    if (Item_IsPlayerSelf(o.playerFrom) && q->ovType != OV_NONE)
    {
        Mark_Set(play, q->ovType, q->sceneId, q->roomId, q->id);
        if (Config_Flag(CFG_MULTIPLAYER) && (q->ovFlags & OVF_RENEW))
        {
            for (int i = 0; i < ARRAY_COUNT(gSharedCustomSave.multi.giSkip); ++i)
            {
                if (gSharedCustomSave.multi.giSkip[i] == GI_NONE)
                {
                    gSharedCustomSave.multi.giSkip[i] = o.gi;
                    break;
                }
            }
        }
    }

    /* Send the item on the network */
    if (q->ovType != OV_NONE)
        Multi_SendItem(o.player, o.gi, (s16)q->ovFlags, Checks_MakeOverrideKey(q));

    return count;
}

int comboAddItemEx(PlayState* play, const ComboItemQuery* q, int updateText)
{
    ComboItemQuery qNothing = ITEM_QUERY_INIT;
    const ComboItemQuery* qPtr;

    if (Mark_Get(play, q->ovType, q->sceneId, q->roomId, q->id) && !(q->ovFlags & OVF_RENEW))
    {
        qNothing.gi = GI_NOTHING;
        qPtr = &qNothing;
    }
    else
        qPtr = q;
    return comboAddItemRawEx(play, qPtr, updateText);
}

void comboPlayerAddItem(PlayState* play, s16 gi)
{
#if defined(GAME_MM)
# define CHEST_OFF 0x388
#else
# define CHEST_OFF 0x428
#endif

    Actor* chest;
    Player* player;
    ComboItemQuery q = ITEM_QUERY_INIT;
    ComboItemOverride o;

    /* Check for a chest */
    player = GET_PLAYER(play);
    chest = *(Actor**)((char*)player + CHEST_OFF);
    if (chest && chest->id == ACTOR_EN_BOX)
    {
        if (g.itemQueryBox)
        {
            memcpy(&q, g.itemQueryBox, sizeof(q));
            g.itemQueryBox = NULL;
        }
        else
            q.gi = gi;
    }
    else
    {
        if (g.itemQuery)
        {
            memcpy(&q, g.itemQuery, sizeof(q));
            g.itemQuery = NULL;
        }
        else
            q.gi = gi;
    }

    if (q.gi < 0)
        q.gi = -q.gi;

    comboItemOverride(&o, &q);
    comboAddItemRawEx(play, &q, 1);
    comboPlayItemFanfare(o.gi, 0);
}

u8 comboItemType(s16 gi)
{
    if (gi == 0)
        return ITT_NONE;
    if (gi < 0)
        gi = -gi;
    return kExtendedGetItems[gi - 1].type;
}

Actor_ItemDecoy* Item_AddWithDecoy(PlayState* play, const ComboItemQuery* q)
{
    return Item_AddWithDecoyNamed(play, q, NULL);
}

Actor_ItemDecoy* Item_AddWithDecoyNamed(PlayState* play, const ComboItemQuery* q, const char* name)
{
    int count;
    Actor_ItemDecoy* decoy;
    ComboItemOverride o;

    comboItemOverride(&o, q);
    count = comboAddItemEx(play, q, FALSE);
    decoy = (Actor_ItemDecoy*)Actor_Spawn(&play->actorCtx, play, ACTOR_ITEM_DECOY, 0, 0, 0, 0, 0, 0, 0);
    if (!decoy)
        return NULL;
    decoy->count = (s16)count;
    decoy->gi = o.gi;
    decoy->player = o.player;
    decoy->playerFrom = o.playerFrom;
    if (name)
        memcpy(decoy->playerName, name, sizeof(decoy->playerName));
    else
        memset(decoy->playerName, 0, sizeof(decoy->playerName));
    g.decoysCount++;

    return decoy;
}

int Item_AddXflagRenew(PlayState* play, XflagID xflag, s16 gi)
{
    ComboItemQuery q;
    ComboItemOverride o;

    /* Make sure there is a valid item */
    if (xflag == XFLAGID_NONE)
        return FALSE;
    Xflag_ItemQuery(&q, xflag, GI_NONE);
    if (Xflag_Get(xflag))
    {
        q.giRenew = gi;
        q.ovFlags |= OVF_RENEW;
    }
    comboItemOverride(&o, &q);
    if (o.gi == GI_NONE)
        return FALSE;

    /* Special case - Nothing */
    if (o.gi != GI_NOTHING)
        Item_AddWithDecoy(play, &q);
    Xflag_Set(xflag);
    return TRUE;
}

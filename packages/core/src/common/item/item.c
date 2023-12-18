#include <combo.h>
#include <combo/item.h>
#include <combo/net.h>
#include <combo/dma.h>

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

static int isPlayerSelf(u8 playerId)
{
    if (playerId == PLAYER_SELF || playerId == gComboData.playerId)
        return 1;
    return 0;
}

void comboSyncItems(void)
{
    if (comboConfig(CFG_SHARED_BOWS))
        gForeignSave.inventory.ammo[ITS_FOREIGN_BOW] = gSave.inventory.ammo[ITS_NATIVE_BOW];

    if (comboConfig(CFG_SHARED_BOMB_BAGS))
        gForeignSave.inventory.ammo[ITS_FOREIGN_BOMBS] = gSave.inventory.ammo[ITS_NATIVE_BOMBS];

    if (comboConfig(CFG_SHARED_MAGIC))
       gForeignSave.playerData.magicAmount = gSave.playerData.magicAmount;

    if (comboConfig(CFG_SHARED_NUTS_STICKS))
    {
        gForeignSave.inventory.ammo[ITS_FOREIGN_NUTS] = gSave.inventory.ammo[ITS_NATIVE_NUTS];
        gForeignSave.inventory.ammo[ITS_FOREIGN_STICKS] = gSave.inventory.ammo[ITS_NATIVE_STICKS];
    }

    if (comboConfig(CFG_SHARED_WALLETS))
        gForeignSave.playerData.rupees = gSave.playerData.rupees;

    if (comboConfig(CFG_SHARED_HEALTH))
    {
        gForeignSave.playerData.healthMax = gSave.playerData.healthMax;
        gForeignSave.playerData.health = gSave.playerData.health;
        gForeignSave.inventory.quest.heartPieces = gSave.inventory.quest.heartPieces;
    }
}

int comboItemPrecondEx(const ComboItemQuery* q, s16 price)
{
    ComboItemOverride o;

    comboSyncItems();
    comboItemOverride(&o, q);
    if (isPlayerSelf(o.player) && (!isItemBuyable(o.gi) || !isItemLicensed(o.gi)))
        return SC_ERR_CANNOTBUY;

    if (gSave.playerData.rupees < price)
        return SC_ERR_NORUPEES;

    if (isItemFastBuy(o.gi))
        return SC_OK_NOCUTSCENE;

    return SC_OK;
}

void comboGiveItem(Actor* actor, GameState_Play* play, const ComboItemQuery* q, float a, float b)
{
    static ComboItemQuery sItemQ;
    static ComboItemQuery sItemQBox;
    ComboItemOverride o;

    comboItemOverride(&o, q);
    if (GiveItem(actor, play, o.gi, a, b))
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

void comboGiveItemNpc(Actor* actor, GameState_Play* play, s16 gi, int npc, float a, float b)
{
    comboGiveItemNpcEx(actor, play, gi, npc, 0, a, b);
}

void comboGiveItemNpcEx(Actor* actor, GameState_Play* play, s16 gi, int npc, int flags, float a, float b)
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
}
ComboOverrideData;

static ComboOverrideData sComboOverridesCache[64];
static int sComboOverridesCacheCursor;
static const ComboOverrideData* sComboOverridesFile;
static u32 sComboOverridesCount;

void comboInitOverride(void)
{
    u64 mask;
    DmaEntry e;

    mask = osSetIntMask(1);
    comboDmaLookup(&e, COMBO_VROM_CHECKS);
    sComboOverridesCount = comboDmaLoadFile(NULL, COMBO_VROM_CHECKS) / sizeof(ComboOverrideData);
    sComboOverridesFile = (void*)(0xb0000000 | e.pstart);
    memset(sComboOverridesCache, 0xff, sizeof(sComboOverridesCache));
    sComboOverridesCacheCursor = 0;
    osSetIntMask(mask);
}

u8 comboSceneKey(u8 sceneId)
{
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

static u32 makeOverrideKey(const ComboItemQuery* q)
{
    u32 key;

    if (q->ovType == OV_NONE)
        return 0;

    key = 0;
    key |= (((u32)(q->ovType & 0xff)) << 24);
    key |= (((u32)(q->sceneId & 0xff)) << 16);
    key |= (((u32)(q->roomId & 0xff)) << 8);
    key |= (((u32)(q->id & 0xff)) << 0);

    return key;
}

static int overrideData(ComboOverrideData* data, u32 key)
{
    ComboOverrideData d;
    u32 min;
    u32 max;
    u32 cursor;

    /* Check the cache */
    for (int i = 0; i < ARRAY_SIZE(sComboOverridesCache); ++i)
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
        memcpy(&d, sComboOverridesFile + cursor, sizeof(d));
        if (d.key == key)
        {
            /* Copy and add to cache */
            memcpy(data, &d, sizeof(*data));
            memcpy(&sComboOverridesCache[sComboOverridesCacheCursor], &d, sizeof(d));
            sComboOverridesCacheCursor = (sComboOverridesCacheCursor + 1) % ARRAY_SIZE(sComboOverridesCache);
            return 1;
        }
        if (key > d.key)
            min = cursor + 1;
        else
            max = cursor;
    }
}

void comboItemOverride(ComboItemOverride* dst, const ComboItemQuery* q)
{
    ComboOverrideData data;
    u64 mask;
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
        mask = osSetIntMask(1);
        isOverride = overrideData(&data, makeOverrideKey(q));
        osSetIntMask(mask);
    }

    if (isOverride)
    {
        dst->player = data.player;
        gi = (s16)data.value;
    }

    if (q->ovFlags & OVF_RENEW)
        gi = comboRenewable(gi, q->giRenew);

    dst->giRaw = gi;

    if (isPlayerSelf(dst->player))
    {
        gi = comboProgressive(gi);
    }

    if (neg)
        gi = -gi;

    dst->gi = gi;
    dst->playerFrom = q->from;
}


int comboAddItemEx(GameState_Play* play, const ComboItemQuery* q, int updateText)
{
    ComboItemOverride o;
    NetContext* net;
    int count;

    comboItemOverride(&o, q);
    count = 0;

    /* Add the item if it's for us */
    if (isPlayerSelf(o.player))
        count = comboAddItem(play, o.gi);
    else
    {
        /* We need to send it */
        net = netMutexLock();
        netWaitCmdClear();
        bzero(&net->cmdOut, sizeof(net->cmdOut));
        net->cmdOut.op = NET_OP_ITEM_SEND;
        net->cmdOut.itemSend.playerFrom = gComboData.playerId;
        net->cmdOut.itemSend.playerTo = o.player;
#if defined(GAME_OOT)
        net->cmdOut.itemSend.game = 0;
#else
        net->cmdOut.itemSend.game = 1;
#endif
        net->cmdOut.itemSend.gi = o.gi;
        net->cmdOut.itemSend.key = makeOverrideKey(q);
        net->cmdOut.itemSend.flags = (s16)q->ovFlags;
        netMutexUnlock();
    }

    /* Update text */
    if (updateText)
        comboTextHijackItemEx(play, &o, count);

    return -1;
}

void comboPlayerAddItem(GameState_Play* play, s16 gi)
{
#if defined(GAME_MM)
# define CHEST_OFF 0x388
#else
# define CHEST_OFF 0x428
#endif

    Actor* chest;
    Actor_Player* player;
    ComboItemQuery q = ITEM_QUERY_INIT;

    /* Check for a chest */
    player = GET_LINK(play);
    chest = *(Actor**)((char*)player + CHEST_OFF);
    if (chest && chest->id == AC_EN_BOX)
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

    comboAddItemEx(play, &q, 1);
}

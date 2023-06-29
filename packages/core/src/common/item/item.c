#include <combo.h>
#include <combo/item.h>
#include <combo/net.h>

#if defined(GAME_OOT)
# define comboAddItemNative             comboAddItemOot
# define comboAddItemSharedNative       comboAddItemSharedOot
# define comboAddItemForeign            comboAddItemMm
# define comboAddItemSharedForeign      comboAddItemSharedMm
#else
# define comboAddItemNative             comboAddItemMm
# define comboAddItemSharedNative       comboAddItemSharedMm
# define comboAddItemForeign            comboAddItemOot
# define comboAddItemSharedForeign      comboAddItemSharedOot
#endif

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

int comboAddItem(GameState_Play* play, s16 gi)
{
    int count;
    int count2;

    if (gi & MASK_FOREIGN_GI)
    {
        count = comboAddItemForeign(gi & ~MASK_FOREIGN_GI, 1);
        comboAddItemSharedForeign(gi & ~MASK_FOREIGN_GI, 0);
        comboAddItemSharedForeignEffect(play, gi & ~MASK_FOREIGN_GI);
    }
    else
    {
        count = comboAddItemNative(gi, 0);
        comboAddItemSharedNative(gi, 0);
        count2 = comboAddItemEffect(play, gi);
        if (!count)
            count = count2;
    }

    return count;
}

int comboAddItemNoEffect(s16 gi)
{
    if (gi & MASK_FOREIGN_GI)
    {
        comboAddItemForeign(gi & ~MASK_FOREIGN_GI, 1);
        comboAddItemSharedForeign(gi & ~MASK_FOREIGN_GI, 1);
    }
    else
    {
        comboAddItemNative(gi, 1);
        comboAddItemSharedNative(gi, 1);
    }
    return -1;
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

#if defined(GAME_OOT)
# define OVERRIDE_ADDR 0x03fe1000
#else
# define OVERRIDE_ADDR 0x03fe9000
#endif

#define OVERRIDE_MAX 1024

typedef struct ComboOverrideData
{
    s16  player;
    s16  pad;
    u16  key;
    u16  value;
}
ComboOverrideData;

static ALIGNED(16) ComboOverrideData gComboOverrides[OVERRIDE_MAX];

void comboInitOverride(void)
{
    DMARomToRam(OVERRIDE_ADDR | PI_DOM1_ADDR2, &gComboOverrides, sizeof(gComboOverrides));
}

static u16 makeOverrideKey(int type, u16 sceneId, u16 id)
{
    switch (type)
    {
    case OV_CHEST:
        id &= 0x3f;
        break;
    case OV_COLLECTIBLE:
        id = (id & 0x3f) | 0x40;
        break;
    case OV_SF:
        id = (id & 0x3f) | 0x80;
        break;
    case OV_NPC:
        sceneId = SCE_NPC;
        break;
    case OV_GS:
        sceneId = SCE_GS;
        break;
    case OV_COW:
        sceneId = SCE_COW;
        break;
    case OV_SHOP:
        sceneId = SCE_SHOP;
        break;
    case OV_SCRUB:
        sceneId = SCE_SCRUB;
        break;
    }

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

    return (sceneId << 8) | id;
}

static const ComboOverrideData* overrideData(u16 key)
{
    for (int i = 0; i < OVERRIDE_MAX; ++i)
    {
        ComboOverrideData* o = &gComboOverrides[i];
        if (o->key == 0xffff)
            break;
        if (o->key == key)
            return o;
    }
    return NULL;
}

void comboItemOverride(ComboItemOverride* dst, const ComboItemQuery* q)
{
    const ComboOverrideData* data;
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
        data = NULL;
    else
        data = overrideData(makeOverrideKey(q->ovType, q->sceneId, q->id));

    if (data)
    {
        dst->player = data->player;
        gi = (s16)data->value;
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


int comboAddItemEx(GameState_Play* play, const ComboItemQuery* q)
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
        net->cmdOut.itemSend.gi = o.gi;
#else
        net->cmdOut.itemSend.game = 1;
        net->cmdOut.itemSend.gi = o.gi ^ MASK_FOREIGN_GI;
#endif
        net->cmdOut.itemSend.key = makeOverrideKey(q->ovType, q->sceneId, q->id);
        net->cmdOut.itemSend.flags = (s16)q->ovFlags;
        netMutexUnlock();
    }

    /* Update text */
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

    comboAddItemEx(play, &q);
}

#include <combo.h>
#include <combo/item.h>

#define SCRUB_ITEM_NUTS             0x00
#define SCRUB_ITEM_STICK            0x01
#define SCRUB_ITEM_HEART_PIECE      0x02
#define SCRUB_ITEM_SEEDS            0x03
#define SCRUB_ITEM_SHIELD_DEKU      0x04
#define SCRUB_ITEM_BOMBS            0x05
#define SCRUB_ITEM_SEEDS_ARROWS     0x06
#define SCRUB_ITEM_POTION_RED       0x07
#define SCRUB_ITEM_POTION_GREEN     0x08
#define SCRUB_ITEM_STICKS_UPGRADE   0x09
#define SCRUB_ITEM_NUTS_UPGRADE     0x0a

#define SCRUB_COND_NORUPEES          0x00
#define SCRUB_COND_CANTBUY           0x01
#define SCRUB_COND_OK                0x02

#define KEY(sceneId, inGrotto, item) \
    (((inGrotto) << 24) | ((sceneId) << 16) | (item))

static int scrubSceneKey(GameState_Play* play, int inGrotto)
{
    s8 roomId;

    if (!inGrotto)
        return play->sceneId;
    roomId = play->roomCtx.curRoom.num;
    if (roomId == 0x06)
        return SCE_OOT_LOST_WOODS;
    if (roomId == 0x01)
        return SCE_OOT_HYRULE_FIELD;
    return gLastScene;
}

static int EnDns_GetID(Actor* this)
{
    int key;
    int lastScene;
    int inGrotto;

    inGrotto = (gPlay->sceneId == SCE_OOT_GROTTOS);
    lastScene = scrubSceneKey(gPlay, inGrotto);
    key = KEY(lastScene, inGrotto, this->variable & 0xff);

    switch (key)
    {
    /* Lost Woods */
    case KEY(SCE_OOT_LOST_WOODS, 0, SCRUB_ITEM_STICKS_UPGRADE): return 0x00;
    case KEY(SCE_OOT_LOST_WOODS, 0, SCRUB_ITEM_STICK):          return 0x01;
    case KEY(SCE_OOT_LOST_WOODS, 0, SCRUB_ITEM_NUTS):           return 0x02;
    case KEY(SCE_OOT_LOST_WOODS, 1, SCRUB_ITEM_NUTS_UPGRADE):   return 0x03;
    case KEY(SCE_OOT_LOST_WOODS, 1, SCRUB_ITEM_SEEDS_ARROWS):   return 0x04;

    /* Sacred Meadow */
    case KEY(SCE_OOT_SACRED_FOREST_MEADOW, 1, SCRUB_ITEM_POTION_GREEN):   return 0x05;
    case KEY(SCE_OOT_SACRED_FOREST_MEADOW, 1, SCRUB_ITEM_POTION_RED):     return 0x06;

    /* Hyrule Field */
    case KEY(SCE_OOT_HYRULE_FIELD, 1, SCRUB_ITEM_HEART_PIECE):   return 0x07;

    /* Lon Lon Ranch */
    case KEY(SCE_OOT_LON_LON_RANCH, 1, SCRUB_ITEM_NUTS):            return 0x08;
    case KEY(SCE_OOT_LON_LON_RANCH, 1, SCRUB_ITEM_SEEDS_ARROWS):    return 0x09;
    case KEY(SCE_OOT_LON_LON_RANCH, 1, SCRUB_ITEM_BOMBS):           return 0x0a;

    /* Goron City */
    case KEY(SCE_OOT_GORON_CITY, 1, SCRUB_ITEM_NUTS):            return 0x0b;
    case KEY(SCE_OOT_GORON_CITY, 1, SCRUB_ITEM_SEEDS_ARROWS):    return 0x0c;
    case KEY(SCE_OOT_GORON_CITY, 1, SCRUB_ITEM_BOMBS):           return 0x0d;

    /* DMC */
    case KEY(SCE_OOT_DEATH_MOUNTAIN_CRATER, 0, SCRUB_ITEM_BOMBS):           return 0x0e;
    case KEY(SCE_OOT_DEATH_MOUNTAIN_CRATER, 1, SCRUB_ITEM_NUTS):            return 0x0f;
    case KEY(SCE_OOT_DEATH_MOUNTAIN_CRATER, 1, SCRUB_ITEM_SEEDS_ARROWS):    return 0x10;
    case KEY(SCE_OOT_DEATH_MOUNTAIN_CRATER, 1, SCRUB_ITEM_BOMBS):           return 0x11;

    /* Zora's River */
    case KEY(SCE_OOT_ZORA_RIVER, 1, SCRUB_ITEM_POTION_GREEN):   return 0x12;
    case KEY(SCE_OOT_ZORA_RIVER, 1, SCRUB_ITEM_POTION_RED):     return 0x13;

    /* Lake Hylia */
    case KEY(SCE_OOT_LAKE_HYLIA, 1, SCRUB_ITEM_NUTS):            return 0x14;
    case KEY(SCE_OOT_LAKE_HYLIA, 1, SCRUB_ITEM_SEEDS_ARROWS):    return 0x15;
    case KEY(SCE_OOT_LAKE_HYLIA, 1, SCRUB_ITEM_BOMBS):           return 0x16;

    /* Gerudo Valley */
    case KEY(SCE_OOT_GERUDO_VALLEY, 1, SCRUB_ITEM_POTION_GREEN):   return 0x17;
    case KEY(SCE_OOT_GERUDO_VALLEY, 1, SCRUB_ITEM_POTION_RED):     return 0x18;

    /* Desert Colossus */
    case KEY(SCE_OOT_DESERT_COLOSSUS, 1, SCRUB_ITEM_POTION_GREEN):   return 0x19;
    case KEY(SCE_OOT_DESERT_COLOSSUS, 1, SCRUB_ITEM_POTION_RED):     return 0x1a;

    /* Deku Tree */
    case KEY(SCE_OOT_INSIDE_DEKU_TREE, 0, SCRUB_ITEM_SHIELD_DEKU):  return 0x1b;

    /* Dodongo Cavern */
    case KEY(SCE_OOT_DODONGO_CAVERN, 0, SCRUB_ITEM_SEEDS):          return 0x1c;
    case KEY(SCE_OOT_DODONGO_CAVERN, 0, SCRUB_ITEM_STICK):          return 0x1d;
    case KEY(SCE_OOT_DODONGO_CAVERN, 0, SCRUB_ITEM_NUTS):           return 0x1e;
    case KEY(SCE_OOT_DODONGO_CAVERN, 0, SCRUB_ITEM_POTION_RED):     return 0x1e;
    case KEY(SCE_OOT_DODONGO_CAVERN, 0, SCRUB_ITEM_SHIELD_DEKU):    return 0x1f;

    /* Jabu-Jabu */
    case KEY(SCE_OOT_INSIDE_JABU_JABU, 0, SCRUB_ITEM_NUTS): return 0x20;

    /* Ganon */
    case KEY(SCE_OOT_INSIDE_GANON_CASTLE, 0, SCRUB_ITEM_POTION_GREEN):  return 0x21;
    case KEY(SCE_OOT_INSIDE_GANON_CASTLE, 0, SCRUB_ITEM_BOMBS):         return 0x22;
    case KEY(SCE_OOT_INSIDE_GANON_CASTLE, 0, SCRUB_ITEM_SEEDS_ARROWS):  return 0x23;
    case KEY(SCE_OOT_INSIDE_GANON_CASTLE, 0, SCRUB_ITEM_POTION_RED):    return 0x24;
    case KEY(SCE_OOT_INSIDE_GANON_CASTLE, 0, SCRUB_ITEM_NUTS):          return 0x25;
    }

    return 0;
}

static void EnDns_ItemQuery(ComboItemQuery* q, int id)
{
    bzero(q, sizeof(*q));

    q->ovType = OV_SCRUB;
    q->gi = GI_OOT_STICK; /* Dummy */
    q->ovFlags = OVF_PRECOND;
    q->id = id;

    if (BITMAP8_GET(gCustomSave.scrubs, id))
        q->ovFlags |= OVF_RENEW;
}

static void EnDns_ItemOverride(ComboItemOverride* o, int id)
{
    ComboItemQuery q;

    EnDns_ItemQuery(&q, id);
    comboItemOverride(o, &q);
}

static void EnDns_PatchPrice(Actor* this, s16 price)
{
    void* scrubData;
    scrubData = *(void**)((char*)this + 0x2b0);
    *(s16*)((char*)scrubData + 0x0) = price;
}

static s16 EnDns_GetPrice(Actor* this)
{
    int id;

    id = EnDns_GetID(this);
    return (s16)gComboData.prices[PRICES_OOT_SCRUBS + id];
}

void EnDns_MaybeDestroy(Actor* this)
{
    ComboItemOverride o;

    EnDns_ItemOverride(&o, EnDns_GetID(this));
    if (o.gi == 0)
        ActorDestroy(this);
}

static int EnDns_CanBuy(Actor* this)
{
    ComboItemQuery q;
    s16 price;

    EnDns_ItemQuery(&q, EnDns_GetID(this));
    price = EnDns_GetPrice(this);
    EnDns_PatchPrice(this, price);

    switch (comboItemPrecondEx(&q, price))
    {
    case SC_OK:
    case SC_OK_NOCUTSCENE:
        return SCRUB_COND_OK;
    case SC_ERR_NORUPEES:
        return SCRUB_COND_NORUPEES;
    default:
        return SCRUB_COND_CANTBUY;
    }
}

PATCH_CALL(0x80a75620, EnDns_CanBuy);

static void EnDns_ShopText(Actor* this, GameState_Play* play)
{
    ComboItemQuery q;
    char* b;
    char* start;
    s16 price;

    EnDns_ItemQuery(&q, EnDns_GetID(this));

    b = play->msgCtx.textBuffer;
    start = b;
    price = EnDns_GetPrice(this);
    comboTextAppendShopHeader(&b, price);

    comboTextAppendStr(&b, "Do you want ");
    comboTextAppendItemNameQuery(&b, &q, TF_PROGRESSIVE);
    comboTextAppendStr(&b, " for " TEXT_COLOR_RED);
    comboTextAppendNum(&b, price);
    comboTextAppendStr(&b, " Rupees");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, "?" TEXT_NL TEXT_CHOICE2 TEXT_COLOR_GREEN);
    comboTextAppendStr(&b, "Yes" TEXT_NL);
    comboTextAppendStr(&b, "No" TEXT_END);
    comboTextAutoLineBreaks(start);
}

static int EnDns_TalkedTo(Actor* this, GameState_Play* play)
{
    if (ActorTalkedTo(this))
    {
        EnDns_ShopText(this, play);
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80a75510, EnDns_TalkedTo);

static int EnDns_HasGivenItem(Actor* this)
{
    Actor_Player* link;
    int id;

    link = GET_LINK(gPlay);
    if (Actor_HasParent(this) && !(link->state & PLAYER_ACTOR_STATE_GET_ITEM))
    {
        id = EnDns_GetID(this);
        BITMAP8_SET(gCustomSave.scrubs, id);
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80a75878, EnDns_HasGivenItem);

static void EnDns_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    EnDns_ItemQuery(&q, EnDns_GetID(actor));
    comboGiveItem(actor, play, &q, a, b);
}

PATCH_CALL(0x80a75734, EnDns_GiveItem);
PATCH_CALL(0x80a75750, EnDns_GiveItem);
PATCH_CALL(0x80a757a8, EnDns_GiveItem);
PATCH_CALL(0x80a757c4, EnDns_GiveItem);
PATCH_CALL(0x80a757e4, EnDns_GiveItem);

#include <combo.h>
#include <combo/item.h>

static u16 kTingleMaps[] = {
    GI_MM_WORLD_MAP_CLOCK_TOWN,
    GI_MM_WORLD_MAP_WOODFALL,
    GI_MM_WORLD_MAP_SNOWHEAD,
    GI_MM_WORLD_MAP_ROMANI_RANCH,
    GI_MM_WORLD_MAP_GREAT_BAY,
    GI_MM_WORLD_MAP_STONE_TOWER,
};

static u8 kTingleNpc[] = {
    NPC_MM_TINGLE_MAP_CLOCK_TOWN,
    NPC_MM_TINGLE_MAP_WOODFALL,
    NPC_MM_TINGLE_MAP_SNOWHEAD,
    NPC_MM_TINGLE_MAP_ROMANI_RANCH,
    NPC_MM_TINGLE_MAP_GREAT_BAY,
    NPC_MM_TINGLE_MAP_STONE_TOWER,
};

static int EnBal_GetTingleId(GameState_Play* play)
{
    switch (play->sceneId)
    {
    case SCE_MM_CLOCK_TOWN_NORTH:
        return TINGLE_MAP_CLOCK_TOWN;
    case SCE_MM_ROAD_SOUTHERN_SWAMP:
        return TINGLE_MAP_WOODFALL;
    case SCE_MM_TWIN_ISLANDS_SPRING:
    case SCE_MM_TWIN_ISLANDS_WINTER:
        return TINGLE_MAP_SNOWHEAD;
    case SCE_MM_MILK_ROAD:
        return TINGLE_MAP_ROMANI_RANCH;
    case SCE_MM_GREAT_BAY_COAST:
        return TINGLE_MAP_GREAT_BAY;
    case SCE_MM_IKANA_CANYON:
        return TINGLE_MAP_STONE_TOWER;
    default:
        UNREACHABLE();
    }
}

static s16 EnBal_GetGI(int mapId, int checkSoldOut, int flags)
{
    s16 gi;
    u8 npcId;
    u8 soldOut;

    npcId = kTingleNpc[mapId];
    gi = comboOverrideEx(OV_NPC, 0, npcId, kTingleMaps[mapId], flags);
    if (checkSoldOut)
    {
        soldOut = MM_GET_EVENT_WEEK(0x118 + mapId);
        if (soldOut && !comboIsItemConsumable(gi))
            return -1;
    }
    return gi;
}

static void EnBal_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    s16 mapId;

    mapId = *(s16*)((char*)this + 0x3ac);
    gi = EnBal_GetGI(mapId, 0, OVF_PROGRESSIVE);
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80a635c4, EnBal_GiveItem);

static int EnBal_AlreadyBoughtItemWrapper(Actor* this, GameState_Play* play)
{
    int (*EnBal_AlreadyBoughtItem)(Actor*, GameState_Play*);
    int precond;
    s16 mapId;
    s16 gi;

    /* Precompte the mapId */
    EnBal_AlreadyBoughtItem = actorAddr(0x176, 0x80a62dcc);
    EnBal_AlreadyBoughtItem(this, play);
    mapId = *(s16*)((char*)this + 0x3ac);

    /* Get the item */
    gi = EnBal_GetGI(mapId, 1, 0);
    if (gi == -1)
        return 1;

    /* Check for preconds (rupees are checked separately) */
    precond = comboItemPrecond(gi, 0);
    if (precond == SC_OK || precond == SC_OK_NOCUTSCENE)
        return 0;
    return 1;
}

PATCH_CALL(0x80a63050, EnBal_AlreadyBoughtItemWrapper);

static void EnBal_OverrideTextMapName(char** b, int mapId, int price)
{
    s16 gi;

    gi = EnBal_GetGI(mapId, 1, 0);
    if (gi == -1)
    {
        comboTextAppendStr(b, TEXT_COLOR_RED "SOLD OUT");
        comboTextAppendClearColor(b);
        comboTextAppendStr(b, TEXT_NL);
    }
    else
    {
        comboTextAppendItemName(b, gi, TF_PREPOS | TF_CAPITALIZE | TF_PROGRESSIVE);
        comboTextAppendStr(b, " - " TEXT_COLOR_RED);
        comboTextAppendNum(b, price);
        comboTextAppendStr(b, " Rupees");
        comboTextAppendClearColor(b);
        comboTextAppendStr(b, TEXT_NL);
    }
}

static void EnBal_DisplayBuyTextBox(GameState_Play* play, u16 messageId, Actor* this)
{
    int tingleId;
    char* b;
    s16 price1;
    s16 price2;

    /* Display the original textbox */
    PlayerDisplayTextBox(play, messageId, this);

    /* Get tingle ID */
    tingleId = EnBal_GetTingleId(play);
    price1 = tingleId == 0 ? 5 : 20;
    price2 = 40;

    /* Override */
    b = play->textBuffer;
    comboTextAppendShopHeader2(&b, price1, price2);

    comboTextAppendStr(&b, TEXT_CHOICE3);
    EnBal_OverrideTextMapName(&b, tingleId, price1);
    EnBal_OverrideTextMapName(&b, tingleId == 5 ? 0 : tingleId + 1, price2);
    comboTextAppendStr(&b, "No Thanks" TEXT_END);
}

PATCH_CALL(0x80a632d4, EnBal_DisplayBuyTextBox);
PATCH_CALL(0x80a632f0, EnBal_DisplayBuyTextBox);
PATCH_CALL(0x80a6330c, EnBal_DisplayBuyTextBox);
PATCH_CALL(0x80a63328, EnBal_DisplayBuyTextBox);
PATCH_CALL(0x80a63344, EnBal_DisplayBuyTextBox);
PATCH_CALL(0x80a63360, EnBal_DisplayBuyTextBox);
PATCH_CALL(0x80a6337c, EnBal_DisplayBuyTextBox);

#include <combo.h>
#include <combo/custom.h>
#include <combo/menu.h>
#include <combo/dma.h>
#include <combo/item.h>

static int checkItemToggle(GameState_Play* play)
{
    PauseContext* p;
    Actor_Player* link;
    s16 itemId;
    s16 itemCursor;
    int ret;
    int press;

    p = &play->pauseCtx;
    ret = 0;
    press = !!(play->gs.input[0].pressed.buttons & (L_TRIG | U_CBUTTONS));

    itemId = ITEM_NONE;
    itemCursor = -1;

    if (p->cursorSpecialPos == 0)
    {
        switch (p->screen_idx)
        {
        case PAUSE_ITEM:
            itemId = p->cursorItem[PAUSE_ITEM];
            itemCursor = p->cursorPoint[PAUSE_ITEM];
            break;
        case PAUSE_EQUIP:
            itemId = p->cursorItem[PAUSE_EQUIP];
            break;
        }
    }

    u8* itemPtr;
    u32 flags;
    const u8* table;
    u32 tableSize;
    if (comboGetSlotExtras(itemCursor, &itemPtr, &flags, &table, &tableSize) >= 0 && play->pauseCtx.cursorItem[0] != 999 && popcount(flags) > 1)
    {
        ret = 1;
        if (press)
        {
            comboToggleSlot(itemCursor);
            if (itemCursor == ITS_OOT_TRADE_CHILD)
            {
                link = GET_LINK(play);
                link->mask = 0;
                Interface_LoadItemIconImpl(play, 0);
            }
        }
    }

    if (itemCursor >= ITS_OOT_BOTTLE && itemCursor <= ITS_OOT_BOTTLE4 && itemId == ITEM_OOT_BIG_POE)
    {
        ret = 1;
        if (press)
        {
            gSave.inventory.items[itemCursor] = ITEM_OOT_BOTTLE_EMPTY;
            reloadSlotOot(play, itemCursor);

            /* Reset one big poe */
            if (play->sceneId == SCE_OOT_HYRULE_FIELD)
                ClearSwitchFlag(play, 0x18);
            else
                gSave.perm[SCE_OOT_HYRULE_FIELD].switches &= ~(1 << 0x18);
        }
    }

    if (itemId >= ITEM_OOT_SHIELD_DEKU && itemId <= ITEM_OOT_SHIELD_MIRROR && gSave.equips.equipment.shields == ((itemId - ITEM_OOT_SHIELD_DEKU) + 1))
    {
        ret = 1;
        if (press)
            gSave.equips.equipment.shields = 0;
    }

    if (gSave.age == AGE_CHILD && itemId == ITEM_OOT_SWORD_KOKIRI && gSave.equips.equipment.swords == 1)
    {
        if (!(GET_LINK(play)->state2 & PLAYER_ACTOR_STATE_WATER))
        {
            ret = 1;
            if (press)
            {
                gSave.equips.equipment.swords = 0;
                gSave.equips.buttonItems[0] = ITEM_NONE;
                gSave.eventsMisc[29] = 1;
            }
        }
    }

    if (ret && press)
    {
        PlaySound(0x4809);
    }

    return ret;
}

void KaleidoSetCursorColor(GameState_Play* play)
{
    PauseContext* p;
    u8 r;
    u8 g;
    u8 b;

    p = &play->pauseCtx;
    r = 0xff;
    g = 0xff;
    b = 0xff;

    /* Update dpad */
    comboDpadUpdate(play);

    /* Not on Z/R */
    if (p->cursorSpecialPos == 0)
    {
        switch (p->screen_idx)
        {
        case PAUSE_ITEM:
            /* Item select */
            r = 0xff;
            g = 0xff;
            b = 0x00;
            if (checkItemToggle(play))
            {
                r = 0x00;
                g = 0xff;
                b = 0xff;
            }
            break;
        case PAUSE_EQUIP:
            /* Equipment */
            r = 0x00;
            g = 0x00;
            b = 0xff;
            if (checkItemToggle(play))
            {
                r = 0x00;
                g = 0xff;
                b = 0xff;
            }
            break;
        }
    }

    OPEN_DISPS(play->gs.gfx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, r, g, b, 0xff);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 0xff);
    CLOSE_DISPS();
}

typedef void (*KaleidoScopeHandler)(GameState_Play*);
typedef void (*KaleidoScopeHandler2)(GameState_Play*, void*);

static void KaleidoScope_HandleMapDungeonMenu(GameState_Play* play, void* unk, u32 overlayAddr)
{
    KaleidoScopeHandler2 handler;
    int onMenu;

    onMenu = play->pauseCtx.screen_idx == 1;
    if (onMenu && play->gs.input[0].pressed.buttons & (L_TRIG | U_CBUTTONS))
        comboMenuNext();

    if (g.menuScreen)
    {
        if (onMenu)
            comboMenuUpdate(play);
        comboMenuDraw(play);
    }
    else
    {
        handler = OverlayAddr(overlayAddr);
        handler(play, unk);
    }
}

static void KaleidoScope_HandleMapMenu(GameState_Play* play, void* unk)
{
    KaleidoScope_HandleMapDungeonMenu(play, unk, 0x8081ce54);
}

PATCH_CALL(0x80820850, KaleidoScope_HandleMapMenu);
PATCH_CALL(0x80820b48, KaleidoScope_HandleMapMenu);

static void KaleidoScope_HandleDungeonMenu(GameState_Play* play, void* unk)
{
    KaleidoScope_HandleMapDungeonMenu(play, unk, 0x8081b660);
}

PATCH_CALL(0x808207e4, KaleidoScope_HandleDungeonMenu);
PATCH_CALL(0x80820ac4, KaleidoScope_HandleDungeonMenu);

static void KaleidoScope_HandleDungeonChests(GameState_Play* play)
{
    KaleidoScopeHandler handler;

    if (!g.menuScreen)
    {
        handler = OverlayAddr(0x808292e8);
        handler(play);
    }
}

PATCH_CALL(0x80820840, KaleidoScope_HandleDungeonChests);
PATCH_CALL(0x80820b38, KaleidoScope_HandleDungeonChests);

static s16 R_PAUSE_OWLWARP_ALPHA = 0;

u16 gPrevPageIndex;
s16 gPrevCursorPoint;
s8 gSoaringIndexSelected = -1;

static u16 sOwlWarpPauseItems[] = {
    0xAF, // OWL_WARP_GREAT_BAY_COAST
    0xB3, // OWL_WARP_ZORA_CAPE
    0xAA, // OWL_WARP_SNOWHEAD
    0xB1, // OWL_WARP_MOUNTAIN_VILLAGE
    0xA9, // OWL_WARP_CLOCK_TOWN
    0xB2, // OWL_WARP_MILK_ROAD
    0xA8, // OWL_WARP_WOODFALL
    0xB0, // OWL_WARP_SOUTHERN_SWAMP
    0xAC, // OWL_WARP_IKANA_CANYON
    0xAE, // OWL_WARP_STONE_TOWER
};

static s16 sVtxPageMapWorldQuadsX[VTX_PAGE_MAP_WORLD_QUADS] = {
    -41,  // mapPageVtx[60] QUAD_MAP_WORLD_CLOUDS_CLOCK_TOWN_1
    9,    // mapPageVtx[64] QUAD_MAP_WORLD_CLOUDS_CLOCK_TOWN_2
    43,   // mapPageVtx[68] QUAD_MAP_WORLD_CLOUDS_WOODFALL_1
    -21,  // mapPageVtx[72] QUAD_MAP_WORLD_CLOUDS_WOODFALL_2
    -37,  // mapPageVtx[76] QUAD_MAP_WORLD_CLOUDS_WOODFALL_3
    43,   // mapPageVtx[80] QUAD_MAP_WORLD_CLOUDS_SNOWHEAD_1
    -69,  // mapPageVtx[84] QUAD_MAP_WORLD_CLOUDS_SNOWHEAD_2
    -69,  // mapPageVtx[88] QUAD_MAP_WORLD_CLOUDS_SNOWHEAD_3
    -32,  // mapPageVtx[92] QUAD_MAP_WORLD_CLOUDS_ROMANI_RANCH
    -109, // mapPageVtx[96] QUAD_MAP_WORLD_CLOUDS_GREAT_BAY_1
    -45,  // mapPageVtx[100] QUAD_MAP_WORLD_CLOUDS_GREAT_BAY_2
    -109, // mapPageVtx[104] QUAD_MAP_WORLD_CLOUDS_GREAT_BAY_3
    -45,  // mapPageVtx[108] QUAD_MAP_WORLD_CLOUDS_GREAT_BAY_4
    59,   // mapPageVtx[112] QUAD_MAP_WORLD_CLOUDS_STONE_TOWER_1
    -5,   // mapPageVtx[116] QUAD_MAP_WORLD_CLOUDS_STONE_TOWER_2
    -70,  // mapPageVtx[120] QUAD_MAP_REGION_GREAT_BAY
    -52,  // mapPageVtx[124] QUAD_MAP_REGION_ZORA_HALL
    -11,  // mapPageVtx[128] QUAD_MAP_REGION_ROMANI_RANCH
    -2,   // mapPageVtx[132] QUAD_MAP_REGION_DEKU_PALACE
    8,    // mapPageVtx[136] QUAD_MAP_REGION_WOODFALL
    2,    // mapPageVtx[140] QUAD_MAP_REGION_CLOCK_TOWN
    1,    // mapPageVtx[144] QUAD_MAP_REGION_SNOWHEAD
    43,   // mapPageVtx[148] QUAD_MAP_REGION_IKANA_GRAVEYARD
    52,   // mapPageVtx[152] QUAD_MAP_REGION_IKANA_CANYON
    54,   // mapPageVtx[156] QUAD_MAP_REGION_GORON_VILLAGE
    62,   // mapPageVtx[160] QUAD_MAP_REGION_STONE_TOWER
    -80,  // mapPageVtx[164] QUAD_MAP_OWL_WARP_GREAT_BAY_COAST
    -64,  // mapPageVtx[168] QUAD_MAP_OWL_WARP_ZORA_CAPE
    -9,   // mapPageVtx[172] QUAD_MAP_OWL_WARP_SNOWHEAD
    -3,   // mapPageVtx[176] QUAD_MAP_OWL_WARP_MOUNTAIN_VILLAGE
    -7,   // mapPageVtx[180] QUAD_MAP_OWL_WARP_CLOCK_TOWN
    -16,  // mapPageVtx[184] QUAD_MAP_OWL_WARP_MILK_ROAD
    -1,   // mapPageVtx[188] QUAD_MAP_OWL_WARP_WOODFALL
    23,   // mapPageVtx[192] QUAD_MAP_OWL_WARP_SOUTHERN_SWAMP
    44,   // mapPageVtx[196] QUAD_MAP_OWL_WARP_IKANA_CANYON
    54,   // mapPageVtx[200] QUAD_MAP_OWL_WARP_STONE_TOWER
};

static s16 gVtxPageMapWorldQuadsWidth[VTX_PAGE_MAP_WORLD_QUADS] = {
    80,  // mapPageVtx[60]  QUAD_MAP_WORLD_CLOUDS_CLOCK_TOWN_1
    64,  // mapPageVtx[64]  QUAD_MAP_WORLD_CLOUDS_CLOCK_TOWN_2
    64,  // mapPageVtx[68]  QUAD_MAP_WORLD_CLOUDS_WOODFALL_1
    64,  // mapPageVtx[72]  QUAD_MAP_WORLD_CLOUDS_WOODFALL_2
    16,  // mapPageVtx[76]  QUAD_MAP_WORLD_CLOUDS_WOODFALL_3
    64,  // mapPageVtx[80]  QUAD_MAP_WORLD_CLOUDS_SNOWHEAD_1
    112, // mapPageVtx[84]  QUAD_MAP_WORLD_CLOUDS_SNOWHEAD_2
    112, // mapPageVtx[88]  QUAD_MAP_WORLD_CLOUDS_SNOWHEAD_3
    48,  // mapPageVtx[92]  QUAD_MAP_WORLD_CLOUDS_ROMANI_RANCH
    64,  // mapPageVtx[96]  QUAD_MAP_WORLD_CLOUDS_GREAT_BAY_1
    64,  // mapPageVtx[100] QUAD_MAP_WORLD_CLOUDS_GREAT_BAY_2
    64,  // mapPageVtx[104] QUAD_MAP_WORLD_CLOUDS_GREAT_BAY_3
    64,  // mapPageVtx[108] QUAD_MAP_WORLD_CLOUDS_GREAT_BAY_4
    48,  // mapPageVtx[112] QUAD_MAP_WORLD_CLOUDS_STONE_TOWER_1
    64,  // mapPageVtx[116] QUAD_MAP_WORLD_CLOUDS_STONE_TOWER_2
    8,   // mapPageVtx[120] QUAD_MAP_REGION_GREAT_BAY
    8,   // mapPageVtx[124] QUAD_MAP_REGION_ZORA_HALL
    8,   // mapPageVtx[128] QUAD_MAP_REGION_ROMANI_RANCH
    8,   // mapPageVtx[132] QUAD_MAP_REGION_DEKU_PALACE
    8,   // mapPageVtx[136] QUAD_MAP_REGION_WOODFALL
    8,   // mapPageVtx[140] QUAD_MAP_REGION_CLOCK_TOWN
    8,   // mapPageVtx[144] QUAD_MAP_REGION_SNOWHEAD
    8,   // mapPageVtx[148] QUAD_MAP_REGION_IKANA_GRAVEYARD
    8,   // mapPageVtx[152] QUAD_MAP_REGION_IKANA_CANYON
    8,   // mapPageVtx[156] QUAD_MAP_REGION_GORON_VILLAGE
    8,   // mapPageVtx[160] QUAD_MAP_REGION_STONE_TOWER
    24,  // mapPageVtx[164] QUAD_MAP_OWL_WARP_GREAT_BAY_COAST
    24,  // mapPageVtx[168] QUAD_MAP_OWL_WARP_ZORA_CAPE
    24,  // mapPageVtx[172] QUAD_MAP_OWL_WARP_SNOWHEAD
    24,  // mapPageVtx[176] QUAD_MAP_OWL_WARP_MOUNTAIN_VILLAGE
    24,  // mapPageVtx[180] QUAD_MAP_OWL_WARP_CLOCK_TOWN
    24,  // mapPageVtx[184] QUAD_MAP_OWL_WARP_MILK_ROAD
    24,  // mapPageVtx[188] QUAD_MAP_OWL_WARP_WOODFALL
    24,  // mapPageVtx[192] QUAD_MAP_OWL_WARP_SOUTHERN_SWAMP
    24,  // mapPageVtx[196] QUAD_MAP_OWL_WARP_IKANA_CANYON
    24,  // mapPageVtx[200] QUAD_MAP_OWL_WARP_STONE_TOWER
};
static s16 sVtxPageMapWorldQuadsY[VTX_PAGE_MAP_WORLD_QUADS] = {
    11,  // mapPageVtx[60] QUAD_MAP_WORLD_CLOUDS_CLOCK_TOWN_1
    22,  // mapPageVtx[64] QUAD_MAP_WORLD_CLOUDS_CLOCK_TOWN_2
    -16, // mapPageVtx[68] QUAD_MAP_WORLD_CLOUDS_WOODFALL_1
    -16, // mapPageVtx[72] QUAD_MAP_WORLD_CLOUDS_WOODFALL_2
    -16, // mapPageVtx[76] QUAD_MAP_WORLD_CLOUDS_WOODFALL_3
    59,  // mapPageVtx[80] QUAD_MAP_WORLD_CLOUDS_SNOWHEAD_1
    24,  // mapPageVtx[84] QUAD_MAP_WORLD_CLOUDS_SNOWHEAD_2
    59,  // mapPageVtx[88] QUAD_MAP_WORLD_CLOUDS_SNOWHEAD_3
    -11, // mapPageVtx[92] QUAD_MAP_WORLD_CLOUDS_ROMANI_RANCH
    -5,  // mapPageVtx[96] QUAD_MAP_WORLD_CLOUDS_GREAT_BAY_1
    -5,  // mapPageVtx[100] QUAD_MAP_WORLD_CLOUDS_GREAT_BAY_2
    59,  // mapPageVtx[104] QUAD_MAP_WORLD_CLOUDS_GREAT_BAY_3
    59,  // mapPageVtx[108] QUAD_MAP_WORLD_CLOUDS_GREAT_BAY_4
    39,  // mapPageVtx[112] QUAD_MAP_WORLD_CLOUDS_STONE_TOWER_1
    31,  // mapPageVtx[116] QUAD_MAP_WORLD_CLOUDS_STONE_TOWER_2
    -10, // mapPageVtx[120] QUAD_MAP_REGION_GREAT_BAY
    -36, // mapPageVtx[124] QUAD_MAP_REGION_ZORA_HALL
    -19, // mapPageVtx[128] QUAD_MAP_REGION_ROMANI_RANCH
    -53, // mapPageVtx[132] QUAD_MAP_REGION_DEKU_PALACE
    -27, // mapPageVtx[136] QUAD_MAP_REGION_WOODFALL
    -2,  // mapPageVtx[140] QUAD_MAP_REGION_CLOCK_TOWN
    41,  // mapPageVtx[144] QUAD_MAP_REGION_SNOWHEAD
    11,  // mapPageVtx[148] QUAD_MAP_REGION_IKANA_GRAVEYARD
    -3,  // mapPageVtx[152] QUAD_MAP_REGION_IKANA_CANYON
    45,  // mapPageVtx[156] QUAD_MAP_REGION_GORON_VILLAGE
    18,  // mapPageVtx[160] QUAD_MAP_REGION_STONE_TOWER
    -8,  // mapPageVtx[164] QUAD_MAP_OWL_WARP_GREAT_BAY_COAST
    -38, // mapPageVtx[168] QUAD_MAP_OWL_WARP_ZORA_CAPE
    39,  // mapPageVtx[172] QUAD_MAP_OWL_WARP_SNOWHEAD
    26,  // mapPageVtx[176] QUAD_MAP_OWL_WARP_MOUNTAIN_VILLAGE
    1,   // mapPageVtx[180] QUAD_MAP_OWL_WARP_CLOCK_TOWN
    -7,  // mapPageVtx[184] QUAD_MAP_OWL_WARP_MILK_ROAD
    -28, // mapPageVtx[188] QUAD_MAP_OWL_WARP_WOODFALL
    -27, // mapPageVtx[192] QUAD_MAP_OWL_WARP_SOUTHERN_SWAMP
    -1,  // mapPageVtx[196] QUAD_MAP_OWL_WARP_IKANA_CANYON
    24,  // mapPageVtx[200] QUAD_MAP_OWL_WARP_STONE_TOWER
};
static s16 gVtxPageMapWorldQuadsHeight[VTX_PAGE_MAP_WORLD_QUADS] = {
    40, // mapPageVtx[60] QUAD_MAP_WORLD_CLOUDS_CLOCK_TOWN_1
    31, // mapPageVtx[64] QUAD_MAP_WORLD_CLOUDS_CLOCK_TOWN_2
    53, // mapPageVtx[68] QUAD_MAP_WORLD_CLOUDS_WOODFALL_1
    53, // mapPageVtx[72] QUAD_MAP_WORLD_CLOUDS_WOODFALL_2
    53, // mapPageVtx[76] QUAD_MAP_WORLD_CLOUDS_WOODFALL_3
    52, // mapPageVtx[80] QUAD_MAP_WORLD_CLOUDS_SNOWHEAD_1
    35, // mapPageVtx[84] QUAD_MAP_WORLD_CLOUDS_SNOWHEAD_2
    35, // mapPageVtx[88] QUAD_MAP_WORLD_CLOUDS_SNOWHEAD_3
    32, // mapPageVtx[92] QUAD_MAP_WORLD_CLOUDS_ROMANI_RANCH
    64, // mapPageVtx[96] QUAD_MAP_WORLD_CLOUDS_GREAT_BAY_1
    64, // mapPageVtx[100] QUAD_MAP_WORLD_CLOUDS_GREAT_BAY_2
    64, // mapPageVtx[104] QUAD_MAP_WORLD_CLOUDS_GREAT_BAY_3
    64, // mapPageVtx[108] QUAD_MAP_WORLD_CLOUDS_GREAT_BAY_4
    82, // mapPageVtx[112] QUAD_MAP_WORLD_CLOUDS_STONE_TOWER_1
    61, // mapPageVtx[116] QUAD_MAP_WORLD_CLOUDS_STONE_TOWER_2
    8,  // mapPageVtx[120] QUAD_MAP_REGION_GREAT_BAY
    8,  // mapPageVtx[124] QUAD_MAP_REGION_ZORA_HALL
    8,  // mapPageVtx[128] QUAD_MAP_REGION_ROMANI_RANCH
    8,  // mapPageVtx[132] QUAD_MAP_REGION_DEKU_PALACE
    8,  // mapPageVtx[136] QUAD_MAP_REGION_WOODFALL
    8,  // mapPageVtx[140] QUAD_MAP_REGION_CLOCK_TOWN
    8,  // mapPageVtx[144] QUAD_MAP_REGION_SNOWHEAD
    8,  // mapPageVtx[148] QUAD_MAP_REGION_IKANA_GRAVEYARD
    8,  // mapPageVtx[152] QUAD_MAP_REGION_IKANA_CANYON
    8,  // mapPageVtx[156] QUAD_MAP_REGION_GORON_VILLAGE
    8,  // mapPageVtx[160] QUAD_MAP_REGION_STONE_TOWER
    12, // mapPageVtx[164] QUAD_MAP_OWL_WARP_GREAT_BAY_COAST
    12, // mapPageVtx[168] QUAD_MAP_OWL_WARP_ZORA_CAPE
    12, // mapPageVtx[172] QUAD_MAP_OWL_WARP_SNOWHEAD
    12, // mapPageVtx[176] QUAD_MAP_OWL_WARP_MOUNTAIN_VILLAGE
    12, // mapPageVtx[180] QUAD_MAP_OWL_WARP_CLOCK_TOWN
    12, // mapPageVtx[184] QUAD_MAP_OWL_WARP_MILK_ROAD
    12, // mapPageVtx[188] QUAD_MAP_OWL_WARP_WOODFALL
    12, // mapPageVtx[192] QUAD_MAP_OWL_WARP_SOUTHERN_SWAMP
    12, // mapPageVtx[196] QUAD_MAP_OWL_WARP_IKANA_CANYON
    12, // mapPageVtx[200] QUAD_MAP_OWL_WARP_STONE_TOWER
};

static s16* sVtxPageQuadsX[VTX_PAGE_MAX] = {
    sVtxPageMapWorldQuadsX,   // VTX_PAGE_MAP_WORLD
};

static s16* sVtxPageQuadsWidth[VTX_PAGE_MAX] = {
    gVtxPageMapWorldQuadsWidth,   // VTX_PAGE_MAP_WORLD
};

static s16* sVtxPageQuadsY[VTX_PAGE_MAX] = {
    sVtxPageMapWorldQuadsY,   // VTX_PAGE_MAP_WORLD
};

static s16* sVtxPageQuadsHeight[VTX_PAGE_MAX] = {
    gVtxPageMapWorldQuadsHeight,   // VTX_PAGE_MAP_WORLD
};

static const u32 sWorldMapClockTownCloud1Tex = 0x0C008418;
static const u32 sWorldMapClockTownCloud2Tex = 0x0C009098;
static const u32 sWorldMapRomaniRanchCloudTex = 0x0C009858;
static const u32 sWorldMapGreatBayCloud1Tex = 0x0C009E58;
static const u32 sWorldMapGreatBayCloud2Tex = 0x0C00AE58;
static const u32 sWorldMapGreatBayCloud3Tex = 0x0C00BE58;
static const u32 sWorldMapGreatBayCloud4Tex = 0x0C00CE58;
static const u32 sWorldMapSnowheadCloud1Tex = 0x0C00DE58;
static const u32 sWorldMapSnowheadCloud2Tex = 0x0C00EB58;
static const u32 sWorldMapSnowheadCloud3Tex = 0x0C00FAA8;
static const u32 sWorldMapStoneTowerCloud1Tex = 0x0C0109F8;
static const u32 sWorldMapStoneTowerCloud2Tex = 0x0C011958;
static const u32 sWorldMapWoodfallCloud1Tex = 0x0C012898;
static const u32 sWorldMapWoodfallCloud2Tex = 0x0C0135D8;
static const u32 sWorldMapWoodfallCloud3Tex = 0x0C014318;

static u32 sCloudTextures[] = {
    sWorldMapClockTownCloud1Tex,  // TINGLE_MAP_CLOCK_TOWN
    sWorldMapClockTownCloud2Tex,  // TINGLE_MAP_CLOCK_TOWN
    sWorldMapWoodfallCloud1Tex,   // TINGLE_MAP_WOODFALL
    sWorldMapWoodfallCloud2Tex,   // TINGLE_MAP_WOODFALL
    sWorldMapWoodfallCloud3Tex,   // TINGLE_MAP_WOODFALL
    sWorldMapSnowheadCloud1Tex,   // TINGLE_MAP_SNOWHEAD
    sWorldMapSnowheadCloud2Tex,   // TINGLE_MAP_SNOWHEAD
    sWorldMapSnowheadCloud3Tex,   // TINGLE_MAP_SNOWHEAD
    sWorldMapRomaniRanchCloudTex, // TINGLE_MAP_ROMANI_RANCH
    sWorldMapGreatBayCloud1Tex,   // TINGLE_MAP_GREAT_BAY
    sWorldMapGreatBayCloud2Tex,   // TINGLE_MAP_GREAT_BAY
    sWorldMapGreatBayCloud3Tex,   // TINGLE_MAP_GREAT_BAY
    sWorldMapGreatBayCloud4Tex,   // TINGLE_MAP_GREAT_BAY
    sWorldMapStoneTowerCloud1Tex, // TINGLE_MAP_STONE_TOWER
    sWorldMapStoneTowerCloud2Tex, // TINGLE_MAP_STONE_TOWER
};

static f32 sOwlWarpWorldMapCursorsX[OWL_WARP_MAX - 1] = {
    -50.0f, // OWL_WARP_GREAT_BAY_COAST
    -38.0f, // OWL_WARP_ZORA_CAPE
    6.0f,   // OWL_WARP_SNOWHEAD
    11.0f,  // OWL_WARP_MOUNTAIN_VILLAGE
    8.0f,   // OWL_WARP_CLOCK_TOWN
    0.0f,   // OWL_WARP_MILK_ROAD
    12.0f,  // OWL_WARP_WOODFALL
    31.0f,  // OWL_WARP_SOUTHERN_SWAMP
    48.0f,  // OWL_WARP_IKANA_CANYON
    56.0f,  // OWL_WARP_STONE_TOWER
};
static f32 sOwlWarpWorldMapCursorsY[OWL_WARP_MAX - 1] = {
    -14.0f, // OWL_WARP_GREAT_BAY_COAST
    -39.0f, // OWL_WARP_ZORA_CAPE
    23.0f,  // OWL_WARP_SNOWHEAD
    11.0f,  // OWL_WARP_MOUNTAIN_VILLAGE
    -8.0f,  // OWL_WARP_CLOCK_TOWN
    -15.0f, // OWL_WARP_MILK_ROAD
    -31.0f, // OWL_WARP_WOODFALL
    -30.0f, // OWL_WARP_SOUTHERN_SWAMP
    -10.0f, // OWL_WARP_IKANA_CANYON
    11.0f,  // OWL_WARP_STONE_TOWER
};

static s16 sCursorPrimColorTarget[][3] = {
    { 255, 255, 255 }, { 255, 255, 255 }, { 255, 255, 0 }, { 255, 255, 0 }, { 100, 150, 255 }, { 100, 255, 255 },
};
static s16 sCursorEnvColorTarget[][3] = {
    { 0, 0, 0 }, { 170, 170, 170 }, { 0, 0, 0 }, { 255, 160, 0 }, { 0, 0, 100 }, { 0, 150, 255 },
};

static f32 sCursorX;
static f32 sCursorY;
static f32 sCursorWidth;
static f32 sCursorHeight;
static f32 sCursorShrinkRate;
static s16 sCursorSpinPhase = 0xE000;
static s16 sCursorPrimR = 0;
static s16 sCursorPrimG = 0;
static s16 sCursorPrimB = 0;
static s16 sCursorEnvR = 0;
static s16 sCursorEnvG = 0;
static s16 sCursorEnvB = 0;
static f32 sCursorCirclesX[4];
static f32 sCursorCirclesY[4];

static u32 sCursorTexs[] = {
    0x08000000 + 0x5C1C0,
    0x08000000 + 0x5C240,
    0x08000000 + 0x5C340,
    0x08000000 + 0x5C2C0,
};

static void KaleidoScope_UpdateSoaringMapCursor(GameState_Play* play)
{
    PauseContext* pauseCtx = &play->pauseCtx;
    s16 oldCursorPoint;

    pauseCtx->cursorColorSet = 4; // PAUSE_CURSOR_COLOR_SET_BLUE;
    oldCursorPoint = pauseCtx->cursorPoint[PAUSE_WORLD_MAP];

    if (pauseCtx->stickAdjX > 30) {
        sCursorShrinkRate = 4.0f;
        do {
            pauseCtx->cursorPoint[PAUSE_WORLD_MAP]++;
            if (pauseCtx->cursorPoint[PAUSE_WORLD_MAP] > OWL_WARP_STONE_TOWER)
            {
                pauseCtx->cursorPoint[PAUSE_WORLD_MAP] = OWL_WARP_GREAT_BAY_COAST;
            }
        } while (!pauseCtx->worldMapPoints[pauseCtx->cursorPoint[PAUSE_WORLD_MAP]]);
    } else if (pauseCtx->stickAdjX < -30) {
        sCursorShrinkRate = 4.0f;
        do {
            pauseCtx->cursorPoint[PAUSE_WORLD_MAP]--;
            if (pauseCtx->cursorPoint[PAUSE_WORLD_MAP] < OWL_WARP_GREAT_BAY_COAST)
            {
                pauseCtx->cursorPoint[PAUSE_WORLD_MAP] = OWL_WARP_STONE_TOWER;;
            }
        } while (!pauseCtx->worldMapPoints[pauseCtx->cursorPoint[PAUSE_WORLD_MAP]]);
    }

    //! TODO: Is the `0xA4` here related to `0xA3` being the last recored item in the `ItemId` enum?
    pauseCtx->cursorItem[PAUSE_MAP] = sOwlWarpPauseItems[pauseCtx->cursorPoint[PAUSE_WORLD_MAP]] - 0xA4;
    // Used as cursor vtxIndex
    pauseCtx->cursorSlot[PAUSE_MAP] = 31 + pauseCtx->cursorPoint[PAUSE_WORLD_MAP];

    if (oldCursorPoint != pauseCtx->cursorPoint[PAUSE_WORLD_MAP]) {
        PlaySound(0x4809); // NA_SE_SY_CURSOR
    }
}

static void LoadMapName(void* dst, u32 mapNameId)
{
    DmaEntry dma;

    comboDmaLookupForeignId(&dma, 18);
    u32 vromAddr = dma.vstart;
    // u32 size = 0x400;
    u32 headerSize;
    LoadFile(&headerSize, vromAddr, sizeof(headerSize));

    u32 offsets[2];
    LoadFile(&offsets, vromAddr + (mapNameId * 4), sizeof(offsets));
    if (mapNameId == 0)
    {
        offsets[0] = 0;
    }

    u32 size = offsets[1] - offsets[0];

    u32 romAddr = dma.pstart;

    DmaCompressed(romAddr + headerSize + offsets[0], dst, size);
}

static void KaleidoScope_UpdateOwlWarpNamePanel(GameState_Play* play)
{
    PauseContext* pauseCtx;
    u16 texIndex;

    pauseCtx = &play->pauseCtx;
    if ((pauseCtx->namedItem != pauseCtx->cursorItem[PAUSE_MAP]) || ((pauseCtx->screen_idx == 1) && (pauseCtx->cursorSpecialPos != 0)))
    {
        pauseCtx->namedItem = pauseCtx->cursorItem[PAUSE_MAP];
        texIndex = pauseCtx->namedItem;

        osCreateMesgQueue(&pauseCtx->loadQueue, &pauseCtx->loadMsg, 1);

        if (texIndex <= 0xF)
        {
            LoadMapName(pauseCtx->nameSegment, texIndex);
            pauseCtx->nameDisplayTimer = 0;
        }
        else
        {
            pauseCtx->nameDisplayTimer = 70;
        }
    }
}

static u32 sMapPageBgTextures[15];

void KaleidoScope_BeforeUpdate(GameState_Play* play)
{
    u32 size;
    static s16 sStickXRepeatTimer = 0;
    static s16 sStickYRepeatTimer = 0;
    static s16 sStickXRepeatState = 0;
    static s16 sStickYRepeatState = 0;
    PauseContext* pauseCtx = &play->pauseCtx;
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    MessageContext* msgCtx = &play->msgCtx;
    ControllerInput* input = &play->gs.input[0];

    if (pauseCtx->state >= PAUSE_STATE_OWLWARP_2 && pauseCtx->state <= PAUSE_STATE_OWLWARP_6)
    {
        pauseCtx->stickAdjX = input->released.x;
        pauseCtx->stickAdjY = input->released.y;

        if (pauseCtx->stickAdjX < -30) {
            if (sStickXRepeatState == -1) {
                sStickXRepeatTimer--;
                if (sStickXRepeatTimer < 0) {
                    // Allow the input to register and apply the delay for all subsequent repeated inputs
                    sStickXRepeatTimer = 2;
                } else {
                    // Cancel the current input
                    pauseCtx->stickAdjX = 0;
                }
            } else {
                // Allow the input to register and apply the delay for the first repeated input
                sStickXRepeatTimer = 10;
                sStickXRepeatState = -1;
            }
        } else if (pauseCtx->stickAdjX > 30) {
            if (sStickXRepeatState == 1) {
                sStickXRepeatTimer--;
                if (sStickXRepeatTimer < 0) {
                    // Allow the input to register and apply the delay for all subsequent repeated inputs
                    sStickXRepeatTimer = 2;
                } else {
                    // Cancel the current input
                    pauseCtx->stickAdjX = 0;
                }
            } else {
                // Allow the input to register and apply the delay for the first repeated input
                sStickXRepeatTimer = 10;
                sStickXRepeatState = 1;
            }
        } else {
            sStickXRepeatState = 0;
        }

        if (pauseCtx->stickAdjY < -30) {
            if (sStickYRepeatState == -1) {
                sStickYRepeatTimer--;
                if (sStickYRepeatTimer < 0) {
                    // Allow the input to register and apply the delay for all subsequent repeated inputs
                    sStickYRepeatTimer = 2;
                } else {
                    // Cancel the current input
                    pauseCtx->stickAdjY = 0;
                }
            } else {
                // Allow the input to register and apply the delay for the first repeated input
                sStickYRepeatTimer = 10;
                sStickYRepeatState = -1;
            }
        } else if (pauseCtx->stickAdjY > 30) {
            if (sStickYRepeatState == 1) {
                sStickYRepeatTimer--;
                if (sStickYRepeatTimer < 0) {
                    // Allow the input to register and apply the delay for all subsequent repeated inputs
                    sStickYRepeatTimer = 2;
                } else {
                    // Cancel the current input
                    pauseCtx->stickAdjY = 0;
                }
            } else {
                // Allow the input to register and apply the delay for the first repeated input
                sStickYRepeatTimer = 10;
                sStickYRepeatState = 1;
            }
        } else {
            sStickYRepeatState = 0;
        }
    }

    switch (pauseCtx->state)
    {
        case PAUSE_STATE_OWLWARP_2:
            u8* sUnpausedButtonStatus = OverlayAddr(0x80830150);
            sUnpausedButtonStatus[0] = gSaveContext.buttonStatus[0];
            sUnpausedButtonStatus[1] = gSaveContext.buttonStatus[1];
            sUnpausedButtonStatus[2] = gSaveContext.buttonStatus[2];
            sUnpausedButtonStatus[3] = gSaveContext.buttonStatus[3];
            sUnpausedButtonStatus[4] = gSaveContext.buttonStatus[4];

            pauseCtx->cursorX[PAUSE_MAP] = 0;
            pauseCtx->cursorSlot[PAUSE_MAP] = pauseCtx->cursorPoint[PAUSE_MAP] = pauseCtx->dungeonMapSlot = VREG(30) + 3;

            WREG(16) = -175;
            WREG(17) = 155;

            u8* dest = PALIGN(play->objectCtx.spaceStart, 16);

            pauseCtx->iconItemSegment = dest;
            size = comboLoadFile(dest, 8); // icon_item_static
            dest = PALIGN(dest + size, 16);

            pauseCtx->iconItemAltSegment = dest;
            size = comboLoadForeignFile(dest, 10); // icon_item_field_static
            dest = PALIGN(dest + size, 16);

            pauseCtx->iconItemLangSegment = dest;
            size = comboLoadForeignFile(dest, 13); // icon_item_jpn_static
            dest = PALIGN(dest + size, 16);

            pauseCtx->nameSegment = dest;
            LoadMapName(dest, pauseCtx->cursorPoint[PAUSE_WORLD_MAP]);
            dest = PALIGN(dest + 0xA00, 16);

            u32* srcMapPageBgTextures = (u32*)OverlayAddr(0x80829B74);
            for (u8 i = 0; i < ARRAY_SIZE(sMapPageBgTextures); i++)
            {
                sMapPageBgTextures[i] = srcMapPageBgTextures[i];
            }
            sMapPageBgTextures[5] = 0x0D006200;

            Interface_SetDoAction(play, 0x06); // DO_ACTION_DECIDE
            Interface_LoadActionLabelB(play, 0x12); // DO_ACTION_STOP
            pauseCtx->state = PAUSE_STATE_OWLWARP_3;
            VREG(88) = 98;
            pauseCtx->promptChoice = 0; // PAUSE_PROMPT_YES
            break;

        case PAUSE_STATE_OWLWARP_3:
            R_PAUSE_OWLWARP_ALPHA += 20;
            pauseCtx->infoPanelOffsetY += 10;
            pauseCtx->mapPageRoll -= 40.0f;
            interfaceCtx->startAlpha += 63;
            VREG(88) -= 3;
            WREG(16) += WREG(25) / WREG(6);
            WREG(17) += WREG(26) / WREG(6);
            pauseCtx->alpha += 31;

            if (pauseCtx->mapPageRoll == 0)
            {
                interfaceCtx->startAlpha = 255;
                WREG(2) = 0;
                pauseCtx->alpha = 255;
                pauseCtx->changing = 0; // PAUSE_MAIN_STATE_IDLE;
                pauseCtx->cursorSpecialPos = 0;
                pauseCtx->state = PAUSE_STATE_OWLWARP_SELECT;
                R_PAUSE_OWLWARP_ALPHA = 120;
            }
            break;

        case PAUSE_STATE_OWLWARP_SELECT:
            // TODO for some reason pressing START causes a "failed ocarina" sfx.
            if ((play->gs.input[0].pressed.buttons & START_BUTTON) || (play->gs.input[0].pressed.buttons & B_BUTTON))
            {
                pauseCtx->state = PAUSE_STATE_OWLWARP_6;
                WREG(2) = -6240;
                func_800C7200(0);
                gSoaringIndexSelected = -1;
                gSaveContext.prevHudVisibilityMode = 50; // HUD_VISIBILITY_ALL;
            }
            else if (play->gs.input[0].pressed.buttons & A_BUTTON)
            {
                msgCtx->msgLength = 0;
                msgCtx->msgMode = 0; // MSGMODE_NONE;
                pauseCtx->state = PAUSE_STATE_OWLWARP_6;
                WREG(2) = -6240;
                func_800C7200(0);
                gSoaringIndexSelected = (u8)pauseCtx->cursorPoint[PAUSE_WORLD_MAP];
                PlaySound(0x4808); // NA_SE_SY_DECIDE
            }
            else
            {
                KaleidoScope_UpdateOwlWarpNamePanel(play);
            }
            KaleidoScope_UpdateSoaringMapCursor(play);
            break;

        case PAUSE_STATE_OWLWARP_6:
            if (pauseCtx->mapPageRoll != 160.0f) {
                R_PAUSE_OWLWARP_ALPHA -= 60;
                if (R_PAUSE_OWLWARP_ALPHA <= 0) {
                    R_PAUSE_OWLWARP_ALPHA = 0;
                }
                pauseCtx->infoPanelOffsetY -= 10;
                pauseCtx->mapPageRoll += 40.0f;
                interfaceCtx->startAlpha -= 63;
                WREG(16) -= WREG(25) / WREG(6);
                WREG(17) -= WREG(26) / WREG(6);
                pauseCtx->alpha -= 63;
                if (pauseCtx->mapPageRoll == 160.0f) {
                    pauseCtx->alpha = 0;
                }
            } else {
                pauseCtx->debugState = 0; // DEBUG_EDITOR_NONE;
                pauseCtx->state = 0x13; // PAUSE_STATE_UNPAUSE_CLOSE;
                pauseCtx->itemPageRoll = pauseCtx->mapPageRoll = pauseCtx->questPageRoll = pauseCtx->maskPageRoll =
                    160.0f;
                pauseCtx->namedItem = PAUSE_ITEM_NONE;
                interfaceCtx->startAlpha = 0;
                pauseCtx->screen_idx = gPrevPageIndex;
                pauseCtx->cursorPoint[PAUSE_WORLD_MAP] = gPrevCursorPoint;
            }
            break;
    }
}

static void KaleidoScope_DrawWorldMap(GameState_Play* play) {
    s16 n;
    s16 j;
    s16 k;
    s16 i;
    PauseContext* pauseCtx = &play->pauseCtx;

    u32 gWorldMapImageTex = 0x0C000000;
    u32 gWorldMapImageTLUT = 0x0C006C00;
    u32 gWorldMapOwlFaceTex = 0x0C014668;

    OPEN_DISPS(play->gs.gfx);

    // KaleidoScope_SetCursorVtxPos(pauseCtx, pauseCtx->cursorSlot[PAUSE_MAP] * 4, pauseCtx->mapPageVtx);

    // Draw the world map angled
    // Because it is at an angle, vertices are used to place it.
    // The structure of the loops here is to satisfy the constraints of both TMEM and the size of the vertex cache.
    // - Each loop iteration loads 9 rows, because 9 is the largest number smaller than
    //   `TMEM_SIZE / 2 / textureWidth`.
    // - Each loop is at most 8 iterations long because each row uses 4 vertices and the vertex cache has size
    //   `32 = 8 * 4`.
    // .
    // Hence there is one loop of length 8, one of length 6, and then the remaining `128 - (8 + 6) * 9 = 2` rows are
    // drawn at the end.

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetTextureFilter(POLY_OPA_DISP++, G_TF_POINT);
    gDPLoadTLUT_pal256(POLY_OPA_DISP++, gWorldMapImageTLUT);
    gDPSetTextureLUT(POLY_OPA_DISP++, G_TT_RGBA16);

    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);

    // Set the vertices for the first 8 quads attached to the world map texture.
    gSPVertex(POLY_OPA_DISP++, &pauseCtx->mapPageVtx[QUAD_MAP_PAGE_WORLD_IMAGE_FIRST * 4], 8 * 4, 0);

    // Process the first 72 rows of pixels for gWorldMapImageTex, 9 rows at a time over 8 iterations
    // Loop over quadIndex of this loop (i), quadIndex of the entire texture (k), vtxIndex (j)
    for (i = 0, k = 0, j = 0; i < 8; i++, k++, j += 4) {
        gDPLoadTextureBlock(
            POLY_OPA_DISP++, (u8*)gWorldMapImageTex + k * (WORLD_MAP_IMAGE_WIDTH * WORLD_MAP_IMAGE_FRAG_HEIGHT),
            G_IM_FMT_CI, G_IM_SIZ_8b, WORLD_MAP_IMAGE_WIDTH, WORLD_MAP_IMAGE_FRAG_HEIGHT, 0,
            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        gSP1Quadrangle(POLY_OPA_DISP++, j, j + 2, j + 3, j + 1, 0);
    }

    // Set the vertices for the last 7 quads attached to the world map texture:
    // 6 quads with a height of 9, 1 quad with a height of 2
    gSPVertex(POLY_OPA_DISP++, &pauseCtx->mapPageVtx[(QUAD_MAP_PAGE_WORLD_IMAGE_FIRST + 8) * 4], (6 + 1) * 4, 0);

    // Process the next 54 rows of pixels for gWorldMapImageTex, 9 rows at a time over 6 iterations
    // Loop over quadIndex of this loop (i), quadIndex of the entire texture (k), vtxIndex (j)
    for (i = 0, j = 0; i < 6; i++, k++, j += 4) {
        gDPLoadTextureBlock(
            POLY_OPA_DISP++, (u8*)gWorldMapImageTex + k * (WORLD_MAP_IMAGE_WIDTH * WORLD_MAP_IMAGE_FRAG_HEIGHT),
            G_IM_FMT_CI, G_IM_SIZ_8b, WORLD_MAP_IMAGE_WIDTH, WORLD_MAP_IMAGE_FRAG_HEIGHT, 0,
            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        gSP1Quadrangle(POLY_OPA_DISP++, j, j + 2, j + 3, j + 1, 0);
    }

    // Process the last 2 rows of pixels for gWorldMapImageTex
    gDPLoadTextureBlock(
        POLY_OPA_DISP++, (u8*)gWorldMapImageTex + k * (WORLD_MAP_IMAGE_WIDTH * WORLD_MAP_IMAGE_FRAG_HEIGHT),
        G_IM_FMT_CI, G_IM_SIZ_8b, WORLD_MAP_IMAGE_WIDTH, WORLD_MAP_IMAGE_HEIGHT % WORLD_MAP_IMAGE_FRAG_HEIGHT, 0,
        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gSP1Quadrangle(POLY_OPA_DISP++, j, j + 2, j + 3, j + 1, 0);

    Gfx_SetupDL_42Opa(play->gs.gfx);

    gDPPipeSync(POLY_OPA_DISP++);

    gDPSetTextureFilter(POLY_OPA_DISP++, G_TF_BILERP);

    gDPSetCombineLERP(POLY_OPA_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, PRIMITIVE,
                      ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);

    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 215, 235, 235, pauseCtx->alpha);
    gDPSetEnvColor(POLY_OPA_DISP++, 40, 60, 100, 0);

    // Draw clouds over the world map
    // Iterate over cloud bits (n)
    for (n = 0; n < WORLD_MAP_NUM_CLOUDS; n++) {
        if (!(((void)0, gMmSave.worldMapCloudVisibility) & (1 << n))) {

            gSPVertex(POLY_OPA_DISP++, &pauseCtx->mapPageVtx[(QUAD_MAP_PAGE_WORLD_CLOUDS_FIRST + n) * 4], 4, 0);

            gDPLoadTextureBlock(POLY_OPA_DISP++, sCloudTextures[n], G_IM_FMT_IA, G_IM_SIZ_8b, gVtxPageMapWorldQuadsWidth[n], gVtxPageMapWorldQuadsHeight[n], 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                                G_TX_NOLOD);

            gSP1Quadrangle(POLY_OPA_DISP++, 0, 2, 3, 1, 0);
        }
    }

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetRenderMode(POLY_OPA_DISP++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 0, 0, R_PAUSE_OWLWARP_ALPHA);
    gDPFillRectangle(POLY_OPA_DISP++, 50, 62, 270, 190);

    Gfx_SetupDL_42Opa(play->gs.gfx);

    // Selecting an owl warp
    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPLoadTextureBlock(POLY_OPA_DISP++, gWorldMapOwlFaceTex, G_IM_FMT_RGBA, G_IM_SIZ_32b, 24, 12, 0,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);

    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);

    // Loop over OwlWarpId (i), unused vtxIndex (j), unused (k)
    for (i = 0, j = 0; i <= 9; i++, k++, j += 4) { // OWL_WARP_STONE_TOWER
        if (pauseCtx->worldMapPoints[i]) {
            gSPVertex(POLY_OPA_DISP++, &pauseCtx->mapPageVtx[(QUAD_MAP_PAGE_WORLD_WARP_FIRST + i) * 4], 4, 0);
            gSP1Quadrangle(POLY_OPA_DISP++, 0, 2, 3, 1, 0);
        }
    }

    gDPPipeSync(POLY_OPA_DISP++);

    CLOSE_DISPS();
}

typedef Gfx* (*KaleidoScope_DrawPageSections_Func)(Gfx*, Vtx*, void*);

static void KaleidoScope_DrawOwlWarpMapPage(GameState_Play* play)
{
    PauseContext* pauseCtx = &play->pauseCtx;

    OPEN_DISPS(play->gs.gfx);

    gDPPipeSync(POLY_OPA_DISP++);

    gDPSetCombineLERP(POLY_OPA_DISP++, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0,
                      SHADE, 0);

    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 180, 180, 120, 255);

    ModelViewTranslate((f32)WREG(3) / 100.0f, (f32)WREG(2) / 100.0f, 0.0f, MAT_SET);
    ModelViewScale(0.78f, 0.78f, 0.78f, MAT_MUL);
    ModelViewRotateZ(-pauseCtx->mapPageRoll / 100.0f, MAT_MUL);
    ModelViewRotateY(-1.57f, MAT_MUL);

    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    KaleidoScope_DrawPageSections_Func KaleidoScope_DrawPageSections = OverlayAddr(0x8081FAD8);
    POLY_OPA_DISP = KaleidoScope_DrawPageSections(POLY_OPA_DISP, pauseCtx->mapPageVtx, sMapPageBgTextures);

    KaleidoScope_DrawWorldMap(play);

    CLOSE_DISPS();
}

static void KaleidoScope_DrawOwlWarpInfoPanel(GameState_Play* play)
{
    static s16 sPauseLRCursorColorTargets[][4] = {
        { 180, 210, 255, 220 },
        { 100, 100, 150, 220 },
    };
    static s16 sPauseLRCursorColorTimer = 20;
    static s16 sPauseLRCursorColorIndex = 0;
    static s16 sPauseLRCursorRed;
    static s16 sPauseLRCursorGreen;
    static s16 sPauseLRCursorBlue;
    static s16 sPauseLRCursorAlpha;
    PauseContext* pauseCtx = &play->pauseCtx;
    s16 stepR;
    s16 stepG;
    s16 stepB;
    s16 stepA;
    s16 y;
    s16 i;
    s16 j;

    OPEN_DISPS(play->gs.gfx);

    stepR =
        ABS_ALT(sPauseLRCursorRed - sPauseLRCursorColorTargets[sPauseLRCursorColorIndex][0]) / sPauseLRCursorColorTimer;
    stepG = ABS_ALT(sPauseLRCursorGreen - sPauseLRCursorColorTargets[sPauseLRCursorColorIndex][1]) /
            sPauseLRCursorColorTimer;
    stepB = ABS_ALT(sPauseLRCursorBlue - sPauseLRCursorColorTargets[sPauseLRCursorColorIndex][2]) /
            sPauseLRCursorColorTimer;
    stepA = ABS_ALT(sPauseLRCursorAlpha - sPauseLRCursorColorTargets[sPauseLRCursorColorIndex][3]) /
            sPauseLRCursorColorTimer;

    if (sPauseLRCursorRed >= sPauseLRCursorColorTargets[sPauseLRCursorColorIndex][0]) {
        sPauseLRCursorRed -= stepR;
    } else {
        sPauseLRCursorRed += stepR;
    }

    if (sPauseLRCursorGreen >= sPauseLRCursorColorTargets[sPauseLRCursorColorIndex][1]) {
        sPauseLRCursorGreen -= stepG;
    } else {
        sPauseLRCursorGreen += stepG;
    }

    if (sPauseLRCursorBlue >= sPauseLRCursorColorTargets[sPauseLRCursorColorIndex][2]) {
        sPauseLRCursorBlue -= stepB;
    } else {
        sPauseLRCursorBlue += stepB;
    }

    if (sPauseLRCursorAlpha >= sPauseLRCursorColorTargets[sPauseLRCursorColorIndex][3]) {
        sPauseLRCursorAlpha -= stepA;
    } else {
        sPauseLRCursorAlpha += stepA;
    }

    sPauseLRCursorColorTimer--;
    if (sPauseLRCursorColorTimer == 0) {
        sPauseLRCursorRed = sPauseLRCursorColorTargets[sPauseLRCursorColorIndex][0];
        sPauseLRCursorGreen = sPauseLRCursorColorTargets[sPauseLRCursorColorIndex][1];
        sPauseLRCursorBlue = sPauseLRCursorColorTargets[sPauseLRCursorColorIndex][2];
        sPauseLRCursorAlpha = sPauseLRCursorColorTargets[sPauseLRCursorColorIndex][3];
        sPauseLRCursorColorTimer = 20;
        sPauseLRCursorColorIndex ^= 1;
    }

    y = pauseCtx->infoPanelOffsetY - 76;
    for (j = 0, i = 0; i < 7; i++, j += 4) {
        pauseCtx->infoPanelVtx[j + 0].v.ob[0] = pauseCtx->infoPanelVtx[j + 2].v.ob[0] = -72;

        pauseCtx->infoPanelVtx[j + 1].v.ob[0] = pauseCtx->infoPanelVtx[j + 3].v.ob[0] = 0;

        pauseCtx->infoPanelVtx[j + 0].v.ob[1] = pauseCtx->infoPanelVtx[j + 1].v.ob[1] = y;

        pauseCtx->infoPanelVtx[j + 2].v.ob[1] = pauseCtx->infoPanelVtx[j + 3].v.ob[1] = y - 24;

        pauseCtx->infoPanelVtx[j + 0].v.ob[2] = pauseCtx->infoPanelVtx[j + 1].v.ob[2] =
            pauseCtx->infoPanelVtx[j + 2].v.ob[2] = pauseCtx->infoPanelVtx[j + 3].v.ob[2] = 0;

        pauseCtx->infoPanelVtx[j + 0].v.flag = pauseCtx->infoPanelVtx[j + 1].v.flag =
            pauseCtx->infoPanelVtx[j + 2].v.flag = pauseCtx->infoPanelVtx[j + 3].v.flag = 0;

        pauseCtx->infoPanelVtx[j + 0].v.tc[0] = pauseCtx->infoPanelVtx[j + 0].v.tc[1] =
            pauseCtx->infoPanelVtx[j + 1].v.tc[1] = pauseCtx->infoPanelVtx[j + 2].v.tc[0] = 0;

        pauseCtx->infoPanelVtx[j + 1].v.tc[0] = pauseCtx->infoPanelVtx[j + 3].v.tc[0] = 72 * (1 << 5);

        pauseCtx->infoPanelVtx[j + 2].v.tc[1] = pauseCtx->infoPanelVtx[j + 3].v.tc[1] = 24 * (1 << 5);

        pauseCtx->infoPanelVtx[j + 0].v.cn[0] = pauseCtx->infoPanelVtx[j + 2].v.cn[0] =
            pauseCtx->infoPanelVtx[j + 0].v.cn[1] = pauseCtx->infoPanelVtx[j + 2].v.cn[1] =
                pauseCtx->infoPanelVtx[j + 0].v.cn[2] = pauseCtx->infoPanelVtx[j + 2].v.cn[2] =
                    pauseCtx->infoPanelVtx[j + 1].v.cn[0] = pauseCtx->infoPanelVtx[j + 3].v.cn[0] =
                        pauseCtx->infoPanelVtx[j + 1].v.cn[1] = pauseCtx->infoPanelVtx[j + 3].v.cn[1] =
                            pauseCtx->infoPanelVtx[j + 1].v.cn[2] = pauseCtx->infoPanelVtx[j + 3].v.cn[2] = 200;

        pauseCtx->infoPanelVtx[j + 0].v.cn[3] = pauseCtx->infoPanelVtx[j + 2].v.cn[3] =
            pauseCtx->infoPanelVtx[j + 1].v.cn[3] = pauseCtx->infoPanelVtx[j + 3].v.cn[3] = pauseCtx->alpha;
    }

    pauseCtx->infoPanelVtx[4].v.ob[0] = pauseCtx->infoPanelVtx[6].v.ob[0] = pauseCtx->infoPanelVtx[0].v.ob[0] + 72;

    pauseCtx->infoPanelVtx[5].v.ob[0] = pauseCtx->infoPanelVtx[7].v.ob[0] = pauseCtx->infoPanelVtx[4].v.ob[0] + 72;

    if ((pauseCtx->cursorSpecialPos == PAUSE_CURSOR_PAGE_LEFT) && (pauseCtx->changing == PAUSE_MAIN_STATE_IDLE)) {
        pauseCtx->infoPanelVtx[8].v.ob[0] = pauseCtx->infoPanelVtx[10].v.ob[0] = WREG(16);

        pauseCtx->infoPanelVtx[9].v.ob[0] = pauseCtx->infoPanelVtx[11].v.ob[0] = pauseCtx->infoPanelVtx[8].v.ob[0] + 24;

        pauseCtx->infoPanelVtx[8].v.ob[1] = pauseCtx->infoPanelVtx[9].v.ob[1] = 10;

        pauseCtx->infoPanelVtx[10].v.ob[1] = pauseCtx->infoPanelVtx[11].v.ob[1] =
            pauseCtx->infoPanelVtx[8].v.ob[1] - 32;
    } else {
        pauseCtx->infoPanelVtx[8].v.ob[0] = pauseCtx->infoPanelVtx[10].v.ob[0] = WREG(16) + 3;

        pauseCtx->infoPanelVtx[9].v.ob[0] = pauseCtx->infoPanelVtx[11].v.ob[0] = pauseCtx->infoPanelVtx[8].v.ob[0] + 18;

        pauseCtx->infoPanelVtx[8].v.ob[1] = pauseCtx->infoPanelVtx[9].v.ob[1] = 10 - 3;

        pauseCtx->infoPanelVtx[10].v.ob[1] = pauseCtx->infoPanelVtx[11].v.ob[1] =
            pauseCtx->infoPanelVtx[8].v.ob[1] - 26;
    }

    if ((pauseCtx->cursorSpecialPos == PAUSE_CURSOR_PAGE_RIGHT) && (pauseCtx->changing == PAUSE_MAIN_STATE_IDLE)) {
        pauseCtx->infoPanelVtx[12].v.ob[0] = pauseCtx->infoPanelVtx[14].v.ob[0] = WREG(17);

        pauseCtx->infoPanelVtx[13].v.ob[0] = pauseCtx->infoPanelVtx[15].v.ob[0] =
            pauseCtx->infoPanelVtx[12].v.ob[0] + 24;

        pauseCtx->infoPanelVtx[12].v.ob[1] = pauseCtx->infoPanelVtx[13].v.ob[1] = 10;

        pauseCtx->infoPanelVtx[14].v.ob[1] = pauseCtx->infoPanelVtx[15].v.ob[1] =
            pauseCtx->infoPanelVtx[12].v.ob[1] - 32;
    } else {
        pauseCtx->infoPanelVtx[12].v.ob[0] = pauseCtx->infoPanelVtx[14].v.ob[0] = WREG(17) + 3;

        pauseCtx->infoPanelVtx[13].v.ob[0] = pauseCtx->infoPanelVtx[15].v.ob[0] =
            pauseCtx->infoPanelVtx[12].v.ob[0] + 18;

        pauseCtx->infoPanelVtx[12].v.ob[1] = pauseCtx->infoPanelVtx[13].v.ob[1] = 10 - 3;

        pauseCtx->infoPanelVtx[14].v.ob[1] = pauseCtx->infoPanelVtx[15].v.ob[1] =
            pauseCtx->infoPanelVtx[12].v.ob[1] - 26;
    }

    pauseCtx->infoPanelVtx[9].v.tc[0] = pauseCtx->infoPanelVtx[11].v.tc[0] = pauseCtx->infoPanelVtx[13].v.tc[0] =
        pauseCtx->infoPanelVtx[15].v.tc[0] = 24 * (1 << 5);

    pauseCtx->infoPanelVtx[10].v.tc[1] = pauseCtx->infoPanelVtx[11].v.tc[1] = pauseCtx->infoPanelVtx[14].v.tc[1] =
        pauseCtx->infoPanelVtx[15].v.tc[1] = 32 * (1 << 5);

    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);

    ModelViewTranslate(0.0f, 0.0f, -144.0f, MAT_SET);
    ModelViewScale(1.0f, 1.0f, 1.0f, MAT_MUL);

    gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 150, 140, 90, 255);
    gSPVertex(POLY_OPA_DISP++, &pauseCtx->infoPanelVtx[0], 16, 0);

    gSPDisplayList(POLY_OPA_DISP++, 0x080885C0); // gItemNamePanelDL

    if (pauseCtx->cursorSpecialPos != 0) {
        j = (pauseCtx->cursorSpecialPos * 4) - 32;
        pauseCtx->cursorVtx[0].v.ob[0] = pauseCtx->infoPanelVtx[j].v.ob[0];
        pauseCtx->cursorVtx[0].v.ob[1] = pauseCtx->infoPanelVtx[j].v.ob[1];
    }

    y = pauseCtx->infoPanelOffsetY - 80;
    pauseCtx->infoPanelVtx[16].v.ob[1] = pauseCtx->infoPanelVtx[17].v.ob[1] = y;

    pauseCtx->infoPanelVtx[18].v.ob[1] = pauseCtx->infoPanelVtx[19].v.ob[1] = pauseCtx->infoPanelVtx[16].v.ob[1] - 16;

    pauseCtx->infoPanelVtx[18].v.tc[1] = pauseCtx->infoPanelVtx[19].v.tc[1] = 16 * (1 << 5);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetCombineLERP(POLY_OPA_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, PRIMITIVE,
                      ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
    gDPSetEnvColor(POLY_OPA_DISP++, 20, 30, 40, 0);

    if ((pauseCtx->state == PAUSE_STATE_OWLWARP_SELECT) && (pauseCtx->namedItem != PAUSE_ITEM_NONE) &&
        (pauseCtx->nameDisplayTimer < 40)) {
        pauseCtx->infoPanelVtx[16].v.ob[0] = pauseCtx->infoPanelVtx[18].v.ob[0] = -63;

        pauseCtx->infoPanelVtx[17].v.ob[0] = pauseCtx->infoPanelVtx[19].v.ob[0] =
            pauseCtx->infoPanelVtx[16].v.ob[0] + 128;

        pauseCtx->infoPanelVtx[17].v.tc[0] = pauseCtx->infoPanelVtx[19].v.tc[0] = 128 * (1 << 5);

        gSPVertex(POLY_OPA_DISP++, &pauseCtx->infoPanelVtx[16], 4, 0);

        if (pauseCtx->nameColorSet == 1) { // PAUSE_NAME_COLOR_SET_GREY
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 70, 70, 70, 255);
        } else {
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
        }

        gDPLoadTextureBlock_4b(POLY_OPA_DISP++, pauseCtx->nameSegment, G_IM_FMT_IA, 128, 16, 0, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        gSP1Quadrangle(POLY_OPA_DISP++, 0, 2, 3, 1, 0);
    }

    CLOSE_DISPS();
}

static void KaleidoScope_DrawCursor(GameState_Play* play) {
    PauseContext* pauseCtx = &play->pauseCtx;
    s16 i;
    s16 stepR;
    s16 stepG;
    s16 stepB;
    static s16 sCursorColorTimer = 10;
    static s16 sCursorColorTargetIndex = 0;


    stepR =
        ABS_ALT(sCursorPrimR - sCursorPrimColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][0]) /
        sCursorColorTimer;
    stepG =
        ABS_ALT(sCursorPrimG - sCursorPrimColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][1]) /
        sCursorColorTimer;
    stepB =
        ABS_ALT(sCursorPrimB - sCursorPrimColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][2]) /
        sCursorColorTimer;

    if (sCursorPrimR >= sCursorPrimColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][0]) {
        sCursorPrimR -= stepR;
    } else {
        sCursorPrimR += stepR;
    }

    if (sCursorPrimG >= sCursorPrimColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][1]) {
        sCursorPrimG -= stepG;
    } else {
        sCursorPrimG += stepG;
    }

    if (sCursorPrimB >= sCursorPrimColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][2]) {
        sCursorPrimB -= stepB;
    } else {
        sCursorPrimB += stepB;
    }

    stepR =
        ABS_ALT(sCursorEnvR - sCursorEnvColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][0]) /
        sCursorColorTimer;
    stepG =
        ABS_ALT(sCursorEnvG - sCursorEnvColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][1]) /
        sCursorColorTimer;
    stepB =
        ABS_ALT(sCursorEnvB - sCursorEnvColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][2]) /
        sCursorColorTimer;

    if (sCursorEnvR >= sCursorEnvColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][0]) {
        sCursorEnvR -= stepR;
    } else {
        sCursorEnvR += stepR;
    }

    if (sCursorEnvG >= sCursorEnvColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][1]) {
        sCursorEnvG -= stepG;
    } else {
        sCursorEnvG += stepG;
    }

    if (sCursorEnvB >= sCursorEnvColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][2]) {
        sCursorEnvB -= stepB;
    } else {
        sCursorEnvB += stepB;
    }

    sCursorColorTimer--;
    if (sCursorColorTimer == 0) {
        sCursorPrimR = sCursorPrimColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][0];
        sCursorPrimG = sCursorPrimColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][1];
        sCursorPrimB = sCursorPrimColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][2];
        sCursorEnvR = sCursorEnvColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][0];
        sCursorEnvG = sCursorEnvColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][1];
        sCursorEnvB = sCursorEnvColorTarget[pauseCtx->cursorColorSet + sCursorColorTargetIndex][2];
        sCursorColorTargetIndex ^= 1;
        sCursorColorTimer = 10;
    }

    OPEN_DISPS(play->gs.gfx);

    if ((pauseCtx->changing == PAUSE_MAIN_STATE_IDLE) ||
        (pauseCtx->changing == PAUSE_MAIN_STATE_IDLE_CURSOR_ON_SONG)) {
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetCombineLERP(POLY_OPA_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                          PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, sCursorPrimR, sCursorPrimG, sCursorPrimB, 255);
        gDPSetEnvColor(POLY_OPA_DISP++, sCursorEnvR, sCursorEnvG, sCursorEnvB, 255);

        ModelViewTranslate(sCursorX, sCursorY, -50.0f, MAT_SET);
        ModelViewScale(1.0f, 1.0f, 1.0f, MAT_MUL);

        for (i = 0; i < 4; i++) {
            MatrixStackDup();
            ModelViewTranslate(sCursorCirclesX[i], sCursorCirclesY[i], -50.0f, MAT_MUL);
            gSPMatrix(POLY_OPA_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gDPPipeSync(POLY_OPA_DISP++);
            gDPLoadTextureBlock_4b(POLY_OPA_DISP++, sCursorTexs[i], G_IM_FMT_IA, 16, 16, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            gSPVertex(POLY_OPA_DISP++, &pauseCtx->cursorVtx[0], 4, 0);
            gSP1Quadrangle(POLY_OPA_DISP++, 0, 2, 3, 1, 0);
            MatrixStackPop();
        }

        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    }

    CLOSE_DISPS();
}

static void KaleidoScope_UpdateCursorSize(GameState_Play* play)
{
    PauseContext* pauseCtx = &play->pauseCtx;

    pauseCtx->cursorVtx[0].v.ob[0] = pauseCtx->cursorVtx[2].v.ob[0] = -8;
    pauseCtx->cursorVtx[1].v.ob[0] = pauseCtx->cursorVtx[3].v.ob[0] = pauseCtx->cursorVtx[0].v.ob[0] + 16;

    pauseCtx->cursorVtx[0].v.ob[1] = pauseCtx->cursorVtx[1].v.ob[1] = 8;
    pauseCtx->cursorVtx[2].v.ob[1] = pauseCtx->cursorVtx[3].v.ob[1] = pauseCtx->cursorVtx[0].v.ob[1] - 16;

    if (pauseCtx->cursorSpecialPos == 0) {
        sCursorX = sOwlWarpWorldMapCursorsX[pauseCtx->cursorPoint[PAUSE_WORLD_MAP]] - 4.0f;
        sCursorY = sOwlWarpWorldMapCursorsY[pauseCtx->cursorPoint[PAUSE_WORLD_MAP]] + 4.0f;
        sCursorHeight = 7.5f;
        sCursorWidth = 7.5f;

        // sCursorSpinPhase += 0x300;
    } else {
        if (pauseCtx->cursorSpecialPos == PAUSE_CURSOR_PAGE_LEFT) {
            sCursorX = -93.0f;
        } else { // PAUSE_CURSOR_PAGE_RIGHT
            sCursorX = 101.0f;
        }
        // sCursorSpinPhase += 0x300;
        sCursorHeight = 8.0f;
        sCursorWidth = 8.0f;
        sCursorY = -4.0f;
    }

    if (sCursorWidth < sCursorShrinkRate) {
        sCursorWidth = 0;
    } else {
        sCursorWidth -= sCursorShrinkRate;
    }

    if (sCursorHeight < sCursorShrinkRate) {
        sCursorHeight = 0.0f;
    } else {
        sCursorHeight -= sCursorShrinkRate;
    }

    if (sCursorShrinkRate != 0.0f) {
        sCursorShrinkRate -= 1.0f;
    }

    sCursorCirclesX[0] = Math_SinS(sCursorSpinPhase) * sCursorWidth;
    sCursorCirclesX[1] = Math_SinS(sCursorSpinPhase + 0x4000) * sCursorWidth;
    sCursorCirclesX[2] = Math_SinS(sCursorSpinPhase + 0x8000) * sCursorWidth;
    sCursorCirclesX[3] = Math_SinS(sCursorSpinPhase + 0xC000) * sCursorWidth;
    sCursorCirclesY[0] = Math_CosS(sCursorSpinPhase) * sCursorHeight;
    sCursorCirclesY[1] = Math_CosS(sCursorSpinPhase + 0x4000) * sCursorHeight;
    sCursorCirclesY[2] = Math_CosS(sCursorSpinPhase + 0x8000) * sCursorHeight;
    sCursorCirclesY[3] = Math_CosS(sCursorSpinPhase + 0xC000) * sCursorHeight;
}

static s16 KaleidoScope_SetPageVertices(GameState_Play* play, Vtx* vtx, s16 vtxPage, s16 numQuads) {
    PauseContext* pauseCtx = &play->pauseCtx;
    s16* quadsX;
    s16* quadsWidth;
    s16* quadsY;
    s16* quadsHeight;
    s16 pageBgQuadX;
    s16 pageBgQuadY;
    s16 i;
    s16 j;
    s16 k;

    // Vertices for KaleidoScope_DrawPageSections

    pageBgQuadX = 0 - (PAGE_BG_COLS * PAGE_BG_QUAD_WIDTH) / 2 - PAGE_BG_QUAD_WIDTH;

    // For each column
    for (k = 0, i = 0; i < 3; i++) {
        pageBgQuadX += PAGE_BG_QUAD_WIDTH;

        // For each row
        for (pageBgQuadY = (PAGE_BG_ROWS * PAGE_BG_QUAD_HEIGHT) / 2, j = 0; j < 5; j++, k += 4, pageBgQuadY -= 32) {
            vtx[k + 0].v.ob[0] = vtx[k + 2].v.ob[0] = pageBgQuadX;
            vtx[k + 1].v.ob[0] = vtx[k + 3].v.ob[0] = vtx[k + 0].v.ob[0] + PAGE_BG_QUAD_WIDTH;

            vtx[k + 0].v.ob[1] = vtx[k + 1].v.ob[1] = pageBgQuadY + pauseCtx->offsetY;
            vtx[k + 2].v.ob[1] = vtx[k + 3].v.ob[1] = vtx[k + 0].v.ob[1] - PAGE_BG_QUAD_HEIGHT;

            vtx[k + 0].v.ob[2] = vtx[k + 1].v.ob[2] = vtx[k + 2].v.ob[2] = vtx[k + 3].v.ob[2] = 0;

            vtx[k + 0].v.flag = vtx[k + 1].v.flag = vtx[k + 2].v.flag = vtx[k + 3].v.flag = 0;

            vtx[k + 0].v.tc[0] = vtx[k + 0].v.tc[1] = 0;
            vtx[k + 1].v.tc[1] = vtx[k + 2].v.tc[0] = 0;
            vtx[k + 1].v.tc[0] = vtx[k + 3].v.tc[0] = PAGE_BG_QUAD_TEX_WIDTH * (1 << 5);
            vtx[k + 2].v.tc[1] = vtx[k + 3].v.tc[1] = PAGE_BG_QUAD_TEX_HEIGHT * (1 << 5);

            vtx[k + 0].v.cn[0] = vtx[k + 2].v.cn[0] = vtx[k + 0].v.cn[1] = vtx[k + 2].v.cn[1] = vtx[k + 0].v.cn[2] =
                vtx[k + 2].v.cn[2] = 0;

            vtx[k + 1].v.cn[0] = vtx[k + 3].v.cn[0] = vtx[k + 1].v.cn[1] = vtx[k + 3].v.cn[1] = vtx[k + 1].v.cn[2] =
                vtx[k + 3].v.cn[2] = 0;

            vtx[k + 0].v.cn[3] = vtx[k + 2].v.cn[3] = vtx[k + 1].v.cn[3] = vtx[k + 3].v.cn[3] = pauseCtx->alpha;
        }
    }

    if (numQuads != 0) {
        quadsX = sVtxPageQuadsX[vtxPage];
        quadsWidth = sVtxPageQuadsWidth[vtxPage];
        quadsY = sVtxPageQuadsY[vtxPage];
        quadsHeight = sVtxPageQuadsHeight[vtxPage];

        for (i = 0; i < numQuads; i++, k += 4) {
            vtx[k + 2].v.ob[0] = vtx[k + 0].v.ob[0] = quadsX[i];

            vtx[k + 1].v.ob[0] = vtx[k + 3].v.ob[0] = vtx[k + 0].v.ob[0] + quadsWidth[i];

            vtx[k + 0].v.ob[1] = vtx[k + 1].v.ob[1] = quadsY[i] + pauseCtx->offsetY;

            vtx[k + 2].v.ob[1] = vtx[k + 3].v.ob[1] = vtx[k + 0].v.ob[1] - quadsHeight[i];

            vtx[k + 0].v.ob[2] = vtx[k + 1].v.ob[2] = vtx[k + 2].v.ob[2] = vtx[k + 3].v.ob[2] = 0;

            vtx[k + 0].v.flag = vtx[k + 1].v.flag = vtx[k + 2].v.flag = vtx[k + 3].v.flag = 0;

            vtx[k + 0].v.tc[0] = vtx[k + 0].v.tc[1] = vtx[k + 1].v.tc[1] = vtx[k + 2].v.tc[0] = 0;
            vtx[k + 1].v.tc[0] = vtx[k + 3].v.tc[0] = quadsWidth[i] << 5;
            vtx[k + 2].v.tc[1] = vtx[k + 3].v.tc[1] = quadsHeight[i] << 5;

            vtx[k + 0].v.cn[0] = vtx[k + 2].v.cn[0] = vtx[k + 0].v.cn[1] = vtx[k + 2].v.cn[1] = vtx[k + 0].v.cn[2] =
                vtx[k + 2].v.cn[2] = 255;

            vtx[k + 1].v.cn[0] = vtx[k + 3].v.cn[0] = vtx[k + 1].v.cn[1] = vtx[k + 3].v.cn[1] = vtx[k + 1].v.cn[2] =
                vtx[k + 3].v.cn[2] = 255;

            vtx[k + 0].v.cn[3] = vtx[k + 2].v.cn[3] = vtx[k + 1].v.cn[3] = vtx[k + 3].v.cn[3] = pauseCtx->alpha;
        }
    }
    return k;
}

static void KaleidoScope_SetVertices(GameState_Play* play, GfxContext* gfxCtx)
{
    PauseContext* pauseCtx = &play->pauseCtx;
    s16 i;
    s16 j;
    s16 vtx_y;

    pauseCtx->offsetY = 0;

    if ((pauseCtx->state == PAUSE_STATE_OWLWARP_3) || (pauseCtx->state >= PAUSE_STATE_OWLWARP_6)) {
        pauseCtx->offsetY = 80;
    }

    pauseCtx->mapPageVtx = GRAPH_ALLOC(
        gfxCtx, ((PAGE_BG_QUADS + VTX_PAGE_MAP_WORLD_QUADS + WORLD_MAP_IMAGE_FRAG_NUM) * 4) * sizeof(Vtx));
    j = KaleidoScope_SetPageVertices(play, pauseCtx->mapPageVtx, VTX_PAGE_MAP_WORLD, VTX_PAGE_MAP_WORLD_QUADS);

    for (i = 0, vtx_y = 59; i < WORLD_MAP_IMAGE_FRAG_NUM; i++, j += 4, vtx_y -= WORLD_MAP_IMAGE_FRAG_HEIGHT) {
        pauseCtx->mapPageVtx[j + 2].v.ob[0] = 0 - (WORLD_MAP_IMAGE_WIDTH / 2) - 1;
        pauseCtx->mapPageVtx[j + 0].v.ob[0] = pauseCtx->mapPageVtx[j + 2].v.ob[0];

        pauseCtx->mapPageVtx[j + 1].v.ob[0] = pauseCtx->mapPageVtx[j + 3].v.ob[0] =
            pauseCtx->mapPageVtx[j + 0].v.ob[0] + WORLD_MAP_IMAGE_WIDTH;

        pauseCtx->mapPageVtx[j + 0].v.ob[1] = pauseCtx->mapPageVtx[j + 1].v.ob[1] = vtx_y + pauseCtx->offsetY;

        pauseCtx->mapPageVtx[j + 2].v.ob[1] = pauseCtx->mapPageVtx[j + 3].v.ob[1] =
            pauseCtx->mapPageVtx[j + 0].v.ob[1] - WORLD_MAP_IMAGE_FRAG_HEIGHT;

        pauseCtx->mapPageVtx[j + 0].v.ob[2] = pauseCtx->mapPageVtx[j + 1].v.ob[2] =
            pauseCtx->mapPageVtx[j + 2].v.ob[2] = pauseCtx->mapPageVtx[j + 3].v.ob[2] = 0;

        pauseCtx->mapPageVtx[j + 0].v.flag = pauseCtx->mapPageVtx[j + 1].v.flag =
            pauseCtx->mapPageVtx[j + 2].v.flag = pauseCtx->mapPageVtx[j + 3].v.flag = 0;

        pauseCtx->mapPageVtx[j + 0].v.tc[0] = pauseCtx->mapPageVtx[j + 0].v.tc[1] =
            pauseCtx->mapPageVtx[j + 1].v.tc[1] = pauseCtx->mapPageVtx[j + 2].v.tc[0] = 0;

        pauseCtx->mapPageVtx[j + 1].v.tc[0] = pauseCtx->mapPageVtx[j + 3].v.tc[0] =
            WORLD_MAP_IMAGE_WIDTH * (1 << 5);
        pauseCtx->mapPageVtx[j + 2].v.tc[1] = pauseCtx->mapPageVtx[j + 3].v.tc[1] =
            WORLD_MAP_IMAGE_FRAG_HEIGHT * (1 << 5);

        pauseCtx->mapPageVtx[j + 0].v.cn[0] = pauseCtx->mapPageVtx[j + 2].v.cn[0] =
            pauseCtx->mapPageVtx[j + 0].v.cn[1] = pauseCtx->mapPageVtx[j + 2].v.cn[1] =
                pauseCtx->mapPageVtx[j + 0].v.cn[2] = pauseCtx->mapPageVtx[j + 2].v.cn[2] =
                    pauseCtx->mapPageVtx[j + 1].v.cn[0] = pauseCtx->mapPageVtx[j + 3].v.cn[0] =
                        pauseCtx->mapPageVtx[j + 1].v.cn[1] = pauseCtx->mapPageVtx[j + 3].v.cn[1] =
                            pauseCtx->mapPageVtx[j + 1].v.cn[2] = pauseCtx->mapPageVtx[j + 3].v.cn[2] =
                                pauseCtx->mapPageVtx[j + 0].v.cn[3] = pauseCtx->mapPageVtx[j + 2].v.cn[3] =
                                    pauseCtx->mapPageVtx[j + 1].v.cn[3] = pauseCtx->mapPageVtx[j + 3].v.cn[3] =
                                        pauseCtx->alpha;
    }

    pauseCtx->mapPageVtx[j - 2].v.ob[1] = pauseCtx->mapPageVtx[j - 1].v.ob[1] =
        pauseCtx->mapPageVtx[j - 4].v.ob[1] - (WORLD_MAP_IMAGE_HEIGHT % WORLD_MAP_IMAGE_FRAG_HEIGHT);

    pauseCtx->mapPageVtx[j - 2].v.tc[1] = pauseCtx->mapPageVtx[j - 1].v.tc[1] =
        (WORLD_MAP_IMAGE_HEIGHT % WORLD_MAP_IMAGE_FRAG_HEIGHT) * (1 << 5);

    pauseCtx->cursorVtx = GRAPH_ALLOC(play->gs.gfx, (PAUSE_QUAD_CURSOR_MAX * 4) * sizeof(Vtx));

    for (i = 0; i < (PAUSE_QUAD_CURSOR_MAX * 4); i++) {
        pauseCtx->cursorVtx[i].v.ob[0] = pauseCtx->cursorVtx[i].v.ob[1] = pauseCtx->cursorVtx[i].v.ob[2] = 0;

        pauseCtx->cursorVtx[i].v.flag = 0;

        pauseCtx->cursorVtx[i].v.tc[0] = pauseCtx->cursorVtx[i].v.tc[1] = 0;

        pauseCtx->cursorVtx[i].v.cn[0] = pauseCtx->cursorVtx[i].v.cn[1] = pauseCtx->cursorVtx[i].v.cn[2] =
            pauseCtx->cursorVtx[i].v.cn[3] = 255;
    }

    // PAUSE_QUAD_CURSOR_0
    pauseCtx->cursorVtx[1].v.tc[0] = pauseCtx->cursorVtx[2].v.tc[1] = pauseCtx->cursorVtx[3].v.tc[0] =
        pauseCtx->cursorVtx[3].v.tc[1]
        // PAUSE_QUAD_CURSOR_1
        = pauseCtx->cursorVtx[5].v.tc[0] = pauseCtx->cursorVtx[6].v.tc[1] = pauseCtx->cursorVtx[7].v.tc[0] =
            pauseCtx->cursorVtx[7].v.tc[1]
        // PAUSE_QUAD_CURSOR_2
        = pauseCtx->cursorVtx[9].v.tc[0] = pauseCtx->cursorVtx[10].v.tc[1] = pauseCtx->cursorVtx[11].v.tc[0] =
            pauseCtx->cursorVtx[11].v.tc[1]
        // PAUSE_QUAD_CURSOR_3
        = pauseCtx->cursorVtx[13].v.tc[0] = pauseCtx->cursorVtx[14].v.tc[1] = pauseCtx->cursorVtx[15].v.tc[0] =
            pauseCtx->cursorVtx[15].v.tc[1] = 16 * (1 << 5);

    // PAUSE_QUAD_CURSOR_4
    pauseCtx->cursorVtx[17].v.tc[0] = pauseCtx->cursorVtx[18].v.tc[1] = pauseCtx->cursorVtx[19].v.tc[0] =
        pauseCtx->cursorVtx[19].v.tc[1] = 32 * (1 << 5);

    pauseCtx->infoPanelVtx = GRAPH_ALLOC(gfxCtx, 28 * sizeof(Vtx));
}

typedef void (*KaleidoScope_SetView_Func)(PauseContext*, f32, f32, f32);

// Returns true if called shouldn't draw menu
s32 KaleidoScope_BeforeDraw(GameState_Play* play)
{
    PauseContext* pauseCtx = &play->pauseCtx;
    if (pauseCtx->debugState)
    {
        return pauseCtx->debugState;
    }

    if (pauseCtx->state >= PAUSE_STATE_OWLWARP_2 && pauseCtx->state <= PAUSE_STATE_OWLWARP_6)
    {
        OPEN_DISPS(play->gs.gfx);

        KaleidoScope_SetView_Func KaleidoScope_SetView = OverlayAddr(0x80823120);
        KaleidoScope_SetView(pauseCtx, pauseCtx->eye.x, pauseCtx->eye.y, pauseCtx->eye.z);

        KaleidoScope_SetVertices(play, play->gs.gfx);

        Gfx_SetupDL_42Opa(play->gs.gfx);

        KaleidoScope_DrawOwlWarpMapPage(play);

        Gfx_SetupDL_42Opa(play->gs.gfx);
        gDPSetCombineLERP(POLY_OPA_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                            PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);

        KaleidoScope_SetView(pauseCtx, 0.0f, 0.0f, 64.0f);
        KaleidoScope_DrawOwlWarpInfoPanel(play);
        KaleidoScope_UpdateCursorSize(play);

        if (pauseCtx->state == PAUSE_STATE_OWLWARP_SELECT)
        {
            KaleidoScope_DrawCursor(play);
        }

        CLOSE_DISPS();

        return 1;
    }

    return 0;
}

u32 GetItemTexture(u32 slotId, u8 item, u32 index)
{
    static void* sExtraIconTradeChild[2];
    static u8 sExtraIconTradeChildItem[2];
    u32* itemToIcon = (u32*)0x800f8d2c;
    switch (slotId)
    {
    case ITS_OOT_TRADE_CHILD:
        if (!sExtraIconTradeChild[index])
        {
            sExtraIconTradeChild[index] = malloc(0x1000);
            sExtraIconTradeChildItem[index] = ITEM_NONE;
        }
        if (sExtraIconTradeChild[index])
        {
            if (sExtraIconTradeChildItem[index] != item)
            {
                sExtraIconTradeChildItem[index] = item;
                comboItemIcon(sExtraIconTradeChild[index], sExtraIconTradeChildItem[index]);
                if (!comboConfig(CFG_OOT_AGELESS_CHILD_TRADE) && gSave.age != AGE_CHILD)
                    Grayscale(sExtraIconTradeChild[index], 0x400);
            }
            return (u32)sExtraIconTradeChild[index] & 0x00ffffff;
        }
        break;
    default:
        return itemToIcon[item];
    }
    return 0;
}

static u8 GetNextItem(u32 slot, s32* outTableIndex)
{
    u32 flags;
    const u8* table;
    u32 tableSize;
    u8* itemPtr;
    *outTableIndex = comboGetSlotExtras(slot, &itemPtr, &flags, &table, &tableSize);
    if (*outTableIndex >= 0)
    {
        return comboGetNextTrade(*itemPtr, flags, table, tableSize);
    }
    return ITEM_NONE;
}

// Vertex buffers.
static Vtx gVertexBufs[(4 * 4) * 2];

// Vertex buffer pointers.
static Vtx* gVertex[4] = {
    &gVertexBufs[(4 * 0) * 2],
    &gVertexBufs[(4 * 1) * 2],
    &gVertexBufs[(4 * 2) * 2],
    &gVertexBufs[(4 * 3) * 2],
};

static Vtx* GetVtxBuffer(GameState_Play* play, u32 vertIdx, u32 slot) {
    // Get vertex of current icon drawing to Item Select screen
    const Vtx* srcVtx = play->pauseCtx.itemVtx + vertIdx;

    // Get dest Vtx (factor in frame counter)
    int framebufIdx = play->gs.gfx->displayListCounter & 1;
    Vtx* dstVtx = gVertex[slot] + (framebufIdx * 4);

    // Copy source Vtx over to dest Vtx
    for (int i = 0; i < 4; i++) {
        dstVtx[i] = srcVtx[i];
    }

    // Adjust X position
    dstVtx[0].v.ob[0] += 0x10;
    dstVtx[2].v.ob[0] += 0x10;

    // Adjust Y position
    dstVtx[0].v.ob[1] -= 0x10;
    dstVtx[1].v.ob[1] -= 0x10;

    return dstVtx;
}

static void DrawIcon(GfxContext* gfxCtx, const Vtx* vtx, u32 segAddr, u16 width, u16 height, u16 qidx) {
    OPEN_DISPS(gfxCtx);
    // Instructions that happen before function
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0xFF, 0xFF, 0xFF, gfxCtx->play->pauseCtx.alpha & 0xFF);
    gSPVertex(POLY_OPA_DISP++, vtx, 4, 0); // Loads 4 vertices from RDRAM
    // Instructions that happen during function.
    gDPLoadTextureBlock(POLY_OPA_DISP++, segAddr, G_IM_FMT_RGBA, G_IM_SIZ_32b, width, height, 0,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSP1Quadrangle(POLY_OPA_DISP++, qidx + 0, qidx + 2, qidx + 3, qidx + 1, 0);
    CLOSE_DISPS();
}

void KaleidoScope_DrawItemHook(GfxContext* gfx, int slotId, int sizeX, int sizeY, int vertIdx)
{
    void (*KaleidoScope_DrawItem)(GfxContext*, u32, int, int, int);
    u32 icon;
    u8 item;

    item = gSave.inventory.items[slotId];

    KaleidoScope_DrawItem = OverlayAddr(0x8081f1e8);

    icon = GetItemTexture(slotId, item, 0);

    if (icon)
        KaleidoScope_DrawItem(gfx, icon, sizeX, sizeY, 0);

    s32 tableIndex;
    u8 next = GetNextItem(slotId, &tableIndex);
    if (next != ITEM_NONE && next != item)
    {
        icon = GetItemTexture(slotId, next, 1);
        Vtx* vtx = GetVtxBuffer(gfx->play, vertIdx, tableIndex);
        DrawIcon(gfx, vtx, icon, sizeX, sizeY, 0);
    }
}

#define ITEM_OFFSET 123

void KaleidoScope_LoadItemName(void* dst, s16 id)
{
    s16 itemId;

    itemId = id - 123;
    if (itemId == ITEM_NONE)
    {
        memset(dst, 0, 0x400);
    }
    else if (itemId == ITEM_OOT_MASK_BLAST)
    {
        comboLoadMmIcon(dst, 0xa27660, ITEM_MM_MASK_BLAST);
    }
    else if (itemId == ITEM_OOT_MASK_STONE)
    {
        comboLoadMmIcon(dst, 0xa27660, ITEM_MM_MASK_STONE);
    }
    else if (itemId == ITEM_OOT_KEATON_MASK)
    {
        /* Fixes 1.0 spelling mistake */
        comboLoadMmIcon(dst, 0xa27660, ITEM_MM_MASK_KEATON);
    }
    else
    {
        LoadFile(dst, 0x880000 + 0x400 * id, 0x400);
    }
}

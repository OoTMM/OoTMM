#include "file_select.h"
#include <combo/config.h>

#define CUSTOM_FILE_INFO_BUFFER_SIZE 0x40000

#define ICON_NONE   0xffff
#define ICONF_MM    0x8000
#define ICONF_DIM   0x4000

void FileSelect_CustomFileInfoInit(FileSelectState* this)
{
    this->customFileInfoBufs[0] = malloc(CUSTOM_FILE_INFO_BUFFER_SIZE);
    this->customFileInfoBufs[1] = malloc(CUSTOM_FILE_INFO_BUFFER_SIZE);
    this->game = 0;
}

void FileSelect_CustomFileInfoFree(FileSelectState* this)
{
    free(this->customFileInfoBufs[0]);
    free(this->customFileInfoBufs[1]);
}

static void FileSelect_CustomFileInfoSwap(FileSelectState* this)
{
    void* tmp;

    tmp = this->customFileInfoBufs[0];
    this->customFileInfoBufs[0] = this->customFileInfoBufs[1];
    this->customFileInfoBufs[1] = tmp;
}

static char* allocBuf(void** end, u32 size)
{
    size = (size + 0xf) & ~0xf;
    *end = (char*)*end - size;
    return (char*)*end;
}

static void* allocItemIconOot(void** end, int itemId)
{
    void* buf = allocBuf(end, 0x1000);
    comboItemIcon(buf, itemId);
    return buf;
}

static void* allocItemIconMm(void** end, int itemId)
{
    void* buf = allocBuf(end, 0x1000);
    LoadMmItemIcon(buf, itemId);
    return buf;
}

static void* allocIconItem24Oot(void** end, int id)
{
    void* buf = allocBuf(end, 0x900);
    LoadIconItem24Oot(buf, id);
    return buf;
}

static void* allocIconItem24Mm(void** end, int id)
{
    void* buf = allocBuf(end, 0x900);
    LoadIconItem24Mm(buf, id);
    return buf;
}

static void quadRGBA8(Gfx** gfx, void* tex, s16 w, s16 h, s16 x, s16 y, float scale)
{
    float revScale;

    revScale = 1.0f / scale;
    gDPLoadTextureBlock((*gfx)++, tex, G_IM_FMT_RGBA, G_IM_SIZ_32b, w, h, 0, G_TX_NOMIRROR | G_TX_CLAMP,
        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gSPTextureRectangle((*gfx)++,
        x << 2, y << 2,
        (int)(x * 4.f + w * scale * 4.f), (int)(y * 4.f + h * scale * 4.f),
        G_TX_RENDERTILE, 0, 0,
        (int)(revScale * (1 << 10)), (int)(revScale * (1 << 10)));
}

static void drawItemIcon(Gfx** list, void** end, s16 x, s16 y, s16 dx, s16 dy, u16 id)
{
    float scale;
    u16 itemId;
    void* tex;

    if (id == ICON_NONE)
        return;

    scale = 0.3333f;
    itemId = id & 0x3fff;
    if (!(id & ICONF_MM))
        tex = allocItemIconOot(end, itemId);
    else
        tex = allocItemIconMm(end, itemId);

    /* Dim */
    if (id & ICONF_DIM)
    {
        gDPSetPrimColor((*list)++, 0, 0, 30, 30, 30, 128);
    }
    else
    {
        gDPSetPrimColor((*list)++, 0, 0, 255, 255, 255, 255);
    }

    /* Draw */
    quadRGBA8(list, tex, 32, 32, x + 32 * dx * scale, y + 32 * dy * scale, scale);
}

static void drawItemIconSimple(Gfx** list, void** end, s16 x, s16 y, s16 dx, s16 dy, u16 id, int noDim)
{
    if (!noDim)
        id |= ICONF_DIM;
    drawItemIcon(list, end, x, y, dx, dy, id);
}

static void drawItemIcon24(Gfx** list, void** end, float scale, s16 x, s16 y, s16 dx, s16 dy, u16 id, int noDim)
{
    u16 itemId;
    void* tex;

    scale *= 0.66667f;
    if (id == ICON_NONE)
        return;

    itemId = id & 0x3fff;
    if (!(id & ICONF_MM))
        tex = allocIconItem24Oot(end, itemId);
    else
        tex = allocIconItem24Mm(end, itemId);

    /* Dim */
    if (!noDim)
    {
        gDPSetPrimColor((*list)++, 0, 0, 30, 30, 30, 128);
    }
    else
    {
        gDPSetPrimColor((*list)++, 0, 0, 255, 255, 255, 255);
    }

    /* Draw */
    quadRGBA8(list, tex, 24, 24, x + 24 * dx * scale, y + 24 * dy * scale, scale);
}

static void FileSelect_CustomFileInfoPrepareOotMedsStones(FileSelectState* this, Gfx** list, void** end, int x, int y)
{
    static const float scale = 0.75f;
    static const float sqrt2_2 = 0.70710677f;

    /* Medallions */
    drawItemIcon24(list, end, scale, x, y, 1, 0, 5, gOotSave.inventory.quest.medallionLight);
    drawItemIcon24(list, end, scale, x, y + sqrt2_2 * scale * 16, 0, 0, 4, gOotSave.inventory.quest.medallionShadow);
    drawItemIcon24(list, end, scale, x, y + sqrt2_2 * scale * 16, 2, 0, 0, gOotSave.inventory.quest.medallionForest);
    drawItemIcon24(list, end, scale, x, y + sqrt2_2 * scale * 16, 0, 1, 3, gOotSave.inventory.quest.medallionSpirit);
    drawItemIcon24(list, end, scale, x, y + sqrt2_2 * scale * 16, 2, 1, 1, gOotSave.inventory.quest.medallionFire);
    drawItemIcon24(list, end, scale, x, y + sqrt2_2 * scale * 16 * 2, 1, 1, 2, gOotSave.inventory.quest.medallionWater);

    /* Stones */
    drawItemIcon24(list, end, scale, x, y, 0, 4, 6, gOotSave.inventory.quest.stoneEmerald);
    drawItemIcon24(list, end, scale, x, y, 1, 4, 7, gOotSave.inventory.quest.stoneRuby);
    drawItemIcon24(list, end, scale, x, y, 2, 4, 8, gOotSave.inventory.quest.stoneSapphire);
}

static void FileSelect_CustomFileInfoPrepareOotInventory(FileSelectState* this, Gfx** list, void** end, int x, int y)
{
    int dx;
    int dy;
    u16 iconOcarina;
    u16 iconHookshot;
    u8 itemId;
    u8 hasBottle;

    /* Pre-compute complex icons */
    if (gOotExtraItems.ocarina & 2)
        iconOcarina = ITEM_OOT_OCARINA_TIME;
    else
        iconOcarina = ITEM_OOT_OCARINA_FAIRY;

    if (gOotExtraItems.hookshot & 2)
        iconHookshot = ITEM_OOT_LONGSHOT;
    else
        iconHookshot = ITEM_OOT_HOOKSHOT;

    hasBottle = 0;
    for (int i = 0; i < 4; ++i)
    {
        itemId = gOotSave.inventory.items[ITS_OOT_BOTTLE + i];
        if (itemId >= ITEM_OOT_BOTTLE_EMPTY && itemId <= ITEM_OOT_POE && itemId != ITEM_OOT_RUTO_LETTER)
        {
            hasBottle = 1;
            break;
        }
    }

    /* Row 1 */
    dx = 0;
    dy = 0;
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_STICK,           gOotSave.inventory.items[ITS_OOT_STICKS] == ITEM_OOT_STICK);
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_NUT,             gOotSave.inventory.items[ITS_OOT_NUTS] == ITEM_OOT_NUT);
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_BOMB,            gOotSave.inventory.items[ITS_OOT_BOMBS] == ITEM_OOT_BOMB);
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_BOW,             gOotSave.inventory.items[ITS_OOT_BOW] == ITEM_OOT_BOW);
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_ARROW_FIRE,      gOotSave.inventory.items[ITS_OOT_ARROW_FIRE] == ITEM_OOT_ARROW_FIRE);
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_SPELL_FIRE,      gOotSave.inventory.items[ITS_OOT_SPELL_FIRE] == ITEM_OOT_SPELL_FIRE);
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_BOTTLE_EMPTY,    hasBottle);

    /* Row 2 */
    dx = 0;
    dy = 1;
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_SLINGSHOT,   gOotSave.inventory.items[ITS_OOT_SLINGSHOT] == ITEM_OOT_SLINGSHOT);
    drawItemIconSimple(list, end, x, y, dx++, dy, iconOcarina,          gOotExtraItems.ocarina);
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_BOMBCHU_10,  gOotSave.inventory.items[ITS_OOT_BOMBCHU] == ITEM_OOT_BOMBCHU_10);
    drawItemIconSimple(list, end, x, y, dx++, dy, iconHookshot,         gOotExtraItems.hookshot);
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_ARROW_ICE,   gOotSave.inventory.items[ITS_OOT_ARROW_ICE] == ITEM_OOT_ARROW_ICE);
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_SPELL_WIND,  gOotSave.inventory.items[ITS_OOT_SPELL_WIND] == ITEM_OOT_SPELL_WIND);
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_RUTO_LETTER, gOotExtraItems.rutoLetter);

    /* Row 3 */
    dx = 0;
    dy = 2;
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_BOOMERANG,   gOotSave.inventory.items[ITS_OOT_BOOMERANG] == ITEM_OOT_BOOMERANG);
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_LENS,        gOotSave.inventory.items[ITS_OOT_LENS] == ITEM_OOT_LENS);
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_MAGIC_BEAN,  gOotSave.inventory.items[ITS_OOT_MAGIC_BEAN] == ITEM_OOT_MAGIC_BEAN);
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_HAMMER,      gOotSave.inventory.items[ITS_OOT_HAMMER] == ITEM_OOT_HAMMER);
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_ARROW_LIGHT, gOotSave.inventory.items[ITS_OOT_ARROW_LIGHT] == ITEM_OOT_ARROW_LIGHT);
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_SPELL_LOVE,  gOotSave.inventory.items[ITS_OOT_SPELL_LOVE] == ITEM_OOT_SPELL_LOVE);

    /* Row 4 - Child Trade */
    dx = 0;
    dy = 4;
    if (Config_Flag(CFG_OOT_SHUFFLE_EGGS))
        drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_WEIRD_EGG,                gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_WEIRD_EGG));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_CHICKEN,                  gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_CHICKEN));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_ZELDA_LETTER,             gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_ZELDA_LETTER));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_KEATON_MASK,              gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_KEATON_MASK));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_SKULL_MASK,               gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_SKULL_MASK));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_SPOOKY_MASK,              gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_SPOOKY_MASK));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_BUNNY_HOOD,               gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_BUNNY_HOOD));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_GORON_MASK,               gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_GORON_MASK));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_ZORA_MASK,                gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_ZORA_MASK));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_GERUDO_MASK,              gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_GERUDO_MASK));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_MASK_OF_TRUTH,            gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_MASK_OF_TRUTH));
    if (Config_Flag(CFG_OOT_MASK_BLAST))
        drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_MM_MASK_BLAST | ICONF_MM,     gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_MASK_BLAST));
    if (Config_Flag(CFG_OOT_MASK_STONE))
        drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_MM_MASK_STONE | ICONF_MM,     gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_MASK_STONE));

    /* Row 5 - Adult Trade */
    dx = 0;
    dy = 5;
    if (Config_Flag(CFG_OOT_SHUFFLE_EGGS))
        drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_POCKET_EGG,                gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_POCKET_EGG));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_POCKET_CUCCO,              gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_POCKET_CUCCO));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_COJIRO,                    gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_COJIRO));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_ODD_MUSHROOM,              gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_ODD_MUSHROOM));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_ODD_POTION,                gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_ODD_POTION));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_POACHER_SAW,               gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_POACHER_SAW));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_GORON_SWORD_BROKEN,        gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_BROKEN_GORON_SWORD));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_PRESCRIPTION,              gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_PRESCRIPTION));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_EYEBALL_FROG,              gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_EYEBALL_FROG));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_EYE_DROPS,                 gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_EYE_DROPS));
    drawItemIconSimple(list, end, x, y, dx++, dy, ITEM_OOT_CLAIM_CHECK,               gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_CLAIM_CHECK));
}

static void FileSelect_CustomFileInfoPrepareOot(FileSelectState* this, Gfx** list, void** end)
{
    FileSelect_CustomFileInfoPrepareOotMedsStones(this, list, end, 80, 94);
    FileSelect_CustomFileInfoPrepareOotInventory(this, list, end, 130, 94);
}

static void FileSelect_CustomFileInfoPrepareMmInventory(FileSelectState* this, Gfx** list, void** end, int x, int y)
{
    u16 iconOcarina;
    u16 iconHookshot;

    /* Pre-compute complex icons */
    iconOcarina = ITEM_OOT_OCARINA_TIME;
    if (Config_Flag(CFG_MM_OCARINA_FAIRY) && !(gMmExtraItems.ocarina & 2))
        iconOcarina = ITEM_OOT_OCARINA_FAIRY;

    iconHookshot = ITEM_MM_HOOKSHOT | ICONF_MM;
    if (Config_Flag(CFG_MM_HOOKSHOT_SHORT) && !(gMmExtraItems.hookshot & 2))
        iconHookshot = ITEM_OOT_HOOKSHOT;

    /* Row 1 */
    drawItemIconSimple(list, end, x, y, 0, 0, iconOcarina,                  gMmExtraItems.ocarina);
    drawItemIconSimple(list, end, x, y, 1, 0, ITEM_MM_BOW | ICONF_MM,       gMmSave.inventory.items[ITS_MM_BOW] == ITEM_MM_BOW);
    drawItemIconSimple(list, end, x, y, 2, 0, ITEM_OOT_ARROW_FIRE,          gMmSave.inventory.items[ITS_MM_ARROW_FIRE] == ITEM_MM_ARROW_FIRE);
    drawItemIconSimple(list, end, x, y, 3, 0, ITEM_OOT_ARROW_ICE,           gMmSave.inventory.items[ITS_MM_ARROW_ICE] == ITEM_MM_ARROW_ICE);
    drawItemIconSimple(list, end, x, y, 4, 0, ITEM_OOT_ARROW_LIGHT,         gMmSave.inventory.items[ITS_MM_ARROW_LIGHT] == ITEM_MM_ARROW_LIGHT);

    /* Row 2 */
    drawItemIconSimple(list, end, x, y, 0, 1, ITEM_OOT_BOMB,                gMmSave.inventory.items[ITS_MM_BOMBS] == ITEM_MM_BOMB);
    drawItemIconSimple(list, end, x, y, 1, 1, ITEM_OOT_BOMBCHU_10,          gMmSave.inventory.items[ITS_MM_BOMBCHU] == ITEM_MM_BOMBCHU);
    drawItemIconSimple(list, end, x, y, 2, 1, ITEM_OOT_STICK,               gMmSave.inventory.items[ITS_MM_STICKS] == ITEM_MM_STICK);
    drawItemIconSimple(list, end, x, y, 3, 1, ITEM_OOT_NUT,                 gMmSave.inventory.items[ITS_MM_NUTS] == ITEM_MM_NUT);
    drawItemIconSimple(list, end, x, y, 4, 1, ITEM_OOT_MAGIC_BEAN,          gMmSave.inventory.items[ITS_MM_BEANS] == ITEM_MM_MAGIC_BEAN);

    /* Row 3 */
    drawItemIconSimple(list, end, x, y, 0, 2, ITEM_MM_POWDER_KEG | ICONF_MM,            gMmSave.inventory.items[ITS_MM_KEG] == ITEM_MM_POWDER_KEG);
    drawItemIconSimple(list, end, x, y, 1, 2, ITEM_MM_PICTOGRAPH_BOX | ICONF_MM,        gMmSave.inventory.items[ITS_MM_PICTOBOX] == ITEM_MM_PICTOGRAPH_BOX);
    drawItemIconSimple(list, end, x, y, 2, 2, ITEM_OOT_LENS,                            gMmSave.inventory.items[ITS_MM_LENS] == ITEM_MM_LENS_OF_TRUTH);
    drawItemIconSimple(list, end, x, y, 3, 2, iconHookshot,                             gMmExtraItems.hookshot);
    drawItemIconSimple(list, end, x, y, 4, 2, ITEM_MM_GREAT_FAIRY_SWORD | ICONF_MM,     gMmExtraItems.hammerGFS & 1);
}

static void FileSelect_CustomFileInfoPrepareMm(FileSelectState* this, Gfx** list, void** end)
{
    FileSelect_CustomFileInfoPrepareMmInventory(this, list, end, 130, 94);
}

void FileSelect_CustomFileInfoPrepare(FileSelectState* this, int slot)
{
    Gfx* list;
    void* end;

    /* Load the saves */
    gSaveContext.fileNum = slot;
    Save_ReadOwn();
    Save_ReadForeign();

    FileSelect_CustomFileInfoSwap(this);
    list = this->customFileInfoBufs[0];
    end = (char*)list + CUSTOM_FILE_INFO_BUFFER_SIZE;

    /* Init */
    gDPSetCombineMode(list++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(list++, 0, 0, 255, 255, 255, 255);
    gDPSetTexturePersp(list++, G_TP_NONE);

    if (!this->game)
        FileSelect_CustomFileInfoPrepareOot(this, &list, &end);
    else
        FileSelect_CustomFileInfoPrepareMm(this, &list, &end);

    gSPEndDisplayList(list++);
}

void FileSelect_CustomFileInfoDraw(FileSelectState* this)
{
    OPEN_DISPS(this->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, this->customFileInfoBufs[0]);
    CLOSE_DISPS();
}

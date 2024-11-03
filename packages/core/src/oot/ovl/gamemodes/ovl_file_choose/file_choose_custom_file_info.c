#include "file_select.h"

#define CUSTOM_FILE_INFO_BUFFER_SIZE 0x40000

#define ICONF_MM    0x8000
#define ICONF_DIM   0x4000

void FileSelect_CustomFileInfoInit(FileSelectState* this)
{
    this->customFileInfoBufs[0] = malloc(CUSTOM_FILE_INFO_BUFFER_SIZE);
    this->customFileInfoBufs[1] = malloc(CUSTOM_FILE_INFO_BUFFER_SIZE);
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

static void drawItemIcon(Gfx** list, void** end, s16 x, s16 y, s16 dx, s16 dy, s16 id)
{
    float scale;
    s16 itemId;
    void* tex;

    scale = 0.5f;
    itemId = id & 0x3fff;
    tex = allocItemIconOot(end, itemId);

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

static void drawItemIconSimple(Gfx** list, void** end, s16 x, s16 y, s16 dx, s16 dy, s16 id, int noDim)
{
    if (!noDim)
        id |= ICONF_DIM;
    drawItemIcon(list, end, x, y, dx, dy, id);
}

static void FileSelect_CustomFileInfoPrepareOotInventory(FileSelectState* this, Gfx** list, void** end, int x, int y)
{
    s16 iconOcarina;
    s16 iconHookshot;
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
    drawItemIconSimple(list, end, x, y, 0, 0, ITEM_OOT_STICK,           gOotSave.inventory.items[ITS_OOT_STICKS] == ITEM_OOT_STICK);
    drawItemIconSimple(list, end, x, y, 1, 0, ITEM_OOT_NUT,             gOotSave.inventory.items[ITS_OOT_NUTS] == ITEM_OOT_NUT);
    drawItemIconSimple(list, end, x, y, 2, 0, ITEM_OOT_BOMB,            gOotSave.inventory.items[ITS_OOT_BOMBS] == ITEM_OOT_BOMB);
    drawItemIconSimple(list, end, x, y, 3, 0, ITEM_OOT_BOW,             gOotSave.inventory.items[ITS_OOT_BOW] == ITEM_OOT_BOW);
    drawItemIconSimple(list, end, x, y, 4, 0, ITEM_OOT_ARROW_FIRE,      gOotSave.inventory.items[ITS_OOT_ARROW_FIRE] == ITEM_OOT_ARROW_FIRE);
    drawItemIconSimple(list, end, x, y, 5, 0, ITEM_OOT_SPELL_FIRE,      gOotSave.inventory.items[ITS_OOT_SPELL_FIRE] == ITEM_OOT_SPELL_FIRE);
    drawItemIconSimple(list, end, x, y, 6, 0, ITEM_OOT_BOTTLE_EMPTY,    hasBottle);

    /* Row 2 */
    drawItemIconSimple(list, end, x, y, 0, 1, ITEM_OOT_SLINGSHOT,   gOotSave.inventory.items[ITS_OOT_SLINGSHOT] == ITEM_OOT_SLINGSHOT);
    drawItemIconSimple(list, end, x, y, 1, 1, iconOcarina,          gOotExtraItems.ocarina);
    drawItemIconSimple(list, end, x, y, 2, 1, ITEM_OOT_BOMBCHU_10,  gOotSave.inventory.items[ITS_OOT_BOMBCHU] == ITEM_OOT_BOMBCHU_10);
    drawItemIconSimple(list, end, x, y, 3, 1, iconHookshot,         gOotExtraItems.hookshot);
    drawItemIconSimple(list, end, x, y, 4, 1, ITEM_OOT_ARROW_ICE,   gOotSave.inventory.items[ITS_OOT_ARROW_ICE] == ITEM_OOT_ARROW_ICE);
    drawItemIconSimple(list, end, x, y, 5, 1, ITEM_OOT_SPELL_WIND,  gOotSave.inventory.items[ITS_OOT_SPELL_WIND] == ITEM_OOT_SPELL_WIND);
    drawItemIconSimple(list, end, x, y, 6, 1, ITEM_OOT_RUTO_LETTER, gOotExtraItems.rutoLetter);

    /* Row 3 */
    drawItemIconSimple(list, end, x, y, 0, 2, ITEM_OOT_BOOMERANG,   gOotSave.inventory.items[ITS_OOT_BOOMERANG] == ITEM_OOT_BOOMERANG);
    drawItemIconSimple(list, end, x, y, 1, 2, ITEM_OOT_LENS,        gOotSave.inventory.items[ITS_OOT_LENS] == ITEM_OOT_LENS);
    drawItemIconSimple(list, end, x, y, 2, 2, ITEM_OOT_MAGIC_BEAN,  gOotSave.inventory.items[ITS_OOT_MAGIC_BEAN] == ITEM_OOT_MAGIC_BEAN);
    drawItemIconSimple(list, end, x, y, 3, 2, ITEM_OOT_HAMMER,      gOotSave.inventory.items[ITS_OOT_HAMMER] == ITEM_OOT_HAMMER);
    drawItemIconSimple(list, end, x, y, 4, 2, ITEM_OOT_ARROW_LIGHT, gOotSave.inventory.items[ITS_OOT_ARROW_LIGHT] == ITEM_OOT_ARROW_LIGHT);
    drawItemIconSimple(list, end, x, y, 5, 2, ITEM_OOT_SPELL_LOVE,  gOotSave.inventory.items[ITS_OOT_SPELL_LOVE] == ITEM_OOT_SPELL_LOVE);
}

static void FileSelect_CustomFileInfoPrepareOot(FileSelectState* this, Gfx** list, void** end)
{
    FileSelect_CustomFileInfoPrepareOotInventory(this, list, end, 130, 94);
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

    FileSelect_CustomFileInfoPrepareOot(this, &list, &end);

    gSPEndDisplayList(list++);
}

void FileSelect_CustomFileInfoDraw(FileSelectState* this)
{
    OPEN_DISPS(this->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, this->customFileInfoBufs[0]);
    CLOSE_DISPS();
}

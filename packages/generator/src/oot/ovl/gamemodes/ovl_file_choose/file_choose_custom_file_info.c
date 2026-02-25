#include "file_select.h"
#include <combo/config.h>
#include <assets/oot/textures/parameter_static.h>

#define CUSTOM_FILE_INFO_BUFFER_SIZE    0x40000

#define ICON_SIZE   12
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

static void quadIA8(Gfx** gfx, void* tex, s16 w, s16 h, float x, float y, float scale)
{
    float revScale;

    revScale = 1.0f / scale;
    gDPLoadTextureBlock((*gfx)++, tex, G_IM_FMT_IA, G_IM_SIZ_8b, w, h, 0, G_TX_NOMIRROR | G_TX_CLAMP,
        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gSPTextureRectangle((*gfx)++,
        x * 4.f, y * 4.f,
        (int)(x * 4.f + w * scale * 4.f), (int)(y * 4.f + h * scale * 4.f),
        G_TX_RENDERTILE, 0, 0,
        (int)(revScale * (1 << 10)), (int)(revScale * (1 << 10)));
}

static void quadRGBA8(Gfx** gfx, void* tex, s16 w, s16 h, float x, float y, float scale)
{
    float revScale;

    revScale = 1.0f / scale;
    gDPLoadTextureBlock((*gfx)++, tex, G_IM_FMT_RGBA, G_IM_SIZ_32b, w, h, 0, G_TX_NOMIRROR | G_TX_CLAMP,
        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gSPTextureRectangle((*gfx)++,
        x * 4.f, y * 4.f,
        (int)(x * 4.f + w * scale * 4.f), (int)(y * 4.f + h * scale * 4.f),
        G_TX_RENDERTILE, 0, 0,
        (int)(revScale * (1 << 10)), (int)(revScale * (1 << 10)));
}

static s16 drawDigit(Gfx** list, s16 x, s16 y, int digit)
{
    void* tex;

    tex = (void*)(((u32)&gAmmoDigit0Tex) + 0x40 * digit);
    quadIA8(list, tex, 8, 8, x, y, 1.0f);
    return 6;
}

static void drawNumber(Gfx** list, s16 x, s16 y, int num, int paddingZeroes)
{
    int divisor = 1000000000;
    int paddingZeroesDivisor = 1;
    int startPrint;
    int digit;

    startPrint = 0;
    if (paddingZeroes)
        paddingZeroes--;
    while (paddingZeroes)
    {
        paddingZeroesDivisor *= 10;
        paddingZeroes--;
    }

    for (;;)
    {
        if (num < divisor)
        {
            if (divisor <= paddingZeroesDivisor)
                startPrint = 1;
            digit = 0;
        }
        else
        {
            digit = num / divisor;
            num -= digit * divisor;
        }
        divisor /= 10;

        if (digit || startPrint)
        {
            startPrint = 1;
            x += drawDigit(list, x, y, digit);
        }

        if (!divisor)
            break;
    }
}

static s16 drawItemIcon(Gfx** list, void** end, s16 x, s16 y, u16 id, int noDim)
{
    static const float scale = (float)ICON_SIZE / 32.f;
    u16 itemId;
    void* tex;

    if (id == ICON_NONE)
        return 0;

    itemId = id & 0x3fff;
    if (!(id & ICONF_MM))
        tex = allocItemIconOot(end, itemId);
    else
        tex = allocItemIconMm(end, itemId);

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
    quadRGBA8(list, tex, 32, 32, x, y, scale);
    return ICON_SIZE;
}

static float drawItemIcon24(Gfx** list, void** end, s16 x, s16 y, u16 id, int noDim)
{
    static const float scale = (float)ICON_SIZE / 24.f;
    u16 itemId;
    void* tex;

    if (id == ICON_NONE)
        return 0;

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
    quadRGBA8(list, tex, 24, 24, x, y, scale);
    return ICON_SIZE;
}

static void drawInfoHeart(Gfx** list, int x, int y, int isDefense, int count)
{
    gDPSetCombineLERP((*list)++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
    gDPSetPrimColor((*list)++, 0, 0, gHeartPrimColors[isDefense][0], gHeartPrimColors[isDefense][1], gHeartPrimColors[isDefense][2], 255);
    gDPSetEnvColor((*list)++, gHeartEnvColors[isDefense][0], gHeartEnvColors[isDefense][1], gHeartEnvColors[isDefense][2], 255);
    quadIA8(list, &gHeartFullTex, 16, 16, x, y, ICON_SIZE / 16.f);
    gDPSetCombineMode((*list)++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor((*list)++, 0, 0, 255, 255, 255, 255);
    drawNumber(list, x + ICON_SIZE - 1, y + 3, count, 2);
}

static void drawInfoRupees(Gfx** list, int x, int y, int index, int count)
{
    u32 c;

    if (index == 0)
        return;

    c = kRupeeColor[index];
    gDPSetPrimColor((*list)++, 0, 0, (c >> 24) & 0xff, (c >> 16) & 0xff, (c >> 8) & 0xff, 255);
    quadIA8(list, &gRupeeCounterIconTex, 16, 16, x, y, ICON_SIZE / 16.f);
    gDPSetPrimColor((*list)++, 0, 0, 255, 255, 255, 255);
    drawNumber(list, x + ICON_SIZE - 1, y + 3, count, kRupeeDigits[index]);
}

static void FileSelect_CustomFileInfoPrepareOotInfos(FileSelectState* this, Gfx** list, void** end, int x, int y)
{
    /* Draw heart */
    drawInfoHeart(list, x, y, gOotSave.info.playerData.doubleDefense, gOotSave.info.playerData.healthCapacity / 0x10);
    drawInfoRupees(list, x, y + ICON_SIZE, Wallet_IndexOot(), gOotSave.info.playerData.rupees);
}

static void FileSelect_CustomFileInfoPrepareMmInfos(FileSelectState* this, Gfx** list, void** end, int x, int y)
{
    /* Draw heart */
    drawInfoHeart(list, x, y, gMmSave.info.playerData.doubleDefense, gMmSave.info.playerData.healthCapacity / 0x10);
    drawInfoRupees(list, x, y + ICON_SIZE, Wallet_IndexMm(), gMmSave.info.playerData.rupees);
}

static void FileSelect_CustomFileInfoPrepareOotMedsStones(FileSelectState* this, Gfx** list, void** end, int x, int y)
{
    static const float sqrt2_2 = 0.70710677f;

    /* Medallions */
    drawItemIcon24(list, end, x + 1 * ICON_SIZE, y, 5, gOotSave.info.inventory.quest.medallionLight);
    drawItemIcon24(list, end, x, y + sqrt2_2 * ICON_SIZE, 4, gOotSave.info.inventory.quest.medallionShadow);
    drawItemIcon24(list, end, x + 2 * ICON_SIZE, y + sqrt2_2 * ICON_SIZE, 0, gOotSave.info.inventory.quest.medallionForest);
    drawItemIcon24(list, end, x, y + ICON_SIZE + sqrt2_2 * ICON_SIZE, 3, gOotSave.info.inventory.quest.medallionSpirit);
    drawItemIcon24(list, end, x + 2 * ICON_SIZE, y + ICON_SIZE + sqrt2_2 * ICON_SIZE, 1, gOotSave.info.inventory.quest.medallionFire);
    drawItemIcon24(list, end, x + 1 * ICON_SIZE, y + ICON_SIZE + sqrt2_2 * ICON_SIZE * 2, 2, gOotSave.info.inventory.quest.medallionWater);

    /* Stones */
    drawItemIcon24(list, end, x + 0 * ICON_SIZE, y + ICON_SIZE * 4, 6, gOotSave.info.inventory.quest.stoneEmerald);
    drawItemIcon24(list, end, x + 1 * ICON_SIZE, y + ICON_SIZE * 4, 7, gOotSave.info.inventory.quest.stoneRuby);
    drawItemIcon24(list, end, x + 2 * ICON_SIZE, y + ICON_SIZE * 4, 8, gOotSave.info.inventory.quest.stoneSapphire);
}

static void FileSelect_CustomFileInfoPrepareOotInventory(FileSelectState* this, Gfx** list, void** end, int x, int y)
{
    int startX;
    u16 iconOcarina;
    u16 iconHookshot;
    u8 itemId;
    u8 bottlesCount;

    /* Pre-compute complex icons */
    if (gOotExtraItems.ocarina & 2)
        iconOcarina = ITEM_OOT_OCARINA_TIME;
    else
        iconOcarina = ITEM_OOT_OCARINA_FAIRY;

    if (gOotExtraItems.hookshot & 2)
        iconHookshot = ITEM_OOT_LONGSHOT;
    else
        iconHookshot = ITEM_OOT_HOOKSHOT;

    bottlesCount = 0;
    for (int i = 0; i < 4; ++i)
    {
        itemId = gOotSave.info.inventory.items[ITS_OOT_BOTTLE + i];
        if (itemId >= ITEM_OOT_BOTTLE_EMPTY && itemId <= ITEM_OOT_POE && itemId != ITEM_OOT_RUTO_LETTER)
            bottlesCount++;
    }

    /* Row 1 */
    startX = x;
    x += drawItemIcon(list, end, x, y, ITEM_OOT_STICK,           gOotSave.info.inventory.items[ITS_OOT_STICKS] == ITEM_OOT_STICK);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_NUT,             gOotSave.info.inventory.items[ITS_OOT_NUTS] == ITEM_OOT_NUT);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_BOMB,            gOotSave.info.inventory.items[ITS_OOT_BOMBS] == ITEM_OOT_BOMB);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_BOW,             gOotSave.info.inventory.items[ITS_OOT_BOW] == ITEM_OOT_BOW);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_ARROW_FIRE,      gOotSave.info.inventory.items[ITS_OOT_ARROW_FIRE] == ITEM_OOT_ARROW_FIRE);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_SPELL_FIRE,      gOotSave.info.inventory.items[ITS_OOT_SPELL_FIRE] == ITEM_OOT_SPELL_FIRE);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_BOTTLE_EMPTY,    bottlesCount);
    if (bottlesCount > 1)
        drawDigit(list, x - 7, y, bottlesCount);

    /* Row 2 */
    x = startX;
    y += ICON_SIZE;
    x += drawItemIcon(list, end, x, y, ITEM_OOT_SLINGSHOT,   gOotSave.info.inventory.items[ITS_OOT_SLINGSHOT] == ITEM_OOT_SLINGSHOT);
    x += drawItemIcon(list, end, x, y, iconOcarina,          gOotExtraItems.ocarina);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_BOMBCHU_10,  gOotSave.info.inventory.items[ITS_OOT_BOMBCHU] == ITEM_OOT_BOMBCHU_10);
    x += drawItemIcon(list, end, x, y, iconHookshot,         gOotExtraItems.hookshot);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_ARROW_ICE,   gOotSave.info.inventory.items[ITS_OOT_ARROW_ICE] == ITEM_OOT_ARROW_ICE);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_SPELL_WIND,  gOotSave.info.inventory.items[ITS_OOT_SPELL_WIND] == ITEM_OOT_SPELL_WIND);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_RUTO_LETTER, gOotExtraItems.rutoLetter);

    /* Row 3 */
    x = startX;
    y += ICON_SIZE;
    x += drawItemIcon(list, end, x, y, ITEM_OOT_BOOMERANG,   gOotSave.info.inventory.items[ITS_OOT_BOOMERANG] == ITEM_OOT_BOOMERANG);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_LENS,        gOotSave.info.inventory.items[ITS_OOT_LENS] == ITEM_OOT_LENS);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_MAGIC_BEAN,  gOotSave.info.inventory.items[ITS_OOT_MAGIC_BEAN] == ITEM_OOT_MAGIC_BEAN);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_HAMMER,      gOotSave.info.inventory.items[ITS_OOT_HAMMER] == ITEM_OOT_HAMMER);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_ARROW_LIGHT, gOotSave.info.inventory.items[ITS_OOT_ARROW_LIGHT] == ITEM_OOT_ARROW_LIGHT);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_SPELL_LOVE,  gOotSave.info.inventory.items[ITS_OOT_SPELL_LOVE] == ITEM_OOT_SPELL_LOVE);

    /* Row 4 - Child Trade */
    x = startX;
    y += ICON_SIZE + 3;
    if (Config_Flag(CFG_OOT_SHUFFLE_EGGS))
        x += drawItemIcon(list, end, x, y, ITEM_OOT_WEIRD_EGG,                gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_WEIRD_EGG));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_CHICKEN,                  gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_CHICKEN));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_ZELDA_LETTER,             gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_ZELDA_LETTER));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_KEATON_MASK,              gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_KEATON_MASK));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_SKULL_MASK,               gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_SKULL_MASK));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_SPOOKY_MASK,              gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_SPOOKY_MASK));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_BUNNY_HOOD,               gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_BUNNY_HOOD));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_GORON_MASK,               gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_GORON_MASK));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_ZORA_MASK,                gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_ZORA_MASK));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_GERUDO_MASK,              gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_GERUDO_MASK));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_MASK_OF_TRUTH,            gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_MASK_OF_TRUTH));
    if (Config_Flag(CFG_OOT_MASK_BLAST))
        x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_BLAST | ICONF_MM,     gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_MASK_BLAST));
    if (Config_Flag(CFG_OOT_MASK_STONE))
        x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_STONE | ICONF_MM,     gOotExtraTradeSave.child & (1 << XITEM_OOT_CHILD_MASK_STONE));

    /* Row 5 - Adult Trade */
    x = startX;
    y += ICON_SIZE;
    if (Config_Flag(CFG_OOT_SHUFFLE_EGGS))
        x += drawItemIcon(list, end, x, y, ITEM_OOT_POCKET_EGG,                gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_POCKET_EGG));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_POCKET_CUCCO,              gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_POCKET_CUCCO));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_COJIRO,                    gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_COJIRO));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_ODD_MUSHROOM,              gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_ODD_MUSHROOM));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_ODD_POTION,                gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_ODD_POTION));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_POACHER_SAW,               gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_POACHER_SAW));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_GORON_SWORD_BROKEN,        gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_BROKEN_GORON_SWORD));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_PRESCRIPTION,              gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_PRESCRIPTION));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_EYEBALL_FROG,              gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_EYEBALL_FROG));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_EYE_DROPS,                 gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_EYE_DROPS));
    x += drawItemIcon(list, end, x, y, ITEM_OOT_CLAIM_CHECK,               gOotExtraTradeSave.adult & (1 << XITEM_OOT_ADULT_CLAIM_CHECK));
}

static void FileSelect_CustomFileInfoPrepareOot(FileSelectState* this, Gfx** list, void** end)
{
    FileSelect_CustomFileInfoPrepareOotInfos(this, list, end, 56, 94);
    FileSelect_CustomFileInfoPrepareOotMedsStones(this, list, end, 96, 94);
    FileSelect_CustomFileInfoPrepareOotInventory(this, list, end, 140, 94);
}

static void FileSelect_CustomFileInfoPrepareMmRemains(FileSelectState* this, Gfx** list, void** end, int x, int y)
{
    static const float scale = 0.75f;
    static const float sqrt2_2 = 0.70710677f;

    drawItemIcon(list, end, x + ICON_SIZE * 1, y,                                      ITEM_MM_REMAINS_ODOLWA | ICONF_MM,      gMmSave.info.inventory.quest.remainsOdolwa);
    drawItemIcon(list, end, x, y + sqrt2_2 * scale * 16,                               ITEM_MM_REMAINS_GYORG | ICONF_MM,       gMmSave.info.inventory.quest.remainsGyorg);
    drawItemIcon(list, end, x + ICON_SIZE * 2, y + sqrt2_2 * scale * 16,               ITEM_MM_REMAINS_GOHT | ICONF_MM,        gMmSave.info.inventory.quest.remainsGoht);
    drawItemIcon(list, end, x + ICON_SIZE * 1, y + sqrt2_2 * scale * 16 * 2,           ITEM_MM_REMAINS_TWINMOLD | ICONF_MM,    gMmSave.info.inventory.quest.remainsTwinmold);
}

static void FileSelect_CustomFileInfoPrepareMmInventory(FileSelectState* this, Gfx** list, void** end, int x, int y)
{
    int startX;
    u16 iconOcarina;
    u16 iconHookshot;
    u16 itemId;
    u8  bottlesCount;

    /* Pre-compute complex icons */
    iconOcarina = ITEM_OOT_OCARINA_TIME;
    if (Config_Flag(CFG_MM_OCARINA_FAIRY) && !(gMmExtraItems.ocarina & 2))
        iconOcarina = ITEM_OOT_OCARINA_FAIRY;

    iconHookshot = ITEM_MM_HOOKSHOT | ICONF_MM;
    if (Config_Flag(CFG_MM_HOOKSHOT_SHORT) && !(gMmExtraItems.hookshot & 2))
        iconHookshot = ITEM_OOT_HOOKSHOT;

    /* Bottle */
    bottlesCount = 0;
    for (int i = 0; i < 6; ++i)
    {
        itemId = gMmSave.info.inventory.items[ITS_MM_BOTTLE + i];
        if (itemId >= ITEM_MM_BOTTLE_EMPTY && itemId <= ITEM_MM_CHATEAU && itemId != ITEM_MM_GOLD_DUST)
        {
            bottlesCount++;
        }
    }

    /* Main Items */
    startX = x;
    x += drawItemIcon(list, end, x, y, iconOcarina,                  gMmExtraItems.ocarina);
    x += drawItemIcon(list, end, x, y, ITEM_MM_BOW | ICONF_MM,       gMmSave.info.inventory.items[ITS_MM_BOW] == ITEM_MM_BOW);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_ARROW_FIRE,          gMmSave.info.inventory.items[ITS_MM_ARROW_FIRE] == ITEM_MM_ARROW_FIRE);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_ARROW_ICE,           gMmSave.info.inventory.items[ITS_MM_ARROW_ICE] == ITEM_MM_ARROW_ICE);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_ARROW_LIGHT,         gMmSave.info.inventory.items[ITS_MM_ARROW_LIGHT] == ITEM_MM_ARROW_LIGHT);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_BOTTLE_EMPTY,        bottlesCount);
    if (bottlesCount > 1)
        drawDigit(list, x - 7, y, bottlesCount);
    x = startX;
    y += ICON_SIZE;
    x += drawItemIcon(list, end, x, y, ITEM_OOT_BOMB,                gMmSave.info.inventory.items[ITS_MM_BOMBS] == ITEM_MM_BOMB);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_BOMBCHU_10,          gMmSave.info.inventory.items[ITS_MM_BOMBCHU] == ITEM_MM_BOMBCHU);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_STICK,               gMmSave.info.inventory.items[ITS_MM_STICKS] == ITEM_MM_STICK);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_NUT,                 gMmSave.info.inventory.items[ITS_MM_NUTS] == ITEM_MM_NUT);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_MAGIC_BEAN,          gMmSave.info.inventory.items[ITS_MM_BEANS] == ITEM_MM_MAGIC_BEAN);
    x += drawItemIcon(list, end, x, y, ITEM_MM_GOLD_DUST | ICONF_MM, gMmExtraItems.goldDust);
    x = startX;
    y += ICON_SIZE;
    x += drawItemIcon(list, end, x, y, ITEM_MM_POWDER_KEG | ICONF_MM,            gMmSave.info.inventory.items[ITS_MM_KEG] == ITEM_MM_POWDER_KEG);
    x += drawItemIcon(list, end, x, y, ITEM_MM_PICTOGRAPH_BOX | ICONF_MM,        gMmSave.info.inventory.items[ITS_MM_PICTOBOX] == ITEM_MM_PICTOGRAPH_BOX);
    x += drawItemIcon(list, end, x, y, ITEM_OOT_LENS,                            gMmSave.info.inventory.items[ITS_MM_LENS] == ITEM_MM_LENS_OF_TRUTH);
    x += drawItemIcon(list, end, x, y, iconHookshot,                             gMmExtraItems.hookshot);
    x += drawItemIcon(list, end, x, y, ITEM_MM_GREAT_FAIRY_SWORD | ICONF_MM,     gMmExtraItems.hammerGFS & 1);

    /* Trade Items */
    x = startX;
    y += ICON_SIZE + 3;
    x += drawItemIcon(list, end, x, y, ITEM_MM_MOON_TEAR | ICONF_MM,             gMmExtraTrade.tradeObtained1 & (1 << XITEM_MM_TRADE1_MOON_TEAR));
    x += drawItemIcon(list, end, x, y, ITEM_MM_DEED_LAND | ICONF_MM,             gMmExtraTrade.tradeObtained1 & (1 << XITEM_MM_TRADE1_DEED_LAND));
    x += drawItemIcon(list, end, x, y, ITEM_MM_DEED_SWAMP | ICONF_MM,            gMmExtraTrade.tradeObtained1 & (1 << XITEM_MM_TRADE1_DEED_SWAMP));
    x += drawItemIcon(list, end, x, y, ITEM_MM_DEED_MOUNTAIN | ICONF_MM,         gMmExtraTrade.tradeObtained1 & (1 << XITEM_MM_TRADE1_DEED_MOUNTAIN));
    x += drawItemIcon(list, end, x, y, ITEM_MM_DEED_OCEAN | ICONF_MM,            gMmExtraTrade.tradeObtained1 & (1 << XITEM_MM_TRADE1_DEED_OCEAN));
    x += drawItemIcon(list, end, x, y, ITEM_MM_ROOM_KEY | ICONF_MM,              gMmExtraTrade.tradeObtained2 & (1 << XITEM_MM_TRADE2_ROOM_KEY));
    x += drawItemIcon(list, end, x, y, ITEM_MM_LETTER_TO_MAMA | ICONF_MM,        gMmExtraTrade.tradeObtained2 & (1 << XITEM_MM_TRADE2_LETTER_TO_MAMA));
    x += drawItemIcon(list, end, x, y, ITEM_MM_LETTER_TO_KAFEI | ICONF_MM,       gMmExtraTrade.tradeObtained3 & (1 << XITEM_MM_TRADE3_LETTER_TO_KAFEI));
    x += drawItemIcon(list, end, x, y, ITEM_MM_PENDANT_OF_MEMORIES | ICONF_MM,   gMmExtraTrade.tradeObtained3 & (1 << XITEM_MM_TRADE3_PENDANT_OF_MEMORIES));

    /* Masks */
    x = startX;
    y += ICON_SIZE + 3;
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_POSTMAN | ICONF_MM,             gMmSave.info.inventory.items[ITS_MM_MASK_POSTMAN] == ITEM_MM_MASK_POSTMAN);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_ALL_NIGHT | ICONF_MM,           gMmSave.info.inventory.items[ITS_MM_MASK_ALL_NIGHT] == ITEM_MM_MASK_ALL_NIGHT);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_BLAST | ICONF_MM,               gMmSave.info.inventory.items[ITS_MM_MASK_BLAST] == ITEM_MM_MASK_BLAST);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_STONE | ICONF_MM,               gMmSave.info.inventory.items[ITS_MM_MASK_STONE] == ITEM_MM_MASK_STONE);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_GREAT_FAIRY | ICONF_MM,         gMmSave.info.inventory.items[ITS_MM_MASK_GREAT_FAIRY] == ITEM_MM_MASK_GREAT_FAIRY);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_DEKU | ICONF_MM,                gMmSave.info.inventory.items[ITS_MM_MASK_DEKU] == ITEM_MM_MASK_DEKU);
    x = startX;
    y += ICON_SIZE;
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_KEATON | ICONF_MM,               gMmSave.info.inventory.items[ITS_MM_MASK_KEATON] == ITEM_MM_MASK_KEATON);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_BREMEN | ICONF_MM,               gMmSave.info.inventory.items[ITS_MM_MASK_BREMEN] == ITEM_MM_MASK_BREMEN);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_BUNNY | ICONF_MM,                gMmSave.info.inventory.items[ITS_MM_MASK_BUNNY] == ITEM_MM_MASK_BUNNY);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_DON_GERO | ICONF_MM,             gMmSave.info.inventory.items[ITS_MM_MASK_DON_GERO] == ITEM_MM_MASK_DON_GERO);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_SCENTS | ICONF_MM,               gMmSave.info.inventory.items[ITS_MM_MASK_SCENTS] == ITEM_MM_MASK_SCENTS);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_GORON | ICONF_MM,                gMmSave.info.inventory.items[ITS_MM_MASK_GORON] == ITEM_MM_MASK_GORON);
    x = startX;
    y += ICON_SIZE;
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_ROMANI | ICONF_MM,               gMmSave.info.inventory.items[ITS_MM_MASK_ROMANI] == ITEM_MM_MASK_ROMANI);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_TROUPE_LEADER | ICONF_MM,        gMmSave.info.inventory.items[ITS_MM_MASK_TROUPE_LEADER] == ITEM_MM_MASK_TROUPE_LEADER);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_KAFEI | ICONF_MM,                gMmSave.info.inventory.items[ITS_MM_MASK_KAFEI] == ITEM_MM_MASK_KAFEI);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_COUPLE | ICONF_MM,               gMmSave.info.inventory.items[ITS_MM_MASK_COUPLE] == ITEM_MM_MASK_COUPLE);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_TRUTH | ICONF_MM,                gMmSave.info.inventory.items[ITS_MM_MASK_TRUTH] == ITEM_MM_MASK_TRUTH);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_ZORA | ICONF_MM,                 gMmSave.info.inventory.items[ITS_MM_MASK_ZORA] == ITEM_MM_MASK_ZORA);
    x = startX;
    y += ICON_SIZE;
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_KAMARO | ICONF_MM,               gMmSave.info.inventory.items[ITS_MM_MASK_KAMARO] == ITEM_MM_MASK_KAMARO);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_GIBDO | ICONF_MM,                gMmSave.info.inventory.items[ITS_MM_MASK_GIBDO] == ITEM_MM_MASK_GIBDO);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_GARO | ICONF_MM,                 gMmSave.info.inventory.items[ITS_MM_MASK_GARO] == ITEM_MM_MASK_GARO);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_CAPTAIN | ICONF_MM,              gMmSave.info.inventory.items[ITS_MM_MASK_CAPTAIN] == ITEM_MM_MASK_CAPTAIN);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_GIANT | ICONF_MM,                gMmSave.info.inventory.items[ITS_MM_MASK_GIANT] == ITEM_MM_MASK_GIANT);
    x += drawItemIcon(list, end, x, y, ITEM_MM_MASK_FIERCE_DEITY | ICONF_MM,         gMmSave.info.inventory.items[ITS_MM_MASK_FIERCE_DEITY] == ITEM_MM_MASK_FIERCE_DEITY);
}

static void FileSelect_CustomFileInfoPrepareMm(FileSelectState* this, Gfx** list, void** end)
{
    FileSelect_CustomFileInfoPrepareMmInfos(this, list, end, 56, 94);
    FileSelect_CustomFileInfoPrepareMmRemains(this, list, end, 96, 94);
    FileSelect_CustomFileInfoPrepareMmInventory(this, list, end, 140, 94);
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

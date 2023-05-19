#include <combo.h>
#include <combo/custom.h>

#define DD_OOT          0x00
#define DD_MM           0x01
#define DD_MISC         0x02
#define DD_MAP_COMPASS  0x04
#define DD_BOSS_KEY     0x08
#define DD_FAIRIES      0x10

typedef struct
{
    const char* name;
    int id;
    int flags;
    int maxKeys;
}
DungeonDef;

typedef struct
{
    s8 keys;
    u8 fairies;
    u8 map:1;
    u8 compass:1;
    u8 bossKey:1;
}
DungeonData;

static u32 kFairyColors[] = {
    0xba5084ff,
    0x45852bff,
    0x7f65ccff,
    0xc2c164ff,
    0xbc702dff,
};

static DungeonDef gDungeonDefs[] = {
    { "Deku",           SCE_OOT_INSIDE_DEKU_TREE,       DD_OOT | DD_MAP_COMPASS, 0 },
    { "Dodongo",        SCE_OOT_DODONGO_CAVERN,         DD_OOT | DD_MAP_COMPASS, 0 },
    { "Jabu",           SCE_OOT_INSIDE_JABU_JABU,       DD_OOT | DD_MAP_COMPASS, 0 },
    { "Forest",         SCE_OOT_TEMPLE_FOREST,          DD_OOT | DD_MAP_COMPASS | DD_BOSS_KEY, 5 },
    { "Fire",           SCE_OOT_TEMPLE_FIRE,            DD_OOT | DD_MAP_COMPASS | DD_BOSS_KEY, 7 },
    { "Water",          SCE_OOT_TEMPLE_WATER,           DD_OOT | DD_MAP_COMPASS | DD_BOSS_KEY, 5 },
    { "Shadow",         SCE_OOT_TEMPLE_SHADOW,          DD_OOT | DD_MAP_COMPASS | DD_BOSS_KEY, 5 },
    { "Spirit",         SCE_OOT_TEMPLE_SPIRIT,          DD_OOT | DD_MAP_COMPASS | DD_BOSS_KEY, 5 },
    { "Well",           SCE_OOT_BOTTOM_OF_THE_WELL,     DD_OOT | DD_MAP_COMPASS, 3 },
    { "Ice",            SCE_OOT_ICE_CAVERN,             DD_OOT | DD_MAP_COMPASS, 0 },
    { "Hideout",        SCE_OOT_THIEVES_HIDEOUT,        DD_OOT, 4 },
    { "GTG",            SCE_OOT_GERUDO_TRAINING_GROUND, DD_OOT, 9 },
    { "Ganon",          SCE_OOT_INSIDE_GANON_CASTLE,    DD_OOT | DD_BOSS_KEY, 2 },
    { "Woodfall",       0,                              DD_MM  | DD_MAP_COMPASS | DD_BOSS_KEY | DD_FAIRIES, 1 },
    { "Snowhead",       1,                              DD_MM  | DD_MAP_COMPASS | DD_BOSS_KEY | DD_FAIRIES, 3 },
    { "Great Bay",      2,                              DD_MM  | DD_MAP_COMPASS | DD_BOSS_KEY | DD_FAIRIES, 1 },
    { "Stone Tower",    3,                              DD_MM  | DD_MAP_COMPASS | DD_BOSS_KEY | DD_FAIRIES, 4 },
    { "Clock Town",     0,                              DD_MISC, 0 },
    { "Tokens",         1,                              DD_MISC, 0 },
    { "Triforce",       2,                              DD_MISC, 0 },
};

static int menuCount(void)
{
    int count;

    count = ARRAY_SIZE(gDungeonDefs);
    if (!comboConfig(CFG_GOAL_TRIFORCE))
        count--;
    return count;
}

static void color4(u8* r, u8* g, u8* b, u8* a, u32 color)
{
    *r = (color >> 24) & 0xff;
    *g = (color >> 16) & 0xff;
    *b = (color >> 8) & 0xff;
    *a = color & 0xff;
}

static Vtx gVtxBuffer[1024];
static int gVtxBufferIndex;

static Vtx* vtxAlloc(GameState_Play* play, int count)
{
    Vtx* v;

    v = gVtxBuffer + gVtxBufferIndex;
    gVtxBufferIndex = (gVtxBufferIndex + count) % ARRAY_SIZE(gVtxBuffer);

    return v;
}

static void drawBackground(GameState_Play* play, float x, float y, float w, float h)
{
    Vtx* v;

    int xx[4];
    int yy[4];

    v = vtxAlloc(play, 4);

    xx[0] = x;
    xx[1] = x + w;
    xx[2] = x + w;
    xx[3] = x;

    yy[0] = y;
    yy[1] = y;
    yy[2] = y - h;
    yy[3] = y - h;

    for (int i = 0; i < 4; ++i)
    {
        v[i].v.ob[0] = xx[i];
        v[i].v.ob[1] = yy[i];
        v[i].v.ob[2] = 0;
        v[i].v.tc[0] = 0;
        v[i].v.tc[1] = 0;
        v[i].v.flag = 0;
        v[i].v.cn[0] = 0x00;
        v[i].v.cn[1] = 0x00;
        v[i].v.cn[2] = 0x00;
        v[i].v.cn[3] = 0xff;
    }

    OPEN_DISPS(play->gs.gfx);
    gDPPipeSync(POLY_OPA_DISP++);
    gSPVertex(POLY_OPA_DISP++, (u32)v & 0xffffffff, 4, 0);
    gSP2Triangles(
        POLY_OPA_DISP++,
        0, 2, 1, 0,
        0, 3, 2, 0
    );
    CLOSE_DISPS();
}

static void drawTexIA4(GameState_Play* play, u32 texAddr, int w, int h, float x, float y)
{
    Vtx* v;
    int xx[4];
    int yy[4];
    int txx[4];
    int tyy[4];

    v = vtxAlloc(play, 4);

    xx[0] = x;
    xx[1] = x + w;
    xx[2] = x + w;
    xx[3] = x;

    yy[0] = y;
    yy[1] = y;
    yy[2] = y - h;
    yy[3] = y - h;

    txx[0] = 0;
    txx[1] = w * 32;
    txx[2] = w * 32;
    txx[3] = 0;

    tyy[0] = 0;
    tyy[1] = 0;
    tyy[2] = h * 32;
    tyy[3] = h * 32;

    for (int i = 0; i < 4; ++i)
    {
        v[i].v.ob[0] = xx[i];
        v[i].v.ob[1] = yy[i];
        v[i].v.ob[2] = 0;
        v[i].v.tc[0] = txx[i];
        v[i].v.tc[1] = tyy[i];
        v[i].v.flag = 0;
        v[i].v.cn[0] = 0xff;
        v[i].v.cn[1] = 0xff;
        v[i].v.cn[2] = 0xff;
        v[i].v.cn[3] = 0xff;
    }

    OPEN_DISPS(play->gs.gfx);
    gDPPipeSync(POLY_OPA_DISP++);
    gSPVertex(POLY_OPA_DISP++, (u32)v & 0xffffffff, 4, 0);
    gDPTileSync(POLY_OPA_DISP++);
    gDPLoadTextureTile_4b(
        POLY_OPA_DISP++,
        texAddr,
        G_IM_FMT_IA,
        w, h,
        0, 0,
        w - 1, h - 1,
        0,
        G_TX_WRAP, G_TX_WRAP,
        G_TX_NOMASK, G_TX_NOMASK,
        G_TX_NOLOD, G_TX_NOLOD
    );
    gDPTileSync(POLY_OPA_DISP++);
    gSP2Triangles(
        POLY_OPA_DISP++,
        0, 2, 1, 0,
        0, 3, 2, 0
    );
    CLOSE_DISPS();
}

static void drawTexRGBA32(GameState_Play* play, u32 texAddr, int w, int h, float x, float y)
{
    Vtx* v;
    int xx[4];
    int yy[4];
    int txx[4];
    int tyy[4];

    v = vtxAlloc(play, 4);

    xx[0] = x;
    xx[1] = x + w;
    xx[2] = x + w;
    xx[3] = x;

    yy[0] = y;
    yy[1] = y;
    yy[2] = y - h;
    yy[3] = y - h;

    txx[0] = 0;
    txx[1] = w * 32;
    txx[2] = w * 32;
    txx[3] = 0;

    tyy[0] = 0;
    tyy[1] = 0;
    tyy[2] = h * 32;
    tyy[3] = h * 32;

    for (int i = 0; i < 4; ++i)
    {
        v[i].v.ob[0] = xx[i];
        v[i].v.ob[1] = yy[i];
        v[i].v.ob[2] = 0;
        v[i].v.tc[0] = txx[i];
        v[i].v.tc[1] = tyy[i];
        v[i].v.flag = 0;
        v[i].v.cn[0] = 0xff;
        v[i].v.cn[1] = 0xff;
        v[i].v.cn[2] = 0xff;
        v[i].v.cn[3] = 0xff;
    }

    OPEN_DISPS(play->gs.gfx);
    gDPPipeSync(POLY_OPA_DISP++);
    gSPVertex(POLY_OPA_DISP++, (u32)v & 0xffffffff, 4, 0);
    gDPTileSync(POLY_OPA_DISP++);
    gDPLoadTextureTile(
        POLY_OPA_DISP++,
        texAddr,
        G_IM_FMT_RGBA,
        G_IM_SIZ_32b,
        w, h,
        0, 0,
        w - 1, h - 1,
        0,
        G_TX_WRAP, G_TX_WRAP,
        G_TX_NOMASK, G_TX_NOMASK,
        G_TX_NOLOD, G_TX_NOLOD
    );
    gDPTileSync(POLY_OPA_DISP++);
    gSP2Triangles(
        POLY_OPA_DISP++,
        0, 2, 1, 0,
        0, 3, 2, 0
    );
    CLOSE_DISPS();
}

static void printChar(GameState_Play* play, char c, float x, float y)
{
    OPEN_DISPS(play->gs.gfx);
    drawTexIA4(play, 0x06000000 | (CUSTOM_KEEP_FONT + (c - ' ') * 0x30), 8, 12, x, y);
    CLOSE_DISPS();
}

static void printStr(GameState_Play* play, const char* str, float x, float y)
{
    int i;
    char c;

    i = 0;
    for (;;)
    {
        c = str[i++];
        if (!c)
            break;
        printChar(play, c, x + 8 * i, y);
    }
}

static void printDigit(GameState_Play* play, int digit, float x, float y)
{
    printChar(play, '0' + digit, x, y);
}

static void printNum(GameState_Play* play, int num, int digits, float x, float y)
{
    int d;
    int denum;
    int zero;

    if (num == 0)
    {
        printDigit(play, 0, x + 8.f * (digits - 1), y);
        return;
    }

    denum = 1;
    for (int i = 1; i < digits; ++i)
        denum *= 10;
    zero = 0;
    while (denum)
    {
        d = (num / denum) % 10;
        if (d || zero)
        {
            printDigit(play, d, x, y);
            zero = 1;
        }
        denum /= 10;
        x += 8.f;
    }
}

static void printNumColored(GameState_Play* play, int num, int max, int digits, float x, float y)
{
    u8 r;
    u8 g;
    u8 b;

    if (num <= 0)
    {
        r = 127;
        g = 127;
        b = 127;
    }
    else if (num >= max)
    {
        r = 200;
        g = 100;
        b = 100;
    }
    else
    {
        r = 255;
        g = 255;
        b = 255;
    }
    OPEN_DISPS(play->gs.gfx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, r, g, b, 255);
    CLOSE_DISPS();

    printNum(play, num, digits, x, y);
}

static void dungeonDataOot(DungeonData* out, const DungeonDef* def)
{
    out->keys = gOotSave.inventory.dungeonItems[def->id].maxKeys;
    out->fairies = 0;
    out->map = gOotSave.inventory.dungeonItems[def->id].map;
    out->compass = gOotSave.inventory.dungeonItems[def->id].compass;
    if (def->id == SCE_OOT_INSIDE_GANON_CASTLE)
        out->bossKey = gOotSave.inventory.dungeonItems[SCE_OOT_GANON_TOWER].bossKey;
    else
        out->bossKey = gOotSave.inventory.dungeonItems[def->id].bossKey;
}

static void dungeonDataMm(DungeonData* out, const DungeonDef* def)
{
    out->keys = gMmSave.inventory.dungeonItems[def->id].maxKeys;
    out->bossKey = gMmSave.inventory.dungeonItems[def->id].bossKey;
    out->fairies = gMmSave.inventory.strayFairies[def->id];
    out->map = gMmSave.inventory.dungeonItems[def->id].map;
    out->compass = gMmSave.inventory.dungeonItems[def->id].compass;
}

static void printDungeonData(GameState_Play* play, int base, int index)
{
    const DungeonDef* def;
    DungeonData data;
    float x;
    float y;
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    OPEN_DISPS(play->gs.gfx);
    def = gDungeonDefs + base + index;
    x = -110.f;
    y = 54.f - 12 * index;
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
    printStr(play, def->name, x, y);
    if (def->flags & DD_MISC)
    {
        switch (def->id)
        {
        case 0:
            /* Town Fairy */
            color4(&r, &g, &b, &a, kFairyColors[4]);
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, r, g, b, a);
            drawTexRGBA32(play, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_FAIRY, 12, 12, x + 174.f, y);
            printNumColored(play, !!MM_GET_EVENT_WEEK(EV_MM_WEEK_TOWN_FAIRY), 1, 2, x + 186.f, y);
            break;
        case 1:
            /* OoT skulls */
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 0, 255);
            drawTexRGBA32(play, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_SKULL, 12, 12, x + 104.f, y);
            printNumColored(play, gOotSave.inventory.goldTokens, 100, 3, x + 116.f, y);

            /* MM skulls - swamp */
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 255, 0, 255);
            drawTexRGBA32(play, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_SKULL, 12, 12, x + 144.f, y);
            printNumColored(play, gMmSave.skullCountSwamp, 30, 2, x + 156.f, y);

            /* MM skulls - ocean */
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 0, 255, 255);
            drawTexRGBA32(play, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_SKULL, 12, 12, x + 184.f, y);
            printNumColored(play, gMmSave.skullCountOcean, 30, 2, x + 196.f, y);
            break;
        case 2:
            /* Triforce */
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
            drawTexRGBA32(play, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_TRIFORCE, 12, 12, x + 104.f, y);
            printNumColored(play, gOotExtraFlags.triforceCount, 30, 3, x + 116.f, y);
            break;
        }
    }
    else
    {
        if (def->flags & DD_MM)
            dungeonDataMm(&data, def);
        else
            dungeonDataOot(&data, def);
        if (def->flags & DD_MAP_COMPASS)
        {
            if (data.map)
                drawTexRGBA32(play, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_MAP, 12, 12, x + 104.f, y);
            if (data.compass)
                drawTexRGBA32(play, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_COMPASS, 12, 12, x + 116.f, y);
        }
        if (def->flags & DD_BOSS_KEY && data.bossKey)
        {
            drawTexRGBA32(play, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_BOSS_KEY, 12, 12, x + 128.f, y);
        }
        if (def->maxKeys)
        {
            drawTexRGBA32(play, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_KEY, 12, 12, x + 144.f, y);
            printNumColored(play, data.keys, def->maxKeys, 1, x + 156.f, y);
        }
        if (def->flags & DD_FAIRIES)
        {
            color4(&r, &g, &b, &a, kFairyColors[def->id]);
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, r, g, b, a);
            drawTexRGBA32(play, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_FAIRY, 12, 12, x + 174.f, y);
            printNumColored(play, data.fairies, 15, 2, x + 186.f, y);
        }
    }
    CLOSE_DISPS();
}

static int menuCursor;

void comboMenuInit(void)
{
    /* MQ Forest */
    if (gComboData.mq & (1 << MQ_TEMPLE_FOREST))
    {
        gDungeonDefs[3].maxKeys = 6;
    }

    /* MQ Fire / keysanity */
    if (gComboData.mq & (1 << MQ_TEMPLE_FIRE))
    {
        gDungeonDefs[4].maxKeys = 5;
    }
    else if (comboConfig(CFG_SMALL_KEY_SHUFFLE))
    {
        gDungeonDefs[4].maxKeys = 8;
    }

    /* MQ Water */
    if (gComboData.mq & (1 << MQ_TEMPLE_WATER))
    {
        gDungeonDefs[5].maxKeys = 2;
    }

    /* MQ Shadow */
    if (gComboData.mq & (1 << MQ_TEMPLE_SHADOW))
    {
        gDungeonDefs[6].maxKeys = 6;
    }

    /* MQ Spirit */
    if (gComboData.mq & (1 << MQ_TEMPLE_SPIRIT))
    {
        gDungeonDefs[7].maxKeys = 7;
    }

    /* MQ Well */
    if (gComboData.mq & (1 << MQ_BOTTOM_OF_THE_WELL))
    {
        gDungeonDefs[8].maxKeys = 2;
    }

    /* MQ GTG */
    if (gComboData.mq & (1 << MQ_GERUDO_TRAINING_GROUNDS))
    {
        gDungeonDefs[11].maxKeys = 3;
    }

    /* MQ Ganon */
    if (gComboData.mq & (1 << MQ_GANON_CASTLE))
    {
        gDungeonDefs[12].maxKeys = 3;
    }

    if (comboConfig(CFG_OOT_CARPENTERS_NONE))
    {
        gDungeonDefs[10].maxKeys = 0;
    }
    else if (comboConfig(CFG_OOT_CARPENTERS_ONE))
    {
        gDungeonDefs[10].maxKeys = 1;
    }

    if (comboConfig(CFG_OOT_NO_SMALL_KEY))
    {
        gDungeonDefs[3].maxKeys = 0;
        gDungeonDefs[4].maxKeys = 0;
        gDungeonDefs[5].maxKeys = 0;
        gDungeonDefs[6].maxKeys = 0;
        gDungeonDefs[7].maxKeys = 0;
        gDungeonDefs[8].maxKeys = 0;
        gDungeonDefs[11].maxKeys = 0;
        gDungeonDefs[12].maxKeys = 0;
    }

    if (comboConfig(CFG_MM_NO_SMALL_KEY))
    {
        gDungeonDefs[13].maxKeys = 0;
        gDungeonDefs[14].maxKeys = 0;
        gDungeonDefs[15].maxKeys = 0;
        gDungeonDefs[16].maxKeys = 0;
    }
}

void comboMenuKeysUpdate(GameState_Play* play)
{
    static int delay;
    int change;

    change = 0;
    if (delay)
    {
        delay--;
    }
    else
    {
        float stickY = play->gs.input[0].current.y / 128.f;
        if (stickY > 0.5f && menuCursor > 0)
        {
            change = 1;
            menuCursor--;
        }

        if (stickY < -0.5f && menuCursor < menuCount() - 10)
        {
            change = 1;
            menuCursor++;
        }

        if (change)
        {
            PlaySound(0x4809);
            delay = 3;
        }
    }
}

void comboMenuKeysDraw(GameState_Play* play)
{
    OPEN_DISPS(play->gs.gfx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 0, 0, 255);
    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    drawBackground(play, -110.f, 59.f, 217.f, 128.f);
    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gSPSegment(POLY_OPA_DISP++, 0x06, gCustomKeep);
    for (int i = 0; i < 10; ++i)
        printDungeonData(play, menuCursor, i);
    CLOSE_DISPS();
}

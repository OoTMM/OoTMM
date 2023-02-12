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
};

static void color4(u8* r, u8* g, u8* b, u8* a, u32 color)
{
    *r = (color >> 24) & 0xff;
    *g = (color >> 16) & 0xff;
    *b = (color >> 8) & 0xff;
    *a = color & 0xff;
}

static void printChar(GameState_Play* play, char c, float x, float y)
{
    OPEN_DISPS(play->gs.gfx);
    comboDrawBlit2D_IA4(&POLY_XLU_DISP, 0x06000000 | (CUSTOM_KEEP_FONT + (c - ' ') * 0x30), 8, 12, x, y, 1.f);
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
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, r, g, b, 255);
    CLOSE_DISPS();

    printNum(play, num, digits, x, y);
}

static void dungeonDataOot(DungeonData* out, const DungeonDef* def)
{
    out->keys = gOotSave.dungeonItems[def->id].maxKeys;
    out->fairies = 0;
    out->map = gOotSave.dungeonItems[def->id].map;
    out->compass = gOotSave.dungeonItems[def->id].compass;
    if (def->id == SCE_OOT_INSIDE_GANON_CASTLE)
        out->bossKey = gOotSave.dungeonItems[SCE_OOT_GANON_TOWER].bossKey;
    else
        out->bossKey = gOotSave.dungeonItems[def->id].bossKey;
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
    float y;
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    OPEN_DISPS(play->gs.gfx);
    def = gDungeonDefs + base + index;
    y = 66.f + 12 * index;
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 255);
    printStr(play, def->name, 46.f, y);
    if (def->flags & DD_MISC)
    {
        switch (def->id)
        {
        case 0:
            /* Town Fairy */
            color4(&r, &g, &b, &a, kFairyColors[4]);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, r, g, b, a);
            comboDrawBlit2D(&POLY_XLU_DISP, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_FAIRY, 12, 12, 220.f, y, 1.f);
            printNumColored(play, !!MM_GET_EVENT_WEEK(EV_MM_WEEK_TOWN_FAIRY), 1, 2, 232.f, y);
            break;
        case 1:
            /* OoT skulls */
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 0, 255);
            comboDrawBlit2D(&POLY_XLU_DISP, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_SKULL, 12, 12, 150.f, y, 1.f);
            printNumColored(play, gOotSave.goldTokens, 100, 3, 162.f, y);

            /* MM skulls - swamp */
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0, 255, 0, 255);
            comboDrawBlit2D(&POLY_XLU_DISP, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_SKULL, 12, 12, 190.f, y, 1.f);
            printNumColored(play, gMmSave.skullCountSwamp, 30, 2, 202.f, y);

            /* MM skulls - ocean */
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0, 0, 255, 255);
            comboDrawBlit2D(&POLY_XLU_DISP, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_SKULL, 12, 12, 230.f, y, 1.f);
            printNumColored(play, gMmSave.skullCountOcean, 30, 2, 242.f, y);
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
                comboDrawBlit2D(&POLY_XLU_DISP, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_MAP, 12, 12, 150.f, y, 1.f);
            if (data.compass)
                comboDrawBlit2D(&POLY_XLU_DISP, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_COMPASS, 12, 12, 162.f, y, 1.f);
        }
        if (def->flags & DD_BOSS_KEY && data.bossKey)
        {
            comboDrawBlit2D(&POLY_XLU_DISP, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_BOSS_KEY, 12, 12, 174.f, y, 1.f);
        }
        if (def->maxKeys)
        {
            comboDrawBlit2D(&POLY_XLU_DISP, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_KEY, 12, 12, 190.f, y, 1.f);
            printNumColored(play, data.keys, def->maxKeys, 1, 202.f, y);
        }
        if (def->flags & DD_FAIRIES)
        {
            color4(&r, &g, &b, &a, kFairyColors[def->id]);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, r, g, b, a);
            comboDrawBlit2D(&POLY_XLU_DISP, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_FAIRY, 12, 12, 220.f, y, 1.f);
            printNumColored(play, data.fairies, 15, 2, 232.f, y);
        }
    }
    CLOSE_DISPS();
}

void comboMenuKeys(GameState_Play* play)
{
    static int cursor;
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
        if (stickY > 0.5f && cursor > 0)
        {
            change = 1;
            cursor--;
        }

        if (stickY < -0.5f && cursor < ARRAY_SIZE(gDungeonDefs) - 10)
        {
            change = 1;
            cursor++;
        }

        if (change)
        {
            PlaySound(0x4809);
            delay = 6;
        }
    }

    /* Handle fire temple keys */
    if (comboConfig(CFG_SMALL_KEY_SHUFFLE))
    {
        gDungeonDefs[4].maxKeys = 8;
    }

    OPEN_DISPS(play->gs.gfx);
    gSPSegment(POLY_XLU_DISP++, 0x06, gCustomKeep);
    comboDrawInit2D(&POLY_XLU_DISP);
    for (int i = 0; i < 10; ++i)
        printDungeonData(play, cursor, i);
    CLOSE_DISPS();
}

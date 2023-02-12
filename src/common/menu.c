#include <combo.h>
#include <combo/custom.h>

#define DD_OOT          0x00
#define DD_MM           0x01
#define DD_MAP_COMPASS  0x02
#define DD_BOSS_KEY     0x04

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
    u8 map:1;
    u8 compass:1;
    u8 bossKey:1;
}
DungeonData;

static DungeonDef gDungeonDefs[] = {
    { "Deku",           SCE_OOT_INSIDE_DEKU_TREE,       DD_OOT | DD_MAP_COMPASS, 0 },
    { "Dodongo",        SCE_OOT_DODONGO_CAVERN,         DD_OOT | DD_MAP_COMPASS, 0 },
    { "Jabu",           SCE_OOT_INSIDE_JABU_JABU,       DD_OOT | DD_MAP_COMPASS, 0 },
    { "Forest",         SCE_OOT_TEMPLE_FOREST,          DD_OOT | DD_MAP_COMPASS | DD_BOSS_KEY, 5 },
    { "Fire",           SCE_OOT_TEMPLE_FIRE,            DD_OOT | DD_MAP_COMPASS | DD_BOSS_KEY, 8 },
    { "Water",          SCE_OOT_TEMPLE_WATER,           DD_OOT | DD_MAP_COMPASS | DD_BOSS_KEY, 5 },
    { "Shadow",         SCE_OOT_TEMPLE_SHADOW,          DD_OOT | DD_MAP_COMPASS | DD_BOSS_KEY, 5 },
    { "Spirit",         SCE_OOT_TEMPLE_SPIRIT,          DD_OOT | DD_MAP_COMPASS | DD_BOSS_KEY, 5 },
    { "Well",           SCE_OOT_BOTTOM_OF_THE_WELL,     DD_OOT | DD_MAP_COMPASS, 3 },
    { "Ice",            SCE_OOT_ICE_CAVERN,             DD_OOT | DD_MAP_COMPASS, 0 },
    { "Hideout",        SCE_OOT_THIEVES_HIDEOUT,        DD_OOT, 4 },
    { "GTG",            SCE_OOT_GERUDO_TRAINING_GROUND, DD_OOT, 9 },
    { "Ganon",          SCE_OOT_INSIDE_GANON_CASTLE,    DD_OOT | DD_MAP_COMPASS | DD_BOSS_KEY, 2 },
    { "Woodfall",       0,                              DD_MM  | DD_MAP_COMPASS | DD_BOSS_KEY,  0 },
    { "Snowhead",       1,                              DD_MM  | DD_MAP_COMPASS | DD_BOSS_KEY,  0 },
    { "Great Bay",      2,                              DD_MM  | DD_MAP_COMPASS | DD_BOSS_KEY,  0 },
    { "Stone Tower",    3,                              DD_MM  | DD_MAP_COMPASS | DD_BOSS_KEY,  0 },
};

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

static void dungeonDataOot(DungeonData* out, const DungeonDef* def)
{
    out->keys = gOotSave.dungeonItems[def->id].maxKeys;
    if (def->id == SCE_OOT_INSIDE_GANON_CASTLE)
        out->bossKey = gOotSave.dungeonItems[SCE_OOT_GANON_TOWER].bossKey;
    else
        out->bossKey = gOotSave.dungeonItems[def->id].bossKey;
}

static void dungeonDataMm(DungeonData* out, const DungeonDef* def)
{
    out->keys = 0;
}

static void printDungeonData(GameState_Play* play, int base, int index)
{
    const DungeonDef* def;
    DungeonData data;
    float y;

    OPEN_DISPS(play->gs.gfx);
    def = gDungeonDefs + base + index;
    if (def->flags & DD_MM)
        dungeonDataMm(&data, def);
    else
        dungeonDataOot(&data, def);
    y = 66.f + 12 * index;
    printStr(play, def->name, 46.f, y);
    if (def->flags & DD_MAP_COMPASS)
    {
        comboDrawBlit2D(&POLY_XLU_DISP, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_MAP, 12, 12, 150.f, y, 1.f);
        comboDrawBlit2D(&POLY_XLU_DISP, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_COMPASS, 12, 12, 162.f, y, 1.f);
    }
    if (def->flags & DD_BOSS_KEY)
    {
        comboDrawBlit2D(&POLY_XLU_DISP, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_BOSS_KEY, 12, 12, 174.f, y, 1.f);
    }
    if (def->maxKeys)
    {
        comboDrawBlit2D(&POLY_XLU_DISP, 0x06000000 | CUSTOM_KEEP_SMALL_ICON_KEY, 12, 12, 190.f, y, 1.f);
        printDigit(play, data.keys, 202.f, y);
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

    OPEN_DISPS(play->gs.gfx);
    gSPSegment(POLY_XLU_DISP++, 0x06, gCustomKeep);
    comboDrawInit2D(&POLY_XLU_DISP);
    for (int i = 0; i < 10; ++i)
        printDungeonData(play, cursor, i);
    CLOSE_DISPS();
}

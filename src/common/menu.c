#include <combo.h>
#include <combo/custom.h>

#define DD_OOT  0
#define DD_MM   1

typedef struct
{
    const char* name;
    int id;
    int flags;
    int maxKeys;
}
DungeonData;

static DungeonData gDungeonData[] = {
    { "Deku",       SCE_OOT_INSIDE_DEKU_TREE,       DD_OOT, 0 },
    { "Dodongo",    SCE_OOT_DODONGO_CAVERN,         DD_OOT, 0 },
    { "Jabu",       SCE_OOT_INSIDE_JABU_JABU,       DD_OOT, 0 },
    { "Forest",     SCE_OOT_TEMPLE_FOREST,          DD_OOT, 0 },
    { "Fire",       SCE_OOT_TEMPLE_FIRE,            DD_OOT, 0 },
    { "Water",      SCE_OOT_TEMPLE_WATER,           DD_OOT, 0 },
    { "Shadow",     SCE_OOT_TEMPLE_SHADOW,          DD_OOT, 0 },
    { "Spirit",     SCE_OOT_TEMPLE_SPIRIT,          DD_OOT, 0 },
    { "Well",       SCE_OOT_BOTTOM_OF_THE_WELL,     DD_OOT, 0 },
    { "Ice",        SCE_OOT_ICE_CAVERN,             DD_OOT, 0 },
    { "Hideout",    SCE_OOT_THIEVES_HIDEOUT,        DD_OOT, 0 },
    { "GTG",        SCE_OOT_GERUDO_TRAINING_GROUND, DD_OOT, 0 },
    { "Ganon",      SCE_OOT_INSIDE_GANON_CASTLE,    DD_OOT, 0 },
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

static void printDungeonData(GameState_Play* play, int index)
{
    const DungeonData* data;
    float y;

    data = gDungeonData + index;
    y = 66.f + 12 * index;
    printStr(play, data->name, 46.f, y);
}

void comboMenuKeys(GameState_Play* play)
{
    OPEN_DISPS(play->gs.gfx);
    gSPSegment(POLY_XLU_DISP++, 0x06, gCustomKeep);
    comboDrawInit2D(&POLY_XLU_DISP);
    for (int i = 0; i < 10; ++i)
        printDungeonData(play, i);
    CLOSE_DISPS();
}

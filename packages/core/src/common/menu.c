#include <combo.h>
#include <combo/custom.h>
#include <combo/dungeon.h>
#include <combo/menu.h>
#include <combo/sr.h>
#include <combo/souls.h>

#if defined(GAME_OOT)
# define SEG1        0x0a
# define SEG2        0x0b
#else
# define SEG1        0x09
# define SEG2        0x0a
#endif

#define SEG1_BASE   (SEG1 << 24)
#define SEG2_BASE   (SEG2 << 24)

#define LINES 9

#define DD_OOT          0x00
#define DD_MM           0x01
#define DD_MISC         0x02
#define DD_MAP_COMPASS  0x04
#define DD_BOSS_KEY     0x08
#define DD_FAIRIES      0x10

#define CK_PTR(addr)    ((const char*)gCustomKeep + (addr))

typedef struct
{
    const char* name;
    int id;
    int dungeonId;
    int flags;
}
DungeonDef;

typedef struct
{
    s8 keys;
    u8 fairies;
    u8 maxKeys;
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
    { "Deku",           SCE_OOT_INSIDE_DEKU_TREE,       DUNGEONID_DEKU_TREE, DD_OOT | DD_MAP_COMPASS },
    { "Dodongo",        SCE_OOT_DODONGO_CAVERN,         DUNGEONID_DODONGOS_CAVERN, DD_OOT | DD_MAP_COMPASS },
    { "Jabu",           SCE_OOT_INSIDE_JABU_JABU,       DUNGEONID_JABU_JABU, DD_OOT | DD_MAP_COMPASS },
    { "Forest",         SCE_OOT_TEMPLE_FOREST,          DUNGEONID_TEMPLE_FOREST, DD_OOT | DD_MAP_COMPASS | DD_BOSS_KEY },
    { "Fire",           SCE_OOT_TEMPLE_FIRE,            DUNGEONID_TEMPLE_FIRE, DD_OOT | DD_MAP_COMPASS | DD_BOSS_KEY },
    { "Water",          SCE_OOT_TEMPLE_WATER,           DUNGEONID_TEMPLE_WATER, DD_OOT | DD_MAP_COMPASS | DD_BOSS_KEY },
    { "Shadow",         SCE_OOT_TEMPLE_SHADOW,          DUNGEONID_TEMPLE_SHADOW, DD_OOT | DD_MAP_COMPASS | DD_BOSS_KEY },
    { "Spirit",         SCE_OOT_TEMPLE_SPIRIT,          DUNGEONID_TEMPLE_SPIRIT, DD_OOT | DD_MAP_COMPASS | DD_BOSS_KEY },
    { "Well",           SCE_OOT_BOTTOM_OF_THE_WELL,     DUNGEONID_BOTTOM_OF_THE_WELL, DD_OOT | DD_MAP_COMPASS },
    { "Ice",            SCE_OOT_ICE_CAVERN,             DUNGEONID_ICE_CAVERN, DD_OOT | DD_MAP_COMPASS },
    { "Hideout",        SCE_OOT_THIEVES_HIDEOUT,        -1, DD_OOT },
    { "GTG",            SCE_OOT_GERUDO_TRAINING_GROUND, DUNGEONID_GERUDO_TRAINING_GROUNDS, DD_OOT },
    { "Ganon",          SCE_OOT_INSIDE_GANON_CASTLE,    DUNGEONID_GANON_CASTLE, DD_OOT | DD_BOSS_KEY },
    { "Chest Game",     SCE_OOT_TREASURE_SHOP,          -1, DD_OOT },
    { "Woodfall",       0,                              DUNGEONID_TEMPLE_WOODFALL, DD_MM  | DD_MAP_COMPASS | DD_BOSS_KEY | DD_FAIRIES },
    { "Snowhead",       1,                              DUNGEONID_TEMPLE_SNOWHEAD, DD_MM  | DD_MAP_COMPASS | DD_BOSS_KEY | DD_FAIRIES },
    { "Great Bay",      2,                              DUNGEONID_TEMPLE_GREAT_BAY, DD_MM  | DD_MAP_COMPASS | DD_BOSS_KEY | DD_FAIRIES },
    { "Stone Tower",    3,                              DUNGEONID_TEMPLE_STONE_TOWER, DD_MM  | DD_MAP_COMPASS | DD_BOSS_KEY | DD_FAIRIES },
    { "Clock Town",     0,                              -1, DD_MISC },
    { "Tokens",         1,                              -1, DD_MISC },
    {},
    {},
    {},
    {},
    {},
};

static int gDungeonDefCount = 20;

void menuInit()
{
    DungeonDef* d;

    d = gDungeonDefs + gDungeonDefCount;
    if (comboConfig(CFG_GOAL_TRIFORCE) || comboConfig(CFG_GOAL_TRIFORCE3))
    {
        d->name = "Triforce";
        d->id = 2;
        d->dungeonId = -1;
        d->flags = DD_MISC;
        gDungeonDefCount++;
        d++;
    }

    if (gComboData.maxCoins[0])
    {
        d->name = "Red Coins";
        d->id = 3;
        d->dungeonId = -1;
        d->flags = DD_MISC;
        gDungeonDefCount++;
        d++;
    }

    if (gComboData.maxCoins[1])
    {
        d->name = "Green Coins";
        d->id = 4;
        d->dungeonId = -1;
        d->flags = DD_MISC;
        gDungeonDefCount++;
        d++;
    }

    if (gComboData.maxCoins[2])
    {
        d->name = "Blue Coins";
        d->id = 5;
        d->dungeonId = -1;
        d->flags = DD_MISC;
        gDungeonDefCount++;
        d++;
    }

    if (gComboData.maxCoins[3])
    {
        d->name = "Yellow Coins";
        d->id = 6;
        d->dungeonId = -1;
        d->flags = DD_MISC;
        gDungeonDefCount++;
        d++;
    }
}

static const char* const kSoulsEnemyOot[] = {
    "Stalfos",
    "Octoroks",
    "Wallmasters",
    "Dodongos",
    "Keeses",
    "Tektites",
    "Leevers",
    "Peahats",
    "Lizalfos/Dinalfos",
    "Gohma Larvae",
    "Shaboms",
    "Baby Dodongos",
    "Dark Link",
    "Biris/Baris",
    "Tailpasarans",
    "Parasites",
    "Skulltulas",
    "Torch Slugs",
    "Moblins",
    "Armos",
    "Deku Babas",
    "Deku Scrubs",
    "Bubbles",
    "Beamos",
    "Floormasters",
    "ReDeads/Gibdos",
    "Skullwalltulas",
    "Flare Dancers",
    "Dead Hands",
    "Shell Blades",
    "Like-Likes",
    "Spikes",
    "Anubis",
    "Iron Knuckles",
    "Skull Kids",
    "Flying Pots",
    "Freezards",
    "Stingers",
    "Wolfos",
    "Stalchildren",
    "Guays",
};

static const char* const kSoulsBossOot[] = {
    "Queen Gohma",
    "King Dodongo",
    "Barinade",
    "Phantom Ganon",
    "Volvagia",
    "Morpha",
    "Bongo-Bongo",
    "Twinrova",
};

static const char* const kSoulsNpcOot[] = {
    "Saria",
    "Darunia",
    "Ruto",
    "King Zora",
    "Zelda",
    "Sheik",
    "Cucco Lady",
    "Carpenters",
    "Guru-Guru",
    "Mido",
    "Kokiri",
    "Kokiri Shopkeeper",
    "Hylian Guard",
    "Hylian Citizen",
    "Malon",
    "Talon",
    "Fishing Pond Owner",
    "Goron",
    "Medigoron",
    "Biggoron",
    "Child Goron",
    "Goron Shopkeeper",
    "Zora",
    "Zora Shopkeeper",
    "Bazaar Shopkeeper",
    "Bombchu Shopkeeper",
    "Potion Shopkeeper",
    "Bombchu Bowling Lady",
    "Treasure Chest Game Owner",
    "Shooting Gallery Owner",
    "Dampe",
    "Graveyard Kid",
    "Poe Collector",
};

static const char* const kSoulsEnemyMm[] = {
    "Octoroks",
    "Wallmasters",
    "Dodongos",
    "Keeses",
    "Tektites",
    "Peahats",
    "Lizalfos/Dinalfos",
    "Skulltulas",
    "Armos",
    "Deku Babas",
    "Deku Scrubs",
    "Bubbles",
    "Beamos",
    "ReDeads/Gibdos",
    "Skullwalltulas",
    "Shell Blades",
    "Like-Likes",
    "Iron Knuckles",
    "Freezards",
    "Wolfos",
    "Guays",
    "Flying Pots",
    "Floormasters",
    "Chuchus",
    "Deep Pythons",
    "Stalchildren",
    "Leevers",
    "Skullfish",
    "Dexihands",
    "Dragonflies",
    "Eenos",
    "Eyegores",
    "Hiploops",
    "Real Bombchu",
    "Takkuri",
    "Boes",
    "Nejirons",
    "Bio Babas",
    "Garo",
    "Wizzrobes",
    "Gomess",
    "Gekkos",
    "Bad Bats",
    "Snappers",
    "Warts",
    "Captain Keeta",
};

static const char* const kSoulsBossMm[] = {
    "Odolwa",
    "Goht",
    "Gyorg",
    "Twinmold",
    "Igos",
};

static const Gfx kDlistQuadRGBA32_12x12[] = {
    gsDPPipeSync(),
    gsSPVertex(SEG1_BASE, 4, 0),
    gsDPTileSync(),
    gsDPLoadTextureTile(
        SEG2_BASE,
        G_IM_FMT_RGBA,
        G_IM_SIZ_32b,
        12, 12,
        0, 0,
        11, 11,
        0,
        G_TX_WRAP, G_TX_WRAP,
        G_TX_NOMASK, G_TX_NOMASK,
        G_TX_NOLOD, G_TX_NOLOD
    ),
    gsDPTileSync(),
    gsSP2Triangles(
        0, 2, 1, 0,
        0, 3, 2, 0
    ),
    gsSPEndDisplayList(),
};

static const Gfx kDlistQuadIA4_8x12[] = {
    gsDPPipeSync(),
    gsSPVertex(SEG1_BASE, 4, 0),
    gsDPTileSync(),
    gsDPLoadTextureTile_4b(
        SEG2_BASE,
        G_IM_FMT_IA,
        8, 12,
        0, 0,
        7, 11,
        0,
        G_TX_WRAP, G_TX_WRAP,
        G_TX_NOMASK, G_TX_NOMASK,
        G_TX_NOLOD, G_TX_NOLOD
    ),
    gsDPTileSync(),
    gsSP2Triangles(
        0, 2, 1, 0,
        0, 3, 2, 0
    ),
    gsSPEndDisplayList(),
};

static int digitCount(int v)
{
    if (v >= 100)
        return 3;
    if (v >= 10)
        return 2;
    return 1;
}

static void color4(u8* r, u8* g, u8* b, u8* a, u32 color)
{
    *r = (color >> 24) & 0xff;
    *g = (color >> 16) & 0xff;
    *b = (color >> 8) & 0xff;
    *a = color & 0xff;
}

Vtx gVtxBuffer[2048];
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

static void drawTexIA4_8x12(GameState_Play* play, const void* texPtr, float x, float y)
{
    Vtx* v;
    int xx[4];
    int yy[4];
    int txx[4];
    int tyy[4];

    v = vtxAlloc(play, 4);

    xx[0] = x;
    xx[1] = x + 8;
    xx[2] = x + 8;
    xx[3] = x;

    yy[0] = y;
    yy[1] = y;
    yy[2] = y - 12;
    yy[3] = y - 12;

    txx[0] = 0;
    txx[1] = 8 * 32;
    txx[2] = 8 * 32;
    txx[3] = 0;

    tyy[0] = 0;
    tyy[1] = 0;
    tyy[2] = 12 * 32;
    tyy[3] = 12 * 32;

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
    gSPSegment(POLY_OPA_DISP++, SEG1, v);
    gSPSegment(POLY_OPA_DISP++, SEG2, texPtr);
    gSPDisplayList(POLY_OPA_DISP++, (u32)kDlistQuadIA4_8x12 & 0xffffff);
    CLOSE_DISPS();
}

static void drawTexRGBA32_12x12(GameState_Play* play, const void* texPtr, float x, float y)
{
    Vtx* v;
    int xx[4];
    int yy[4];
    int txx[4];
    int tyy[4];

    v = vtxAlloc(play, 4);

    xx[0] = x;
    xx[1] = x + 12;
    xx[2] = x + 12;
    xx[3] = x;

    yy[0] = y;
    yy[1] = y;
    yy[2] = y - 12;
    yy[3] = y - 12;

    txx[0] = 0;
    txx[1] = 12 * 32;
    txx[2] = 12 * 32;
    txx[3] = 0;

    tyy[0] = 0;
    tyy[1] = 0;
    tyy[2] = 12 * 32;
    tyy[3] = 12 * 32;

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
    gSPSegment(POLY_OPA_DISP++, SEG1, v);
    gSPSegment(POLY_OPA_DISP++, SEG2, texPtr);
    gSPDisplayList(POLY_OPA_DISP++, (u32)kDlistQuadRGBA32_12x12 & 0xffffff);
    CLOSE_DISPS();
}

static void printChar(GameState_Play* play, char c, float x, float y)
{
    drawTexIA4_8x12(play, CK_PTR(CUSTOM_KEEP_FONT + (c - ' ') * 0x30), x, y);
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

static void printNum(GameState_Play* play, int num, int max, int digits, float x, float y, int showMax)
{
    int d;
    int denum;
    int zero;

    if (num == 0)
    {
        x += 8.f * (digits - 1);
        printDigit(play, 0, x, y);
        x += 8.f;
    }
    else
    {
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

    if (showMax)
    {
        printChar(play, '/', x, y);
        x += 8.f;
        printNum(play, max, max, digits, x, y, 0);
    }
}

static void printNumColored(GameState_Play* play, int num, int max, int digits, float x, float y, int showMax)
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

    printNum(play, num, max, digits, x, y, showMax);
}

static void dungeonDataOot(DungeonData* out, const DungeonDef* def)
{
    out->maxKeys = g.maxKeysOot[def->id];
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
    out->maxKeys = g.maxKeysMm[def->id];
    out->keys = gMmSave.inventory.dungeonItems[def->id].maxKeys;
    out->bossKey = gMmSave.inventory.dungeonItems[def->id].bossKey;
    out->fairies = gMmSave.inventory.strayFairies[def->id];
    out->map = gMmSave.inventory.dungeonItems[def->id].map;
    out->compass = gMmSave.inventory.dungeonItems[def->id].compass;
}

static void printDungeonSilverRupees(GameState_Play* play, float x, float y, int srBase, int srCount)
{
    const ComboSilverRupeeData* data;
    int sr;
    int digits;
    int count;

    if (gSilverRupeeData[srBase].count == 0)
        return;

    OPEN_DISPS(play->gs.gfx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
    drawTexRGBA32_12x12(play, CK_PTR(CUSTOM_KEEP_SMALL_ICON_RUPEE), x, y);
    x += 12.f;

    for (int i = 0; i < srCount; ++i)
    {
        sr = srBase + i;
        data = gSilverRupeeData + sr;
        if (data->count == 0)
            continue;
        digits = digitCount(data->count);
        count = comboSilverRupeesGetCount(sr);
        printNumColored(play, count, data->count, digits, x, y, 0);
        x += 8.f * digits + 3.f;
    }
    CLOSE_DISPS();
}

static void printSoul(GameState_Play* play, const char* const* names, int soulBase, int base, int index, int mm)
{
    const char* name;
    float x;
    float y;
    int id;
    int hasSoul;

    id = base + index;
    hasSoul = mm ? comboHasSoulMm(soulBase + id) : comboHasSoulOot(soulBase + id);
    name = names[id];
    x = -110.f;
    y = 42.f - 12 * index;

    OPEN_DISPS(play->gs.gfx);
    if (hasSoul)
    {
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
    }
    else
    {
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 70, 70, 70, 255);
    }
    printStr(play, name, x, y);
    CLOSE_DISPS();
}

static void printDungeonData(GameState_Play* play, int base, int index)
{
    const DungeonDef* def;
    DungeonData data;
    int triforceMax;
    int triforceDigits;
    int srBase;
    int srCount;
    float x;
    float y;
    float offX;
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    triforceMax = comboConfig(CFG_GOAL_TRIFORCE3) ? 3 : (gOotExtraFlags.triforceWin ? gComboData.triforcePieces : gComboData.triforceGoal);
    triforceDigits = digitCount(triforceMax);

    offX = 0.f;
    if (comboConfig(CFG_OOT_SILVER_RUPEE_SHUFFLE) && ((base + index) < 13))
        offX = -30.f;

    OPEN_DISPS(play->gs.gfx);
    def = gDungeonDefs + base + index;

    switch (base + index)
    {
    case 1:
        srBase = SR_DC;
        srCount = 1;
        break;
    case 6:
        srBase = SR_SHADOW1;
        srCount = 4;
        break;
    case 7:
        srBase = SR_SPIRIT1;
        srCount = 3;
        break;
    case 8:
        srBase = SR_BOTW;
        srCount = 1;
        break;
    case 9:
        srBase = SR_IC1;
        srCount = 2;
        break;
    case 11:
        srBase = SR_GTG1;
        srCount = 3;
        break;
    case 12:
        srBase = SR_GANON1;
        srCount = 4;
        break;
    default:
        srBase = -1;
        srCount = 0;
        break;
    }

    x = -110.f;
    y = 42.f - 12 * index;
    if (def->dungeonId != -1 && gComboData.preCompleted & (1 << def->dungeonId))
    {
        r = 0x3f;
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, r, r, r, 255);
        printStr(play, def->name, x, y);
        r = 0xff;
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, r, r, r, 255);
    }
    else
    {
        r = 0xff;
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, r, r, r, 255);
        printStr(play, def->name, x, y);
    }
    x += offX;
    if (def->flags & DD_MISC)
    {
        switch (def->id)
        {
        case 0:
            /* Town Fairy */
            color4(&r, &g, &b, &a, kFairyColors[4]);
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, r, g, b, a);
            drawTexRGBA32_12x12(play, CK_PTR(CUSTOM_KEEP_SMALL_ICON_FAIRY), x + 174.f, y);
            printNumColored(play, !!MM_GET_EVENT_WEEK(EV_MM_WEEK_TOWN_FAIRY), 1, 2, x + 186.f, y, 0);
            break;
        case 1:
            /* OoT skulls */
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 0, 255);
            drawTexRGBA32_12x12(play, CK_PTR(CUSTOM_KEEP_SMALL_ICON_SKULL), x + 104.f, y);
            printNumColored(play, gOotSave.inventory.goldTokens, 100, 3, x + 116.f, y, 0);

            /* MM skulls - swamp */
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 255, 0, 255);
            drawTexRGBA32_12x12(play, CK_PTR(CUSTOM_KEEP_SMALL_ICON_SKULL), x + 144.f, y);
            printNumColored(play, gMmSave.skullCountSwamp, 30, 2, x + 156.f, y, 0);

            /* MM skulls - ocean */
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 0, 255, 255);
            drawTexRGBA32_12x12(play, CK_PTR(CUSTOM_KEEP_SMALL_ICON_SKULL), x + 184.f, y);
            printNumColored(play, gMmSave.skullCountOcean, 30, 2, x + 196.f, y, 0);
            break;
        case 2:
            /* Triforce */
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
            drawTexRGBA32_12x12(play, CK_PTR(CUSTOM_KEEP_SMALL_ICON_TRIFORCE), x + 104.f, y);
            printNumColored(play, gTriforceCount, triforceMax, triforceDigits, x + 116.f, y, 1);
            break;
        case 3:
            /* Red Coins */
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 0, 0, 255);
            drawTexRGBA32_12x12(play, CK_PTR(CUSTOM_KEEP_SMALL_ICON_COIN), x + 104.f, y);
            printNumColored(play, gSharedCustomSave.coins[0], gComboData.maxCoins[0], digitCount(gComboData.maxCoins[0]), x + 116.f, y, 1);
            break;
        case 4:
            /* Green Coins */
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 255, 0, 255);
            drawTexRGBA32_12x12(play, CK_PTR(CUSTOM_KEEP_SMALL_ICON_COIN), x + 104.f, y);
            printNumColored(play, gSharedCustomSave.coins[1], gComboData.maxCoins[1], digitCount(gComboData.maxCoins[1]), x + 116.f, y, 1);
            break;
        case 5:
            /* Blue Coins */
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 0, 255, 255);
            drawTexRGBA32_12x12(play, CK_PTR(CUSTOM_KEEP_SMALL_ICON_COIN), x + 104.f, y);
            printNumColored(play, gSharedCustomSave.coins[2], gComboData.maxCoins[2], digitCount(gComboData.maxCoins[2]), x + 116.f, y, 1);
            break;
        case 6:
            /* Yellow Coins */
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 0, 255);
            drawTexRGBA32_12x12(play, CK_PTR(CUSTOM_KEEP_SMALL_ICON_COIN), x + 104.f, y);
            printNumColored(play, gSharedCustomSave.coins[3], gComboData.maxCoins[3], digitCount(gComboData.maxCoins[3]), x + 116.f, y, 1);
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
                drawTexRGBA32_12x12(play, CK_PTR(CUSTOM_KEEP_SMALL_ICON_MAP), x + 104.f, y);
            if (data.compass)
                drawTexRGBA32_12x12(play, CK_PTR(CUSTOM_KEEP_SMALL_ICON_COMPASS), x + 116.f, y);
        }
        if ((def->flags & DD_BOSS_KEY) && data.bossKey)
        {
            drawTexRGBA32_12x12(play, CK_PTR(CUSTOM_KEEP_SMALL_ICON_BOSS_KEY), x + 128.f, y);
        }
        if (data.maxKeys)
        {
            drawTexRGBA32_12x12(play, CK_PTR(CUSTOM_KEEP_SMALL_ICON_KEY), x + 144.f, y);
            printNumColored(play, data.keys, data.maxKeys, 1, x + 156.f, y, 0);
        }
        if (def->flags & DD_FAIRIES)
        {
            color4(&r, &g, &b, &a, kFairyColors[def->id]);
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, r, g, b, a);
            drawTexRGBA32_12x12(play, CK_PTR(CUSTOM_KEEP_SMALL_ICON_FAIRY), x + 174.f, y);
            printNumColored(play, data.fairies, 15, 2, x + 186.f, y, 0);
        }

        if (comboConfig(CFG_OOT_SILVER_RUPEE_SHUFFLE))
            printDungeonSilverRupees(play, x + 170.f, y, srBase, srCount);
    }
    CLOSE_DISPS();
}

static void updateCursor(GameState_Play* play)
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
        if (stickY > 0.5f && g.menuCursor > 0)
        {
            change = 1;
            g.menuCursor--;
        }

        if (stickY < -0.5f && g.menuCursor < g.menuCursorMax - LINES)
        {
            change = 1;
            g.menuCursor++;
        }

        if (change)
        {
            PlaySound(0x4809);
            delay = 3;
        }
    }
}

void comboMenuUpdate(GameState_Play* play)
{
    switch (g.menuScreen)
    {
    case MENU_INFO:
        g.menuCursorMax = gDungeonDefCount;
        break;
    case MENU_SOULS_OOT_ENEMY:
        g.menuCursorMax = ARRAY_SIZE(kSoulsEnemyOot);
        break;
    case MENU_SOULS_OOT_BOSS:
        g.menuCursorMax = ARRAY_SIZE(kSoulsBossOot);
        break;
    case MENU_SOULS_OOT_NPC:
        g.menuCursorMax = ARRAY_SIZE(kSoulsNpcOot);
        break;
    case MENU_SOULS_MM_ENEMY:
        g.menuCursorMax = ARRAY_SIZE(kSoulsEnemyMm);
        break;
    case MENU_SOULS_MM_BOSS:
        g.menuCursorMax = ARRAY_SIZE(kSoulsBossMm);
        break;
    }

    updateCursor(play);
}

static int min(int a, int b)
{
    return a < b ? a : b;
}

static void drawMenuSouls(GameState_Play* play, const char* title, const char* const* names, int soulBase, int mm)
{
    OPEN_DISPS(play->gs.gfx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 0, 255);
    printStr(play, title, -110.f, 54.f);
    for (int i = 0; i < min(LINES, g.menuCursorMax); ++i)
        printSoul(play, names, soulBase, g.menuCursor, i, mm);
    CLOSE_DISPS();
}

static void drawMenuInfo(GameState_Play* play)
{
    OPEN_DISPS(play->gs.gfx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 0, 255);
    printStr(play, "Information", -110.f, 54.f);
    for (int i = 0; i < min(LINES, g.menuCursorMax); ++i)
        printDungeonData(play, g.menuCursor, i);
    CLOSE_DISPS();
}

void comboMenuDraw(GameState_Play* play)
{
    /* Draw the black background */
    OPEN_DISPS(play->gs.gfx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 0, 0, 255);
    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    drawBackground(play, -110.f, 59.f, 217.f, 128.f);
    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gSPSegment(POLY_OPA_DISP++, 0x06, gCustomKeep);
    CLOSE_DISPS();

    switch (g.menuScreen)
    {
    case MENU_INFO:
        drawMenuInfo(play);
        break;
    case MENU_SOULS_OOT_ENEMY:
        drawMenuSouls(play, "OoT Enemy Souls", kSoulsEnemyOot, GI_OOT_SOUL_ENEMY_STALFOS, 0);
        break;
    case MENU_SOULS_OOT_BOSS:
        drawMenuSouls(play, "OoT Boss Souls", kSoulsBossOot, GI_OOT_SOUL_BOSS_QUEEN_GOHMA, 0);
        break;
    case MENU_SOULS_OOT_NPC:
        drawMenuSouls(play, "OoT NPC Souls", kSoulsNpcOot, GI_OOT_SOUL_NPC_SARIA, 0);
        break;
    case MENU_SOULS_MM_ENEMY:
        drawMenuSouls(play, "MM Enemy Souls", kSoulsEnemyMm, GI_MM_SOUL_ENEMY_OCTOROK, 1);
        break;
    case MENU_SOULS_MM_BOSS:
        drawMenuSouls(play, "MM Boss Souls", kSoulsBossMm, GI_MM_SOUL_BOSS_ODOLWA, 1);
        break;
    }
}

void comboMenuNext(void)
{
    if(comboConfig(CFG_MENU_NOTEBOOK) && !gMmSave.inventory.quest.notebook) {
        PlaySound(0x4806);
        return;
    }
    PlaySound(0x4809);
    g.menuScreen++;
    g.menuCursor = 0;
    g.menuCursorMax = 0;

    if (g.menuScreen == MENU_SOULS_OOT_ENEMY && !comboConfig(CFG_OOT_SOULS_ENEMY))
        g.menuScreen++;
    if (g.menuScreen == MENU_SOULS_OOT_BOSS && !comboConfig(CFG_OOT_SOULS_BOSS))
        g.menuScreen++;
    if (g.menuScreen == MENU_SOULS_OOT_NPC && !comboConfig(CFG_OOT_SOULS_NPC))
        g.menuScreen++;
    if (g.menuScreen == MENU_SOULS_MM_ENEMY && !comboConfig(CFG_MM_SOULS_ENEMY))
        g.menuScreen++;
    if (g.menuScreen == MENU_SOULS_MM_BOSS && !comboConfig(CFG_MM_SOULS_BOSS))
        g.menuScreen++;

    if (g.menuScreen >= MENU_MAX)
        g.menuScreen = MENU_NONE;
}

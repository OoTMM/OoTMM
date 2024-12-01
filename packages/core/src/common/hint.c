#include <combo.h>
#include <combo/dma.h>
#include <combo/item.h>
#include <combo/config.h>
#include <combo/hint.h>

#if defined(GAME_OOT)
# define HINTS_ADDR 0x03ff1000
#else
# define HINTS_ADDR 0x03ff2000
#endif

typedef struct PACKED ALIGNED(2)
{
    u8 key;
    u8 type;
    u8 region;
    u8 world;
    s16 items[3];
    u8  players[3];
    s8  itemImportances[3];
}
Hint;

static const char* kJunkHints[] = {
    "THEW ORLD ISSQ UARE",                                       /* Final Fantasy 6 */
    "SWITCH targeting is the superior option",                   /* Blatant truth */
    "getting 100 coins gives you a star",                        /* Mario 64 */
    "the main character is not actually called Zelda",           /* ... */
    "nothing is forever",                                        /* Useful life advice */
    "the yes needs the no, to win against the no",               /* French politics */
    "a winner is you",                                           /* Meme */
    "your princess is in another castle",                        /* Mario 1 */
    "it's a secret to everybody",                                /* Zelda 1 */
    "Ocarina of Time is the better game",                        /* OoTMM */
    "Majora's Mask is the better game",                          /* OoTMM */
    "the cake is a lie",                                         /* Portal */
    "I am Error",                                                /* Zelda 2 */
    "plundering Area 51 is a foolish choice",                    /* Useful life advice */
    "there are 118 known elements",                              /* Chemistry */
    "Nax's House is on the Way of the Hero",                     /* OoTMM */
    "this is not the hint you are looking for",                  /* OoTMM */
    "you can find Luigi in Super Mario 64",                      /* Mario 64 */
    "using the Fishing Pole in Twilight Princess is WoTH",       /* Twilight Princess */
    "23 is number 1",                                            /* Deku Tree */
    "keys are always in the last place you look in",             /* Facts */
    "a famous code is UUDD LRLR BAS",                            /* Konami Code */
    "there's only 151 Pokemons",                                 /* Pokemon Gen 1 */
    "you can unlock Sonic and Tails in Super Smash Bros Melee",  /* SSBM */
    "one should not forget to check the chests in Mido's House", /* OoTMM */
    "Gossip Stones tell lies",                                   /* OoTMM */
    "it's dangerous to go alone!",                               /* Zelda 2 */
    "they're never gonna give you up, never gonna let you down", /* Rickroll */
    "your seed got 10 percents better",                          /* DK64R inspired */
    "your seed got 10 percents worse",                           /* DK64R inspired */
};

static Hint* gHints;

static const Hint* findHint(u8 key)
{
    int i;

    i = 0;
    for (;;)
    {
        const Hint* h = &gHints[i];
        if (h->key == 0xff)
            return NULL;
        if (h->key == key)
            return h;
        i++;
    }
}

void Hint_Init(void)
{
    size_t size;

    size = comboDmaLoadFile(NULL, COMBO_VROM_HINTS);
    gHints = malloc(size);
    comboDmaLoadFile(gHints, COMBO_VROM_HINTS);
}

static void appendCorrectItemName(char** b, s16 gi, u8 player, u8 importance)
{
    comboTextAppendItemName(b, gi, TF_PROGRESSIVE);

    comboTextAppendItemImportance(b, gi, importance);

    if (player != 0 && player != 0xff && player != gComboConfig.playerId)
    {
        comboTextAppendStr(b, " for " TEXT_COLOR_YELLOW "Player ");
        comboTextAppendNum(b, player);
        comboTextAppendClearColor(b);
    }
}

#define PATH_WOTH       0
#define PATH_TRIFORCE   1
#define PATH_DUNGEON    2
#define PATH_BOSS       3
#define PATH_END_BOSS   4
#define PATH_EVENT      5

static const char* kPathTriforceNames[] = {
    TEXT_COLOR_RED    "Path of Power",
    TEXT_COLOR_GREEN  "Path of Courage",
    TEXT_COLOR_BLUE   "Path of Wisdom",
};

static const char* kPathEndBossNames[] = {
    TEXT_COLOR_RED    "Path to Ganon",
    TEXT_COLOR_PINK   "Path to Majora",
};

static const char* kPathEventNames[] = {
    TEXT_COLOR_TEAL     "Path to Time Travel",
    TEXT_COLOR_YELLOW   "Path to Bridge",
    TEXT_COLOR_PINK     "Path to Termina",
    TEXT_COLOR_RED      "Path to Moon",
};

static void appendPathName(char** b, u8 path, u8 subPath)
{
    switch (path)
    {
    case PATH_WOTH:
        comboTextAppendStr(b, TEXT_COLOR_YELLOW "Way of the Hero");
        break;
    case PATH_TRIFORCE:
        comboTextAppendStr(b, kPathTriforceNames[subPath]);
        break;
    case PATH_DUNGEON:
        comboTextAppendDungeonName(b, subPath, "Path to ");
        break;
    case PATH_BOSS:
        comboTextAppendBossName(b, subPath, "Path to ");
        break;
    case PATH_END_BOSS:
        comboTextAppendStr(b, kPathEndBossNames[subPath]);
        break;
    case PATH_EVENT:
        comboTextAppendStr(b, kPathEventNames[subPath]);
        break;
    }
}

static void Hint_DisplayRaw(PlayState* play, const Hint* hint)
{
    char* b;
    char* start;
    int itemIndex;
    int tmp;

#if defined(GAME_OOT)
    b = play->msgCtx.textBuffer;
#else
    b = play->msgCtx.font.textBuffer.schar;
#endif

    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "They say that ");

    /* Fetch the hint and check */
    itemIndex = 0;
    switch (hint->type)
    {
    case HINT_TYPE_PATH:
        comboTextAppendRegionName(&b, hint->region, hint->world, 0);
        comboTextAppendStr(&b, " is on the ");
        appendPathName(&b, hint->items[0], hint->items[1]);
        comboTextAppendClearColor(&b);
        break;
    case HINT_TYPE_FOOLISH:
        comboTextAppendStr(&b, "plundering ");
        comboTextAppendRegionName(&b, hint->region, hint->world, 0);
        comboTextAppendStr(&b, " is a " TEXT_COLOR_PINK "foolish choice");
        comboTextAppendClearColor(&b);
        break;
    case HINT_TYPE_ITEM_EXACT:
        tmp = comboTextAppendCheckName(&b, hint->region, hint->world);
        comboTextAppendStr(&b, (tmp & TF_PLURAL) ? " give " : " gives ");
        appendCorrectItemName(&b, hint->items[itemIndex], hint->players[itemIndex], hint->itemImportances[itemIndex]);
        if (hint->items[2] != -1)
        {
            itemIndex++;
            comboTextAppendStr(&b, ", ");
            appendCorrectItemName(&b, hint->items[itemIndex], hint->players[itemIndex], hint->itemImportances[itemIndex]);
        }
        if (hint->items[1] != -1)
        {
            itemIndex++;
            comboTextAppendStr(&b, " and ");
            appendCorrectItemName(&b, hint->items[itemIndex], hint->players[itemIndex], hint->itemImportances[itemIndex]);
        }
        break;
    case HINT_TYPE_ITEM_REGION:
        appendCorrectItemName(&b, hint->items[0], hint->players[0], hint->itemImportances[0]);
        comboTextAppendStr(&b, " can be found ");
        comboTextAppendRegionName(&b, hint->region, hint->world, TF_PREPOS);
        break;
    case HINT_TYPE_JUNK:
        comboTextAppendStr(&b, kJunkHints[((u16)hint->items[0]) % ARRAY_COUNT(kJunkHints)]);
        break;
    }
    comboTextAppendStr(&b, "." TEXT_END);
    comboTextAutoLineBreaks(start);
}

void Hint_DisplayJunk(PlayState* play, u32 index)
{
    Hint h;

    memset(&h, 0, sizeof(h));
    h.type = HINT_TYPE_JUNK;
    h.items[0] = (index % ARRAY_COUNT(kJunkHints));
    Hint_DisplayRaw(play, &h);
}

void Hint_Display(PlayState* play, u8 key)
{
    const Hint* hint;

    hint = findHint(key);
    if (hint != NULL)
        Hint_DisplayRaw(play, hint);
    else
        Hint_DisplayJunk(play, key);
}

void Hint_DisplayJunkRandom(PlayState* play)
{
    int key;

    key = Rand_S16Offset(0, 0x7fff);
    Hint_DisplayJunk(play, key);
}

#include <combo.h>
#include <combo/dma.h>
#include <combo/item.h>

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
    "THEW ORLD ISSQ UARE",                                      /* Final Fantasy 6 */
    "SWITCH targeting is the superior option",                  /* Blatant truth */
    "getting 100 coins gives you a star",                       /* Mario 64 */
    "the main character is not actually called Zelda",          /* ... */
    "nothing is forever",                                       /* Useful life advice */
    "the yes needs the no, to win against the no",              /* French politics */
    "a winner is you",                                          /* Meme */
    "your princess is in another castle",                       /* Mario 1 */
    "it's a secret to everybody",                               /* Zelda 1 */
    "Ocarina of Time is the better game",                       /* OoTMM */
    "Majora's Mask is the better game",                         /* OoTMM */
    "the cake is a lie",                                        /* Portal */
    "I am Error",                                               /* Zelda 2 */
    "plundering Area 51 is a foolish choice",                   /* Useful life advice */
    "there are 118 known elements",                             /* Chemistry */
    "Nax's House is on the Way of the Hero",                    /* OoTMM */
    "this is not the hint you are looking for",                 /* OoTMM */
    "you can find Luigi in Super Mario 64",                     /* Mario 64 */
    "using the Fishing Pole in Twilight Princess is WoTH",      /* Twilight Princess */
    "23 is number 1",                                           /* Deku Tree */
    "keys are always in the last place you look in",            /* Facts */
    "a famous code is UUDD LRLR BAS",                           /* Konami Code */
    "there's only 151 Pokemons",                                /* Pokemon Gen 1 */
    "you can unlock Sonic and Tails in Super Smash Bros Melee", /* SSBM */
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

void comboInitHints(void)
{
    size_t size;

    size = comboDmaLoadFile(NULL, COMBO_VROM_HINTS);
    gHints = malloc(size);
    comboDmaLoadFile(gHints, COMBO_VROM_HINTS);
}

static void appendCorrectItemName(char** b, s16 gi, u8 player, u8 importance)
{
#if defined(GAME_MM)
    gi ^= MASK_FOREIGN_GI;
#endif
    comboTextAppendItemName(b, gi, TF_PROGRESSIVE);

    if (comboConfig(CFG_HINT_IMPORTANCE) && !isItemFastBuy(gi))
    {
        switch (importance)
        {
        case 0:
            comboTextAppendStr(b, " (" TEXT_COLOR_PINK "not required");
            comboTextAppendClearColor(b);
            comboTextAppendStr(b, ")");
            break;
        case 1:
            comboTextAppendStr(b, " (" TEXT_COLOR_TEAL "sometimes required");
            comboTextAppendClearColor(b);
            comboTextAppendStr(b, ")");
            break;
        case 2:
            comboTextAppendStr(b, " (" TEXT_COLOR_YELLOW "required");
            comboTextAppendClearColor(b);
            comboTextAppendStr(b, ")");
            break;
        }
    }

    if (player != 0 && player != 0xff && player != gComboData.playerId)
    {
        comboTextAppendStr(b, " for " TEXT_COLOR_YELLOW "Player ");
        comboTextAppendNum(b, player);
        comboTextAppendClearColor(b);
    }
}

static const char* kPathNames[] = {
    TEXT_COLOR_YELLOW "Way of the Hero",
    TEXT_COLOR_RED    "Path of Power",
    TEXT_COLOR_GREEN  "Path of Courage",
    TEXT_COLOR_BLUE   "Path of Wisdom",
};

void comboHintGossip(u8 key, GameState_Play* play)
{
    char* b;
    char* start;
    const Hint* hint;
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
    hint = findHint(key);
    if (hint == NULL)
    {
        /* (Fake) junk hint */
        comboTextAppendStr(&b, kJunkHints[key % ARRAY_SIZE(kJunkHints)]);
    }
    else
    {
        itemIndex = 0;
        switch (hint->type)
        {
        case HINT_TYPE_PATH:
            comboTextAppendRegionName(&b, hint->region, hint->world, 0);
            comboTextAppendStr(&b, " is on the ");
            comboTextAppendStr(&b, kPathNames[hint->items[0]]);
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
            comboTextAppendStr(&b, kJunkHints[((u16)hint->items[0]) % ARRAY_SIZE(kJunkHints)]);
            break;
        }
    }
    comboTextAppendStr(&b, "." TEXT_END);
    comboTextAutoLineBreaks(start);
}

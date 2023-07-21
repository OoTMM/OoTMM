#include <combo.h>
#include <combo/dma.h>

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
    s16 item;
    s16 item2;
    u8  player;
    u8  player2;
}
Hint;

static const char* kJunkHints[] = {
    "THEW ORLD ISSQ UARE",                              /* Final Fantasy 6 */
    "SWITCH targeting is the superior option",          /* Blatant truth */
    "getting 100 coins gives you a star",               /* Mario 64 */
    "the main character is not actually called Zelda",  /* ... */
    "nothing is forever",                               /* Useful life advice */
    "the yes needs the no, to win against the no",      /* French politics */
    "a winner is you",                                  /* Meme */
    "your princess is in another castle",               /* Mario 1 */
    "it's a secret to everybody",                       /* Zelda 1 */
    "Ocarina of Time is the better game",               /* OoTMM */
    "Majora's Mask is the better game",                 /* OoTMM */
    "the cake is a lie",                                /* Portal */
    "I am Error",                                       /* Zelda 2 */
    "plundering Area 51 is a foolish choice",           /* Useful life advice */
    "there are 118 known elements",                     /* Chemistry */
    "Nax's House is on the Way of the Hero",            /* OoTMM */
};

ALIGNED(16) Hint gHints[0x40];

static const Hint* findHint(u8 key)
{
    for (int i = 0; i < ARRAY_SIZE(gHints); ++i)
    {
        const Hint* h = &gHints[i];
        if (h->key == 0xff)
            break;
        if (h->key == key)
            return h;
    }
    return NULL;
}

void comboInitHints(void)
{
    memset(gHints, 0xff, sizeof(gHints));
    comboDmaLoadFile(gHints, COMBO_VROM_HINTS);
}

static void appendCorrectItemName(char** b, s16 gi, u8 player)
{
#if defined(GAME_MM)
    gi ^= MASK_FOREIGN_GI;
#endif
    comboTextAppendItemName(b, gi, TF_PROGRESSIVE);

    if (player != 0 && player != 0xff && player != gComboData.playerId)
    {
        comboTextAppendStr(b, " for " TEXT_COLOR_YELLOW "Player ");
        comboTextAppendNum(b, player);
        comboTextAppendClearColor(b);
    }
}

void comboHintGossip(u8 key, GameState_Play* play)
{
    char* b;
    char* start;
    const Hint* hint;

#if defined(GAME_OOT)
    b = play->msgCtx.textBuffer;
#else
    b = play->textBuffer;
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
        switch (hint->type)
        {
        case HINT_TYPE_HERO:
            comboTextAppendRegionName(&b, hint->region, hint->world, 0);
            comboTextAppendStr(&b, " is on the " TEXT_COLOR_YELLOW "Way of the Hero");
            comboTextAppendClearColor(&b);
            break;
        case HINT_TYPE_FOOLISH:
            comboTextAppendStr(&b, "plundering ");
            comboTextAppendRegionName(&b, hint->region, hint->world, 0);
            comboTextAppendStr(&b, " is a " TEXT_COLOR_PINK "foolish choice");
            comboTextAppendClearColor(&b);
            break;
        case HINT_TYPE_ITEM_EXACT:
            comboTextAppendCheckName(&b, hint->region, hint->world);
            comboTextAppendStr(&b, " gives ");
            appendCorrectItemName(&b, hint->item, hint->player);
            if (hint->item2 != -1)
            {
                comboTextAppendStr(&b, " and ");
                appendCorrectItemName(&b, hint->item2, hint->player2);
            }
            break;
        case HINT_TYPE_ITEM_REGION:
            appendCorrectItemName(&b, hint->item, hint->player);
            comboTextAppendStr(&b, " can be found ");
            comboTextAppendRegionName(&b, hint->region, hint->world, TF_PREPOS);
            break;
        case HINT_TYPE_JUNK:
            comboTextAppendStr(&b, kJunkHints[((u16)hint->item) % ARRAY_SIZE(kJunkHints)]);
            break;
        }
    }
    comboTextAppendStr(&b, "." TEXT_END);
    comboTextAutoLineBreaks(start);
}

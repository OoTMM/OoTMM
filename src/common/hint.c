#include <combo.h>

#if defined(GAME_OOT)
# define HINTS_ADDR 0x03fe5000
#else
# define HINTS_ADDR 0x03fe6000
#endif

typedef struct PACKED ALIGNED(2)
{
    u8 key;
    u8 type;
    u8 region;
    u8 unused;
    s16 item;
    s16 item2;
}
Hint;

static const char* kJunkHints[] = {
    "THEW ORLD ISSQ UARE",                              /* Final Fantasy 6 */
    "SWITCH targeting is the superior option",          /* Blatant truth */
    "getting 100 coins gives you a star",               /* Mario 64 */
    "the main character is not actually called Zelda",  /* ... */
    "nothing is forever",                               /* Useful life advice */
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
    DMARomToRam(HINTS_ADDR | PI_DOM1_ADDR2, gHints,  sizeof(gHints));
}

static void appendCorrectItemName(char** b, s16 gi)
{
    s16 itemId;

#if defined(GAME_MM)
    gi ^= MASK_FOREIGN_GI;
#endif
    itemId = comboItemFromGI(gi);
    comboTextAppendItemName(b, itemId, TF_PROGRESSIVE);
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
        /* Junk hint */
        comboTextAppendStr(&b, kJunkHints[key % ARRAY_SIZE(kJunkHints)]);
    }
    else
    {
        switch (hint->type)
        {
        case HINT_TYPE_HERO:
            comboTextAppendRegionName(&b, hint->region, 0);
            comboTextAppendStr(&b, " is on the " TEXT_COLOR_YELLOW "Way of the Hero");
            comboTextAppendClearColor(&b);
            break;
        case HINT_TYPE_FOOLISH:
            comboTextAppendStr(&b, "plundering ");
            comboTextAppendRegionName(&b, hint->region, 0);
            comboTextAppendStr(&b, " is a " TEXT_COLOR_PINK "foolish choice");
            comboTextAppendClearColor(&b);
            break;
        case HINT_TYPE_ITEM_EXACT:
            comboTextAppendCheckName(&b, hint->region);
            comboTextAppendStr(&b, " gives ");
            appendCorrectItemName(&b, hint->item);
            if (hint->item2 != -1)
            {
                comboTextAppendStr(&b, " and ");
                appendCorrectItemName(&b, hint->item2);
            }
            break;
        case HINT_TYPE_ITEM_REGION:
            appendCorrectItemName(&b, hint->item);
            comboTextAppendStr(&b, " can be found ");
            comboTextAppendRegionName(&b, hint->region, TF_PREPOS);
            break;
        }
    }
    comboTextAppendStr(&b, "." TEXT_END);
    comboTextAutoLineBreaks(start);
}

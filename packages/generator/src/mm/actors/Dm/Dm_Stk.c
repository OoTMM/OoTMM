#include <combo.h>
#include <combo/config.h>
#include <combo/actor.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x290) = (h); } while (0)

static void hintOathToOrderSingle(PlayState* play)
{
    char* b;
    char* start;

    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b,
        "Have you found the " TEXT_COLOR_PINK "Oath to Order "
    );
    comboTextAppendClearColor(&b);
    comboTextAppendRegionName(&b, gComboConfig.hints.oathToOrder[0].region, gComboConfig.hints.oathToOrder[0].world, TF_PREPOS);
    comboTextAppendStr(&b, "?" TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void hintOathToOrderNotes(PlayState* play)
{
    int count;
    int found;
    u8 locWorld[6];
    u8 locRegion[6];
    u8 locCount[6];
    const ItemHint* ih;

    char* b;
    char* start;

    /* Prepare the buffers */
    count = 0;
    for (int i = 0; i < 6; ++i)
    {
        ih = &gComboConfig.hints.oathToOrder[i];
        if (!ih->region)
            break;

        /* Look for an identical region */
        found = 0;
        for (int j = 0; j < count; ++j)
        {
            if (locRegion[j] == ih->region && locWorld[j] == ih->world)
            {
                locCount[j]++;
                found = 1;
                break;
            }
        }

        if (!found)
        {
            locRegion[count] = ih->region;
            locWorld[count] = ih->world;
            locCount[count] = 1;
            count++;
        }
    }

    if (count == 0)
    {
        /* Safety */
        hintOathToOrderSingle(play);
        return;
    }

    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b,
        "Have you found the " TEXT_COLOR_PINK "Oath to Order Notes"
    );
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, "?" TEXT_NL);

    for (int i = 0; i < count; ++i)
    {
        comboTextAppendNum(&b, locCount[i]);
        comboTextAppendStr(&b, " ");
        comboTextAppendRegionName(&b, locRegion[i], locWorld[i], TF_PREPOS);
        if (i + 1 < count)
            comboTextAppendStr(&b, TEXT_NL);
    }
    comboTextAppendStr(&b, TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void DmStk_Start(Actor* this, PlayState* play)
{
    if (Config_SpecialCond(SPECIAL_MOON) && !Config_Flag(CFG_MM_OPEN_MOON))
    {
        PlayerDisplayTextBox(play, 0x2013, this);
        if (Config_Flag(CFG_SONG_NOTES))
            hintOathToOrderNotes(play);
        else
            hintOathToOrderSingle(play);
    }
    PlayMusic(0, 0x38, 0, 7, 0xff);
}

void DmStk_StartCutsceneWithOcarina(Actor* this, PlayState* play)
{
    DmStk_Start(this, play);
    SET_HANDLER(this, actorAddr(0x191, 0x80aa192c));
}

PATCH_FUNC(0x80aa18d8, DmStk_StartCutsceneWithOcarina);

void DmStk_StartCutsceneWithoutOcarina(Actor* this, PlayState* play)
{
    DmStk_Start(this, play);
    SET_HANDLER(this, actorAddr(0x191, 0x80aa19ec));
}

PATCH_FUNC(0x80aa1998, DmStk_StartCutsceneWithOcarina);

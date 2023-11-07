#include <combo.h>

#define PICTOBOX_SWAMP          (1 <<  1)
#define PICTOBOX_MONKEY         (1 <<  2)
#define PICTOBOX_BIG_OCTO       (1 <<  3)
#define PICTOBOX_LULU1          (1 <<  4)
#define PICTOBOX_LULU2          (1 <<  5)
#define PICTOBOX_LULU3          (1 <<  6)
#define PICTOBOX_SCARECROW      (1 <<  7)
#define PICTOBOX_TINGLE         (1 <<  8)
#define PICTOBOX_PIRATE_GOOD    (1 <<  9)
#define PICTOBOX_DEKU_KING      (1 << 10)
#define PICTOBOX_PIRATE_BAD     (1 << 11)

static const char* pictoText(void)
{
    static const u32 luluMask = PICTOBOX_LULU1 | PICTOBOX_LULU2 | PICTOBOX_LULU3;
    u32 f;

    f = gSave.pictoFlags0;

    if (f & PICTOBOX_MONKEY)
        return "picture of a monkey";
    if (f & PICTOBOX_BIG_OCTO)
        return "picture of a big octo";
    if (f & PICTOBOX_DEKU_KING)
        return "picture of the Deku King";
    if ((f & luluMask) == luluMask)
        return "good picture of Lulu";
    if (f & PICTOBOX_LULU1)
        return "bad picture of Lulu";
    if (f & PICTOBOX_SCARECROW)
        return "picture of a scarecrow";
    if (f & PICTOBOX_TINGLE)
        return "picture of Tingle";
    if (f & PICTOBOX_PIRATE_GOOD)
        return "good picture of a pirate";
    if (f & PICTOBOX_PIRATE_BAD)
        return "bad picture of a pirate";
    if (f & PICTOBOX_SWAMP)
        return "picture of the swamp";
    return "picture";
}

static void PictoHijackText(GameState_Play* play)
{
    char* b;

    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);

    comboTextAppendStr(&b, "Keep this " TEXT_COLOR_RED);
    comboTextAppendStr(&b, pictoText());
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, "?" TEXT_NL TEXT_NL TEXT_CHOICE2 TEXT_COLOR_GREEN "Yes" TEXT_NL "No" TEXT_END);
}

static void PictoDisplayTextBox(GameState_Play* play, s16 messageId, Actor* actor)
{
    if (gPictoboxPhotoTaken == 1)
    {
        PictoUpdateFlags(play);
    }

    PlayerDisplayTextBox(play, messageId, actor);
    PictoHijackText(play);
}

PATCH_CALL(0x80120c34, PictoDisplayTextBox);

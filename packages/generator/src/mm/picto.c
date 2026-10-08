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

    f = gSave.info.pictoFlags0;

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

static void PictoHijackText(PlayState* play)
{
    char* b;

    b = play->msgCtx.font.textBuffer.schar;
    comboTextAppendHeader(&b);

    comboTextAppendStr(&b, "Keep this " TEXT_COLOR_RED);
    comboTextAppendStr(&b, pictoText());
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, "?" TEXT_NL TEXT_NL TEXT_CHOICE2 TEXT_COLOR_GREEN "Yes" TEXT_NL "No" TEXT_END);
}

static void PictoDisplayTextBox(PlayState* play, s16 messageId, Actor* actor)
{
    if (gPictoboxPhotoTaken == 1)
    {
        PictoUpdateFlags(play);
    }

    PlayerDisplayTextBox(play, messageId, actor);
    PictoHijackText(play);
}

PATCH_CALL(0x80120c34, PictoDisplayTextBox);

//pictograph box fix

typedef struct
{
    u16 width;
    u16 height;
    u8 pad[0x0c];
    u16* fbuf;
    u16* fbufSave;
} PictoPreRender;

static void PictoConvert(u8* dst, const u16* src, s32 stride, s32 left, s32 top, s32 right, s32 bottom)
{
    s32 x;
    s32 y;
    s32 index = 0;

    for (y = top; y <= bottom; y++)
    {
        for (x = left; x <= right; x++)
        {
            u16 pixel = src[y * stride + x];
            u32 r = (pixel >> 11) & 0x1f;
            u32 g = (pixel >> 6) & 0x1f;
            u32 b = (pixel >> 1) & 0x1f;
            u32 intensity = ((r * 2 + g * 4 + b) * 255) / (31 * 7);
            dst[index++] = (u8)intensity;
        }
    }
}

static void PictoTakePhotoDirect(PictoPreRender* prerender)
{
    const u16* src;
    s32 width;
    s32 height;

    src = prerender->fbuf;
    width = prerender->width;
    height = prerender->height;

    if (src == NULL)
        return;

    if (width != 320 || height != 240)
        return;

    osInvalDCache((void*)src, width * height * sizeof(u16));
    PictoConvert(((u8*)0x80780000), src, width, 80, 64, 239, 175);
}

PATCH_FUNC(0x80165e1c, PictoTakePhotoDirect);

static u8 sPictoCaptureActive = 0;
static u8 sPictoCaptureStarted = 0;
static u8 sPictoCleanFrames = 0;

int Picto_IsCapturing(void)
{
    return sPictoCaptureActive;
}

void Picto_PrepareDraw(void)
{
    if (!sPictoCaptureActive)
    {
        if (R_PICTO_PHOTO_STATE == 1)
        {
            sPictoCaptureActive = 1;
            sPictoCaptureStarted = 0;
            sPictoCleanFrames = 3;
            R_PICTO_PHOTO_STATE = 0;
        }
        return;
    }
    if (sPictoCaptureStarted)
    {
        if (R_PICTO_PHOTO_STATE == 0)
        {
            sPictoCaptureActive = 0;
            sPictoCaptureStarted = 0;
        }
        return;
    }
    if (sPictoCleanFrames > 0)
    {
        sPictoCleanFrames--;
        return;
    }
    sPictoCaptureStarted = 1;
    R_PICTO_PHOTO_STATE = 1;
}

// Needed in order to not break photos on hardware with the above rework

typedef struct
{
    u8 pad[0x10];
    u16* fbuf;
    u16* fbufSave;
    u8* cvgSave;
} PictoCoveragePreRender;

extern void PreRender_FetchFbufCoverage(PictoCoveragePreRender* prerender, Gfx** gfx);
extern void PreRender_CoverageRgba16ToI8(PictoCoveragePreRender* prerender, Gfx** gfx, void* src, void* dst);


static void PictoDrawCoverage(PictoCoveragePreRender* prerender, Gfx** gfx)
{
    if (R_PICTO_PHOTO_STATE == 2)
        return;

    PreRender_FetchFbufCoverage(prerender, gfx);

    if (prerender->cvgSave != NULL)
        PreRender_CoverageRgba16ToI8(prerender, gfx, prerender->fbuf, prerender->cvgSave);
}

PATCH_FUNC(0x80170730, PictoDrawCoverage);
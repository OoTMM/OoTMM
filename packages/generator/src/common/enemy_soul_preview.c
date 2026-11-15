#include <combo.h>
#include <combo/config.h>
#include <combo/enemy.h>
#include <combo/enemy_keep.h>
#include <combo/enemy_soul_preview.h>

#if defined(GAME_OOT)
#define PreviewFree ((void (*)(void*))0x80066c90)
#else
#define PreviewFree ((void (*)(void*))0x80102ce0)
#endif

static struct {
    ForeignEnemyOverlay code;
    u32 table;
} sPreviewConfig __attribute__((section(".data"))) = { 0 };

#if defined(GAME_OOT)
EXPORT_SYMBOL(ENEMY_SOUL_PREVIEW_OOT_V1, sPreviewConfig);
#else
EXPORT_SYMBOL(ENEMY_SOUL_PREVIEW_MM_V1, sPreviewConfig);
#endif

static void* sCode;
static int (*sDispatch)(int, EnemySoulPreviewRequest*);
static s16 sMissingSoul;
static u8 sMissingGame;
u32 gEnemySoulPreviewSerial;

void comboEnemySoulPreviewGateBegin(void)
{
    sMissingSoul = 0;
}

void comboEnemySoulPreviewObserve(int game, s16 gi, int present, int enemy)
{
    if (!present && enemy)
    {
        sMissingSoul = gi;
        sMissingGame = game;
    }
}

static int previewLoad(void)
{
    if (!sDispatch)
    {
        u32 size = MAX(sPreviewConfig.code.fileSize, sPreviewConfig.code.memorySize);
        if (size > 0x10000 || !size)
            return 0;
        sCode = comboEnemyArenaAlloc(size);
        if (!sCode)
            return 0;
        ActorProfile* profile = comboLoadForeignEnemyOverlay(sCode, size, &sPreviewConfig.code);
        if (!profile)
        {
            PreviewFree(sCode);
            sCode = NULL;
            return 0;
        }
        sDispatch = (void*)profile->init;
    }
    return 1;
}

int comboEnemySoulPreviewGate(PlayState* play, int game, s16 id, u16 params, f32 x, f32 y, f32 z, s16 yaw, int allowed)
{
    if (!Config_Flag(CFG_SOULS_ENEMY_LENS) || !play)
        return allowed;
    if (allowed <= 0 && (!sMissingSoul || !sPreviewConfig.table || !previewLoad()))
        return allowed;
    if (!sDispatch)
        return allowed;
    EnemySoulPreviewRequest request = { play, sPreviewConfig.table, x, y, z, id, yaw, sMissingSoul, params, game, sMissingGame };
    if (allowed <= 0)
    {
        if (sDispatch(0, &request))
            ++gEnemySoulPreviewSerial;
    }
    else if (sDispatch(4, &request))
        return 0;
    return allowed;
}

void comboEnemySoulPreviewDraw(PlayState* play)
{
    if (sDispatch)
    {
        EnemySoulPreviewRequest request = { .play = play };
        sDispatch(1, &request);
    }
}

void comboEnemySoulPreviewReset(void)
{
    if (sDispatch)
        sDispatch(2, NULL);
    if (sCode)
        PreviewFree(sCode);
    sCode = NULL;
    sDispatch = NULL;
    sMissingSoul = 0;
}

int comboEnemyFindFloor(PlayState* play, Actor* actor, Vec3f* position, f32 height, u16 radius, int grounded)
{
    if (!previewLoad())
        return 0;
    EnemySoulPreviewRequest request = { .play = play, .actor = actor, .position = position, .height = height, .radius = radius };
    return sDispatch(grounded ? 5 : 3, &request);
}

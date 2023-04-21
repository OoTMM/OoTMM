#include <combo.h>

static int sIsSmallChest;

void BgMoriHineri_DrawWrapper(Actor* this, GameState_Play* play)
{
    ActorCallback draw;
    s16 gi;
    float* f;

    /* Init CSMC */
    gi = comboOverrideEx(OV_CHEST, play->sceneId, 0xe, GI_OOT_BOSS_KEY, 0);
    comboCsmcPreDraw(this, play, gi);

    /* Check for CSMC and small chest */
    sIsSmallChest = 0;
    if (comboCsmcChestSize(gi) == 0)
    {
        sIsSmallChest = 1;
        f = actorAddr(0x068, 0x80913d74);
        *f = 0.005f;
        f = actorAddr(0x068, 0x80913d80);
        *f = 0.005f;
    }

    /* Draw */
    draw = actorAddr(0x068, 0x80913944);
    draw(this, play);
}

void BgMoriHineri_TranslateLid(void)
{
    if (sIsSmallChest)
        ModelViewTranslate(157.f, -231.5f, -453.f, MAT_MUL);
    else
        ModelViewTranslate(167.f, -218.f, -453.f, MAT_MUL);
}

PATCH_CALL(0x80913c58, BgMoriHineri_TranslateLid);

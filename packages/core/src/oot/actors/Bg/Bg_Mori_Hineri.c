#include <combo.h>
#include <combo/item.h>
#include <combo/csmc.h>
#include <combo/actor.h>

static int sIsSmallChest;

static void BgMoriHineri_ItemQuery(ComboItemQuery* q, int flags)
{
    bzero(q, sizeof(*q));

    q->ovType = OV_CHEST;
    q->sceneId = SCE_OOT_TEMPLE_FOREST;
    q->gi = GI_OOT_BOSS_KEY;
    q->id = 0x0e;
    q->ovFlags = flags;
}

static void BgMoriHineri_ItemOverride(ComboItemOverride* o, int flags)
{
    ComboItemQuery q;

    BgMoriHineri_ItemQuery(&q, flags);
    comboItemOverride(o, &q);
}

void BgMoriHineri_DrawWrapper(Actor* this, PlayState* play)
{
    ComboItemOverride o;
    ActorCallback draw;
    float* f;

    /* Init CSMC */
    BgMoriHineri_ItemOverride(&o, 0);
    csmcChestPreDraw(this, play, o.gi, o.cloakGi);

    /* Check for CSMC and small chest */
    sIsSmallChest = 0;
    if (csmcChestLarge(o.gi, o.cloakGi) == 0)
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
        Matrix_Translate(157.f, -231.5f, -453.f, MTXMODE_APPLY);
    else
        Matrix_Translate(167.f, -218.f, -453.f, MTXMODE_APPLY);
}

PATCH_CALL(0x80913c58, BgMoriHineri_TranslateLid);

#include <combo.h>
#include <combo/csmc.h>
#include <combo/custom.h>
#include <combo/dungeon.h>

#define CSMC_NAVI_NORMAL     0x00
#define CSMC_NAVI_BOSS_KEY   0x01
#define CSMC_NAVI_MAJOR      0x02
#define CSMC_NAVI_KEY        0x03
#define CSMC_NAVI_SPIDER     0x04
#define CSMC_NAVI_FAIRY      0x05
#define CSMC_NAVI_HEART      0x06

typedef struct {
    /* 0x00 */ Color_RGBA8 inner;
    /* 0x04 */ Color_RGBA8 outer;
} NaviColor; // size = 0x8

#if defined(GAME_OOT)
static NaviColor sNaviColorList[] = {
    { { 0, 255, 0, 255 }, { 0, 255, 0, 0 } },
    { { 0, 0, 255, 255 }, { 0, 0, 255, 0 } },
    { { 255, 255, 0, 255 }, { 255, 255, 0, 0 } },
    { { 68, 68, 68, 255 }, { 68, 68, 68, 0 } },
    { { 255, 255, 255, 255 }, { 255, 255, 255, 0 } },
    { { 255, 122, 251, 255 }, { 255, 122, 251, 0 } },
    { { 255, 0, 0, 255 }, { 255, 0, 0, 0 } },
};

static int csmcNaviId(s16 gi)
{
    int csmcId;

    if (gi == 0)
        return CSMC_NORMAL;
    if (!csmcEnabled())
        return CSMC_NORMAL;

    csmcId = csmcFromItem(gi);
    switch (csmcId)
    {
    case CSMC_NORMAL:       return CSMC_NAVI_NORMAL;
    case CSMC_BOSS_KEY:     return CSMC_NAVI_BOSS_KEY;
    case CSMC_MAJOR:        return CSMC_NAVI_MAJOR;
    case CSMC_KEY:          return CSMC_NAVI_KEY;
    case CSMC_SPIDER:       return CSMC_NAVI_SPIDER;
    case CSMC_FAIRY:        return CSMC_NAVI_FAIRY;
    case CSMC_HEART:        return CSMC_NAVI_HEART;
    case CSMC_SOUL:         return CSMC_NAVI_BOSS_KEY;
    default:                return CSMC_NAVI_MAJOR;
    }
}

void Actor_AfterSetNaviToActor(TargetContext* targetCtx, Actor* actor, s32 actorCategory, GameState_Play* play) {
    int type;
    ComboItemOverride o;
    ComboItemQuery q;

    if (actor->id == AC_SHOT_SUN) {
        Actor_ShotSun* shotSun = (Actor_ShotSun*)actor;
        comboXflagItemQuery(&q, &shotSun->xflag, GI_OOT_FAIRY_BIG);
        q.giRenew = GI_OOT_FAIRY_BIG;
        if (comboXflagsGet(&shotSun->xflag)) {
            q.ovFlags = OVF_RENEW;
        }
        comboItemOverride(&o, &q);
        type = csmcNaviId(o.gi);
        NaviColor* naviColor = &sNaviColorList[type];
        targetCtx->naviInner.r = naviColor->inner.r;
        targetCtx->naviInner.g = naviColor->inner.g;
        targetCtx->naviInner.b = naviColor->inner.b;
        targetCtx->naviInner.a = naviColor->inner.a;
        targetCtx->naviOuter.r = naviColor->outer.r;
        targetCtx->naviOuter.g = naviColor->outer.g;
        targetCtx->naviOuter.b = naviColor->outer.b;
        targetCtx->naviOuter.a = naviColor->outer.a;
    }
}
#endif

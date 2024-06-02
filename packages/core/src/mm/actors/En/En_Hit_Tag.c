#include <combo.h>
#include <combo/global.h>
#include <combo/item.h>
#include <combo/draw.h>
#include <combo/mm/actors/En_Hit_Tag.h>

static Actor_EnHitTag* sHitTag;
static u8 sHitTagCount;

static void EnHitTag_DrawGlitter(Actor_EnHitTag* this, GameState_Play* play)
{
    ComboItemOverride o;
    Xflag x;
    s16 giList[3];
    s16 gi;
    u8 giCount;

    /* Get the gi list */
    giCount = 0;
    memcpy(&x, &this->xflag, sizeof(Xflag));
    for (int i = 0; i < 3; ++i)
    {
        x.sliceId = i;
        if (comboXflagsGet(&x))
            continue;
        comboXflagItemOverride(&o, &x, 0);
        if (o.gi == 0)
            continue;
        giList[giCount++] = o.gi;
    }

    if (giCount == 0)
    {
        this->base.draw = NULL;
        return;
    }

    switch (giCount)
    {
    case 1:
        gi = giList[0];
        break;
    case 2:
        gi = giList[(play->gs.frameCount % 12) / 6];
        break;
    case 3:
        gi = giList[(play->gs.frameCount % 12) / 4];
        break;
    default:
        UNREACHABLE();
    }

    Draw_GlitterGi(play, &this->base, gi);
}

static void EnHitTag_ItemDropCollectible(GameState_Play* play, const Vec3f* pos, int param)
{
    Actor_EnHitTag* this;
    ComboItemOverride o;
    Xflag x;

    this = sHitTag;
    if (!this->isExtended)
    {
        Item_DropCollectible(play, pos, param);
        return;
    }

    memcpy(&x, &this->xflag, sizeof(Xflag));
    x.sliceId = sHitTagCount;
    sHitTagCount++;
    comboXflagItemOverride(&o, &x, 0);

    if (comboXflagsGet(&x) || o.gi == 0)
        Item_DropCollectible(play, pos, param);
    else
        EnItem00_DropCustom(play, pos, &x);
}

PATCH_CALL(0x80be215c, EnHitTag_ItemDropCollectible);

void EnHitTag_InitWrapper(Actor_EnHitTag* this, GameState_Play* play)
{
    int switchFlag;
    ComboItemOverride o;
    EnHitTagFunc init;
    Xflag x;

    /* Setup the xflag */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = g.sceneSetupId;
    this->xflag.roomId = this->base.room;
    this->xflag.sliceId = 0;
    this->xflag.id = g.actorIndex;

    this->isExtended = 0;
    memcpy(&x, &this->xflag, sizeof(Xflag));
    for (int i = 0; i < 3; ++i)
    {
        x.sliceId = i;
        comboXflagItemOverride(&o, &x, 0);
        if (o.gi && !comboXflagsGet(&x))
        {
            this->isExtended = 1;
            break;
        }
    }

    /* Switch flag */
    switchFlag = (this->base.variable & 0xfe00) >> 9;
    if (this->isExtended && GetSwitchFlag(play, switchFlag))
        ClearSwitchFlag(play, switchFlag);

    /* Forward */
    init = actorAddr(AC_EN_HIT_TAG, 0x80be2030);
    init(this, play);

    if (this->isExtended)
        this->base.draw = EnHitTag_DrawGlitter;
}

void EnHitTag_Update(Actor_EnHitTag* this, GameState_Play* play)
{
    sHitTag = this;
    sHitTagCount = 0;
    this->handler(this, play);
}

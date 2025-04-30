#include <combo.h>
#include <combo/global.h>
#include <combo/item.h>
#include <combo/draw.h>
#include <combo/mm/actors/En_Hit_Tag.h>

static Actor_EnHitTag* sHitTag;
static u8 sHitTagCount;

static void EnHitTag_DrawGlitter(Actor_EnHitTag* this, PlayState* play)
{
    ComboItemOverride o;
    Xflag x;
    s16 giList[3];
    s16 giCloakList[3];
    s16 gi;
    s16 giCloak;
    u8 giCount;
    int index;

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
        giList[giCount] = o.gi;
        giCloakList[giCount] = o.cloakGi;
        giCount++;
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
        giCloak = giCloakList[0];
        break;
    case 2:
        index = (play->state.frameCount % 12) / 6;
        gi = giList[index];
        giCloak = giCloakList[index];
        break;
    case 3:
        index = (play->state.frameCount % 12) / 4;
        gi = giList[index];
        giCloak = giCloakList[index];
        break;
    default:
        UNREACHABLE();
    }

    Draw_GlitterGi(play, &this->base, gi, giCloak);
}

static void EnHitTag_ItemDropCollectible(PlayState* play, const Vec3f* pos, int param)
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

void EnHitTag_InitWrapper(Actor_EnHitTag* this, PlayState* play)
{
    int switchFlag;
    EnHitTagFunc init;
    Xflag x;

    /* Setup the xflag */
    comboXflagInit(&this->xflag, &this->base, play);
    this->isExtended = FALSE;
    memcpy(&x, &this->xflag, sizeof(Xflag));
    for (int i = 0; i < 3; ++i)
    {
        x.sliceId = i;
        if (Xflag_IsShuffled(&x))
        {
            this->isExtended = TRUE;
            break;
        }
    }

    /* Switch flag */
    switchFlag = (this->base.params & 0xfe00) >> 9;
    if (this->isExtended && Flags_GetSwitch(play, switchFlag))
        ClearSwitchFlag(play, switchFlag);

    /* Forward */
    init = actorAddr(ACTOR_EN_HIT_TAG, 0x80be2030);
    init(this, play);

    if (this->isExtended)
        this->base.draw = EnHitTag_DrawGlitter;
}

void EnHitTag_Update(Actor_EnHitTag* this, PlayState* play)
{
    sHitTag = this;
    sHitTagCount = 0;
    this->handler(this, play);
}

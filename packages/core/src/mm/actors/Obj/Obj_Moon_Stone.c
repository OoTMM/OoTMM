#include <combo.h>
#include <combo/item.h>
#include <combo/draw.h>

static void ObjMoonStone_ItemQuery(ComboItemQuery* q, PlayState* play)
{
    memset(q, 0, sizeof(*q));

    if (gMmExtraFlags2.moonTear && play->sceneId == SCE_MM_TERMINA_FIELD)
    {
        q->ovType = OV_NONE;
        q->gi = GI_MM_RECOVERY_HEART;
        return;
    }

    q->ovType = OV_NPC;
    q->id = NPC_MM_MOON_TEAR;
    q->gi = GI_MM_MOON_TEAR;
}

void ObjMoonStone_Draw(Actor* this, PlayState* play)
{
    ComboItemQuery q;
    ComboItemOverride o;
    s16 gi;

    ObjMoonStone_ItemQuery(&q, play);
    comboItemOverride(&o, &q);
    gi = o.gi;
    if (o.cloakGi)
        gi = o.cloakGi;
    Draw_Gi(play, this, gi, 0);
}

PATCH_FUNC(0x80c06910, ObjMoonStone_Draw);

int ObjMoonStone_HasGivenItem(Actor* this)
{
    if (Actor_HasParentZ(this))
    {
        gMmExtraFlags2.moonTear = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80c067ec, ObjMoonStone_HasGivenItem);

void ObjMoonStone_GiveItem(Actor* this, PlayState* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    ObjMoonStone_ItemQuery(&q, play);
    comboGiveItem(this, play, &q, a, b);
}

PATCH_CALL(0x80c06840, ObjMoonStone_GiveItem);

#include <combo.h>
#include <combo/item.h>

static void ObjMoonStone_ItemQuery(ComboItemQuery* q, GameState_Play* play, int flags)
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
    q->ovFlags = flags;
}

void ObjMoonStone_Draw(Actor* this, GameState_Play* play)
{
    ComboItemQuery q;
    ComboItemOverride o;

    ObjMoonStone_ItemQuery(&q, play, OVF_PROGRESSIVE);
    comboItemOverride(&o, &q);
    comboDrawGI(play, this, o.gi, 0);
}

PATCH_FUNC(0x80c06910, ObjMoonStone_Draw);

int ObjMoonStone_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        gMmExtraFlags2.moonTear = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80c067ec, ObjMoonStone_HasGivenItem);

void ObjMoonStone_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;

    ObjMoonStone_ItemQuery(&q, play, OVF_PROGRESSIVE | OVF_DOWNGRADE);
    comboGiveItem(this, play, &q, a, b);
}

PATCH_CALL(0x80c06840, ObjMoonStone_GiveItem);

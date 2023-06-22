#include <combo.h>
#include <combo/item.h>

static void DmChar02_ItemQuery(ComboItemQuery* q, int npc, int flags)
{
    bzero(q, sizeof(*q));
    q->ovType = OV_NPC;
    q->gi = npc == NPC_MM_SKULL_KID_OCARINA ? GI_MM_OCARINA_OF_TIME : GI_MM_SONG_TIME;
    q->id = npc;
    q->ovFlags = flags;
}

static void DmChar02_ItemOverride(ComboItemOverride* o, int npc, int flags)
{
    ComboItemQuery q;

    DmChar02_ItemQuery(&q, npc, flags);
    comboItemOverride(o, &q);
}

void DmChar02_InitScaleHook(Actor* this, float scale)
{
    this->position.y = -1000.0f;
    ActorSetScale(this, scale);
}

PATCH_CALL(0x80aab008, DmChar02_InitScaleHook);

int DmChar02_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
    {
        if (!gMmExtraFlags2.ocarina)
        {
            gMmExtraFlags2.ocarina = 1;
            this->attachedA = NULL;
            return 0;
        }
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80aab1d4, DmChar02_HasGivenItem);

void DmChar02_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;
    Actor_Player* link;
    int npc;

    link = GET_LINK(play);
    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return;

    if (!gMmExtraFlags2.ocarina)
        npc = NPC_MM_SKULL_KID_OCARINA;
    else
        npc = NPC_MM_SKULL_KID_SONG;
    DmChar02_ItemQuery(&q, npc, OVF_PROGRESSIVE | OVF_DOWNGRADE);
    comboGiveItem(this, play, &q, a, b);
}

PATCH_CALL(0x80aab1fc, DmChar02_GiveItem);

void DmChar02_DrawOcarina(Actor* this, GameState_Play* play)
{
    static const float scale = 25.0f;
    ComboItemOverride o;

    DmChar02_ItemOverride(&o, NPC_MM_SKULL_KID_OCARINA, OVF_PROGRESSIVE);
    ModelViewScale(scale, scale, scale, MAT_MUL);
    ModelViewTranslate(0.0f, 20.0f, 0.0f, MAT_MUL);
    comboDrawGI(play, this, o.gi, DRAW_RAW);
}

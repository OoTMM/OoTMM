#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/draw.h>

static void DmChar02_ItemQuery(ComboItemQuery* q, int npc)
{
    bzero(q, sizeof(*q));
    q->ovType = OV_NPC;
    q->gi = npc == NPC_MM_SKULL_KID_OCARINA ? GI_MM_OCARINA_OF_TIME : GI_MM_SONG_TIME;
    q->id = npc;
}

static void DmChar02_ItemOverride(ComboItemOverride* o, int npc)
{
    ComboItemQuery q;

    DmChar02_ItemQuery(&q, npc);
    comboItemOverride(o, &q);
}

void DmChar02_InitScaleHook(Actor* this, float scale)
{
    this->world.pos.y = -1000.0f;
    Actor_SetScale(this, scale);
}

PATCH_CALL(0x80aab008, DmChar02_InitScaleHook);

int DmChar02_HasGivenItem(Actor* this)
{
    if (Actor_HasParentZ(this))
    {
        if (!gMmExtraFlags2.ocarina)
        {
            gMmExtraFlags2.ocarina = 1;
            this->parent = NULL;
            return 0;
        }
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80aab1d4, DmChar02_HasGivenItem);

void DmChar02_GiveItem(Actor* this, PlayState* play, s16 gi, float a, float b)
{
    ComboItemQuery q;
    Player* link;
    int npc;

    link = GET_PLAYER(play);
    if (link->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM)
        return;

    if (!gMmExtraFlags2.ocarina)
        npc = NPC_MM_SKULL_KID_OCARINA;
    else
        npc = NPC_MM_SKULL_KID_SONG;
    DmChar02_ItemQuery(&q, npc);
    comboGiveItem(this, play, &q, a, b);
}

PATCH_CALL(0x80aab1fc, DmChar02_GiveItem);

void DmChar02_DrawOcarina(Actor* this, PlayState* play)
{
    static const float scale = 25.0f;
    ComboItemOverride o;
    s16 gi;

    DmChar02_ItemOverride(&o, NPC_MM_SKULL_KID_OCARINA);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    Matrix_Translate(0.0f, 20.0f, 0.0f, MTXMODE_APPLY);
    gi = o.cloakGi ? o.cloakGi : o.gi;
    Draw_Gi(play, this, gi, DRAW_RAW);
}

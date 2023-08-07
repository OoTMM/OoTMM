#include <combo.h>
#include <combo/item.h>
#include <combo/csmc.h>

static void EnBox_ItemQuery(ComboItemQuery* q, Actor* this, GameState_Play* play, s16 gi)
{
    memset(q, 0, sizeof(*q));

    q->gi = gi;
    if (play->sceneId == SCE_MM_TREASURE_SHOP && gi == -GI_MM_HEART_PIECE)
    {
        q->ovType = OV_NPC;
        q->id = NPC_MM_CHEST_GAME;
    }
    else
    {
        q->ovType = OV_CHEST;
        q->sceneId = comboSceneKey(play->sceneId);
        q->id = this->variable & 0x1f;
    }
}

static s16 EnBox_Item(Actor* this, GameState_Play* play, s16 gi)
{
    ComboItemQuery q;
    ComboItemOverride ov;

    EnBox_ItemQuery(&q, this, play, gi);
    comboItemOverride(&ov, &q);

    return ov.gi;
}

static s16 EnBox_GetGI(Actor* this)
{
    s32 raw;

    raw = *(s32*)((char*)this + 0x21c);
    return (s16)(-raw);
}

void EnBox_GiveItemDefaultRange(Actor* actor, GameState_Play* play, s16 gi)
{
    ComboItemQuery q;

    EnBox_ItemQuery(&q, actor, play, gi);
    comboGiveItem(actor, play, &q, 50.f, 10.f);
}

PATCH_CALL(0x80868fe0, EnBox_GiveItemDefaultRange);

void EnBox_InitWrapper(Actor* this, GameState_Play* play)
{
    ActorCallback init;
    s16 gi;

    /* Init the chest */
    init = actorAddr(AC_EN_BOX, 0x808680ac);
    init(this, play);

    /* Resize chest */
    gi = EnBox_Item(this, play, EnBox_GetGI(this));
    csmcChestInit(this, play, gi);
}

void EnBox_DrawWrapper(Actor* this, GameState_Play* play)
{
    ActorCallback draw;
    s16 gi;

    /* Prepare the segments */
    gi = EnBox_Item(this, play, EnBox_GetGI(this));
    csmcChestPreDraw(this, play, gi);

    /* Draw */
    draw = actorAddr(AC_EN_BOX, 0x808698f4);
    draw(this, play);
}

#include <combo.h>
#include <combo/item.h>
#include <combo/csmc.h>

static s16 convertGi(s16 initial)
{
    switch (initial)
    {
    case 0x01: return GI_MM_RUPEE_GREEN;
    case 0x02: return GI_MM_RUPEE_BLUE;
    case 0x04: return GI_MM_RUPEE_RED;
    case 0x05: return GI_MM_RUPEE_PURPLE;
    case 0x06: return GI_MM_RUPEE_SILVER;
    case 0x07: return GI_MM_RUPEE_GOLD;
    case 0x0c: return GI_MM_HEART_PIECE;
    case 0x2a: return GI_MM_NUTS_10;
    }

    return initial;
}

static s16 EnBox_GetGI(Actor* this)
{
    s16 raw;

    raw = (s16)*(s32*)((char*)this + 0x21c);
    raw = convertGi(raw);
    return (s16)(-raw);
}

static void EnBox_ItemQuery(ComboItemQuery* q, Actor* this, GameState_Play* play)
{
    s16 gi;
    memset(q, 0, sizeof(*q));

    gi = EnBox_GetGI(this);
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

static s16 EnBox_Item(Actor* this, GameState_Play* play)
{
    ComboItemQuery q;
    ComboItemOverride ov;

    EnBox_ItemQuery(&q, this, play);
    comboItemOverride(&ov, &q);

    return ov.gi;
}

void EnBox_GiveItemDefaultRange(Actor* actor, GameState_Play* play, s16 gi)
{
    ComboItemQuery q;

    EnBox_ItemQuery(&q, actor, play);
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
    gi = EnBox_Item(this, play);
    csmcChestInit(this, play, gi);
}

void EnBox_DrawWrapper(Actor* this, GameState_Play* play)
{
    ActorCallback draw;
    s16 gi;

    /* Prepare the segments */
    gi = EnBox_Item(this, play);
    csmcChestPreDraw(this, play, gi);

    /* Draw */
    draw = actorAddr(AC_EN_BOX, 0x808698f4);
    draw(this, play);
}

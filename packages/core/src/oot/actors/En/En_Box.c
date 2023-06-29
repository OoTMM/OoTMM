#include <combo.h>
#include <combo/item.h>

static void EnBox_ItemQuery(ComboItemQuery* q, Actor* this, GameState_Play* play, s16 gi)
{
    memset(q, 0, sizeof(*q));

    q->gi = gi;
    if (!(play->sceneId == SCE_OOT_TREASURE_SHOP && (this->variable & 0x1f) == 0x0a && gi == -GI_OOT_TC_RUPEE_PURPLE))
    {
        q->ovType = OV_CHEST;
        q->sceneId = play->sceneId;
        q->id = this->variable & 0x1f;
    }
    else
    {
        q->ovType = OV_NONE;
    }
}

static s16 EnBox_Item(Actor* this, GameState_Play* play, s16 gi)
{
    ComboItemQuery q;
    ComboItemOverride o;

    EnBox_ItemQuery(&q, this, play, gi);
    comboItemOverride(&o, &q);

    return o.gi;
}

void EnBox_GiveItem(Actor* actor, GameState_Play* play, s16 gi)
{
    ComboItemQuery q;

    EnBox_ItemQuery(&q, actor, play, gi);
    comboGiveItem(actor, play, &q, 50.f, 10.f);
}

PATCH_CALL(0x808696bc, &EnBox_GiveItem);

void EnBox_InitWrapper(Actor* this, GameState_Play* play)
{
    ActorCallback init;
    s16 gi;

    /* Init the chest */
    init = actorAddr(AC_EN_BOX, 0x808687e8);
    init(this, play);

    /* Resize chest */
    gi = EnBox_Item(this, play, -1);
    comboCsmcInit(this, play, gi);
}

void EnBox_DrawWrapper(Actor* this, GameState_Play* play)
{
    ActorCallback draw;
    s16 gi;

    /* Prepare the segments */
    gi = EnBox_Item(this, play, -1);
    comboCsmcPreDraw(this, play, gi);

    /* Draw */
    draw = actorAddr(AC_EN_BOX, 0x80869e68);
    draw(this, play);
}

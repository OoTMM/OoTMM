#include <combo.h>
#include <combo/item.h>
#include <combo/csmc.h>
#include <combo/config.h>
#include <combo/actor.h>

static void EnBox_ItemQuery(ComboItemQuery* q, Actor* this, PlayState* play, s16 gi)
{
    memset(q, 0, sizeof(*q));

    q->gi = gi;
    if (!(play->sceneId == SCE_OOT_TREASURE_SHOP && gi != -GI_OOT_TC_HEART_PIECE && !Config_Flag(CFG_OOT_CHEST_GAME_SHUFFLE)))
    {
        q->ovType = OV_CHEST;
        q->sceneId = play->sceneId;
        q->id = this->params & 0x1f;
    }
    else
    {
        q->ovType = OV_NONE;
    }
}

static void EnBox_ItemOverride(ComboItemOverride* o, Actor* this, PlayState* play, s16 gi)
{
    ComboItemQuery q;

    EnBox_ItemQuery(&q, this, play, gi);
    comboItemOverride(o, &q);
}

static s16 EnBox_GetGI(Actor* this)
{
    return -((this->params >> 5) & 0x7f);
}

void EnBox_GiveItem(Actor* actor, PlayState* play, s16 gi)
{
    ComboItemQuery q;

    EnBox_ItemQuery(&q, actor, play, gi);
    comboGiveItem(actor, play, &q, 50.f, 10.f);
}

PATCH_CALL(0x808696bc, EnBox_GiveItem);

void EnBox_InitWrapper(Actor* this, PlayState* play)
{
    ActorCallback init;
    ComboItemOverride o;

    /* Init the chest */
    init = actorAddr(ACTOR_EN_BOX, 0x808687e8);
    init(this, play);

    /* Resize chest */
    EnBox_ItemOverride(&o, this, play, EnBox_GetGI(this));
    csmcChestInit(this, play, o.giRaw, o.cloakGi);
}

void EnBox_DrawWrapper(Actor* this, PlayState* play)
{
    ActorCallback draw;
    ComboItemOverride o;

    /* Prepare the segments */
    EnBox_ItemOverride(&o, this, play, EnBox_GetGI(this));
    csmcChestPreDraw(this, play, o.giRaw, o.cloakGi);

    /* Draw */
    draw = actorAddr(ACTOR_EN_BOX, 0x80869e68);
    draw(this, play);
}

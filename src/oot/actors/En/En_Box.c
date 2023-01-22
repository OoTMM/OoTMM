#include <combo.h>

static s16 EnBox_Item(Actor* this, GameState_Play* play, s16 gi, int progressive)
{
    if (!(play->sceneId == SCE_OOT_TREASURE_SHOP && (this->variable & 0x1f) == 0x0a && gi == -GI_OOT_TC_RUPEE_PURPLE))
        gi = comboOverrideEx(OV_CHEST, play->sceneId, this->variable & 0x1f, gi, progressive ? OVF_PROGRESSIVE | OVF_DOWNGRADE : 0);
    return gi;
}

int EnBox_GiveItem(Actor* actor, GameState_Play* play, s16 gi)
{
    gi = EnBox_Item(actor, play, gi, 1);
    return GiveItemDefaultRange(actor, play, gi);
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
    gi = EnBox_Item(this, play, -1, 0);
    comboCsmcInit(this, play, gi);
}

void EnBox_DrawWrapper(Actor* this, GameState_Play* play)
{
    ActorCallback draw;
    s16 gi;

    /* Prepare the segments */
    gi = EnBox_Item(this, play, -1, 0);
    comboCsmcPreDraw(this, play, gi);

    /* Draw */
    draw = actorAddr(AC_EN_BOX, 0x80869e68);
    draw(this, play);
}

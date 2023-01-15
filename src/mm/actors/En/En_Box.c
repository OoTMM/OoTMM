#include <combo.h>

static s16 EnBox_Item(Actor* this, GameState_Play* play, s16 gi, int progressive)
{
    if (play->sceneId == SCE_MM_TREASURE_SHOP && gi == -GI_MM_HEART_PIECE)
    {
        gi = comboOverrideEx(OV_NPC, 0, NPC_MM_CHEST_GAME, gi, progressive ? 0 : OVF_NO_PROGRESSIVE);
    }
    else
    {
        gi = comboOverrideEx(OV_CHEST, play->sceneId, this->variable & 0x1f, gi, progressive ? 0 : OVF_NO_PROGRESSIVE);
    }
    return gi;
}

int EnBox_GiveItemDefaultRange(Actor* actor, GameState_Play* play, s16 gi)
{
    gi = EnBox_Item(actor, play, gi, 1);
    return GiveItemDefaultRange(actor, play, gi);
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
    draw = actorAddr(AC_EN_BOX, 0x808698f4);
    draw(this, play);
}

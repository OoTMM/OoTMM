#include <combo.h>

static s16 EnBox_Item(Actor* this, GameState_Play* play, s16 gi, int progressive)
{
    if (play->sceneId == SCE_MM_TREASURE_SHOP && gi == -GI_MM_HEART_PIECE)
    {
        gi = comboOverrideEx(OV_NPC, 0, NPC_MM_CHEST_GAME, gi, progressive ? OVF_PROGRESSIVE | OVF_DOWNGRADE : 0);
    }
    else
    {
        gi = comboOverrideEx(OV_CHEST, play->sceneId, this->variable & 0x1f, gi, progressive ? OVF_PROGRESSIVE | OVF_DOWNGRADE : 0);
    }
    return gi;
}

static s16 EnBox_GetGI(Actor* this)
{
    s32 raw;

    raw = *(s32*)((char*)this + 0x21c);
    return (s16)(-raw);
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
    gi = EnBox_Item(this, play, EnBox_GetGI(this), 0);
    comboCsmcInit(this, play, gi);
}

void EnBox_DrawWrapper(Actor* this, GameState_Play* play)
{
    ActorCallback draw;
    s16 gi;

    /* Prepare the segments */
    gi = EnBox_Item(this, play, EnBox_GetGI(this), 0);
    comboCsmcPreDraw(this, play, gi);

    /* Draw */
    draw = actorAddr(AC_EN_BOX, 0x808698f4);
    draw(this, play);
}

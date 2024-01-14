#include <combo.h>
#include <combo/item.h>

int ShotSun_GiveFireArrow(Actor* actor, GameState_Play* play)
{
    PlaySound(0x4802);
    return gOotExtraFlags.fireArrow ? 0 : 1;
}

void ShotSun_Aliases(Actor_ShotSun* this, GameState_Play* play)
{
    /* Set the extended properties */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = g.sceneSetupId;
    this->xflag.roomId = this->base.room;
    this->xflag.sliceId = 0;
    this->xflag.id = g.actorIndex;

    switch (this->xflag.sceneId)
    {
    case SCE_OOT_HYRULE_FIELD:
        this->xflag.setupId = 0;
        this->xflag.id = 0x48;
        break;
    case SCE_OOT_LAKE_HYLIA:
        this->xflag.setupId = 0;
        this->xflag.id = 0x37;
        break;
    case SCE_OOT_LOST_WOODS:
        this->xflag.setupId = 0;
        this->xflag.id = 0x03;
        break;
    case SCE_OOT_DEATH_MOUNTAIN_TRAIL:
        this->xflag.setupId = 0;
        this->xflag.id = 0x26;
        break;
    }
}

void ShotSun_InitWrapper(Actor_ShotSun* this, GameState_Play* play)
{
    ActorCallback init;
    u16 variable = this->base.variable & 0xFF;

    // Fairy Spawner
    if (variable == 0x40 || variable == 0x41) {
        ShotSun_Aliases(this, play);
    }

    init = actorAddr(AC_SHOT_SUN, 0x80b226c0);
    init(&this->base, play);
}

void ShotSun_SpawnFairy(Actor_ShotSun* this, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable)
{
    ComboItemQuery q;
    ComboItemOverride o;
    Actor_EnElf* fairy = (Actor_EnElf*)SpawnActor(&play->actorCtx, play, actorId, x, y, z, rx, ry, rz, variable);

    if (!fairy)
    {
        return;
    }

    /* Copy the extended flag */
    memcpy(&fairy->xflag, &this->xflag, sizeof(Xflag));

    /* Query the item */
    EnElf_ItemQuery(&q, fairy);
    comboItemOverride(&o, &q);

    if (o.gi != fairy->extendedGi)
    {
        fairy->itemGiven = 0;
        fairy->extendedGiDraw = o.gi;
        fairy->base.draw = EnElf_Draw;
    }
}

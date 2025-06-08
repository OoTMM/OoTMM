#include <combo.h>
#include <combo/item.h>
#include <combo/global.h>
#include <combo/actor.h>

int ShotSun_GiveFireArrow(Actor* actor, PlayState* play)
{
    PlaySound(0x4802);
    return gOotExtraFlags.fireArrow ? 0 : 1;
}

void ShotSun_Aliases(Xflag* xf)
{
    switch (xf->sceneId)
    {
    case SCE_OOT_HYRULE_FIELD:
        xf->setupId = 0;
        xf->id = 0x48;
        break;
    case SCE_OOT_LAKE_HYLIA:
        xf->setupId = 0;
        xf->id = 0x37;
        break;
    case SCE_OOT_LOST_WOODS:
        xf->setupId = 0;
        xf->id = 0x03;
        break;
    case SCE_OOT_DEATH_MOUNTAIN_TRAIL:
        xf->setupId = 0;
        xf->id = 0x26;
        break;
    }
}

void ShotSun_InitWrapper(Actor_ShotSun* this, PlayState* play)
{
    ActorCallback init;
    u16 variable = this->base.params & 0xFF;

    /* Fairy Spawner */
    if (variable == 0x40 || variable == 0x41)
    {
        if (comboXflagInit(&this->xflag, &this->base, play))
            ShotSun_Aliases(&this->xflag);
    }

    init = actorAddr(ACTOR_SHOT_SUN, 0x80b226c0);
    init(&this->base, play);
}

void ShotSun_SpawnFairy(Actor_ShotSun* this, PlayState* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable)
{
    memcpy(&g.xflag, &this->xflag, sizeof(Xflag));
    g.xflagOverride = TRUE;
    Actor_Spawn(&play->actorCtx, play, actorId, x, y, z, rx, ry, rz, variable);
    g.xflagOverride = FALSE;
}

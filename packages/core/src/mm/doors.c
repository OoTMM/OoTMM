#include <combo.h>
#include <combo/config.h>
#include <combo/actor.h>

#define DOOR_NONE       0
#define DOOR_SMALL_KEY  1
#define DOOR_BOSS_KEY   2

static int doorType(PlayState* play, Actor* this)
{
    u8 tmp;

    switch (this->id)
    {
    case ACTOR_DOOR_SHUTTER:
        tmp = (this->params >> 7) & 7;
        if (tmp == 0x04)
            return DOOR_SMALL_KEY;
        if (tmp == 0x05)
            return DOOR_BOSS_KEY;
        break;
    }

    return DOOR_NONE;
}

int comboDoorIsUnlocked(PlayState* play, Actor* actor)
{
    int type;
    int flag;

    type = doorType(play, actor);
    flag = actor->params & 0x7f;

    if (type == DOOR_BOSS_KEY && Config_Flag(CFG_MM_NO_BOSS_KEY))
        return 1;
    else if (type == DOOR_SMALL_KEY && Config_Flag(CFG_MM_NO_SMALL_KEY))
        return 1;

    return Flags_GetSwitch(play, flag);
}

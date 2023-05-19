#include <combo.h>

#define DOOR_NONE       0
#define DOOR_SMALL_KEY  1
#define DOOR_BOSS_KEY   2

static int doorType(GameState_Play* play, Actor* this)
{
    u8 tmp;

    switch (this->id)
    {
    case AC_DOOR_SHUTTER:
        tmp = (this->variable >> 7) & 7;
        if (tmp == 0x04)
            return DOOR_SMALL_KEY;
        if (tmp == 0x05)
            return DOOR_BOSS_KEY;
        break;
    }

    return DOOR_NONE;
}

int comboDoorIsUnlocked(GameState_Play* play, Actor* actor)
{
    int type;
    int flag;

    type = doorType(play, actor);
    flag = actor->variable & 0x7f;

    if (type == DOOR_BOSS_KEY && comboConfig(CFG_MM_NO_BOSS_KEY))
        return 1;
    else if (type == DOOR_SMALL_KEY && comboConfig(CFG_MM_NO_SMALL_KEY))
        return 1;

    return GetSwitchFlag(play, flag);
}

#include <combo.h>

#define DOOR_NONE       0
#define DOOR_SMALL_KEY  1
#define DOOR_BOSS_KEY   2

static int doorType(GameState_Play* play, Actor* this)
{
    u8 tmp;

    if (play->sceneId == SCE_OOT_GERUDO_FORTRESS || play->sceneId == SCE_OOT_TREASURE_SHOP)
        return DOOR_NONE;

    switch (this->id)
    {
    case AC_EN_DOOR:
        tmp = (this->variable >> 7) & 7;
        if (tmp == 1)
            return DOOR_SMALL_KEY;
        break;
    case AC_DOOR_SHUTTER:
        tmp = (this->variable >> 6) & 0xf;
        if (tmp == 0x05)
            return DOOR_BOSS_KEY;
        if (tmp == 0x0b)
            return DOOR_SMALL_KEY;
        break;
    }

    return DOOR_NONE;
}

int comboDoorIsUnlocked(GameState_Play* play, Actor* actor)
{
    u16 sceneId;
    int type;
    int flag;

    sceneId = play->sceneId;
    type = doorType(play, actor);
    flag = actor->variable & 0x3f;

    /* Fire temple 1st door */
    if (sceneId == SCE_OOT_TEMPLE_FIRE && flag == 0x17 && !comboConfig(CFG_SMALL_KEY_SHUFFLE) && !(gComboData.mq & (1 << MQ_TEMPLE_FIRE)))
        return 1;

    /* Water temple water raise door */
    if (sceneId == SCE_OOT_TEMPLE_WATER && flag == 0x15 && !(gComboData.mq & (1 << MQ_TEMPLE_WATER)))
        return 1;

    if (type == DOOR_BOSS_KEY)
    {
        if (sceneId == SCE_OOT_GANON_TOWER)
        {
            if (comboConfig(CFG_GOAL_TRIFORCE))
                return gOotExtraFlags.triforceWin;
            if (comboConfig(CFG_GANON_NO_BOSS_KEY))
                return 1;
        }
        else
        {
            if (comboConfig(CFG_OOT_NO_BOSS_KEY))
                return 1;
        }
    }
    else if (type == DOOR_SMALL_KEY && comboConfig(CFG_OOT_NO_SMALL_KEY))
        return 1;

    return GetSwitchFlag(play, flag);
}

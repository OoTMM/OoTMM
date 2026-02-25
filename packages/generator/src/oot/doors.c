#include <combo.h>
#include <combo/dungeon.h>
#include <combo/config.h>
#include <combo/actor.h>
#include <combo/oot/actors/En_Door.h>

#define DOOR_NONE       0
#define DOOR_SMALL_KEY  1
#define DOOR_BOSS_KEY   2

static int doorType(PlayState* play, Actor* this)
{
    u8 tmp;

    if (play->sceneId == SCE_OOT_GERUDO_FORTRESS || play->sceneId == SCE_OOT_TREASURE_SHOP)
        return DOOR_NONE;

    switch (this->id)
    {
    case ACTOR_EN_DOOR:
        tmp = (this->params >> 7) & 7;
        if (tmp == 1)
            return DOOR_SMALL_KEY;
        break;
    case ACTOR_DOOR_SHUTTER:
        tmp = (this->params >> 6) & 0xf;
        if (tmp == 0x05)
            return DOOR_BOSS_KEY;
        if (tmp == 0x0b)
            return DOOR_SMALL_KEY;
        break;
    }

    return DOOR_NONE;
}

int comboDoorIsUnlocked(PlayState* play, Actor* actor)
{
    u16 sceneId;
    int type;
    int flag;

    sceneId = play->sceneId;
    type = doorType(play, actor);
    flag = actor->params & 0x3f;

    /* Fire temple 1st door */
    if (sceneId == SCE_OOT_TEMPLE_FIRE && flag == 0x17 && !Config_Flag(CFG_SMALL_KEY_SHUFFLE) && !(Config_IsMq(MQ_TEMPLE_FIRE)))
        return 1;

    /* Water temple water raise door */
    if (sceneId == SCE_OOT_TEMPLE_WATER && flag == 0x15 && !(Config_IsMq(MQ_TEMPLE_WATER)))
        return 1;

    if (type == DOOR_BOSS_KEY)
    {
        if (sceneId == SCE_OOT_GANON_TOWER)
        {
            if (Config_Flag(CFG_GOAL_TRIFORCE) || Config_Flag(CFG_GOAL_TRIFORCE3))
                return gOotExtraFlags.triforceWin;
            if (Config_Flag(CFG_GANON_NO_BOSS_KEY))
                return 1;
        }
        else
        {
            if (Config_Flag(CFG_OOT_NO_BOSS_KEY))
                return 1;
        }
    }
    else if (type == DOOR_SMALL_KEY && Config_Flag(CFG_OOT_NO_SMALL_KEY))
        return 1;

    return Flags_GetSwitch(play, flag);
}

void Door_DrawDoorLock(PlayState* play, Actor_EnDoor* door, s32 type)
{
    s32 yaw = Math_Vec3f_Yaw(&play->view.eye, &door->actor.world.pos);
    if (ABS((s16)(door->actor.shape.rot.y - yaw)) < 0x4000) {
        Matrix_RotateY(M_PI, MTXMODE_APPLY);
    }
    Actor_DrawDoorLock(play, door->lockTimer, type);
}

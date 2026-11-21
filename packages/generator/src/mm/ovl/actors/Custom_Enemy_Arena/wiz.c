#include "family.h"
typedef struct {
    Actor actor;
    SkelAnime skel;
    Vec3s joint[20], morph[20];
    SkelAnime ghostSkel;
    Vec3s ghostJoint[20], ghostMorph[20];
    ActorFunc actionFunc;
    s16 action, timer, introTimer, fightState, flameScroll, projectile;
    s16 ranPlatforms, unused, angularVelocity, alpha, lightAlpha, targetLightAlpha, cameraAngle;
    u8 startTimer, introState;
} ScriptedWiz;
ASSERT_OFFSET(ScriptedWiz, actionFunc, 0x3ac);
ASSERT_OFFSET(ScriptedWiz, introState, 0x3cb);
int ScriptedWiz_Ready(Actor* actor, PlayState* play)
{
    ScriptedWiz* wiz = (ScriptedWiz*)actor;
    return actor->id == 0x15d && !actor->init && actor->colChkInfo.health && wiz->introState == 7 && wiz->action != 5 && wiz->action != 6;
}

void ScriptedWiz_Begin(Actor* actor, PlayState* play) {}
void ScriptedWiz_Complete(Actor* actor, PlayState* play)
{
    actor->colChkInfo.health = 0;
    ((void (*)(Actor*))actorAddr(0x15d, 0x80a4767c))(actor);
}

int ScriptedWiz_Completed(PlayState* play, const ActorEntry* original)
{
    s16 flag = original->params & 0x7f;
    return flag == 0x7f ? comboEnemyScriptedRoomComplete(play) : Flags_GetSwitch(play, flag);
}

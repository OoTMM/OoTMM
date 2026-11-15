#include "family.h"

int ScriptedPametfrog_Ready(Actor* actor, PlayState* play)
{
    ActorFunc action;
    if (actor->id != 7 || actor->init || !actor->colChkInfo.health)
        return 0;
    action = *(ActorFunc*)((char*)actor + 0x2a8);
    if (actor->params == 0 && action == (ActorFunc)actorAddr(7, 0x8086c72c))
    {
        actor->colChkInfo.health = 0;
        ((void (*)(Actor*))actorAddr(7, 0x8086c5a8))(actor);
        return 0;
    }
    return actor->params == 3 && action == (ActorFunc)actorAddr(7, 0x8086a964) &&
           scriptedFamily(actor, play, 0x144, 0) == 1;
}

void ScriptedPametfrog_Begin(Actor* actor, PlayState* play)
{
    if (scriptedFamily(actor, play, 0x144, 1) != 1)
        comboEnemyScriptedBlock(actor);
}

void ScriptedPametfrog_Complete(Actor* actor, PlayState* play)
{
    comboEnemyScriptedReleaseChildren(actor);
    actor->colChkInfo.health = 0;
    ((void (*)(Actor*))actorAddr(7, 0x8086c5a8))(actor);
}

int ScriptedPametfrog_Completed(PlayState* play, const ActorEntry* original)
{
    return comboEnemyScriptedRoomComplete(play);
}

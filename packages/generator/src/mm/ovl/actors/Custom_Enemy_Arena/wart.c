#include "family.h"

int ScriptedWart_Ready(Actor* actor, PlayState* play)
{
    return actor->id == 0x12c && !actor->init && actor->colChkInfo.health &&
           *(ActorFunc*)((char*)actor + 0x204) == (ActorFunc)actorAddr(0x12c, 0x809ecd18) &&
           scriptedFamily(actor, play, 0x23e, 0) == 83;
}

void ScriptedWart_Begin(Actor* actor, PlayState* play)
{
    if (scriptedFamily(actor, play, 0x23e, 1) != 83)
        comboEnemyScriptedBlock(actor);
}

void ScriptedWart_Complete(Actor* actor, PlayState* play)
{
    comboEnemyScriptedReleaseChildren(actor);
    actor->colChkInfo.health = 0;
    ((void (*)(Actor*))actorAddr(0x12c, 0x809ed224))(actor);
}

int ScriptedWart_Completed(PlayState* play, const ActorEntry* original)
{
    return comboEnemyScriptedRoomComplete(play);
}

#include "family.h"

int ScriptedBigslime_Ready(Actor* actor, PlayState* play)
{
    ActorFunc action;
    if (actor->id != 0x65 || actor->init || !actor->colChkInfo.health)
        return 0;
    action = *(ActorFunc*)((char*)actor + 0x188);
    if (action == (ActorFunc)actorAddr(0x65, 0x808ea7a4))
    {
        if (scriptedFamily(actor, play, 0xd9, 0) != 10)
            return 0;
        ((void (*)(Actor*))actorAddr(0x65, 0x808ea264))(actor);
        ((void (*)(Actor*))actorAddr(0x65, 0x808eb83c))(actor);
        return 0;
    }
    return action == (ActorFunc)actorAddr(0x65, 0x808e71fc) && *(s16*)((char*)actor + 0x2c6) == -1 &&
           scriptedFamily(actor, play, 0xd9, 0) == 10;
}

void ScriptedBigslime_Begin(Actor* actor, PlayState* play)
{
    if (scriptedFamily(actor, play, 0xd9, 1) != 10)
        comboEnemyScriptedBlock(actor);
}

void ScriptedBigslime_Complete(Actor* actor, PlayState* play)
{
    comboEnemyScriptedReleaseChildren(actor);
    ((ActorFunc)actorAddr(0x65, 0x808e6570))(actor, play);
    actor->colChkInfo.health = 0;
    ((void (*)(Actor*))actorAddr(0x65, 0x808eb83c))(actor);
}

int ScriptedBigslime_Completed(PlayState* play, const ActorEntry* original)
{
    return comboEnemyScriptedRoomComplete(play);
}

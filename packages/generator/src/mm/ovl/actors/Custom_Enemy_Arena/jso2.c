#include <combo.h>
#include <combo/enemy_scripted.h>

int ScriptedJso2_Ready(Actor* actor, PlayState* play)
{
    return actor->id == 0x182 && !actor->init && actor->colChkInfo.health && *(s16*)((char*)actor + 0x284) == 3 &&
           *(ActorFunc*)((char*)actor + 0x278) == (ActorFunc)actorAddr(0x182, 0x80a78868);
}

void ScriptedJso2_Begin(Actor* actor, PlayState* play) {}
void ScriptedJso2_Complete(Actor* actor, PlayState* play)
{
    actor->colChkInfo.health = 0;
    ((ActorFunc)actorAddr(0x182, 0x80a7998c))(actor, play);
}

int ScriptedJso2_Completed(PlayState* play, const ActorEntry* original)
{
    return Flags_GetClear(play, play->roomCtx.curRoom.num);
}

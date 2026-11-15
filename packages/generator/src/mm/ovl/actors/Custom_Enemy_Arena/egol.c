#include <combo.h>
#include <combo/enemy_scripted.h>

int ScriptedEgol_Ready(Actor* actor, PlayState* play)
{
    s16 action = *(s16*)((char*)actor + 0x2b2);
    return actor->id == 0x184 && !actor->init && actor->colChkInfo.health && action >= 2 && action <= 12;
}

void ScriptedEgol_Begin(Actor* actor, PlayState* play) {}
void ScriptedEgol_Complete(Actor* actor, PlayState* play)
{
    actor->colChkInfo.health = 0;
    actor->speed = 0.f;
    ((void (*)(Actor*))actorAddr(0x184, 0x80a7eb90))(actor);
}

int ScriptedEgol_Completed(PlayState* play, const ActorEntry* original)
{
    u16 flag = ((u16)original->params >> 6) & 0x7f;
    return flag != 0x7f && Flags_GetSwitch(play, flag);
}

#include <combo.h>
#include <combo/enemy_scripted.h>

void Enemy_StartFinishingBlow(PlayState*, Actor*);
void Audio_RestorePrevBgm(void);

static Actor* ScriptedDeath_Bat(Actor* root, PlayState* play, int index)
{
    Actor* candidate = ((Actor**)((char*)root + 0x6e4))[index];
    for (int category = 0; category < 12; ++category)
        for (Actor* actor = play->actorCtx.actors[category].first; actor; actor = actor->next)
            if (actor == candidate && actor->id == 0x44 && actor->parent == root && !actor->init && actor->update)
                return actor;
    return NULL;
}

int ScriptedDeath_Ready(Actor* actor, PlayState* play)
{
    if (actor->id != 0x43 || actor->init || !actor->colChkInfo.health ||
        *(ActorFunc*)((char*)actor + 0x188) != (ActorFunc)actorAddr(0x43, 0x808c64dc))
        return 0;
    for (int i = 0; i < 20; ++i)
        if (!ScriptedDeath_Bat(actor, play, i))
            return 0;
    return 1;
}

void ScriptedDeath_Begin(Actor* actor, PlayState* play)
{
    for (int i = 0; i < 20; ++i)
    {
        Actor* bat = ScriptedDeath_Bat(actor, play, i);
        if (!bat || !comboEnemyScriptedHoldChild(actor, bat))
        {
            comboEnemyScriptedBlock(actor);
            return;
        }
    }
}

void ScriptedDeath_Complete(Actor* actor, PlayState* play)
{
    for (int i = 0; i < 20; ++i)
    {
        Actor* bat = ScriptedDeath_Bat(actor, play, i);
        if (!bat)
        {
            comboEnemyScriptedBlock(actor);
            return;
        }
        ((void (*)(Actor*))actorAddr(0x44, 0x808cb524))(bat);
    }
    actor->colChkInfo.health = 0;
    actor->speed = 0.f;
    Enemy_StartFinishingBlow(play, actor);
    Audio_RestorePrevBgm();
    ((void (*)(Actor*))actorAddr(0x43, 0x808c7cfc))(actor);
}

int ScriptedDeath_Completed(PlayState* play, const ActorEntry* original)
{
    return comboEnemyScriptedRoomComplete(play);
}

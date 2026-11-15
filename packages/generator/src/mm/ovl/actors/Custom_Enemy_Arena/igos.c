#include <combo.h>
#include <combo/enemy_scripted.h>

static int igosGuards(Actor* root, PlayState* play, int hold, int finish)
{
    int count = 0;
    for (int category = 0; category < ACTORCAT_MAX; ++category)
        for (Actor* actor = play->actorCtx.actors[category].first; actor; actor = actor->next)
            if (actor != root && actor->id == 0x115 && actor->params == 35 && actor->room == root->room &&
                actor->update && !actor->init)
            {
                ++count;
                if (hold && !comboEnemyScriptedHoldChild(root, actor))
                    comboEnemyScriptedBlock(root);
                if (finish)
                    ((ActorFunc)actorAddr(0x115, 0x809b5634))(actor, play);
            }
    return count;
}

static int igosCurtains(Actor* root, PlayState* play, int hold)
{
    int count = 0;
    for (int category = 0; category < ACTORCAT_MAX; ++category)
        for (Actor* actor = play->actorCtx.actors[category].first; actor; actor = actor->next)
            if (actor->id == 0x12e && actor->parent == root && actor->room == root->room &&
                (actor->params == 0 || actor->params == 1) && actor->update && !actor->init)
            {
                ++count;
                if (hold)
                {
                    ActorFunc draw = actor->draw;
                    if (!comboEnemyScriptedHoldChild(root, actor))
                        comboEnemyScriptedBlock(root);
                    else
                        actor->draw = draw;
                }
            }
    return count;
}

int ScriptedIgos_Ready(Actor* actor, PlayState* play)
{
    return actor->id == 0x115 && actor->params == 0 && !actor->init && actor->colChkInfo.health &&
           CutsceneManager_GetCurrentCsId() == -1 &&
           *(ActorFunc*)((char*)actor + 0x420) == (ActorFunc)actorAddr(0x115, 0x809b6764) &&
           igosGuards(actor, play, 0, 0) == 2 && igosCurtains(actor, play, 0) == 2 && !*(u8*)((char*)actor + 0x151);
}

void ScriptedIgos_Begin(Actor* actor, PlayState* play)
{
    if (igosGuards(actor, play, 1, 0) != 2 || igosCurtains(actor, play, 1) != 2)
        comboEnemyScriptedBlock(actor);
}

void ScriptedIgos_Complete(Actor* actor, PlayState* play)
{
    if (igosGuards(actor, play, 0, 0) != 2)
    {
        comboEnemyScriptedBlock(actor);
        return;
    }
    igosGuards(actor, play, 0, 1);
    actor->colChkInfo.health = 0;
    ((ActorFunc)actorAddr(0x115, 0x809b5634))(actor, play);
}

int ScriptedIgos_Completed(PlayState* play, const ActorEntry* original)
{
    return Flags_GetClear(play, play->roomCtx.curRoom.num);
}

#include <combo.h>
#include <combo/enemy_scripted.h>

int ScriptedKeeta_Ready(Actor* actor, PlayState* play)
{
    return actor->id == 0x28f && !(actor->params & 0x8000) && !actor->init && actor->colChkInfo.health &&
           CutsceneManager_GetCurrentCsId() == -1 && *(void**)((char*)actor + 0x288) &&
           *(s16*)((char*)actor + 0x2b4) == 4 && *(s32*)((char*)actor + 0x2dc);
}

void ScriptedKeeta_Begin(Actor* actor, PlayState* play) {}
void ScriptedKeeta_Complete(Actor* actor, PlayState* play)
{
    actor->colChkInfo.health = 0;
    Enemy_StartFinishingBlow(play, actor);
    Actor_PlaySfx(actor, 0x3aa6);
    actor->flags |= ACTOR_FLAG_LOCK_ON_DISABLED;
    actor->flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    Actor_ChangeCategory(play, &play->actorCtx, actor, ACTORCAT_NPC);
    ((ActorFunc)actorAddr(0x28f, 0x80c0d3c0))(actor, play);
}

int ScriptedKeeta_Completed(PlayState* play, const ActorEntry* original)
{
    return !!(gSave.info.weekEventReg[23] & 4);
}

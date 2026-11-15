#ifndef COMBO_ENEMY_BODYBREAK_IMPL_H
#define COMBO_ENEMY_BODYBREAK_IMPL_H

#include <combo/enemy_bodybreak.h>

extern Actor* comboEnemyActorSpawnAsChild(ActorContext*, Actor*, PlayState*, s16, float, float, float, s16, s16, s16, s32);

static void enemyBodyBreakAlloc(BodyBreak* body, s32 count, PlayState* play)
{
    if (body->matrices)
        free(body->matrices);
    memset(body, 0, sizeof(*body));
    body->play = play;
    body->prevLimbIndex = -1;
    if (count < 1 || count > 60)
        return;
    body->matrices = malloc((count + 1) * (sizeof(MtxF) + sizeof(Gfx*) + sizeof(s16)));
    if (!body->matrices)
        return;
    body->dLists = (Gfx**)(body->matrices + count + 1);
    body->objectSlots = (s16*)(body->dLists + count + 1);
    body->capacity = count;
    body->val = 1;
}

static void enemyBodyBreakInfo(BodyBreak* body, s32 limb, s32 first, s32 last, u32 count, Gfx** dList, s16 objectSlot)
{
    if (body->val <= 0 || !body->play || body->play->actorCtx.freezeFlashTimer)
        return;
    if (limb >= first && limb <= last && *dList && body->val <= body->capacity)
    {
        body->dLists[body->val] = *dList;
        Matrix_Get(&body->matrices[body->val]);
        body->objectSlots[body->val++] = objectSlot;
    }
    if (body->prevLimbIndex != limb)
        body->count++;
    if ((u32)body->count >= count)
    {
        body->count = body->val - 1;
        body->val = -1;
    }
    body->prevLimbIndex = limb;
}

static s32 enemyBodyBreakSpawn(Actor* actor, BodyBreak* body, PlayState* play, s16 type)
{
    if (!body->matrices)
        return 1;
    if (body->val != -1)
        return 0;
    if (type != 1 && type != 3 && type != 11)
        return 0;
    while (body->count > 0)
    {
        s32 index = body->count--;
        if (!actor->scale.x || !actor->scale.y || !actor->scale.z)
            continue;
        MtxF matrix;
        Matrix_Put(&body->matrices[index]);
        Matrix_Scale(1.f / actor->scale.x, 1.f / actor->scale.y, 1.f / actor->scale.z, MTXMODE_APPLY);
        Matrix_Get(&matrix);
        s16 slot = body->objectSlots[index] >= 0 ? body->objectSlots[index] : actor->objectSlot;
        Actor* savedChild = actor->child;
        EnemyBodyPart* part = (EnemyBodyPart*)comboEnemyActorSpawnAsChild(&play->actorCtx, actor, play, 7,
            matrix.xw, matrix.yw, matrix.zw, 0, 0, slot, type);
        if (part)
        {
            ((void (*)(MtxF*, Vec3s*, s32))0x801822c4)(&matrix, &part->actor.shape.rot, 0);
            part->displayList = body->dLists[index];
            part->actor.scale = actor->scale;
            part->actor.parent = NULL;
            actor->child = savedChild;
            Actor_SetObjectDependency(play, &part->actor);
        }
    }
    free(body->matrices);
    body->matrices = NULL;
    body->val = 0;
    return 1;
}

#endif

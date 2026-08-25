#include <combo.h>

#include <combo/mm/actors/En_Bal.h>

s32 Camera_ShouldSetFocalActor(Camera *camera, Actor *actor)
{
    if (actor == NULL)
        return false;

    s32 shouldSetFocalActor = true;
    switch(actor->id)
    {
    case ACTOR_EN_BAL: // Tingle with Balloon
    {
        shouldSetFocalActor = false;
        EnBal* tingle = (EnBal*)actor;
        tingle->timer = 29;
        tingle->eyeTimer = 6;
        break;
    }
    }
    return shouldSetFocalActor;
}

void Camera_SetFocalActorHook(Camera *camera, Actor *actor)
{
    if (!Camera_ShouldSetFocalActor(camera, actor))
        return;

    _Camera_SetFocalActor(camera, actor);
}

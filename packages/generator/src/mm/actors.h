#ifndef ACTORS_H
#define ACTORS_H

#include <combo.h>

void Actor_InitFaroresWind(PlayState* play);
void Actor_ChangeCategory(PlayState* play, ActorContext* actorCtx, Actor* actor, u8 actorCategory);
void Actor_SetColorFilter(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration);
void Actor_DrawDamageEffects(PlayState* play, Actor* actor, Vec3f bodyPartsPos[], s16 bodyPartsCount, f32 effectScale, f32 frozenSteamScale, f32 effectAlpha, u8 type);
void Actor_SetFocus(Actor* actor, f32 height);
void Actor_RequestQuakeAndRumble(Actor* actor, PlayState* play, s16 quakeY, s16 quakeDuration);
s16 Actor_WorldYawTowardActor(Actor* actorA, Actor* actorB);
s32 func_800BE184(PlayState* play, Actor* actor, f32 xzDist, s16 arg3, s16 arg4, s16 arg5);
void Actor_SetDropFlag(Actor* actor, ColliderElement* elem);
void func_800B8050(Actor* actor, PlayState* play, s32 flag);
void func_800B8118(Actor* actor, PlayState* play, s32 flag);
void func_800BC620(Vec3f* pos, Vec3f* scale, u8 alpha, PlayState* play);

#endif

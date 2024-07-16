#ifndef ACTORS_H
#define ACTORS_H

#include <combo.h>

void Actor_InitFaroresWind(GameState_Play* play);
void Actor_ChangeCategory(GameState_Play* play, ActorContext* actorCtx, Actor* actor, u8 actorCategory);
void Actor_SetColorFilter(Actor* actor, u16 colorFlag, u16 colorIntensityMax, u16 bufFlag, u16 duration);
void Actor_DrawDamageEffects(GameState_Play* play, Actor* actor, Vec3f bodyPartsPos[], s16 bodyPartsCount, f32 effectScale, f32 frozenSteamScale, f32 effectAlpha, u8 type);
void Actor_SetFocus(Actor* actor, f32 height);
void Actor_RequestQuakeAndRumble(Actor* actor, GameState_Play* play, s16 quakeY, s16 quakeDuration);

#endif

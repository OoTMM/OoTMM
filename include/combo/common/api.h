#ifndef COMBO_COMMON_API_H
#define COMBO_COMMON_API_H

#include <combo/types.h>

typedef struct GameState_Play GameState_Play;
typedef struct Actor Actor;

void    LoadFile(void* dst, u32 vromAddr, u32 size);

Actor*  SpawnActor(void* const_1, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable);
void    ActorDestroy(Actor* actor);
int     HasActorGivenItem(Actor* actor);
void    ActorSetScale(Actor* actor, float scale);

u32     GetCollectibleFlag(GameState_Play* play, int flag);
void    SetCollectibleFlag(GameState_Play* play, int flag);
u32     GetSwitchFlag(GameState_Play* play, int flag);
u32     SetSwitchFlag(GameState_Play* play, int flag);

#endif

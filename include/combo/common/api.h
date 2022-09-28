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
void    ActorEnableInteraction(Actor* actor, GameState_Play* play);
void    ActorSetCollisionCylinder(GameState_Play* play, Actor* actor, float unk_3, float unk_4, float unk_5, u32 unk_6);
void    ActorUpdateVelocity(Actor* actor);

u32     GetCollectibleFlag(GameState_Play* play, int flag);
void    SetCollectibleFlag(GameState_Play* play, int flag);
u32     GetSwitchFlag(GameState_Play* play, int flag);
u32     SetSwitchFlag(GameState_Play* play, int flag);

#if defined(GAME_MM)
Actor*  SpawnActorEx(void* const_1, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable, int, int, int);
#endif

void DMARomToRam(u32 romAddr, void* dramAddr, u32 size);

void ConvertMatrix(const float* in, u16* out);
void MatrixRotation(u16 angle, int unk_1);

#endif

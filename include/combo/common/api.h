#ifndef COMBO_COMMON_API_H
#define COMBO_COMMON_API_H

#include <combo/types.h>

typedef struct GameState_Play GameState_Play;
typedef struct Actor Actor;

int    LoadFile(void* dst, u32 vromAddr, u32 size);

Actor*  SpawnActor(void* const_1, GameState_Play* play, s16 actorId, float x, float y, float z, s16 rx, s16 ry, s16 rz, u16 variable);
void    ActorDestroy(Actor* actor);
int     Actor_HasParent(Actor* actor);
void    ActorSetScale(Actor* actor, float scale);
void    ActorEnableGrab(Actor* actor, GameState_Play* play);
void    ActorEnableTalk(Actor* actor, GameState_Play* play, float range);
void    ActorEnableTalkEx(Actor* actor, GameState_Play* play, float range, u32 unk);
void    ActorSetCollisionCylinder(GameState_Play* play, Actor* actor, float unk_3, float unk_4, float unk_5, u32 unk_6);
void    ActorUpdateVelocity(Actor* actor);
int     ActorTalkedTo(Actor* actor);

u32     GetChestFlag(GameState_Play* play, int flag);
u32     SetChestFlag(GameState_Play* play, int flag);
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

void DisplayTextBox(GameState_Play* play, u16 textId);
void DisplayTextBox2(GameState_Play* play, u16 textId);

int GiveItem(Actor* actor, GameState_Play* play, s16 itemId, float a, float b);
int GiveItemDefaultRange(Actor* actor, GameState_Play* play, s16 itemId);

void PlayerDisplayTextBox(GameState_Play* play, u16 messageId, void* unk);

#define TEXT_STATE_NONE     0
#define TEXT_STATE_CLOSING  2

#if defined(GAME_OOT)
int  Message_GetState(MessageContext* ctx);
#endif
void Message_Close(GameState_Play* play);
int  Message_IsClosed(Actor* actor, GameState_Play* play);

void OcarinaAction(GameState_Play *play, u16 ocarinaAction);
void OcarinaAction2(GameState_Play *play, u16 ocarinaAction);

/* Matrix Ops */
#define MAT_SET 0
#define MAT_MUL 1

extern float* gModelView;

void ModelViewUnkTransform(float* unk);
void ModelViewRotateY(float angle, int mode);
void ModelViewTranslate(float tx, float ty, float tz, int mode);
void ModelViewScale(float sx, float sy, float sz, int mode);

float RandFloat(void);

int GetActiveItem(GameState_Play* play);

u8 AddItem(GameState_Play* play, u8 itemId);
u8 GetItemCollectBehavior(s16 itemId);

#if defined(GAME_MM)
void PrepareSave(SramContext* sram);
#endif

s16 RandIntRange(s16 base, s16 amplitude);

extern u32 gSegments[16];

#endif

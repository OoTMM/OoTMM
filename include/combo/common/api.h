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
void    ActorSetUnk(Actor* actor, float unk);
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
void    SetRoomClear(GameState_Play* play, int flag);

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
#else
int  Message_GetState(void* ctx);
#endif

void Message_Close(GameState_Play* play);
int  Message_ShouldAdvance(GameState_Play* play);
int  Message_IsClosed(Actor* actor, GameState_Play* play);

void OcarinaAction(GameState_Play *play, u16 ocarinaAction);
void OcarinaAction2(GameState_Play *play, u16 ocarinaAction);

/* Matrix Ops */
#define MAT_SET 0
#define MAT_MUL 1

extern float* gModelView;

void ModelViewUnkTransform(float* unk);
void ModelViewRotateX(float angle, int mode);
void ModelViewRotateY(float angle, int mode);
void ModelViewRotateZ(float angle, int mode);
void ModelViewTranslate(float tx, float ty, float tz, int mode);
void ModelViewScale(float sx, float sy, float sz, int mode);

float RandFloat(void);

int GetActiveItem(GameState_Play* play);

u8 AddItem(GameState_Play* play, u8 itemId);
u8 GetItemCollectBehavior(s16 itemId);

#if defined(GAME_MM)
void PrepareSave(SramContext* sram);
void Sram_SaveEndOfCycle(GameState_Play* play);
void Sram_SaveNewDay(GameState_Play* play);
#endif

s16 RandIntRange(s16 base, s16 amplitude);

extern u32 gSegments[16];

#define GET_LINK(play) ((Actor_Player*)(play->actors[2].first))

int  ActorCutscene_GetCanPlayNext(int cutscene);
s16  ActorCutscene_GetCurrentIndex(void);
void ActorCutscene_Stop(s16 cutsceneId);
void Cutscene_End(GameState_Play* play, void* unk);

void RemoveItem(s16 item, s16 slot);

void* ActorAlloc(u32 size);
void  ActorFree(void* data);

void Sram_CopySave(void*, void*);

void Play_Init(GameState_Play*);
void Play_Draw(GameState_Play*);

void Interface_LoadItemIconImpl(GameState_Play* play, int slot);
void UpdateEquipment(GameState_Play* play, Actor_Player* link);
void PlayStoreFlags(GameState_Play* play);

void Player_Update(Actor_Player* this, GameState_Play* play);

void* OverlayAddr(u32 addr);

void LoadIcon(u32 vaddr, int iconId, void* buffer, int size);

int Player_UsingItem(Actor_Player* link);

void PlaySound(u16 soundId);
void PlaySoundSpecial(u16 soundId);
void PlayMusic(int arg0, int arg1, int arg2, int arg3, int arg4);

int Actor_RunByteCode(Actor* this, GameState_Play* play, void* bytecode, void* unk1, void* unk2);
void Enemy_StartFinishingBlow(GameState_Play* play, Actor* this);

#endif

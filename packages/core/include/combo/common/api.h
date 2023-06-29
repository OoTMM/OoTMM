#ifndef COMBO_COMMON_API_H
#define COMBO_COMMON_API_H

#include <combo/types.h>
#include <combo/gi.h>

#define PLAYER_ACTOR_STATE_TRANSITION           0x00000001
#define PLAYER_ACTOR_STATE_TRANSFORM            0x00000002
#define PLAYER_ACTOR_STATE_CLIMB                0x00000004
#define PLAYER_ACTOR_STATE_DEATH                0x00000080
#define PLAYER_ACTOR_STATE_FROZEN               0x00000200
#define PLAYER_ACTOR_STATE_GET_ITEM             0x00000400
#define PLAYER_ACTOR_STATE_HOLD_ITEM            0x00000800
#define PLAYER_ACTOR_STATE_CLIMB2               0x00200000
#define PLAYER_ACTOR_STATE_EPONA                0x00800000
#define PLAYER_ACTOR_STATE_WATER                0x08000000
#define PLAYER_ACTOR_STATE_USE_ITEM             0x10000000
#define PLAYER_ACTOR_STATE_CUTSCENE_FROZEN      0x20000000

#if defined(GAME_MM)
# define PLAYER_MASK_NONE               0x00
# define PLAYER_MASK_TRUTH              0x01
# define PLAYER_MASK_KAFEI              0x02
# define PLAYER_MASK_ALL_NIGHT          0x03
# define PLAYER_MASK_BUNNY              0x04
# define PLAYER_MASK_KEATON             0x05
# define PLAYER_MASK_GARO               0x06
# define PLAYER_MASK_ROMANI             0x07
# define PLAYER_MASK_TROUPE_LEADER      0x08
# define PLAYER_MASK_POSTMAN            0x09
# define PLAYER_MASK_COUPLE             0x0a
# define PLAYER_MASK_GREAT_FAIRY        0x0b
# define PLAYER_MASK_GIBDO              0x0c
# define PLAYER_MASK_DON_GERO           0x0d
# define PLAYER_MASK_KAMARO             0x0e
# define PLAYER_MASK_CAPTAIN            0x0f
# define PLAYER_MASK_STONE              0x10
# define PLAYER_MASK_BREMEN             0x11
# define PLAYER_MASK_BLAST              0x12
# define PLAYER_MASK_SCENTS             0x13
# define PLAYER_MASK_GIANT              0x14
# define PLAYER_MASK_FIERCE_DEITY       0x15
# define PLAYER_MASK_GORON              0x16
# define PLAYER_MASK_ZORA               0x17
# define PLAYER_MASK_DEKU               0x18
#endif

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
void    Actor_SetCollisionCylinder(GameState_Play* play, Actor* actor, float unk_3, float unk_4, float unk_5, u32 unk_6);
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

#if defined(GAME_OOT)
void Yaz0_Decompress(u32 romStart, void* dst, u32 size);
#endif

void ConvertMatrix(const float* in, u16* out);
void MatrixRotation(u16 angle, int unk_1);

void DisplayTextBox(GameState_Play* play, u16 textId);
void DisplayTextBox2(GameState_Play* play, u16 textId);
void DisplayTextBox3(GameState_Play* play, Actor* actor, u16 textId);

int GiveItem(Actor* actor, GameState_Play* play, s16 itemId, float a, float b);
void GiveItemDefaultRange(Actor* actor, GameState_Play* play, s16 itemId);

void PlayerDisplayTextBox(GameState_Play* play, u16 messageId, void* unk);

#if defined(GAME_MM)
void Message_ShowMessageAtYPosition(GameState_Play* play, u16 messageId, u8 yPosition);
#endif

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

f32 Math_CosS(s16 angle);
f32 Math_SinS(s16 angle);

void MatrixStackDup(void);
void MatrixStackPop(void);

float RandFloat(void);

int GetActiveItem(GameState_Play* play);

u8 AddItem(GameState_Play* play, u8 itemId);
u8 AddItemWithIcon(GameState_Play* play, Actor_Player* link, const GetItem* giEntry);
u8 GetItemCollectBehavior(s16 itemId);
void Interface_SetDoAction(GameState_Play* play, u16 action);
void Interface_LoadActionLabelB(GameState_Play* play, u16 action);

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

#if defined(GAME_MM)
void AudioOcarina_SetInstrument(u8 ocarinaInstrumentId);
void Player_DrawHookshotReticle(GameState_Play* play, Actor_Player* player, f32 distance);
#endif

int Actor_RunByteCode(Actor* this, GameState_Play* play, void* bytecode, void* unk1, void* unk2);
void Enemy_StartFinishingBlow(GameState_Play* play, Actor* this);

void DeathWarp(GameState_Play* play);

#if defined(GAME_MM)
void DrawDungeonUnk2(void* unk);
#endif

void DrawSimpleOpa(GameState_Play* play, u32 segAddr);

void AddRupees(s16 delta);

/* Shaders */
void Shader_Opa0_Xlu1(GameState_Play*, s16);
void Shader_Opa0_Xlu12(GameState_Play*, s16);
void Shader_Opa0(GameState_Play*, s16);
void Shader_Opa01(GameState_Play*, s16);
void Shader_Opa1023(GameState_Play*, s16);
void Shader_Opa10_Xlu2(GameState_Play*, s16);
void Shader_Opa10_Xlu234(GameState_Play*, s16);
void Shader_Opa10_Xlu32(GameState_Play*, s16);
void Shader_Opa10234567(GameState_Play*, s16);
void Shader_Xlu01(GameState_Play*, s16);
void Shader_BlueFire(GameState_Play*, s16);
void Shader_BombchuMask(GameState_Play*, s16);
void Shader_Compass(GameState_Play*, s16);
void Shader_DekuNut(GameState_Play*, s16);
void Shader_Fairy(GameState_Play*, s16);
void Shader_Fish(GameState_Play*, s16);
void Shader_GiantKnife(GameState_Play*, s16);
void Shader_GS(GameState_Play*, s16);
void Shader_Heart(GameState_Play*, s16);
void Shader_Medallion(GameState_Play*, s16);
void Shader_MirrorShield(GameState_Play*, s16);
void Shader_Poe(GameState_Play*, s16);
void Shader_Potion(GameState_Play*, s16);
void Shader_Rupee(GameState_Play*, s16);
void Shader_Scale(GameState_Play*, s16);
void Shader_SoldOut(GameState_Play*, s16);
void Shader_Spell(GameState_Play*, s16);
void Shader_MoonTear(GameState_Play*, s16);

void SpawnRoomActors(GameState_Play* play, int id);

extern u8 gPlayedOcarinaSong;
extern u8 gIsOcarinaEnabled;
extern u32 gOcarinaFlags;
extern u8 gOcarinaButtonPitchMap[];
extern u8 gOcarinaSongBuffer[];
extern u8 gOcarinaSongBufferCursor;

void HandleMessages(GameState_Play* play, void* unk);

#if defined(GAME_OOT)
void func_800C7200(u8 arg0);
void Letterbox_SetSizeTarget(s32 target);
u32 Letterbox_GetSizeTarget(void);
#endif

void Play_LoadKaleidoOverlay(void*);
void LoadMapMark(void*);

#define TINGLE_MAP_CLOCK_TOWN       0
#define TINGLE_MAP_WOODFALL         1
#define TINGLE_MAP_SNOWHEAD         2
#define TINGLE_MAP_ROMANI_RANCH     3
#define TINGLE_MAP_GREAT_BAY        4
#define TINGLE_MAP_STONE_TOWER      5
void Inventory_SetWorldMapCloudVisibility(s16 tingleId);

#endif

#if defined(GAME_MM)
s16 ActorCutscene_StartAndSetFlag(s16 index, Actor* actor);
#endif

void DrawHUD(GameState_Play* play);
